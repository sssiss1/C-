/*
 * GameViewModel.cpp
 *
 * 文件用途：
 * 实现GameViewModel类的所有功能。
 * 这是连接Model和View的逻辑核心，但与View完全解耦。
 * 包含将Model数据转换为View显示信息的表示逻辑（Presentation Logic）。
 * 以及处理用户命令、管理UI状态（如计时器）等。
 */

// 包含对应的头文件，这是C++源文件的标准做法。
#include "GameViewModel.h"
// 包含QSettings，用于读写应用程序的设置，这里主要用来持久化存储排行榜数据。
#include <QSettings>
// 包含<climits>，主要为了使用INT_MAX，作为一个足够大的初始值来比较和存储最短时间。
#include <climits>
// 包含<algorithm>，用于对自定义模式的得分进行排序。
#include <algorithm>
// 包含QPoint，用于方便地表示和处理二维坐标（例如问号的位置）。
#include <QPoint>
// 包含游戏配置文件，这里主要是为了获取自定义模式排行榜的最大条目数（MAX_CUSTOM_SCORES）。
#include "../common/GameConfig.h"

// GameViewModel的构造函数实现。
GameViewModel::GameViewModel(GameModel& model, QObject *parent)
    : QObject(parent), // 调用基类QObject的构造函数，并传递parent指针，以便加入Qt对象树。
      m_model(model), // 使用初始化列表，将传入的model引用赋值给成员变量m_model。
      m_gameTimer(new QTimer(this)) // 创建一个新的QTimer对象，并将其父对象设置为当前ViewModel实例(`this`)，这样当ViewModel被销毁时，QTimer也会被自动删除。
{
    // 在这里建立ViewModel和其内部成员（Model, Timer）之间的信号-槽连接。

    // 当Model的数据发生变化（发出modelChanged信号）时，调用本类的onModelChanged槽函数进行处理。
    connect(&m_model, &GameModel::modelChanged, this, &GameViewModel::onModelChanged);
    // 当Model判断游戏结束（发出gameOver信号）时，调用本类的onGameOver槽函数。
    connect(&m_model, &GameModel::gameOver, this, &GameViewModel::onGameOver);
    // 当游戏计时器每秒触发一次（发出timeout信号）时，调用本类的onTimerTick槽函数来更新时间。
    connect(m_gameTimer, &QTimer::timeout, this, &GameViewModel::onTimerTick);
}

// 内部开始游戏的辅助函数。
void GameViewModel::startGameInternal(const GameDifficulty& difficulty) {
    // 保存当前游戏难度设置到成员变量中。
    m_currentDifficulty = difficulty;

    // 发出modeConfigured信号，通知View根据当前模式和难度配置UI。
    // `m_currentCampaignLevel + 1` 是为了给用户显示从1开始的关卡号。
    emit modeConfigured(m_currentGameMode, m_currentDifficulty,
                       m_currentCampaignLevel + 1, CAMPAIGN_LEVELS.size());

    // 发出boardSizeChanged信号，通知View根据新的行列数重建游戏棋盘。
    emit boardSizeChanged(QSize(m_currentDifficulty.cols, m_currentDifficulty.rows));

    // 调用Model的startGame函数，用最终确定的参数去初始化核心游戏逻辑。
    m_model.startGame(m_currentDifficulty.rows, m_currentDifficulty.cols, m_currentDifficulty.mines);
}

// 公共槽：处理来自View的“开始新游戏”命令。
void GameViewModel::startNewGame(GameMode mode, GameDifficulty difficulty) {
    // 更新当前游戏模式。
    m_currentGameMode = mode;
    // 停止计时器，以防上一局游戏正在进行。
    m_gameTimer->stop();
    // 重置计时秒数为0。
    m_elapsedSeconds = 0;
    // 重置提示机会为可用。
    m_isHintAvailable = true;

    // 发射信号，立即将计时器显示更新为0。
    emit timerDisplayUpdated(m_elapsedSeconds);
    // 发射信号，启用提示按钮。
    emit hintButtonEnabled(m_isHintAvailable);

    // 根据传入的游戏模式，确定实际的游戏难度。
    // 这是重要的逻辑，确保了无论View如何调用，ViewModel都会使用正确的预设难度。
    GameDifficulty actualDifficulty = difficulty; // 先默认使用传入的difficulty（对自定义模式有效）。
    switch (m_currentGameMode) {
        case GameMode::Simple: // 如果是简单模式
            actualDifficulty = EASY_DIFFICULTY; // 使用预定义的简单难度。
            break;
        case GameMode::Normal: // 如果是普通模式
            actualDifficulty = NORMAL_DIFFICULTY; // 使用预定义的普通难度。
            break;
        case GameMode::Hard: // 如果是困难模式
            actualDifficulty = HARD_DIFFICULTY; // 使用预定义的困难难度。
            break;
        case GameMode::Campaign: // 如果是闯关模式
            m_currentCampaignLevel = 0; // 重置关卡到第一关。
            actualDifficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel]; // 获取第一关的难度配置。
            break;
        case GameMode::Custom: // 如果是自定义模式
            // 传入的difficulty参数是有效的，所以什么都不做，直接使用。
            break;
    }

    // 调用内部辅助函数，使用最终确定的难度来开始游戏。
    startGameInternal(actualDifficulty);
}

