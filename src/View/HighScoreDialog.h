// #ifndef 和 #define 是标准的头文件保护宏（Header Guard）。
// 它们可以防止同一个头文件在一次编译中被包含多次，从而避免因重复定义而导致的编译错误。
#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H


#include <QDialog>
#include "../common/GameData.h"

// 使用 `namespace Ui` 来组织由 Qt Designer (uic) 自动生成的 UI 类。
namespace Ui {
    // 前向声明 (Forward Declaration) `HighScoreDialog` 类。
    // 这是一种优化技术，它告诉编译器存在一个名为 `HighScoreDialog` 的类，但不需要在此处包含完整的类定义。
    // 这减少了头文件之间的依赖，可以加快编译速度。
    class HighScoreDialog;
}

// 定义 HighScoreDialog 类，它是一个用于显示游戏排行榜的对话框。
// 它公开继承自 QDialog，具备对话框的所有基本行为。
class HighScoreDialog : public QDialog {
    // Q_OBJECT 是一个必须包含的宏，用于任何定义了自定义信号或槽的 QObject 子类。
    // 它启动了 Qt 的元对象系统（Meta-Object System），这是信号和槽机制能够工作的基础。
    Q_OBJECT

public:
    // 声明构造函数。
    // `explicit` 关键字防止编译器进行意外的隐式类型转换。
    // `QWidget *parent = nullptr` 是 Qt 对象树机制的一部分，用于内存管理和窗口层级关系。
    explicit HighScoreDialog(QWidget *parent = nullptr);

    // 声明析构函数，负责在对象销毁时释放资源（主要是ui指针）。
    ~HighScoreDialog();

public slots:
    // 声明一个名为 onHighScoresReady 的公共槽函数。
    // 这个槽函数被设计用来接收一个 QVector<HighScoreItem> 类型的数据。
    // 当 ViewModel 准备好排行榜数据后，会发出一个信号连接到这个槽，从而将数据传递给对话框进行显示。
    void onHighScoresReady(const QVector<HighScoreItem>& scores);

// `signals:` 声明了该类可以发出的信号。
// 这个信号是“命令绑定”的发送端。
signals:
    // 声明一个名为 clearHighScoresRequested 的信号。
    // 当用户在对话框中执行了希望清空排行榜的操作时（例如点击“清空”按钮），这个信号会被发射出去。
    // 外部世界（通常是App Controller）会监听这个信号，并调用 ViewModel 的相应方法来处理这个请求。
    void clearHighScoresRequested();

// `private slots:` 声明了仅在类内部使用的槽函数。
// 它们通常用于响应本类UI控件的信号（由Qt的 `on_<objectName>_<signalName>` 命名约定自动连接）。
private slots:
    // 声明一个私有槽函数，其名称遵循 Qt 的自动连接约定。
    // 它会自动连接到名为 "clearButton" 的 QPushButton 的 "clicked()" 信号。
    // 当用户点击界面上的“清空排行榜”按钮时，这个槽函数会被调用。
    void on_clearButton_clicked();

// `private:` 声明了类的私有成员。
private:
    // 声明一个指向 `Ui::HighScoreDialog` 类型的指针 `ui`。
    // `Ui::HighScoreDialog` 是由 Qt UIC（UI Compiler）根据 `HighScoreDialog.ui` 文件自动生成的类。
    // 这个 `ui` 指针持有所有在 Qt Designer 中设计的UI控件的实例（如 QTableWidget, QPushButton 等）。
    Ui::HighScoreDialog *ui;
};

// 头文件保护宏的结束部分。
#endif // HIGHSCOREDIALOG_H