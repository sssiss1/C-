#include "GameViewModel.h"

GameViewModel::GameViewModel(GameModel& model, QObject *parent)
    : QObject(parent), m_model(model), m_flagsRemaining(0) {
    connect(&m_model, &GameModel::modelChanged, this, &GameViewModel::onModelChanged);
    connect(&m_model, &GameModel::gameOver, this, &GameViewModel::onGameOver);
}

void GameViewModel::startNewGame(int rows, int cols, int mines) {
    m_model.startGame(rows, cols, mines);

    // 更新ViewModel状态并发出带参数的信号
    if (m_statusText != "Game in progress...") {
        m_statusText = "Game in progress...";
        emit statusTextChanged(m_statusText); // 修改点
    }

    if (m_boardSize != QSize(rows, cols)) {
        m_boardSize = QSize(rows, cols);
        emit boardSizeChanged(m_boardSize); // 修改点
    }

    updateProperties();
    onModelChanged();
}

void GameViewModel::revealCellRequest(int row, int col) {
    m_model.revealCell(row, col);
}

void GameViewModel::toggleFlagRequest(int row, int col) {
    m_model.flagCell(row, col);
}

QString GameViewModel::statusText() const {
    return m_statusText;
}

int GameViewModel::flagsRemaining() const {
    return m_flagsRemaining;
}

QSize GameViewModel::boardSize() const {
    return m_boardSize;
}

void GameViewModel::onModelChanged() {
    updateProperties();

    for (int r = 0; r < m_model.getRows(); ++r) {
        for (int c = 0; c < m_model.getCols(); ++c) {
            const Cell& cell = m_model.getCell(r, c);
            CellUpdateInfo info{r, c, "", "background-color: #c0c0c0;", true};

            if (m_model.getGameState() == GameState::Lost && cell.isMine) {
                info.text = "💣";
                info.styleSheet = "background-color: red;";
            } else if (cell.isFlagged) {
                info.text = "🚩";
            } else if (cell.isRevealed) {
                info.enabled = false;
                info.styleSheet = "background-color: #e0e0e0; border: 1px solid #808080;";
                if (cell.adjacentMines > 0) {
                    info.text = QString::number(cell.adjacentMines);
                    switch (cell.adjacentMines) {
                        case 1: info.styleSheet += "color: blue;"; break;
                        case 2: info.styleSheet += "color: green;"; break;
                        case 3: info.styleSheet += "color: red;"; break;
                        case 4: info.styleSheet += "color: darkblue;"; break;
                        case 5: info.styleSheet += "color: brown;"; break;
                        default: info.styleSheet += "color: black;"; break;
                    }
                }
            }
            emit cellUpdated(info);
        }
    }
}

void GameViewModel::onGameOver(bool victory) {
    if (victory) {
        m_statusText = "You Win! :)";
        emit showGameOverDialog("Congratulations! You've cleared the minefield!");
    } else {
        m_statusText = "You Lost! :(";
        emit showGameOverDialog("Boom! You hit a mine.");
    }
    emit statusTextChanged(m_statusText); // 修改点
}

void GameViewModel::updateProperties() {
    const int newFlagsRemaining = m_model.getMineCount() - m_model.getFlagCount();

    if (m_flagsRemaining != newFlagsRemaining) {
        m_flagsRemaining = newFlagsRemaining;
        emit flagsRemainingChanged(m_flagsRemaining); // 修改点
    }
}