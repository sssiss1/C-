/*
* HighScoreDialog.h
 *
 * 文件用途：
 * 定义HighScoreDialog类，这是一个View组件
 * 它提供一个对话框界面，用于显示从ViewModel获取的排行榜数据
 * 它也包含一个“清空”按钮，用户的点击事件会通过IGameCommands接口转发出去
 */

#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>
#include "../common/IGameUI.h" //仅为了引入HighScoreItem结构体定义

//前向声明IGameCommands接口和UI类
class IGameCommands;

namespace Ui {
    class HighScoreDialog;
}

//排行榜对话框类
class HighScoreDialog : public QDialog {
    Q_OBJECT

public:
    //构造函数，接收一个IGameCommands指针用于发送“清空”命令
    explicit HighScoreDialog(IGameCommands* commands, QWidget *parent = nullptr);
    //析构函数
    ~HighScoreDialog();

public slots:
    //这是一个公共槽，用于接收来自ViewModel的排行榜数据并更新UI
    void onHighScoresReady(const QVector<HighScoreItem>& scores);

private slots:
    //当用户点击“清空排行榜”按钮时，此私有槽被调用
    void on_clearButton_clicked();

private:
    //指向由UIC生成的UI类的指针
    Ui::HighScoreDialog *ui;
    //持有IGameCommands接口的指针，用于发送命令
    IGameCommands* m_commands;
};

#endif // HIGHSCOREDIALOG_H