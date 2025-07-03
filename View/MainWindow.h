#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
在MVVM架构中，View的职责应非常“薄”和“被动”：
1.展示UI：将界面元素（按钮、标签等）渲染到屏幕上
2.接收用户输入：响应用户的鼠标点击、键盘输入等事件
3.转发用户操作：将用户的操作意图通过IGameCommands接口转换成“命令”发送出去，它不关心谁会处理这个命令
4.执行更新指令：实现IGameUI接口，被动地接收来自外部的指令来更新自己的外观
*/

#include <QMainWindow>  //包含Qt的主窗口基类，提供了应用程序主窗口的标准框架
#include <QVector>  //包含Qt的动态数组容器，用于存储指向扫雷格子的按钮指针
#include "../common/IGameUI.h"
#include "../common/IGameCommands.h"

//--- 前向声明 ---
//可以减少头文件的物理依赖，加快编译速度
//因为在这里我们只需要用到这些类的指针或引用，而不需要知道它们的完整定义
class QPushButton;

//标准的Qt样板代码，用于处理.ui文件生成的类
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }  //前向声明由uic（UI 编译器）从MainWindow.ui文件生成的UI定义类
QT_END_NAMESPACE

//MainWindow类是游戏的主界面窗口
//它从QMainWindow继承，以获得窗口功能（如菜单栏、状态栏等）
//它从IGameUI 继承，以承诺自己会实现UI层应有的所有更新功能
class MainWindow : public QMainWindow, public IGameUI {
    Q_OBJECT  //Qt 宏，必须包含，用于启用元对象系统

public:
    //构造函数，它不依赖于任何具体的ViewModel或Model
    explicit MainWindow(QWidget *parent = nullptr);
    //析构函数，负责清理动态分配的资源
    ~MainWindow();

    //提供一个方法来设置View要与之通信的命令处理对象
    //参数是一个指向IGameCommands接口的指针，View只知道它在和“命令契约”对话，而不知道具体的命令处理类是什么
    void setCommands(IGameCommands* commands);

    //--- IGameUI 接口的实现声明 ---
    //override关键字确保这些函数正确地覆盖了基类IGameUI中的纯虚函数
public:
    void onBoardSizeChanged(const QSize& newSize) override;
    void onCellUpdated(const CellUpdateInfo& info) override;
    void onShowGameOverDialog(const QString& message) override;
    void updateFlagsLabel(int flags) override;
    void updateStatusLabel(const QString& text) override;

protected:
    //重写QObject的事件过滤器方法，用于捕获和处理子控件的特定事件（此处为右键点击）
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    //--- 槽函数 ---
    //这个槽函数用于响应界面上“New Game”按钮的点击事件
    //它的命名遵循Qt的自动连接约定 (on_<objectName>_<signalName>)，所以无需手动connect
    void on_newGameButton_clicked();

private:
    //私有辅助函数，用于清理和删除所有动态创建的格子按钮
    void clearBoard();

    //--- 私有成员变量 ---
    Ui::MainWindow *ui;  //指向由Designer生成的UI类的指针，通过它，可以访问在.ui文件中定义的所有控件

    //指向命令接口的指针，当用户操作时，View会通过这个指针发出命令
    IGameCommands* m_commands = nullptr;

    //二维动态数组，用于存储所有动态生成的扫雷格子按钮的指针
    //这使得我们可以方便地通过行列索引来访问和管理每一个格子按钮
    QVector<QVector<QPushButton*>> m_cellButtons;
};

#endif // MAINWINDOW_H