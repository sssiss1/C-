/*
 * GameViewModel.cpp
 *
 * 文件用途：
 * 实现GameViewModel类的所有功能
 * 这是连接Model和View的逻辑核心
 * 包含将Model数据转换为View显示信息的表示逻辑（Presentation Logic）
 * 以及处理用户命令、管理UI状态（如计时器）等
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

//依赖注入：设置UI接口
void GameViewModel::setUI(IGameUI* ui) { m_ui = ui; }
//依赖注入：设置App命令接口
void GameViewModel::setApp(IGameCommands* appCommands) { m_appCommands = appCommands; }

//内部通用的开始游戏逻辑
void GameViewModel::startGameInternal(const GameDifficulty& difficulty) {
    m_currentDifficulty = difficulty;
    if (m_ui) {
        //通过UI接口配置主窗口以适应当前模式和难度
        m_ui->configureForMode(m_currentGameMode, m_currentDifficulty, m_currentCampaignLevel + 1, CAMPAIGN_LEVELS.size());
    }
    //调用Model的startGame来初始化游戏核心数据
    m_model.startGame(m_currentDifficulty.rows, m_currentDifficulty.cols, m_currentDifficulty.mines);
}

//处理开始新游戏的命令
void GameViewModel::startNewGame(GameMode mode, GameDifficulty difficulty) {
    m_currentGameMode = mode;
    m_gameTimer->stop(); //停止计时器
    m_elapsedSeconds = 0; //重置时间
    m_isHintAvailable = true; //重置提示机会

    if (m_ui) {
        //通过UI接口更新视图
        m_ui->updateTimerDisplay(m_elapsedSeconds);
        m_ui->setHintButtonEnabled(m_isHintAvailable);
    }

    if (m_currentGameMode == GameMode::Campaign) {
        m_currentCampaignLevel = 0; //闯关模式从第一关开始
        difficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel]; //获取第一关的难度设置
    }
    //调用内部函数开始游戏
    startGameInternal(difficulty);
}

//处理开始下一关的命令
void GameViewModel::startNextCampaignLevel() {
    if (m_currentGameMode == GameMode::Campaign) {
        m_currentCampaignLevel++;
        //检查是否还有下一关
        if (m_currentCampaignLevel < CAMPAIGN_LEVELS.size()) {
            if (m_ui) {
                //每关重置提示按钮状态（如果设计如此）
                m_ui->setHintButtonEnabled(m_isHintAvailable);
            }
            GameDifficulty nextLevelDifficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel];
            //开始下一关
            startGameInternal(nextLevelDifficulty);
        }
    }
}

//处理揭开格子的请求
void GameViewModel::revealCellRequest(int row, int col) {
    //如果游戏处于准备状态且计时器未启动，则启动计时器
    if (m_model.getGameState() == GameState::Ready && !m_gameTimer->isActive()) {
        m_gameTimer->start(1000); //每秒触发一次
    }
    //将请求转发给Model
    m_model.revealCell(row, col);
}

//处理标记旗帜的请求
void GameViewModel::toggleFlagRequest(int row, int col) {
    //如果想插旗，但旗帜已经用完
    if (!m_model.getCell(row, col).isFlagged && m_model.getFlagCount() >= m_model.getMineCount()) {
        if (m_ui) m_ui->showTemporaryMessage("旗帜已用完! 没有更多雷了!", 2000); //通过UI接口显示提示
        return;
    }
    //将请求转发给Model
    m_model.flagCell(row, col);
}

//处理循环标记（问号）的请求
void GameViewModel::cycleMarkRequest(int row, int col) {
    //将请求转发给Model
    m_model.cycleCellMark(row, col);
}

//处理提示请求
void GameViewModel::hintRequest() {
    if (!m_ui || m_model.getGameState() != GameState::Playing) return; //游戏不在进行中则无效

    if (!m_isHintAvailable) {
        m_ui->showTemporaryMessage("提示机会已使用!", 2000);
        return;
    }

    //提示功能要求用户在棋盘上标记一个“?”
    if (m_model.getQuestionMarkCount() != 1) {
        m_ui->showTemporaryMessage("请在棋盘上标记一个“?”作为提示位置!", 3000);
        return;
    }

    //找到问号的位置
    QPoint pos = m_model.findFirstQuestionMark();
    if (pos.x() != -1) {
        //让Model处理这个提示，并返回该位置是否是雷
        bool wasMine = m_model.resolveHint(pos.y(), pos.x());

        //根据结果显示不同的消息
        if (wasMine) {
            m_ui->showTemporaryMessage("提示机会已使用：本格有雷", 3000);
        } else {
            m_ui->showTemporaryMessage("提示机会已使用：本格无雷", 3000);
        }

        m_isHintAvailable = false; //消耗提示机会
        m_ui->setHintButtonEnabled(false); //禁用提示按钮
    }
}

//处理返回主页的请求
void GameViewModel::returnToHomeRequest() {
    m_gameTimer->stop(); //停止计时器
    //通过App命令接口回调App，执行窗口切换
    if (m_appCommands) m_appCommands->returnToHomeRequest();
}

//处理加载排行榜的请求
void GameViewModel::loadHighScoresRequest() {
    QVector<HighScoreItem> scores;
    QSettings settings("MyCompany", "MineSweeper"); //使用QSettings进行持久化存储
    settings.beginGroup("bestTime"); //进入"bestTime"分组

    //一个辅助lambda函数，用于从settings中读取条目并添加到scores列表
    auto addEntry = [&](const QString& modeName, const GameDifficulty& diff, const QString& key){
        int time = settings.value(key, INT_MAX).toInt(); //读取时间，默认为最大整数
        if (time != INT_MAX) { //如果存在记录
            QString difficultyText = "-";
            if (modeName != "闯关模式") {
                difficultyText = QString("%1x%2, %3雷").arg(diff.rows).arg(diff.cols).arg(diff.mines);
            }
            scores.append({modeName, difficultyText, QString::number(time) + " 秒"});
        }
    };

    //加载预设模式的成绩
    addEntry("简单模式", EASY_DIFFICULTY, "Simple");
    addEntry("普通模式", NORMAL_DIFFICULTY, "Normal");
    addEntry("困难模式", HARD_DIFFICULTY, "Hard");
    addEntry("闯关模式", {}, "Campaign");

    //加载自定义模式的成绩
    QList<QPair<int, QString>> customScores;
    //遍历所有以"Custom_"开头的键
    for (const QString& key : settings.childKeys()) {
        if (key.startsWith("Custom_")) {
            customScores.append({settings.value(key, INT_MAX).toInt(), key});
        }
    }
    std::sort(customScores.begin(), customScores.end()); //按时间排序

    int count = 0;
    for (const auto& scorePair : customScores) {
        if (count >= MAX_CUSTOM_SCORES) break; //只显示前N条记录
        QStringList parts = scorePair.second.split('_'); //从键名解析出难度
        if (parts.size() == 4) {
            GameDifficulty diff = {parts[1].toInt(), parts[2].toInt(), parts[3].toInt()};
            scores.append({"自定义模式", QString("%1x%2, %3雷").arg(diff.rows).arg(diff.cols).arg(diff.mines), QString::number(scorePair.first) + " 秒"});
            count++;
        }
    }

    settings.endGroup();
    emit highScoresReady(scores); //发射信号，将准备好的数据传出
}

//处理清空排行榜的请求
void GameViewModel::clearHighScoresRequest() {
    QSettings settings("MyCompany", "MineSweeper");
    settings.remove("bestTime"); //删除整个"bestTime"分组
    loadHighScoresRequest(); //重新加载（此时会得到一个空列表并更新UI）
}

//当Model数据变化时，此槽函数被调用
void GameViewModel::onModelChanged() {
    if (!m_ui) return;
    //根据游戏状态更新状态标签
    switch(m_model.getGameState()){
        case GameState::Playing: m_ui->updateStatusLabel("游戏中..."); break;
        case GameState::Ready: m_ui->updateStatusLabel("准备开始"); break;
        default: break;
    }
    //更新剩余旗帜（或地雷）数量的标签
    m_ui->updateFlagsLabel(m_model.getMineCount() - m_model.getFlagCount());

    //--- 核心表示逻辑 ---
    //遍历Model中的每一个Cell
    for (int r = 0; r < m_model.getRows(); ++r) {
        for (int c = 0; c < m_model.getCols(); ++c) {
            const Cell& cell = m_model.getCell(r, c);
            //创建一个用于UI更新的信息包，并设置默认值（未揭开的格子）
            CellUpdateInfo info{r, c, "", "background-color: #c0c0c0;", true};

            //根据Cell的不同状态，将原始数据转换为UI表示
            if (m_model.getGameState() == GameState::Lost && cell.isMine) {
                //游戏失败时，显示所有地雷
                info.text = "💣"; info.styleSheet = "background-color: red;";
            } else if (cell.isFlagged) {
                //显示旗帜
                info.text = "🚩";
            } else if (cell.isQuestionMark) {
                //显示问号
                info.text = "❓";
            } else if (cell.isRevealed) {
                //处理已揭开的格子
                info.styleSheet = "background-color: #e0e0e0; border: 1px solid #808080;";
                if (cell.adjacentMines > 0) {
                    //显示周围地雷数
                    info.text = QString::number(cell.adjacentMines);
                    //根据数字设置不同的颜色
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
            //通过UI接口将转换后的信息包发给View进行更新
            m_ui->onCellUpdated(info);
        }
    }
}

//当游戏结束时，此槽函数被调用
void GameViewModel::onGameOver(bool victory) {
    if (!m_ui) return;
    m_gameTimer->stop(); //停止计时器
    m_ui->setHintButtonEnabled(false); //游戏结束，禁用提示按钮

    //判断是否是闯关模式的最后一关
    bool isLastLevel = (m_currentCampaignLevel == CAMPAIGN_LEVELS.size() - 1);

    //如果胜利，并且不是闯关模式中途，则保存最佳时间
    if (victory && (m_currentGameMode != GameMode::Campaign || isLastLevel)) {
        saveBestTime();
    }

    QString message;
    if (victory) {
        m_ui->updateStatusLabel("你赢了! :)");
        //根据不同模式生成不同的胜利消息
        message = (m_currentGameMode == GameMode::Campaign)
                  ? (isLastLevel ? "恭喜! 你已完成所有关卡挑战!" : "恭喜! 你通过了本关!")
                  : "恭喜! 你成功清除了所有地雷!";
    } else {
        m_ui->updateStatusLabel("你输了! :(");
        message = "砰! 你踩到地雷了!";
    }
    //通过UI接口通知View显示游戏结束对话框，并传递所需信息
    m_ui->onShowGameOverDialog(message, m_currentGameMode == GameMode::Campaign, victory, isLastLevel);
}

//计时器每秒触发
void GameViewModel::onTimerTick() {
    m_elapsedSeconds++;
    if (m_ui) m_ui->updateTimerDisplay(m_elapsedSeconds); //更新UI上的时间显示
}

//保存最佳时间
void GameViewModel::saveBestTime() {
    QSettings settings("MyCompany", "MineSweeper");
    QString key;
    //根据当前游戏模式确定在QSettings中使用的键
    switch (m_currentGameMode) {
        case GameMode::Simple:   key = "Simple"; break;
        case GameMode::Normal:   key = "Normal"; break;
        case GameMode::Hard:     key = "Hard"; break;
        case GameMode::Campaign: key = "Campaign"; break;
        case GameMode::Custom:
            //自定义模式的键包含其难度参数，以区分不同的自定义设置
            key = QString("Custom_%1_%2_%3").arg(m_currentDifficulty.rows).arg(m_currentDifficulty.cols).arg(m_currentDifficulty.mines);
            //对于自定义模式，需要检查是否超过了最大保存数量
            settings.beginGroup("bestTime");
            QStringList customKeys;
            for(const auto& k : settings.childKeys()) if(k.startsWith("Custom_")) customKeys.append(k);
            //如果当前成绩不存在且列表已满，则不保存
            if(!customKeys.contains(key) && customKeys.size() >= MAX_CUSTOM_SCORES) {
                settings.endGroup();
                return; //不保存
            }
            settings.endGroup();
            break;
    }
    QString fullKey = "bestTime/" + key; //完整的键路径
    //只有当本次时间优于记录时才更新
    if (m_elapsedSeconds < settings.value(fullKey, INT_MAX).toInt()) {
        settings.setValue(fullKey, m_elapsedSeconds);
    }
}