/*
* CustomGameDialog.h
 *
 * 文件用途：
 * 定义CustomGameDialog类，这是一个View组件。
 * 它提供一个对话框界面，让用户可以自定义游戏的行数、列数和地雷数。
 */

// #ifndef 和 #define 是标准的头文件保护宏（Header Guard）。
// 它们可以防止同一个头文件在一次编译中被包含多次，从而避免因重复定义而导致的编译错误。
#ifndef CUSTOMGAMEDIALOG_H
#define CUSTOMGAMEDIALOG_H


#include <QDialog>
// 包含 "../common/GameModes.h" 头文件，以便使用在整个项目中共享的 GameDifficulty 结构体。
// 这个结构体用于封装和传递自定义的游戏设置。
#include "../common/GameModes.h"

// 使用 `namespace Ui` 来组织由 Qt Designer (uic) 自动生成的 UI 类。
namespace Ui {
    // 前向声明 (Forward Declaration) `CustomGameDialog` 类。
    // 这是一种优化技术，它告诉编译器存在一个名为 `CustomGameDialog` 的类，
    // 但不需要在此处包含完整的类定义。这减少了头文件之间的依赖，可以加快编译速度。
    // 实际的类定义在 `ui_CustomGameDialog.h` 文件中，该文件会在 `.cpp` 文件中被包含。
    class CustomGameDialog;
}

// 定义 CustomGameDialog 类，它是一个用于设置自定义游戏参数的对话框。
// 它公开继承自 QDialog，这意味着它具备了对话框的所有基本行为（如模态显示、接受/拒绝按钮等）。
class CustomGameDialog : public QDialog {
    Q_OBJECT

public:
    // 声明构造函数。
    // `explicit` 关键字防止编译器进行意外的隐式类型转换。
    // `QWidget *parent = nullptr` 是 Qt 对象树机制的一部分。
    // 当这个对话框创建时，可以指定一个父窗口（如 StartWindow），
    // 这样对话框会显示在父窗口之上，并且在父窗口被销毁时，对话框也可能被自动销毁。
    explicit CustomGameDialog(QWidget *parent = nullptr);


    ~CustomGameDialog();

    // 声明一个公共成员函数 `getDifficulty`。
    // 这个函数是该对话框的主要对外接口。
    // 当对话框被用户确认（点击“OK”）后，调用者（如 StartWindow）可以通过这个函数获取用户设置的游戏难度。
    // `const` 关键字表示这个函数不会修改类的任何成员变量，是一个只读操作。
    GameDifficulty getDifficulty() const;


private slots:
    // 声明一个名为 `updateMineCountRange` 的私有槽函数。
    // 这个槽将连接到UI上行数和列数 `QSpinBox` 的 `valueChanged` 信号。
    // 每当用户改变行或列的数值时，这个槽就会被触发，以动态计算并更新地雷数输入框的最大允许值。
    void updateMineCountRange();


private:
    // 声明一个指向 `Ui::CustomGameDialog` 类型的指针 `ui`。
    // `Ui::CustomGameDialog` 是由 Qt UIC（UI Compiler）根据 `CustomGameDialog.ui` 文件自动生成的类。
    // 这个 `ui` 指针将持有所有在 Qt Designer 中设计的UI控件（如 QSpinBox, QLabel 等）的实例。
    // 通过 `ui->` 的方式，我们可以在代码中访问和操作这些界面元素。
    Ui::CustomGameDialog *ui;
};

// 头文件保护宏的结束部分。
#endif // CUSTOMGAMEDIALOG_H