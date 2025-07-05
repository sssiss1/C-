/*
 * GameViewModel.h
 *
 * 文件用途：
 * 定义GameViewModel类，这是MVVM中的ViewModel层
 * 它是Model和View之间的桥梁，负责：
 * 1. 接收View的命令请求（实现IGameCommands接口），并调用Model执行相应操作
 * 2. 监听Model的状态变化，将纯数据转换为View可以直接使用的显示信息（如文本、颜色）
 * 3. 通过IGameUI接口更新View，驱动UI变化
 * 4. 管理与UI显示相关的状态，如计时器、提示机会等
 */

#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <QObject>
#include <QTimer>
#include "../Model/GameModel.h"
#include "../common/IGameCommands.h"
#include "../common/IGameUI.h"

//视图模型类，继承QObject以使用信号槽，并实现IGameCommands接口来接收命令
class GameViewModel : public QObject, public IGameCommands {
    Q_OBJECT

public:
    //构造函数，接收一个对Model的引用
    explicit GameViewModel(GameModel& model, QObject *parent = nullptr);

    //设置UI接口，由App层在初始化时调用（依赖注入）
    void setUI(IGameUI* ui);
    //设置App命令接口，用于回调App执行窗口切换等操作（依赖注入）
    void setApp(IGameCommands* appCommands);

    //--- IGameCommands接口实现 ---
    //这些命令由View发出，由ViewModel处理

    void startNewGame(GameMode mode, GameDifficulty difficulty) override;
    void startNextCampaignLevel() override;
    void revealCellRequest(int row, int col) override;
    void toggleFlagRequest(int row, int col) override;
    void cycleMarkRequest(int row, int col) override;
    void hintRequest() override;
    void returnToHomeRequest() override;
    void loadHighScoresRequest() override;
    void clearHighScoresRequest() override;

signals:
    //当排行榜数据准备好后，发射此信号，将数据传递给HighScoreDialog
    void highScoresReady(const QVector<HighScoreItem>& scores);

private slots:
    //当Model数据变化时，此槽函数被调用，负责更新整个UI
    void onModelChanged();
    //当Model发出游戏结束信号时，此槽函数被调用
    void onGameOver(bool victory);
    //当游戏计时器每秒触发时，此槽函数被调用
    void onTimerTick();

private:
    //内部使用的开始游戏函数，用于处理通用逻辑
    void startGameInternal(const GameDifficulty& difficulty);
    //当游戏胜利时，保存最佳时间
    void saveBestTime();

    //持有Model的引用，ViewModel不拥有Model
    GameModel& m_model;
    //持有UI接口的指针，ViewModel不拥有View
    IGameUI* m_ui = nullptr;
    //持有App命令接口的指针，用于回调
    IGameCommands* m_appCommands = nullptr;

    //与UI显示相关的状态
    GameMode m_currentGameMode; //当前游戏模式
    GameDifficulty m_currentDifficulty; //当前游戏难度
    int m_currentCampaignLevel = 0; //当前闯关模式的关卡
    bool m_isHintAvailable = true; //提示机会是否可用

    //游戏计时器
    QTimer* m_gameTimer;
    int m_elapsedSeconds = 0; //已用时间（秒）
};

#endif // GAMEVIEWMODEL_H