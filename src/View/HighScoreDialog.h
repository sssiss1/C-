// HighScoreDialog.h
#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSettings> // 包含 QSettings 头文件

class HighScoreDialog : public QDialog {
    Q_OBJECT

public:
    explicit HighScoreDialog(QWidget *parent = nullptr);

private:
    void loadScores();
    void addScoresForMode(const QString& modeName, const QString& keyPrefix);

    QTableWidget* m_table;
};

#endif // HIGHSCOREDIALOG_H
