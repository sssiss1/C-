// HighScoreDialog.cpp
#include "HighScoreDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <climits> // 包含 climits 头文件用于 INT_MAX

HighScoreDialog::HighScoreDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("扫雷排行榜");
    setFixedSize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 添加标题
    QLabel *titleLabel = new QLabel("最佳时间排行榜", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(titleLabel);

    // 创建表格
    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({"模式", "难度", "最佳时间"});
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);

    layout->addWidget(m_table);

    // 加载分数
    loadScores();
}

void HighScoreDialog::loadScores() {
    QSettings settings("MyCompany", "MineSweeper");

    // 清空表格
    m_table->setRowCount(0);

    // 添加预定义模式的分数
    addScoresForMode("简单模式", "bestTime/Simple");
    addScoresForMode("普通模式", "bestTime/Normal");
    addScoresForMode("困难模式", "bestTime/Hard");
    addScoresForMode("闯关模式", "bestTime/Campaign");

    // 添加自定义模式的分数
    settings.beginGroup("bestTime");
    QStringList customKeys = settings.childKeys();
    for (const QString &key : customKeys) {
        if (key.startsWith("Custom_")) {
            QStringList parts = key.split('_');
            if (parts.size() == 4) {
                int rows = parts[1].toInt();
                int cols = parts[2].toInt();
                int mines = parts[3].toInt();

                int time = settings.value(key, INT_MAX).toInt();
                if (time != INT_MAX) {
                    int row = m_table->rowCount();
                    m_table->insertRow(row);

                    m_table->setItem(row, 0, new QTableWidgetItem("自定义模式"));
                    m_table->setItem(row, 1, new QTableWidgetItem(
                        QString("%1x%2, %3雷").arg(rows).arg(cols).arg(mines)));
                    m_table->setItem(row, 2, new QTableWidgetItem(
                        QString("%1秒").arg(time)));
                }
            }
        }
    }
    settings.endGroup();
}

void HighScoreDialog::addScoresForMode(const QString& modeName, const QString& key) {
    QSettings settings("MyCompany", "MineSweeper");
    int time = settings.value(key, INT_MAX).toInt();

    if (time != INT_MAX) {
        int row = m_table->rowCount();
        m_table->insertRow(row);

        m_table->setItem(row, 0, new QTableWidgetItem(modeName));
        m_table->setItem(row, 1, new QTableWidgetItem("-"));
        m_table->setItem(row, 2, new QTableWidgetItem(QString("%1秒").arg(time)));
    }
}