// 公共槽：处理来自View的“进入下一关”命令。
void GameViewModel::startNextCampaignLevel() {
    // 确认当前是在闯关模式下。
    if (m_currentGameMode == GameMode::Campaign) {
        // 关卡数加一。
        m_currentCampaignLevel++;
        // 检查新的关卡是否在预定义的关卡列表范围内。
        if (m_currentCampaignLevel < CAMPAIGN_LEVELS.size()) {
            // 重置提示机会，让玩家每关都有一次提示。
            m_isHintAvailable = true;
            // 发射信号，启用提示按钮。
            emit hintButtonEnabled(m_isHintAvailable);

            // 获取下一关的难度配置。
            GameDifficulty nextLevelDifficulty = CAMPAIGN_LEVELS[m_currentCampaignLevel];
            // 调用内部辅助函数开始下一关。
            startGameInternal(nextLevelDifficulty);
        }
    }
}

// 公共槽：处理来自View的“揭开单元格”命令。
void GameViewModel::revealCellRequest(int row, int col) {
    // 检查游戏是否处于“准备”状态且计时器未启动。这通常是玩家的第一次点击。
    if (m_model.getGameState() == GameState::Ready && !m_gameTimer->isActive()) {
        // 如果是，启动计时器，每1000毫秒（1秒）触发一次。
        m_gameTimer->start(1000);
    }
    // 将请求直接转发给Model层去处理核心的游戏逻辑。
    m_model.revealCell(row, col);
}

// 公共槽：处理来自View的“切换旗帜”命令。
void GameViewModel::toggleFlagRequest(int row, int col) {
    // 这是一个业务逻辑检查：如果玩家试图放置一个新旗帜，但已放置的旗帜数已达到雷数上限。
    if (!m_model.getCell(row, col).isFlagged && m_model.getFlagCount() >= m_model.getMineCount()) {
        // 发出事件通知信号，让View显示一条临时消息。ViewModel不直接创建UI。
        emit temporaryMessageShown("旗帜已用完! 没有更多雷了!", 2000);
        return; // 提前返回，不执行后续操作。
    }
    // 将请求转发给Model层。
    m_model.flagCell(row, col);
}

// 公共槽：处理来自View的“循环标记”命令（？号）。
void GameViewModel::cycleMarkRequest(int row, int col) {
    // 将请求转发给Model层。
    m_model.cycleCellMark(row, col);
}

// 公共槽：处理来自View的“提示”命令。
void GameViewModel::hintRequest() {
    // 业务逻辑检查：如果游戏不在进行中（例如已结束或未开始），则提示无效。
    if (m_model.getGameState() != GameState::Playing) return;

    // 业务逻辑检查：如果本局的提示机会已经用完。
    if (!m_isHintAvailable) {
        // 发出事件通知，告知用户。
        emit temporaryMessageShown("提示机会已使用!", 2000);
        return;
    }

    // 业务逻辑检查：提示功能的使用规则是，棋盘上必须且只能有一个问号标记。
    if (m_model.getQuestionMarkCount() != 1) {
        // 发出事件通知，引导用户如何使用。
        emit temporaryMessageShown("请在棋盘上标记一个“?”作为提示位置!", 3000);
        return;
    }

    // 调用Model查找那个问号的位置。
    QPoint pos = m_model.findFirstQuestionMark();
    // 确认找到了有效位置（x不为-1）。
    if (pos.x() != -1) {
        // 调用Model的核心提示逻辑，Model会揭开或标记该格，并返回它之前是否是雷。
        bool wasMine = m_model.resolveHint(pos.y(), pos.x());

        // 根据Model返回的结果，发出不同的事件通知。
        if (wasMine) {
            emit temporaryMessageShown("提示机会已使用：本格有雷", 3000);
        } else {
            emit temporaryMessageShown("提示机会已使用：本格无雷", 3000);
        }

        // 消耗掉提示机会。
        m_isHintAvailable = false;
        // 发射信号，禁用提示按钮。
        emit hintButtonEnabled(false);
    }
}

// 公共槽：响应从游戏界面返回主页的请求。
void GameViewModel::onRequestReturnToHome() {
    // 停止游戏计时器，防止在主菜单界面后台计时。
    m_gameTimer->stop();
    // 发出信号通知App层执行窗口切换。ViewModel只提请求，不执行。
    emit returnToHomeRequested();
}

