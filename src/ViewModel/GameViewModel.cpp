/*
 * GameViewModel.cpp
 *
 * 文件用途：
 * 实现GameViewModel类的所有功能。
 * 这是连接Model和View的逻辑核心，但与View完全解耦。
 * 包含将Model数据转换为View显示信息的表示逻辑（Presentation Logic）。
 * 以及处理用户命令、管理UI状态（如计时器）等。
 */

#include "GameViewModel.h"
#include <QSettings>
#include <climits>
#include <algorithm>
#include <QPoint>
#include "../common/GameConfig.h"

//构造函数
GameViewModel::GameViewModel(GameModel& model, QObject *parent)
    : QObject(parent),
      m_model(model), //保存Model的引用
      m_gameTimer(new QTimer(this)) //创建计时器
{
    //连接Model的信号到ViewModel的槽函数
    connect(&m_model, &GameModel::modelChanged, this, &GameViewModel::onModelChanged);
    connect(&m_model, &GameModel::gameOver, this, &GameViewModel::onGameOver);
    //连接计时器的timeout信号到onTimerTick槽函数
    connect(m_gameTimer, &QTimer::timeout, this, &GameViewModel::onTimerTick);
}

// 内部通用的开始游戏逻辑
void GameViewModel::startGameInternal(const GameDifficulty& difficulty) {
    m_currentDifficulty = difficulty; // 保存正确的难度

    emit modeConfigured(m_currentGameMode, m_currentDifficulty,
                       m_currentCampaignLevel + 1, CAMPAIGN_LEVELS.size());

    emit boardSizeChanged(QSize(m_currentDifficulty.cols, m_currentDifficulty.rows));

    m_model.startGame(m_currentDifficulty.rows, m_currentDifficulty.cols, m_currentDifficulty.mines);
}
// 处理开始新游戏的命令
void GameViewModel::startNewGame(GameMode mode, GameDifficulty difficulty) { // difficulty 参数只对自定义模式有意义
    m_currentGameMode = mode;
    m_gameTimer->stop();
    m_elapsedSeconds = 0;
    m_isHintAvailable = true;

    emit timerDisplayUpdated(m_elapsedSeconds);
    emit hintButtonEnabled(m_isHintAvailable);

    // --- 关键修复：根据模式重新确定难度，而不是依赖传入的参数 ---
    GameDifficulty actualDifficulty = difficulty; // 默认使用传入的
    switch (m_currentGameMode) {
        case GameMode::Simple:
            actualDifficulty = EASY_DIFFICULTY;
            break;
        case GameMode::Normal:
            actualDifficulty = NORMAL_DIFFICULTY;
            break;
        case GameMode::Hard:
            actualDifficulty = HARD_DIFFICULTY;
            break;
        case GameMode::Campaign:
            m_currentCampaignLevel = 0;
            actualDifficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel];
            break;
        case GameMode::Custom:
            // 自定义模式，传入的difficulty是有效的，不做任何事
            break;
    }

    // 调用内部函数，使用我们确定的、绝对正确的难度开始游戏
    startGameInternal(actualDifficulty);
}

// 处理开始下一关的命令
void GameViewModel::startNextCampaignLevel() {
    if (m_currentGameMode == GameMode::Campaign) {
        m_currentCampaignLevel++;
        //检查是否还有下一关
        if (m_currentCampaignLevel < CAMPAIGN_LEVELS.size()) {
            //每关重置提示按钮状态
            m_isHintAvailable = true;
            emit hintButtonEnabled(m_isHintAvailable);

            GameDifficulty nextLevelDifficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel];
            //开始下一关
            startGameInternal(nextLevelDifficulty);
        }
    }
}

// 处理揭开格子的请求
void GameViewModel::revealCellRequest(int row, int col) {
    //如果游戏处于准备状态且计时器未启动，则启动计时器
    if (m_model.getGameState() == GameState::Ready && !m_gameTimer->isActive()) {
        m_gameTimer->start(1000); //每秒触发一次
    }
    //将请求转发给Model
    m_model.revealCell(row, col);
}

// 处理标记旗帜的请求
void GameViewModel::toggleFlagRequest(int row, int col) {
    //如果想插旗，但旗帜已经用完
    if (!m_model.getCell(row, col).isFlagged && m_model.getFlagCount() >= m_model.getMineCount()) {
        emit temporaryMessageShown("旗帜已用完! 没有更多雷了!", 2000); //通过信号显示提示
        return;
    }
    //将请求转发给Model
    m_model.flagCell(row, col);
}

// 处理循环标记（问号）的请求
void GameViewModel::cycleMarkRequest(int row, int col) {
    //将请求转发给Model
    m_model.cycleCellMark(row, col);
}

// 处理提示请求
void GameViewModel::hintRequest() {
    if (m_model.getGameState() != GameState::Playing) return; //游戏不在进行中则无效

    if (!m_isHintAvailable) {
        emit temporaryMessageShown("提示机会已使用!", 2000);
        return;
    }

    //提示功能要求用户在棋盘上标记一个“?”
    if (m_model.getQuestionMarkCount() != 1) {
        emit temporaryMessageShown("请在棋盘上标记一个“?”作为提示位置!", 3000);
        return;
    }

    //找到问号的位置
    QPoint pos = m_model.findFirstQuestionMark();
    if (pos.x() != -1) {
        //让Model处理这个提示，并返回该位置是否是雷
        bool wasMine = m_model.resolveHint(pos.y(), pos.x());

        //根据结果显示不同的消息
        if (wasMine) {
            emit temporaryMessageShown("提示机会已使用：本格有雷", 3000);
        } else {
            emit temporaryMessageShown("提示机会已使用：本格无雷", 3000);
        }

        m_isHintAvailable = false; //消耗提示机会
        emit hintButtonEnabled(false); //禁用提示按钮
    }
}

