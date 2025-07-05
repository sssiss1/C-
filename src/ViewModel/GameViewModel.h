#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <QObject>
#include "../Model/GameModel.h"
#include "../common/IGameCommands.h"
#include "../common/IGameUI.h"

// GameViewModel是Model和View之间的桥梁
// 它监听Model的变化，将数据转换为UI可显示的格式，并通过IGameUI接口更新View
// 它也实现IGameCommands接口，接收来自View的用户操作，并调用Model的相应方法
class GameViewModel : public QObject, public IGameCommands {
    Q_OBJECT

public:
    explicit GameViewModel(GameModel& model, QObject *parent = nullptr);

    // 设置UI接口，用于依赖注入
    void setUI(IGameUI* ui);
    // 设置App命令接口，用于处理窗口切换等应用级命令
    void setApp(IGameCommands* appCommands);

    // --- IGameCommands 接口实现 ---
    void startNewGame(GameMode mode, GameDifficulty difficulty) override;
    void startNextCampaignLevel() override;
    void revealCellRequest(int row, int col) override;
    void toggleFlagRequest(int row, int col) override;
    void cycleMarkRequest(int row, int col) override;
    void returnToHomeRequest() override;
    // 实现撤销命令
    void undoLastFlagRequest() override;

private slots:
    // 当Model数据改变时被调用的槽函数
    void onModelChanged();
    // 当Model发出游戏结束信号时被调用的槽函数
    void onGameOver(bool victory);

private:
    // 私有辅助方法，用于启动游戏的核心逻辑
    void startGameInternal(const GameDifficulty& difficulty);

    GameModel& m_model;  // 对Model的引用
    IGameUI* m_ui = nullptr;  // 指向UI接口的指针，用于更新视图
    IGameCommands* m_appCommands = nullptr;  // 指向App命令接口的指针

    // --- 状态管理 ---
    GameMode m_currentGameMode;  // 当前的游戏模式
    GameDifficulty m_currentDifficulty;  // 当前的难度设置
    int m_currentCampaignLevel = 0;  // 在闯关模式下的当前关卡索引
};

#endif // GAMEVIEWMODEL_H