// 公共槽：处理加载排行榜的请求。
void GameViewModel::loadHighScoresRequest() {
    // 创建一个空的QVector用于存储要发给View的排行榜条目。
    QVector<HighScoreItem> scores;
    // 创建QSettings对象，指定公司和应用名，用于定位配置文件。
    QSettings settings("MyCompany", "MineSweeper");
    // 进入名为"bestTime"的组，方便管理所有时间记录。
    settings.beginGroup("bestTime");

    // 定义一个Lambda表达式，用于简化从QSettings读取数据并添加到scores向量的过程。
    auto addEntry = [&](const QString& modeName, const GameDifficulty& diff, const QString& key){
        // 读取指定key的值，如果不存在，则返回INT_MAX。
        int time = settings.value(key, INT_MAX).toInt();
        // 如果时间不是INT_MAX，说明该模式有记录。
        if (time != INT_MAX) {
            QString difficultyText = "-"; // 默认难度文本。
            // 闯关模式不显示具体难度。
            if (modeName != "闯关模式") {
                difficultyText = QString("%1x%2, %3雷").arg(diff.rows).arg(diff.cols).arg(diff.mines);
            }
            // 将格式化好的条目追加到scores向量中。
            scores.append({modeName, difficultyText, QString::number(time) + " 秒"});
        }
    };

    // 使用上面定义的Lambda表达式，依次添加预设模式的排行榜记录。
    addEntry("简单模式", EASY_DIFFICULTY, "Simple");
    addEntry("普通模式", NORMAL_DIFFICULTY, "Normal");
    addEntry("困难模式", HARD_DIFFICULTY, "Hard");
    addEntry("闯关模式", {}, "Campaign");

    // 处理自定义模式的排行榜。
    QList<QPair<int, QString>> customScores; // 创建一个列表来存储（时间，key）对，以便排序。
    // 遍历"bestTime"组下的所有键。
    for (const QString& key : settings.childKeys()) {
        // 如果键名以"Custom_"开头，说明是自定义模式的记录。
        if (key.startsWith("Custom_")) {
            // 将（时间，key）对添加到列表中。
            customScores.append({settings.value(key, INT_MAX).toInt(), key});
        }
    }
    // 使用std::sort对自定义模式的记录按时间升序排序。
    std::sort(customScores.begin(), customScores.end());

    int count = 0; // 计数器，用于限制显示的自定义记录数量。
    // 遍历排序后的自定义记录。
    for (const auto& scorePair : customScores) {
        // 如果已达到最大显示数量，则跳出循环。
        if (count >= MAX_CUSTOM_SCORES) break;
        // 分割key字符串（例如 "Custom_9_9_10"）来解析出难度参数。
        QStringList parts = scorePair.second.split('_');
        if (parts.size() == 4) { // 确认格式正确
            // 从分割后的部分创建GameDifficulty结构体。
            GameDifficulty diff = {parts[1].toInt(), parts[2].toInt(), parts[3].toInt()};
            // 将自定义模式的记录添加到scores向量中。
            scores.append({"自定义模式", QString("%1x%2, %3雷").arg(diff.rows).arg(diff.cols).arg(diff.mines),
                          QString::number(scorePair.first) + " 秒"});
            count++; // 计数器加一。
        }
    }

    // 结束对"bestTime"组的操作。
    settings.endGroup();
    // 发出信号，将准备好的排行榜数据发送给View。
    emit highScoresReady(scores);
}

// 公共槽：处理清空排行榜的请求。
void GameViewModel::clearHighScoresRequest() {
    // 创建QSettings对象。
    QSettings settings("MyCompany", "MineSweeper");
    // 直接移除整个"bestTime"组，清空所有记录。
    settings.remove("bestTime");
    // 重新加载一次排行榜（此时会加载一个空的列表），并通知View更新。
    loadHighScoresRequest();
}