// 响应返回主页的请求
void GameViewModel::onRequestReturnToHome() {
    m_gameTimer->stop(); //停止计时器
    emit returnToHomeRequested(); // 发射信号通知App层
}


// 处理加载排行榜的请求
void GameViewModel::loadHighScoresRequest() {
    QVector<HighScoreItem> scores;
    QSettings settings("MyCompany", "MineSweeper");
    settings.beginGroup("bestTime");

    auto addEntry = [&](const QString& modeName, const GameDifficulty& diff, const QString& key){
        int time = settings.value(key, INT_MAX).toInt();
        if (time != INT_MAX) {
            QString difficultyText = "-";
            if (modeName != "闯关模式") {
                difficultyText = QString("%1x%2, %3雷").arg(diff.rows).arg(diff.cols).arg(diff.mines);
            }
            scores.append({modeName, difficultyText, QString::number(time) + " 秒"});
        }
    };

    addEntry("简单模式", EASY_DIFFICULTY, "Simple");
    addEntry("普通模式", NORMAL_DIFFICULTY, "Normal");
    addEntry("困难模式", HARD_DIFFICULTY, "Hard");
    addEntry("闯关模式", {}, "Campaign");

    QList<QPair<int, QString>> customScores;
    for (const QString& key : settings.childKeys()) {
        if (key.startsWith("Custom_")) {
            customScores.append({settings.value(key, INT_MAX).toInt(), key});
        }
    }
    std::sort(customScores.begin(), customScores.end());

    int count = 0;
    for (const auto& scorePair : customScores) {
        if (count >= MAX_CUSTOM_SCORES) break;
        QStringList parts = scorePair.second.split('_');
        if (parts.size() == 4) {
            GameDifficulty diff = {parts[1].toInt(), parts[2].toInt(), parts[3].toInt()};
            scores.append({"自定义模式", QString("%1x%2, %3雷").arg(diff.rows).arg(diff.cols).arg(diff.mines),
                          QString::number(scorePair.first) + " 秒"});
            count++;
        }
    }

    settings.endGroup();
    emit highScoresReady(scores);
}

// 处理清空排行榜的请求
void GameViewModel::clearHighScoresRequest() {
    QSettings settings("MyCompany", "MineSweeper");
    settings.remove("bestTime");
    loadHighScoresRequest();
}

// 当Model数据变化时，此槽函数被调用
void GameViewModel::onModelChanged() {
    switch(m_model.getGameState()){
        case GameState::Playing: emit statusLabelUpdated("游戏中..."); break;
        case GameState::Ready: emit statusLabelUpdated("准备开始"); break;
        default: break;
    }
    emit flagsLabelUpdated(m_model.getMineCount() - m_model.getFlagCount());

    // 只有在游戏开始或棋盘尺寸变化时才发射 boardSizeChanged
    // 在这里每次都发射会导致不必要的棋盘重建
    // 我们将 boardSizeChanged 的发射移到 modeConfigured 信号之后

    for (int r = 0; r < m_model.getRows(); ++r) {
        for (int c = 0; c < m_model.getCols(); ++c) {
            const Cell& cell = m_model.getCell(r, c);
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
            emit cellUpdated(info);
        }
    }
}

// 当游戏结束时，此槽函数被调用
void GameViewModel::onGameOver(bool victory) {
    m_gameTimer->stop();
    emit hintButtonEnabled(false);

    bool isLastLevel = (m_currentGameMode == GameMode::Campaign &&
                        m_currentCampaignLevel == CAMPAIGN_LEVELS.size() - 1);

    if (victory && (m_currentGameMode != GameMode::Campaign || isLastLevel)) {
        saveBestTime();
    }

    QString message;
    if (victory) {
        emit statusLabelUpdated("你赢了! :)");
        message = (m_currentGameMode == GameMode::Campaign)
                  ? (isLastLevel ? "恭喜! 你已完成所有关卡挑战!" : "恭喜! 你通过了本关!")
                  : "恭喜! 你成功清除了所有地雷!";
    } else {
        emit statusLabelUpdated("你输了! :(");
        message = "砰! 你踩到地雷了!";
    }
    emit showGameOverDialog(message, m_currentGameMode == GameMode::Campaign, victory, isLastLevel);
}

// 计时器每秒触发
void GameViewModel::onTimerTick() {
    m_elapsedSeconds++;
    emit timerDisplayUpdated(m_elapsedSeconds);
}

// 保存最佳时间
void GameViewModel::saveBestTime() {
    QSettings settings("MyCompany", "MineSweeper");
    QString key;
    switch (m_currentGameMode) {
        case GameMode::Simple:   key = "Simple"; break;
        case GameMode::Normal:   key = "Normal"; break;
        case GameMode::Hard:     key = "Hard"; break;
        case GameMode::Campaign: key = "Campaign"; break;
        case GameMode::Custom:
            key = QString("Custom_%1_%2_%3").arg(m_currentDifficulty.rows).arg(m_currentDifficulty.cols).arg(m_currentDifficulty.mines);
            settings.beginGroup("bestTime");
            QStringList customKeys;
            for(const auto& k : settings.childKeys()) if(k.startsWith("Custom_")) customKeys.append(k);
            if(!customKeys.contains(key) && customKeys.size() >= MAX_CUSTOM_SCORES) {
                settings.endGroup();
                return;
            }
            settings.endGroup();
            break;
    }
    QString fullKey = "bestTime/" + key;
    if (m_elapsedSeconds < settings.value(fullKey, INT_MAX).toInt()) {
        settings.setValue(fullKey, m_elapsedSeconds);
    }
}