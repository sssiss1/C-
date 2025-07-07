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
    void startNewGame(GameMode mode, GameDifficulty difficulty);
    void startNextCampaignLevel();
    void revealCellRequest(int row, int col);
    void toggleFlagRequest(int row, int col);
    void cycleMarkRequest(int row, int col);
    void hintRequest();
    void loadHighScoresRequest();
    void clearHighScoresRequest();
    void onRequestReturnToHome(); // 响应返回主页的请求

signals:
    // --- 通知View更新的信号 ---
    void highScoresReady(const QVector<HighScoreItem>& scores);
    void boardSizeChanged(const QSize& newSize);
    void cellUpdated(const CellUpdateInfo& info);
    void showGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel);
    void flagsLabelUpdated(int flags);
    void statusLabelUpdated(const QString& text);
    void temporaryMessageShown(const QString& message, int durationMs);
    void modeConfigured(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels);
    void timerDisplayUpdated(int seconds);
    void hintButtonEnabled(bool enabled);

    // --- 通知App层进行应用级操作的信号 ---
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
    GameMode m_currentGameMode;
    GameDifficulty m_currentDifficulty;
    int m_currentCampaignLevel = 0;
    bool m_isHintAvailable = true;

    // 游戏计时器
    QTimer* m_gameTimer;
    int m_elapsedSeconds = 0;
};

#endif // GAMEVIEWMODEL_H