#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSize>
#include <QTimer>
#include "../common/GameModes.h"
#include "../common/GameData.h"

class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void onBoardSizeChanged(const QSize& newSize);
    void onCellUpdated(const CellUpdateInfo& info);
    void onShowGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel);
    void updateFlagsLabel(int flags);
    void updateStatusLabel(const QString& text);
    void showTemporaryMessage(const QString& message, int durationMs);
    void configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels);
    void updateTimerDisplay(int seconds);
    void setHintButtonEnabled(bool enabled);

    signals:
        void newGameRequested(GameMode mode, GameDifficulty difficulty);
    void nextCampaignLevelRequested();
    void returnToHomeRequested();
    void hintRequested();
    void revealCellRequested(int row, int col);
    void toggleFlagRequested(int row, int col);
    void cycleMarkRequested(int row, int col);

private slots:
    void on_newGameButton_clicked();
    void on_homeButton_clicked();
    void on_hintButton_clicked();

private:
    void clearBoard();
    Ui::MainWindow *ui;
    QVector<QVector<QPushButton*>> m_cellButtons;
    GameMode m_currentMode;
    GameDifficulty m_currentDifficulty;
    QSize m_boardSize;
};

#endif // MAINWINDOW_H