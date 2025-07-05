#ifndef MINESWEEPERAPP_H
#define MINESWEEPERAPP_H

#include "../Model/GameModel.h"
#include "../ViewModel/GameViewModel.h"
#include "../View/StartWindow.h"
#include "../View/MainWindow.h"

// App层负责顶层逻辑，如窗口切换
// 它也实现IGameCommands接口的一部分来处理来自ViewModel的窗口切换请求
class MinesweeperApp : public IGameCommands {
public:
    // 构造函数，负责初始化和组装所有核心组件
    MinesweeperApp();
    // 析构函数
    ~MinesweeperApp();

    // 运行应用程序，显示初始界面
    void run();

    // --- IGameCommands 接口实现 (部分) ---
    // App只处理它关心的命令，主要是窗口导航
    void startNewGame(GameMode mode, GameDifficulty difficulty) override;
    void returnToHomeRequest() override;

    // 其他命令不归App管理，提供空实现以满足接口要求
    void startNextCampaignLevel() override {}
    void revealCellRequest(int, int) override {}
    void toggleFlagRequest(int, int) override {}
    void cycleMarkRequest(int, int) override {}
    // 添加撤销命令的空实现
    void undoLastFlagRequest() override {}
private:
    GameModel m_model;  // 游戏核心数据和逻辑
    GameViewModel m_viewModel;  // 连接Model和View的视图模型

    // App拥有所有窗口实例
    StartWindow m_startWindow;  // 开始窗口
    MainWindow m_gameWindow;  // 游戏主窗口
};

#endif //MINESWEEPERAPP_H