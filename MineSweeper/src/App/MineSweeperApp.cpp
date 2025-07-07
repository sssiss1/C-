/*
* MineSweeperApp.cpp
 *
 * 文件用途：
 * 实现MinesweeperApp类的功能
 * 核心是构造函数中的依赖注入过程，它将MVVM的各个部分正确地连接起来
 * 同时实现应用级别的命令，如窗口切换
 */

#include "MinesweeperApp.h"

//构造函数是整个MVVM架构组装的核心
MinesweeperApp::MinesweeperApp()
    : m_viewModel(m_model) //初始化ViewModel，并将Model的引用传递给它
{
    //--- 依赖注入（Dependency Injection）---
    //通过接口将各个组件连接起来，实现高内聚、低耦合

    //1. 将ViewModel(作为IGameUI)注入到游戏窗口(MainWindow)
    //这样ViewModel就可以通过IGameUI接口来更新游戏窗口的显示，而无需知道MainWindow的具体实现
    m_viewModel.setUI(&m_gameWindow);

    //2. 将App自身和ViewModel都注入到开始窗口(StartWindow)
    //App(作为IGameCommands)处理窗口切换命令（如开始游戏）
    //ViewModel(作为IGameCommands)处理数据请求命令（如加载排行榜）
    m_startWindow.setCommands(this, &m_viewModel);

    //3. 将ViewModel(作为IGameCommands)注入到游戏窗口(MainWindow)
    //这样游戏窗口就可以将所有游戏操作（如点击格子）转发给ViewModel处理
    m_gameWindow.setCommands(&m_viewModel);

    //4. 将App自身(作为IGameCommands的另一部分)注入到ViewModel
    //这允许ViewModel在需要时（如用户点击“回到主页”）调用App的命令来执行窗口切换
    m_viewModel.setApp(this);
}

//默认析构函数
MinesweeperApp::~MinesweeperApp() = default;

//运行应用，程序的起点是显示开始窗口
void MinesweeperApp::run() {
    m_startWindow.show();
}

//--- IGameCommands 接口实现 ---

//处理开始新游戏的命令
void MinesweeperApp::startNewGame(GameMode mode, GameDifficulty difficulty) {
    //通知ViewModel准备新游戏的数据
    m_viewModel.startNewGame(mode, difficulty);
    //隐藏开始窗口
    m_startWindow.hide();
    //显示游戏主窗口
    m_gameWindow.show();
}

//处理返回主页的请求
void MinesweeperApp::returnToHomeRequest() {
    //隐藏游戏主窗口
    m_gameWindow.hide();
    //显示开始窗口
    m_startWindow.show();
}