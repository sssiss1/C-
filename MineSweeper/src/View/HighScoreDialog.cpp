/*
* HighScoreDialog.cpp
 *
 * 文件用途：
 * 实现HighScoreDialog类的功能
 * 主要逻辑包括：
 * 1. 接收ViewModel传递过来的排行榜数据并填充到QTableWidget中
 * 2. 处理“清空”按钮的点击事件，向ViewModel发送清空请求
 */

#include "HighScoreDialog.h"
#include "ui_HighScoreDialog.h"
#include "../common/IGameCommands.h" //包含IGameCommands接口定义
#include <QMessageBox>

//构造函数
HighScoreDialog::HighScoreDialog(IGameCommands* commands, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HighScoreDialog),
    m_commands(commands) //保存命令接口指针
{
    ui->setupUi(this);
    //设置表格列的宽度调整模式，使布局更美观
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

//析构函数
HighScoreDialog::~HighScoreDialog() {
    delete ui;
}

//接收到排行榜数据后的处理槽函数
void HighScoreDialog::onHighScoresReady(const QVector<HighScoreItem>& scores) {
    ui->highScoreTable->setRowCount(0); //清空表格现有内容
    //遍历从ViewModel传来的数据
    for (const auto& score : scores) {
        int row = ui->highScoreTable->rowCount(); //获取当前行数
        ui->highScoreTable->insertRow(row); //插入新行
        //在新行中填充数据
        ui->highScoreTable->setItem(row, 0, new QTableWidgetItem(score.mode));
        ui->highScoreTable->setItem(row, 1, new QTableWidgetItem(score.difficulty));
        ui->highScoreTable->setItem(row, 2, new QTableWidgetItem(score.time));
    }
}

//“清空排行榜”按钮的点击槽函数
void HighScoreDialog::on_clearButton_clicked() {
    //显示一个确认对话框，防止用户误操作
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认", "确定要清空所有排行榜记录吗？此操作不可恢复",
                                  QMessageBox::Yes|QMessageBox::No);
    //如果用户确认
    if (reply == QMessageBox::Yes) {
        //通过命令接口向ViewModel发送清空请求
        if (m_commands) {
            m_commands->clearHighScoresRequest();
        }
    }
}