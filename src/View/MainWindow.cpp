#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_boardSize(0,0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    clearBoard();
    delete ui;
}

void MainWindow::clearBoard() {
    for (auto& row : m_cellButtons) {
        for (auto* button : row) {
            if(button) {
                ui->gridLayout->removeWidget(button);
                delete button;
            }
        }
    }
    m_cellButtons.clear();
}

void MainWindow::onBoardSizeChanged(const QSize& newSize) {
    // 只有当新尺寸和当前尺寸不同时，才重建棋盘
    if (m_boardSize == newSize) return;
    m_boardSize = newSize;

    clearBoard();
    const int rows = newSize.height();
    const int cols = newSize.width();
    m_cellButtons.assign(rows, QVector<QPushButton*>(cols, nullptr));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = new QPushButton(ui->grids);
            button->setFixedSize(30, 30);
            button->setFont(QFont("Arial", 12, QFont::Bold));
            connect(button, &QPushButton::clicked, this, [this, r, c]() {
                emit revealCellRequested(r, c);
            });
            button->installEventFilter(this);
            button->setProperty("row", r);
            button->setProperty("col", c);
            ui->gridLayout->addWidget(button, r, c);
            m_cellButtons[r][c] = button;
        }
    }
    QTimer::singleShot(0, this, [this]() {
        this->adjustSize();
        this->setFixedSize(this->size());
    });
}

void MainWindow::onCellUpdated(const CellUpdateInfo &info) {
    if (info.row >= 0 && info.row < m_cellButtons.size() &&
        info.col >= 0 && info.col < m_cellButtons[info.row].size()) {
        QPushButton* button = m_cellButtons[info.row][info.col];
        if(button) {
            button->setText(info.text);
            button->setStyleSheet(info.styleSheet);
            button->setEnabled(info.enabled);
        }
    }
}

void MainWindow::configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) {
    m_currentMode = mode;
    m_currentDifficulty = difficulty;
    if (mode == GameMode::Campaign) {
        ui->newGameButton->hide();
        setWindowTitle("扫雷 - 闯关模式");
        ui->campaignStatusLabel->setText(QString("第 %1 / %2 关").arg(currentLevel).arg(totalLevels));
        ui->campaignStatusLabel->show();
    } else {
        ui->newGameButton->show();
        setWindowTitle("扫雷");
        ui->campaignStatusLabel->hide();
    }
}

void MainWindow::onShowGameOverDialog(const QString &message, bool isCampaign, bool isVictory, bool isLastLevel) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(message);
    QPushButton *restartButton = nullptr;
    QPushButton *nextLevelButton = nullptr;
    QPushButton *homeButton = msgBox.addButton("回到主页", QMessageBox::RejectRole);

    if (isCampaign) {
        if (isVictory && !isLastLevel) {
            nextLevelButton = msgBox.addButton("下一关", QMessageBox::AcceptRole);
        }
    } else {
        restartButton = msgBox.addButton("重新开始", QMessageBox::AcceptRole);
    }

    msgBox.exec();

    if (msgBox.clickedButton() == restartButton) {
        QTimer::singleShot(0, this, &MainWindow::on_newGameButton_clicked);
    } else if (msgBox.clickedButton() == nextLevelButton) {
        QTimer::singleShot(0, this, [this](){ emit nextCampaignLevelRequested(); });
    } else if (msgBox.clickedButton() == homeButton) {
        QTimer::singleShot(0, this, &MainWindow::on_homeButton_clicked);
    }
}

void MainWindow::updateFlagsLabel(int flags) { ui->flagsLabel->setText(QString("旗帜: %1").arg(flags)); }
void MainWindow::updateStatusLabel(const QString& text) { ui->statusLabel->setText(text); }
void MainWindow::showTemporaryMessage(const QString& message, int durationMs) { statusBar()->showMessage(message, durationMs); }
void MainWindow::updateTimerDisplay(int seconds) { ui->timeLabel->setText(QString("时间: %1").arg(seconds)); }
void MainWindow::setHintButtonEnabled(bool enabled) { ui->hintButton->setEnabled(enabled); }

void MainWindow::on_newGameButton_clicked(){ emit newGameRequested(m_currentMode, m_currentDifficulty); }
void MainWindow::on_homeButton_clicked() { emit returnToHomeRequested(); }
void MainWindow::on_hintButton_clicked() { emit hintRequested(); }

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (auto *button = qobject_cast<QPushButton*>(watched)) {
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            int row = button->property("row").toInt();
            int col = button->property("col").toInt();
            if (mouseEvent->button() == Qt::RightButton) {
                emit toggleFlagRequested(row, col);
                return true;
            } else if (mouseEvent->button() == Qt::MiddleButton) {
                emit cycleMarkRequested(row, col);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}