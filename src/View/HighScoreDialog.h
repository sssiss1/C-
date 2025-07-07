#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>
#include "../common/GameData.h"

namespace Ui {
    class HighScoreDialog;
}

class HighScoreDialog : public QDialog {
    Q_OBJECT

public:
    explicit HighScoreDialog(QWidget *parent = nullptr);
    ~HighScoreDialog();

public slots:
    void onHighScoresReady(const QVector<HighScoreItem>& scores);

    signals:
        void clearHighScoresRequested();

private slots:
    void on_clearButton_clicked();

private:
    Ui::HighScoreDialog *ui;
};

#endif // HIGHSCOREDIALOG_H