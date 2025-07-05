/*
* StartWindow.h
 *
 * 文件用途：
 * 定义StartWindow类，这是应用的起始界面View
 * 它提供按钮让用户选择游戏模式或查看排行榜
 * 它将用户的选择作为命令通过IGameCommands接口发送出去
 */

#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QtWidgets/QWidget>
#include "../common/IGameCommands.h"

//前向声明HighScoreDialog和GameViewModel类
//GameViewModel使用前向声明，因为头文件只需要它的指针类型，避免循环包含
class HighScoreDialog;
class GameViewModel;

namespace Ui {
    class StartWindow;
}

//开始窗口类
class StartWindow : public QWidget {
    Q_OBJECT

public:
    //构造函数
    explicit StartWindow(QWidget *parent = nullptr);
    //析构函数
    ~StartWindow();

    //设置命令接口，由App在初始化时调用（依赖注入）
    //接收两个接口：一个用于应用级命令（窗口切换），一个用于ViewModel级命令（数据请求）
    void setCommands(IGameCommands* appCommands, GameViewModel* vm);

private slots:
    //Qt自动连接的槽函数，处理各个按钮的点击事件
    void on_simpleButton_clicked();
    void on_normalButton_clicked();
    void on_hardButton_clicked();
    void on_customButton_clicked();
    void on_campaignButton_clicked();
    void on_highScoreButton_clicked();

private:
    //指向由UIC生成的UI类的指针
    Ui::StartWindow *ui;
    //持有App的命令接口指针，用于发送开始游戏等命令
    IGameCommands* m_appCommands = nullptr;
    //直接持有ViewModel的指针，用于请求排行榜数据
    GameViewModel* m_viewModel = nullptr;
    //指向排行榜对话框的指针，使用懒加载模式创建
    HighScoreDialog* m_highScoreDialog = nullptr;
};

#endif // STARTWINDOW_H