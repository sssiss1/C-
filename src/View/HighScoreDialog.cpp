#include "HighScoreDialog.h"
#include "ui_HighScoreDialog.h"
#include <QMessageBox>

HighScoreDialog::HighScoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HighScoreDialog)
{
    ui->setupUi(this);
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

HighScoreDialog::~HighScoreDialog() {
    delete ui;
}

void HighScoreDialog::onHighScoresReady(const QVector<HighScoreItem>& scores) {
    ui->highScoreTable->setRowCount(0);
    for (const auto& score : scores) {
        int row = ui->highScoreTable->rowCount();
        ui->highScoreTable->insertRow(row);
        ui->highScoreTable->setItem(row, 0, new QTableWidgetItem(score.mode));
        ui->highScoreTable->setItem(row, 1, new QTableWidgetItem(score.difficulty));
        ui->highScoreTable->setItem(row, 2, new QTableWidgetItem(score.time));
    }
}

void HighScoreDialog::on_clearButton_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认", "确定要清空所有排行榜记录吗？此操作不可恢复",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit clearHighScoresRequested();
    }
}