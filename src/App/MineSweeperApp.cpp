#include "MinesweeperApp.h"

MinesweeperApp::MinesweeperApp()
    : m_viewModel(m_model)  // 在初始化列表中创建ViewModel并传入Model的引用
{
    // --- 依赖注入 ---
    // 在此将各个组件之间的依赖关系连接起来

    // 1. 将ViewModel(作为IGameUI)注入到游戏窗口，使ViewModel可以更新游戏窗口UI
    m_viewModel.setUI(&m_gameWindow);

    // 2. 将App自身(作为IGameCommands)注入到开始窗口
    //    这样开始窗口的命令会直接由App处理
    m_startWindow.setCommands(this);

    // 3. 将ViewModel(作为IGameCommands)注入到游戏窗口
    //    游戏过程中的命令由ViewModel处理
    m_gameWindow.setCommands(&m_viewModel);

    // 4. 将App自身(作为IGameCommands)注入到ViewModel，用于处理返回主页等请求
    m_viewModel.setApp(this);
}

MinesweeperApp::~MinesweeperApp() = default;  // 使用默认的析构函数

void MinesweeperApp::run() {
    // 启动时显示开始窗口
    m_startWindow.show();
}

// --- IGameCommands 接口实现 ---

void MinesweeperApp::startNewGame(GameMode mode, GameDifficulty difficulty) {
    // App层的职责：协调其他组件并管理窗口状态
    // 1. 命令ViewModel准备游戏数据
    m_viewModel.startNewGame(mode, difficulty);

    // 2. 切换窗口
    m_startWindow.hide();
    m_gameWindow.show();
}

void MinesweeperApp::returnToHomeRequest() {
    // App层的职责：切换窗口
    m_gameWindow.hide();
    m_startWindow.show();
}