#ifndef CUSTOMGAMEDIALOG_H
#define CUSTOMGAMEDIALOG_H

#include <QDialog>
#include "../common/GameModes.h"

namespace Ui {
    class CustomGameDialog;
}

// 自定义游戏设置对话框
class CustomGameDialog : public QDialog {
    Q_OBJECT

public:
    explicit CustomGameDialog(QWidget *parent = nullptr);
    ~CustomGameDialog();

    // 获取用户在对话框中设置的难度
    GameDifficulty getDifficulty() const;

private slots:
    // 当行数或列数变化时，更新地雷数上限的槽函数
    void updateMineCountRange();

private:
    Ui::CustomGameDialog *ui;  // 指向UI界面的指针
};

#endif // CUSTOMGAMEDIALOG_H