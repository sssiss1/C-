#include "MinesweeperApp.h"

MinesweeperApp::MinesweeperApp()
    : m_viewModel(m_model),
      m_highScoreDialog(nullptr)
{
    // =================================================================
    //                --- 接线中心 (Wiring Center) ---
    // =================================================================

    // --- 1. ViewModel -> View (数据流向UI) ---
    connect(&m_viewModel, &GameViewModel::boardSizeChanged, &m_gameWindow, &MainWindow::onBoardSizeChanged);
    connect(&m_viewModel, &GameViewModel::cellUpdated, &m_gameWindow, &MainWindow::onCellUpdated);
    connect(&m_viewModel, &GameViewModel::showGameOverDialog, &m_gameWindow, &MainWindow::onShowGameOverDialog);
    connect(&m_viewModel, &GameViewModel::flagsLabelUpdated, &m_gameWindow, &MainWindow::updateFlagsLabel);
    connect(&m_viewModel, &GameViewModel::statusLabelUpdated, &m_gameWindow, &MainWindow::updateStatusLabel);
    connect(&m_viewModel, &GameViewModel::temporaryMessageShown, &m_gameWindow, &MainWindow::showTemporaryMessage);
    connect(&m_viewModel, &GameViewModel::modeConfigured, &m_gameWindow, &MainWindow::configureForMode);
    connect(&m_viewModel, &GameViewModel::timerDisplayUpdated, &m_gameWindow, &MainWindow::updateTimerDisplay);
    connect(&m_viewModel, &GameViewModel::hintButtonEnabled, &m_gameWindow, &MainWindow::setHintButtonEnabled);

    // --- 2. View -> ViewModel (用户操作) ---
    // StartWindow
    connect(&m_startWindow, &StartWindow::gameModeSelected, this, &MinesweeperApp::onStartNewGame);
    connect(&m_startWindow, &StartWindow::highScoresRequested, this, &MinesweeperApp::onShowHighScores);

    // MainWindow
    connect(&m_gameWindow, &MainWindow::revealCellRequested, &m_viewModel, &GameViewModel::revealCellRequest);
    connect(&m_gameWindow, &MainWindow::toggleFlagRequested, &m_viewModel, &GameViewModel::toggleFlagRequest);
    connect(&m_gameWindow, &MainWindow::cycleMarkRequested, &m_viewModel, &GameViewModel::cycleMarkRequest);
    connect(&m_gameWindow, &MainWindow::hintRequested, &m_viewModel, &GameViewModel::hintRequest);
    connect(&m_gameWindow, &MainWindow::newGameRequested, &m_viewModel, &GameViewModel::startNewGame);
    connect(&m_gameWindow, &MainWindow::nextCampaignLevelRequested, &m_viewModel, &GameViewModel::startNextCampaignLevel);

    // --- 3. ViewModel/View -> App (应用级操作) ---
    connect(&m_viewModel, &GameViewModel::returnToHomeRequested, this, &MinesweeperApp::onReturnToHome);
    connect(&m_gameWindow, &MainWindow::returnToHomeRequested, this, &MinesweeperApp::onReturnToHome);
}

MinesweeperApp::~MinesweeperApp() {
    delete m_highScoreDialog;
}

void MinesweeperApp::run() {
    m_startWindow.show();
}

void MinesweeperApp::onStartNewGame(GameMode mode, GameDifficulty difficulty) {
    m_viewModel.startNewGame(mode, difficulty);
    m_startWindow.hide();
    m_gameWindow.show();
}

void MinesweeperApp::onReturnToHome() {
    m_gameWindow.hide();
    m_startWindow.show();
}

void MinesweeperApp::onShowHighScores() {
    if (!m_highScoreDialog) {
        m_highScoreDialog = new HighScoreDialog(&m_startWindow); // Pass parent
        // ViewModel -> Dialog
        connect(&m_viewModel, &GameViewModel::highScoresReady, m_highScoreDialog, &HighScoreDialog::onHighScoresReady);
        // Dialog -> ViewModel
        connect(m_highScoreDialog, &HighScoreDialog::clearHighScoresRequested, &m_viewModel, &GameViewModel::clearHighScoresRequest);
    }
    m_viewModel.loadHighScoresRequest();
    m_highScoreDialog->exec();
}