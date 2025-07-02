#ifndef MINESWEEPER_GAMEVIEWMODEL_H
#define MINESWEEPER_GAMEVIEWMODEL_H

#include <QObject>
#include <QSize>
#include "../Model/GameModel.h"

class GameModel;

struct CellUpdateInfo {
    int row, col;
    QString text;
    QString styleSheet;
    bool enabled;
};

class GameViewModel : public QObject {
    Q_OBJECT
    // 属性的 NOTIFY 信号现在将携带参数
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(int flagsRemaining READ flagsRemaining NOTIFY flagsRemainingChanged)
    Q_PROPERTY(QSize boardSize READ boardSize NOTIFY boardSizeChanged)

public:
    explicit GameViewModel(GameModel& model, QObject *parent = nullptr);

    Q_INVOKABLE void startNewGame(int rows, int cols, int mines);
    Q_INVOKABLE void revealCellRequest(int row, int col);
    Q_INVOKABLE void toggleFlagRequest(int row, int col);

    QString statusText() const;
    int flagsRemaining() const;
    QSize boardSize() const;

    signals:
        void cellUpdated(const CellUpdateInfo& info);
    void showGameOverDialog(const QString& message);

    // --- 修改点：为信号添加参数 ---
    void statusTextChanged(const QString& statusText);
    void flagsRemainingChanged(int flagsRemaining);
    void boardSizeChanged(const QSize& boardSize);

private slots:
    void onModelChanged();
    void onGameOver(bool victory);

private:
    void updateProperties();

    GameModel& m_model;

    QString m_statusText;
    int m_flagsRemaining;
    QSize m_boardSize;
};

#endif //MINESWEEPER_GAMEVIEWMODEL_H