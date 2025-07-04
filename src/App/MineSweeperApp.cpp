#include "MinesweeperApp.h"

//构造函数：执行依赖注入
MinesweeperApp::MinesweeperApp()
    //使用成员初始化列表来构造ViewModel，并将Model的引用“注入”到其构造函数中
    //这是构造函数注入，确保ViewModel在创建时就有一个有效的Model
    : m_viewModel(m_model)
{
    //组装逻辑移到这里，“设值注入”
    //1.告诉ViewModel它的UI是谁（通过IGameUI接口）
    m_viewModel.setUI(&m_mainWindow);

    //2.告诉View它的命令处理器是谁（通过IGameCommands接口）
    m_mainWindow.setCommands(&m_viewModel);
}

//析构函数可以为空，因为所有成员变量都是栈对象，它们的析构函数会在MinesweeperApp对象销毁时被自动调用
MinesweeperApp::~MinesweeperApp() = default;

//run方法负责显示窗口
void MinesweeperApp::run() {
    m_mainWindow.show();
}