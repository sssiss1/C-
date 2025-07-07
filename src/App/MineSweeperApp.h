#ifndef MINESWEEPERAPP_H
#define MINESWEEPERAPP_H

#include <QObject>
#include "../Model/GameModel.h"
#include "../ViewModel/GameViewModel.h"
#include "../View/StartWindow.h"
#include "../View/MainWindow.h"
#include "../View/HighScoreDialog.h"

class MinesweeperApp : public QObject {
    Q_OBJECT
public:
    MinesweeperApp();
    ~MinesweeperApp();
    void run();

public slots:
    void onStartNewGame(GameMode mode, GameDifficulty difficulty);
    void onReturnToHome();
    void onShowHighScores();

private:
    GameModel m_model;
    GameViewModel m_viewModel;
    StartWindow m_startWindow;
    MainWindow m_gameWindow;
    HighScoreDialog* m_highScoreDialog;
};

#endif //MINESWEEPERAPP_H