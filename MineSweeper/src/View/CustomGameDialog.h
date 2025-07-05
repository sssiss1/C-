/*
* CustomGameDialog.h
 *
 * 文件用途：
 * 定义CustomGameDialog类，这是一个View组件
 * 它提供一个对话框界面，让用户可以自定义游戏的行数、列数和地雷数
 */

#ifndef CUSTOMGAMEDIALOG_H
#define CUSTOMGAMEDIALOG_H

#include <QDialog>
#include "../common/GameModes.h"

//前向声明UI类
namespace Ui {
    class CustomGameDialog;
}

//自定义游戏设置对话框类，继承自QDialog
class CustomGameDialog : public QDialog {
    Q_OBJECT

public:
    //构造函数
    explicit CustomGameDialog(QWidget *parent = nullptr);
    //析构函数
    ~CustomGameDialog();

    //获取用户在对话框中设置的游戏难度
    GameDifficulty getDifficulty() const;

private slots:
    //当行数或列数输入框的值变化时，此槽函数被调用以更新地雷数的上限
    void updateMineCountRange();

private:
    //指向由Qt UIC生成的UI类的指针
    Ui::CustomGameDialog *ui;
};

#endif // CUSTOMGAMEDIALOG_H