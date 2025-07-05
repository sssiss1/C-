#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QtWidgets/QWidget>

#include "../common/IGameCommands.h"

namespace Ui {
    class StartWindow;
}
// 前向声明 HighScoreDialog
class HighScoreDialog;
// 开始窗口类，提供游戏模式选择
class StartWindow : public QWidget {
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    // 设置命令接口，用于将用户操作转发出去
    void setCommands(IGameCommands* commands);

private slots:
    // --- 按钮点击事件的槽函数 ---
    void on_simpleButton_clicked();
    void on_normalButton_clicked();
    void on_hardButton_clicked();
    void on_customButton_clicked();
    void on_campaignButton_clicked();
    void on_highScoreButton_clicked(); // 添加排行榜按钮槽函数

private:
    Ui::StartWindow *ui;
    IGameCommands* m_commands = nullptr;
    HighScoreDialog* m_highScoreDialog = nullptr; // 排行榜对话框
};

#endif // STARTWINDOW_H
