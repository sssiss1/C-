#ifndef STARTWINDOW_H
#define STARTWINDOW_H

// 包含 <QtWidgets/QWidget> 头文件，因为 StartWindow 是一个基本的UI窗口，继承自 QWidget。
#include <QtWidgets/QWidget>
// 包含共享的游戏模式定义，以便 StartWindow 能够使用 GameMode 和 GameDifficulty 等类型
// 来作为 gameModeSelected 信号的参数，这是它与外部世界的“通信语言”。
#include "../common/GameModes.h"

// 前向声明 (Forward Declaration) HighScoreDialog 和 GameViewModel 类。
// 这告诉编译器这些类的存在，但不需要包含它们的完整头文件。
// 在这个头文件中，这两个类名并未被实际使用（没有创建实例或作为成员），
// 所以这是良好的编程习惯，可以减少编译依赖。如果后续需要，可以改为 #include。
class HighScoreDialog;
class GameViewModel;

// 使用 `namespace Ui` 来组织由 Qt Designer (uic) 自动生成的 UI 类。
namespace Ui {
    // 前向声明 `StartWindow` UI类。
    // 实际的类定义在 `ui_StartWindow.h` 文件中，该文件会在 `StartWindow.cpp` 中被包含。
    class StartWindow;
}

// 定义 StartWindow 类，它作为游戏的启动/主菜单窗口。
// 公开继承自 QWidget，是Qt中所有用户界面对象的基础类。
class StartWindow : public QWidget {
    Q_OBJECT

public:

    // `QWidget *parent = nullptr` 是 Qt 对象树机制的一部分，用于内存管理和窗口层级关系。
    explicit StartWindow(QWidget *parent = nullptr);

    // 声明析构函数，负责在对象销毁时释放资源（主要是ui指针）。
    ~StartWindow();


signals:
    // 声明一个名为 gameModeSelected 的信号。
    // 当用户选择了一个游戏模式时，这个信号会被发射。
    // 它携带两个参数：GameMode（模式枚举）和 GameDifficulty（难度结构体），
    // 将用户的选择数据传递给监听者（即 App Controller）。
    void gameModeSelected(GameMode mode, GameDifficulty difficulty);

    // 声明一个名为 highScoresRequested 的信号。
    // 当用户点击“排行榜”按钮时，这个信号会被发射，表示一个查看排行榜的请求。
    void highScoresRequested();

// `private slots:` 声明了仅在类内部使用的槽函数。
// 它们通过Qt的命名约定 `on_<objectName>_<signalName>` 自动连接到对应UI控件的信号。
private slots:
    // 自动连接到 "simpleButton" 的 clicked() 信号。
    void on_simpleButton_clicked();
    // 自动连接到 "normalButton" 的 clicked() 信号。
    void on_normalButton_clicked();
    // 自动连接到 "hardButton" 的 clicked() 信号。
    void on_hardButton_clicked();
    // 自动连接到 "customButton" 的 clicked() 信号。
    void on_customButton_clicked();
    // 自动连接到 "campaignButton" 的 clicked() 信号。
    void on_campaignButton_clicked();
    // 自动连接到 "highScoreButton" 的 clicked() 信号。
    void on_highScoreButton_clicked();

// `private:` 声明了类的私有成员。
private:
    // 声明一个指向 `Ui::StartWindow` 类型的指针 `ui`。
    // `Ui::StartWindow` 是由 Qt UIC（UI Compiler）根据 `StartWindow.ui` 文件自动生成的类。
    // 这个 `ui` 指针持有所有在 Qt Designer 中设计的UI控件的实例（如此处的所有QPushButton）。
    // 将 ui 设为私有，封装了UI实现的细节。
    Ui::StartWindow *ui;
};

// 头文件保护宏的结束部分。
#endif // STARTWINDOW_H