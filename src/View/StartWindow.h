#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QtWidgets/QWidget>
#include "../common/GameModes.h"

class HighScoreDialog;
class GameViewModel;

namespace Ui {
    class StartWindow;
}

class StartWindow : public QWidget {
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    signals:
        void gameModeSelected(GameMode mode, GameDifficulty difficulty);
    void highScoresRequested();

private slots:
    void on_simpleButton_clicked();
    void on_normalButton_clicked();
    void on_hardButton_clicked();
    void on_customButton_clicked();
    void on_campaignButton_clicked();
    void on_highScoreButton_clicked();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H