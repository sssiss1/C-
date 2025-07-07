/*
 * GameViewModel.h
 *
 * 文件用途：
 * 定义GameViewModel类，这是MVVM中的ViewModel层。
 * 它与View和App层完全解耦，通过信号和槽进行通信。
 * 1. 提供公共槽(public slots)来接收来自View的命令。
 * 2. 监听Model的变化，并将原始数据转换为UI可用的格式。
 * 3. 通过信号(signals)将格式化后的数据和状态变化广播出去。
 */

#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <QObject>
#include <QTimer>
#include <QSize>
#include "../Model/GameModel.h"
#include "../common/GameModes.h"
#include "../common/GameData.h" // 包含共享的数据结构

// 视图模型类，现在是一个纯粹的QObject
class GameViewModel : public QObject {
    Q_OBJECT

public:
    // 构造函数，接收一个对Model的引用
    explicit GameViewModel(GameModel& model, QObject *parent = nullptr);

public slots:
    // 这些是新的公共槽，用于响应来自View的信号
    // / 响应开始一个新游戏的命令（例如，从 StartWindow 选择模式，或在 MainWindow 点击“重新开始”）。
    void startNewGame(GameMode mode, GameDifficulty difficulty);
    // 响应进入下一关的命令（仅在闯关模式下）。
    void startNextCampaignLevel();
    // 响应揭开一个单元格的命令（例如，用户左键点击棋盘格）。
    void revealCellRequest(int row, int col);
    // 响应切换旗帜标记的命令（例如，用户右键点击棋盘格）。
    void toggleFlagRequest(int row, int col);
    // 响应循环切换单元格标记的命令（例如，用户中键点击棋盘格，在'?'和空白间切换）。
    void cycleMarkRequest(int row, int col);
    // 响应请求提示的命令（例如，用户点击“提示”按钮）。
    void hintRequest();
    // 响应加载排行榜数据的命令。
    void loadHighScoresRequest();
    // 响应清空排行榜数据的命令。
    void clearHighScoresRequest();
    // 响应从游戏界面返回主菜单的命令。
    void onRequestReturnToHome(); // 响应返回主页的请求

signals:
    // --- 通知View更新的信号 ---
    // 事件通知绑定：当排行榜数据准备好后，发出此信号，并携带数据。
    void highScoresReady(const QVector<HighScoreItem>& scores);
    // 属性绑定：当棋盘尺寸改变时，发出此信号。View 监听到后会重建UI网格。
    void boardSizeChanged(const QSize& newSize);
    // 属性绑定：当单个单元格的状态（外观、文本等）需要更新时，发出此信号。
    void cellUpdated(const CellUpdateInfo& info);
    // 事件通知绑定：当游戏结束时，发出此信号，通知 View 显示一个结束对话框。参数用于定制对话框内容。
    void showGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel);
    // 属性绑定：当剩余旗帜（或雷数）标签需要更新时，发出此信号
    void flagsLabelUpdated(int flags);
    // 属性绑定：当游戏状态标签（如“进行中...”、“你赢了!”）需要更新时，发出此信号。
    void statusLabelUpdated(const QString& text);
    // 事件通知绑定：通知 View 在状态栏等位置显示一条临时消息（如“旗帜已用完！”）。
    void temporaryMessageShown(const QString& message, int durationMs);
    // 属性绑定：当游戏模式配置完成时，发出此信号，通知 View 根据模式调整其UI（如隐藏/显示“重新开始”按钮）。
    void modeConfigured(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels);
    // 属性绑定：当计时器显示需要更新时，发出此信号。
    void timerDisplayUpdated(int seconds);
    // 属性绑定：当“提示”按钮的可用状态改变时，发出此信号。
    void hintButtonEnabled(bool enabled);

    // --- 通知App层进行应用级操作的信号 ---
    // 事件通知绑定：当需要返回主菜单时，向顶层的 App 类发出请求。
    void returnToHomeRequested();

private slots:
    // 当Model数据变化时，此槽函数被调用，负责发射信号通知View更新
    void onModelChanged();
    // 当Model发出游戏结束信号时，此槽函数被调用
    void onGameOver(bool victory);
    // 当游戏计时器每秒触发时，此槽函数被调用
    void onTimerTick();

private:
    // 内部使用的开始游戏函数，用于处理通用逻辑
    void startGameInternal(const GameDifficulty& difficulty);
    // 当游戏胜利时，保存最佳时间
    void saveBestTime();

    // 持有Model的引用，ViewModel不拥有Model
    GameModel& m_model;

    // 与UI显示相关的状态
    GameMode m_currentGameMode; // 当前的游戏模式。
    GameDifficulty m_currentDifficulty;   // 当前的游戏难度设置。
    int m_currentCampaignLevel = 0;  // 在闯关模式下，记录当前是第几关（从0开始索引）。
    bool m_isHintAvailable = true; // 记录本局游戏中“提示”功能是否还可用。

    // 游戏计时器
    QTimer* m_gameTimer; // 指向 QTimer 对象的指针，用于游戏计时。ViewModel 拥有这个计时器。
    int m_elapsedSeconds = 0;// 记录游戏已进行的秒数。
};

#endif // GAMEVIEWMODEL_H