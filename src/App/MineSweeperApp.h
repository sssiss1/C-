// #ifndef 和 #define 是标准的头文件保护宏（Header Guard）。
// 它们可以防止同一个头文件在一次编译中被包含多次，从而避免因重复定义而导致的编译错误。
#ifndef MINESWEEPERAPP_H
#define MINESWEEPERAPP_H

// 包含 QObject 基类，以便 MinesweeperApp 可以使用信号和槽机制。
// 它需要接收来自View的信号，所以它必须是一个QObject。
#include <QObject>
// 包含所有核心模块的头文件。
// 因为 MinesweeperApp 是这些模块的创建者和“接线员”，所以它需要知道所有这些类的完整定义。
#include "../Model/GameModel.h"
#include "../ViewModel/GameViewModel.h"
#include "../View/StartWindow.h"
#include "../View/MainWindow.h"
#include "../View/HighScoreDialog.h"

// 定义 MinesweeperApp 类，它作为整个应用程序的顶层控制器。
// 继承自 QObject，使其能够拥有信号和槽的功能。
class MinesweeperApp : public QObject {

    Q_OBJECT

public:
    // 声明默认构造函数。
    // 构造函数将是所有对象创建和信号-槽连接的中心。
    MinesweeperApp();

    // 声明析构函数。
    // 负责在应用退出时释放动态分配的资源（如此处的 m_highScoreDialog）。
    ~MinesweeperApp();

    // 声明一个公共的 run 方法。
    // 这是应用程序启动逻辑的入口点，通常在 main.cpp 中被调用，负责显示初始窗口。
    void run();

public slots:
    // 响应来自 StartWindow 的 gameModeSelected 信号。
    // 负责协调开始一个新游戏的整个流程：通知ViewModel准备游戏，并切换显示的窗口。
    void onStartNewGame(GameMode mode, GameDifficulty difficulty);

    // 响应来自 ViewModel 的 returnToHomeRequested 信号。
    // 负责将界面从游戏窗口切换回主菜单窗口。
    void onReturnToHome();

    // 响应来自 StartWindow 的 highScoresRequested 信号。
    // 负责创建并显示排行榜对话框，并协调ViewModel加载数据。
    void onShowHighScores();

// `private:` 声明了类的私有成员。
private:
    // --- 成员变量 ---
    // 这些成员变量是应用程序的核心组件实例。
    // MinesweeperApp 作为所有者，负责它们的创建和生命周期管理。

    // Model层实例。存储所有游戏的核心数据和业务逻辑。
    GameModel m_model;

    // ViewModel层实例。作为View和Model之间的桥梁。
    // 注意：它的初始化依赖于 m_model，这将在构造函数的实现中体现。
    GameViewModel m_viewModel;

    // View层实例：开始窗口/主菜单。
    StartWindow m_startWindow;

    // View层实例：游戏主窗口。
    MainWindow m_gameWindow;

    // View层实例：排行榜对话框。
    // 使用指针类型，因为这个对话框可能不是一直需要存在的，可以被懒加载（lazy-loaded），
    // 即只有在用户第一次点击“排行榜”时才被创建。
    HighScoreDialog* m_highScoreDialog;
};

// 头文件保护宏的结束部分。
#endif //MINESWEEPERAPP_H