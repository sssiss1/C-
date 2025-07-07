// #ifndef 和 #define 是标准的头文件保护宏（Header Guard）。
// 它们可以防止同一个头文件在一次编译中被包含多次，从而避免因重复定义而导致的编译错误。
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// 包含 QMainWindow 头文件，因为 MainWindow 是应用程序的主窗口，继承自 QMainWindow。
#include <QMainWindow>
// 包含 QVector 头文件，用于存储二维的 QPushButton 指针数组，即游戏棋盘上的所有格子按钮。
#include <QVector>
// 包含 QSize 头文件，用于处理和存储棋盘的尺寸（行数和列数）。
#include <QSize>
// 包含 QTimer 头文件，这里主要用于 QTimer::singleShot，一个非阻塞的延迟调用功能。
#include <QTimer>
// 包含共享的游戏模式定义，以便 MainWindow 能理解 GameMode 和 GameDifficulty 等类型。
#include "../common/GameModes.h"
// 包含共享的游戏数据结构定义，如 CellUpdateInfo，这是 MainWindow 与 ViewModel 之间通信的“数据契约”。
#include "../common/GameData.h"

// 前向声明 QPushButton 类。因为头文件中只用到了 QPushButton 的指针类型，
// 所以不需要包含完整的 <QPushButton> 头文件，使用前向声明可以减少编译依赖。
class QPushButton;

// 这是Qt的标准宏，用于在全局命名空间中正确处理 Qt 的类。
QT_BEGIN_NAMESPACE
// 使用 `namespace Ui` 来组织由 Qt Designer (uic) 自动生成的 UI 类。
namespace Ui { class MainWindow; }
// Qt标准宏的结束。
QT_END_NAMESPACE

// 定义 MainWindow 类，它作为游戏的主界面。
// 公开继承自 QMainWindow，拥有菜单栏、工具栏、状态栏等主窗口特性。
class MainWindow : public QMainWindow {
    // Q_OBJECT 是一个必须包含的宏，用于任何定义了自定义信号或槽的 QObject 子类。
    // 它启动了 Qt 的元对象系统（Meta-Object System），这是信号和槽机制能够工作的基础。
    Q_OBJECT

public:
    // 声明构造函数。
    // `explicit` 关键字防止编译器进行意外的隐式类型转换。
    // `QWidget *parent = nullptr` 是 Qt 对象树机制的一部分。
    explicit MainWindow(QWidget *parent = nullptr);
    // 声明析构函数，负责在对象销毁时释放资源。
    ~MainWindow();

// `protected:` 声明了受保护的成员。这些成员可以被本类和其子类访问。
protected:
    // 重写（override）了 QObject 的 eventFilter 方法。
    // 这是一种强大的事件处理机制，允许 MainWindow 拦截并处理其子控件（这里是棋盘格按钮）的事件。
    // 我们用它来捕捉鼠标的右键和中键点击，因为 QPushButton 的标准 clicked() 信号只响应左键。
    bool eventFilter(QObject *watched, QEvent *event) override;

// `public slots:` 声明了可以从外部连接的槽函数。
// 这些槽是“数据绑定”和“事件通知”的接收端，用于响应来自 ViewModel 的更新。
public slots:
    // 当棋盘尺寸需要改变时，调用此槽来重建UI网格。
    void onBoardSizeChanged(const QSize& newSize);
    // 当单个单元格的外观需要更新时，调用此槽。
    void onCellUpdated(const CellUpdateInfo& info);
    // 当游戏结束，需要显示对话框时，调用此槽。
    void onShowGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel);
    // 更新旗帜数量标签的文本。
    void updateFlagsLabel(int flags);
    // 更新游戏状态标签的文本（如“进行中...”、“你赢了!”）。
    void updateStatusLabel(const QString& text);
    // 在状态栏显示一条临时消息。
    void showTemporaryMessage(const QString& message, int durationMs);
    // 根据游戏模式配置窗口的UI（如显示/隐藏特定按钮）。
    void configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels);
    // 更新计时器标签的显示。
    void updateTimerDisplay(int seconds);
    // 设置“提示”按钮的启用/禁用状态。
    void setHintButtonEnabled(bool enabled);

// `signals:` 声明了该类可以发出的信号。
// 这些信号是“命令绑定”的发送端，将用户的操作请求通知给外部。
signals:
    // 当用户请求开始一个新游戏时发出此信号（携带当前模式和难度）。
    void newGameRequested(GameMode mode, GameDifficulty difficulty);
    // 当用户在闯关模式胜利后，请求进入下一关时发出此信号。
    void nextCampaignLevelRequested();
    // 当用户请求返回主菜单时发出此信号。
    void returnToHomeRequested();
    // 当用户请求提示时发出此信号。
    void hintRequested();
    // 当用户请求揭开一个单元格时发出此信号（携带行列坐标）。
    void revealCellRequested(int row, int col);
    // 当用户请求标记/取消标记旗帜时发出此信号（携带行列坐标）。
    void toggleFlagRequested(int row, int col);
    // 当用户请求循环标记（问号）时发出此信号（携带行列坐标）。
    void cycleMarkRequested(int row, int col);

// `private slots:` 声明了仅在类内部使用的槽函数。
// 它们通过Qt的命名约定自动连接到对应UI控件的信号。
private slots:
    // 自动连接到名为 "newGameButton" 的按钮的 clicked() 信号。
    void on_newGameButton_clicked();
    // 自动连接到名为 "homeButton" 的按钮的 clicked() 信号。
    void on_homeButton_clicked();
    // 自动连接到名为 "hintButton" 的按钮的 clicked() 信号。
    void on_hintButton_clicked();

// `private:` 声明了类的私有成员。
private:
    // 一个私有辅助函数，用于清空并删除当前棋盘上所有的 QPushButton，以便重建。
    void clearBoard();

    // 指向由 Qt UIC 根据 .ui 文件生成的 UI 类的指针。通过它访问所有UI控件。
    Ui::MainWindow *ui;
    // 一个二维向量，存储了指向棋盘上所有 QPushButton 的指针。这使得我们可以通过行列坐标快速访问任何一个格子按钮。
    QVector<QVector<QPushButton*>> m_cellButtons;
    // 存储当前游戏模式。当用户点击“重新开始”时，需要用这个信息来请求一个同样模式的新游戏。
    GameMode m_currentMode;
    // 存储当前游戏难度。同上，用于“重新开始”功能。
    GameDifficulty m_currentDifficulty;
    // 存储当前棋盘的尺寸，用于在 onBoardSizeChanged 中判断是否需要重建棋盘，避免不必要的UI刷新。
    QSize m_boardSize;
};

#endif // MAINWINDOW_H