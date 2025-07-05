#include "GameViewModel.h"

// 构造函数，传入Model引用并建立信号槽连接
GameViewModel::GameViewModel(GameModel& model, QObject *parent)
    : QObject(parent), m_model(model) {
    connect(&m_model, &GameModel::modelChanged, this, &GameViewModel::onModelChanged);
    connect(&m_model, &GameModel::gameOver, this, &GameViewModel::onGameOver);
}

// 设置UI接口
void GameViewModel::setUI(IGameUI* ui) { m_ui = ui; }

// 设置App命令接口
void GameViewModel::setApp(IGameCommands* appCommands) { m_appCommands = appCommands; }

// --- 私有辅助方法 ---
// 封装了开始一局游戏的通用逻辑
void GameViewModel::startGameInternal(const GameDifficulty& difficulty) {
    m_currentDifficulty = difficulty;

    if (m_ui) {
        // 在开始游戏前，立即配置UI模式以准备好界面
        m_ui->configureForMode(m_currentGameMode, m_currentDifficulty, m_currentCampaignLevel + 1, CAMPAIGN_LEVELS.size());
    }

    // 命令Model准备数据，这会触发 onModelChanged 来刷新棋盘
    m_model.startGame(m_currentDifficulty.rows, m_currentDifficulty.cols, m_currentDifficulty.mines);
}

// --- IGameCommands 接口的实现 ---
void GameViewModel::startNewGame(GameMode mode, GameDifficulty difficulty) {
    m_currentGameMode = mode;

    if (m_currentGameMode == GameMode::Campaign) {
        m_currentCampaignLevel = 0;  // 从主菜单开始闯关，总是从第一关开始
        difficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel];  // 获取第一关的难度
    }

    startGameInternal(difficulty);  // 调用内部启动逻辑
}

void GameViewModel::startNextCampaignLevel() {
    if (m_currentGameMode == GameMode::Campaign) {
        m_currentCampaignLevel++;  // 关卡索引加一
        if (m_currentCampaignLevel < CAMPAIGN_LEVELS.size()) {  // 检查是否还有下一关
            GameDifficulty nextLevelDifficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel];
            startGameInternal(nextLevelDifficulty);  // 启动下一关
        }
    }
}

void GameViewModel::revealCellRequest(int row, int col) {
    m_model.revealCell(row, col);  // 直接将请求转发给Model
}

void GameViewModel::toggleFlagRequest(int row, int col) {
    const int currentFlags = m_model.getFlagCount();
    const int mineCount = m_model.getMineCount();
    const bool isCurrentlyFlagged = m_model.getCell(row, col).isFlagged;

    // 如果想新插一个旗帜但旗帜已用完
    if (!isCurrentlyFlagged && currentFlags >= mineCount) {
        if (m_ui) m_ui->showTemporaryMessage("旗帜已用完!", 2000);  // 通过UI提示用户
        return;
    }
    m_model.flagCell(row, col);  // 转发请求给Model
}

void GameViewModel::cycleMarkRequest(int row, int col) {
    m_model.cycleCellMark(row, col);  // 直接将请求转发给Model
}

void GameViewModel::returnToHomeRequest() {
    if (m_appCommands) m_appCommands->returnToHomeRequest();  // 将窗口切换命令转发给App层
}

// --- 槽函数的实现 ---

// 当Model数据改变时，此函数被调用以更新整个UI
void GameViewModel::onModelChanged() {
    if (!m_ui) return;  // 如果UI未设置，则不执行任何操作

    // 根据游戏状态更新状态标签
    switch(m_model.getGameState()){
        case GameState::Playing: m_ui->updateStatusLabel("游戏中..."); break;
        case GameState::Ready: m_ui->updateStatusLabel("准备开始"); break;
        default: break;
    }

    // 更新剩余旗帜数量
    const int flagsLeft = m_model.getMineCount() - m_model.getFlagCount();
    m_ui->updateFlagsLabel(flagsLeft);

    // 遍历Model中的每个格子，并生成对应的UI更新信息
    for (int r = 0; r < m_model.getRows(); ++r) {
        for (int c = 0; c < m_model.getCols(); ++c) {
            const Cell& cell = m_model.getCell(r, c);
            // 默认是未翻开的格子样式
            CellUpdateInfo info{r, c, "", "background-color: #c0c0c0;", true};

            if (m_model.getGameState() == GameState::Lost && cell.isMine) {
                info.text = "💣";
                info.styleSheet = "background-color: red;";
            } else if (cell.isFlagged) {
                info.text = "🚩";
            } else if (cell.isQuestionMark) {
                info.text = "❓";
            } else if (cell.isRevealed) {
                info.styleSheet = "background-color: #e0e0e0; border: 1px solid #808080;";
                if (cell.adjacentMines > 0) {
                    info.text = QString::number(cell.adjacentMines);
                    // 根据数字设置不同颜色
                    switch (cell.adjacentMines) {
                        case 1: info.styleSheet += "color: blue;"; break;
                        case 2: info.styleSheet += "color: green;"; break;
                        case 3: info.styleSheet += "color: red;"; break;
                        case 4: info.styleSheet += "color: darkblue;"; break;
                        case 5: info.styleSheet += "color: brown;"; break;
                        default: info.styleSheet += "color: black;"; break;
                    }
                }
            }
            m_ui->onCellUpdated(info);  // 通过UI接口更新单个格子
        }
    }
}

// 当游戏结束时，此函数被调用
void GameViewModel::onGameOver(bool victory) {
    if (!m_ui) return;
    QString message;
    bool isLastLevel = (m_currentCampaignLevel == CAMPAIGN_LEVELS.size() - 1);

    if (victory) {
        m_ui->updateStatusLabel("你赢了! :)");
        if (m_currentGameMode == GameMode::Campaign) {
            message = isLastLevel ? "恭喜! 你已完成所有关卡挑战!" : "恭喜! 你通过了本关!";
        } else {
            message = "恭喜! 你成功清除了所有地雷!";
        }
    } else {
        m_ui->updateStatusLabel("你输了! :(");
        message = "砰! 你踩到地雷了!";
    }
    // 调用UI接口显示游戏结束对话框
    m_ui->onShowGameOverDialog(message, m_currentGameMode == GameMode::Campaign, victory, isLastLevel);
}
// GameViewModel.cpp
void GameViewModel::undoLastFlagRequest() {
    m_model.undoLastFlag();
}