// 私有槽：当Model数据发生变化时被调用。
void GameViewModel::onModelChanged() {
    // 根据Model的当前游戏状态，更新View的状态标签。
    switch(m_model.getGameState()){
        case GameState::Playing: emit statusLabelUpdated("游戏中..."); break;
        case GameState::Ready: emit statusLabelUpdated("准备开始"); break;
        default: break; // 其他状态（Won, Lost）在onGameOver中处理。
    }
    // 发射信号，更新剩余旗帜/雷数标签。计算方式为总雷数减去已插旗数。
    emit flagsLabelUpdated(m_model.getMineCount() - m_model.getFlagCount());

    // 这是一个循环，遍历棋盘上的每一个单元格。
    for (int r = 0; r < m_model.getRows(); ++r) {
        for (int c = 0; c < m_model.getCols(); ++c) {
            // 从Model获取指定位置单元格的原始数据。
            const Cell& cell = m_model.getCell(r, c);
            // 创建一个CellUpdateInfo结构体，用于封装要发送给View的显示信息。
            // 默认设置为一个未被点击的普通格子的外观。
            CellUpdateInfo info{r, c, "", "background-color: #c0c0c0;", true};

            // 这是表示逻辑（Presentation Logic）的核心部分：
            // 将Model的原始状态（isMine, isFlagged, adjacentMines等）转换为View的具体显示属性（文本，样式表）。

            // 如果游戏失败且当前格子是雷，显示炸弹。
            if (m_model.getGameState() == GameState::Lost && cell.isMine) {
                info.text = "💣";
                info.styleSheet = "background-color: red;";
            } else if (cell.isFlagged) { // 如果格子被标记为旗帜
                info.text = "🚩";
            } else if (cell.isQuestionMark) { // 如果格子被标记为问号
                info.text = "❓";
            } else if (cell.isRevealed) { // 如果格子已被揭开
                info.styleSheet = "background-color: #e0e0e0; border: 1px solid #808080;"; // 设置为已揭开的样式
                if (cell.adjacentMines > 0) { // 如果周围有雷
                    info.text = QString::number(cell.adjacentMines); // 显示数字
                    // 根据数字设置不同的颜色
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
            // 发出cellUpdated信号，将这个格子的新外观信息发送给View。
            emit cellUpdated(info);
        }
    }
}

// 私有槽：当游戏结束时被调用。
void GameViewModel::onGameOver(bool victory) {
    // 停止计时器。
    m_gameTimer->stop();
    // 游戏结束，禁用提示按钮。
    emit hintButtonEnabled(false);

    // 判断是否是闯关模式的最后一关。
    bool isLastLevel = (m_currentGameMode == GameMode::Campaign &&
                        m_currentCampaignLevel == CAMPAIGN_LEVELS.size() - 1);

    // 如果游戏胜利，并且（不是闯关模式，或者是闯关模式的最后一关），则保存最佳时间。
    if (victory && (m_currentGameMode != GameMode::Campaign || isLastLevel)) {
        saveBestTime();
    }

    // 准备要显示在游戏结束对话框中的消息。
    QString message;
    if (victory) { // 如果胜利
        emit statusLabelUpdated("你赢了! :)"); // 更新主窗口的状态标签
        // 根据是否是闯关模式生成不同的胜利消息。
        message = (m_currentGameMode == GameMode::Campaign)
                  ? (isLastLevel ? "恭喜! 你已完成所有关卡挑战!" : "恭喜! 你通过了本关!")
                  : "恭喜! 你成功清除了所有地雷!";
    } else { // 如果失败
        emit statusLabelUpdated("你输了! :("); // 更新主窗口的状态标签
        message = "砰! 你踩到地雷了!";
    }
    // 发出事件通知信号，请求View显示游戏结束对话框，并传递所有必要信息。
    emit showGameOverDialog(message, m_currentGameMode == GameMode::Campaign, victory, isLastLevel);
}

// 私有槽：计时器每秒触发时调用。
void GameViewModel::onTimerTick() {
    // 已用秒数加一。
    m_elapsedSeconds++;
    // 发射信号，通知View更新计时器显示。
    emit timerDisplayUpdated(m_elapsedSeconds);
}

// 私有辅助函数：保存最佳时间。
void GameViewModel::saveBestTime() {
    // 创建QSettings对象。
    QSettings settings("MyCompany", "MineSweeper");
    // 声明用于存储设置key的字符串。
    QString key;
    // 根据当前游戏模式确定要使用的key。
    switch (m_currentGameMode) {
        case GameMode::Simple:   key = "Simple"; break;
        case GameMode::Normal:   key = "Normal"; break;
        case GameMode::Hard:     key = "Hard"; break;
        case GameMode::Campaign: key = "Campaign"; break;
        case GameMode::Custom:
            // 自定义模式的key由其难度参数动态生成，以区分不同的自定义设置。
            key = QString("Custom_%1_%2_%3").arg(m_currentDifficulty.rows).arg(m_currentDifficulty.cols).arg(m_currentDifficulty.mines);

            // 检查自定义模式记录是否已满，如果满了且当前记录不存在，则不保存。
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
    // 构造完整的设置路径，例如 "bestTime/Simple"。
    QString fullKey = "bestTime/" + key;
    // 如果当前游戏时间小于已保存的最佳时间（或未保存过），则更新记录。
    if (m_elapsedSeconds < settings.value(fullKey, INT_MAX).toInt()) {
        settings.setValue(fullKey, m_elapsedSeconds);
    }
}