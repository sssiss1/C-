#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QSize> // 引入 QSize
#include<QLabel>
#include "../common/IGameUI.h"
#include "../common/IGameCommands.h"

class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 游戏主窗口类，实现了IGameUI接口以接收来自ViewModel的更新
class MainWindow : public QMainWindow, public IGameUI {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    // 显式删除拷贝构造函数和赋值运算符
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    ~MainWindow();

    // 设置命令接口，用于将用户在游戏中的操作转发出去
    void setCommands(IGameCommands* commands);

    // --- IGameUI 接口实现 ---
    void onBoardSizeChanged(const QSize& newSize) override;
    void onCellUpdated(const CellUpdateInfo& info) override;
    void onShowGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel) override;
    void updateFlagsLabel(int flags) override;
    void updateStatusLabel(const QString& text) override;
    void showTemporaryMessage(const QString& message, int durationMs) override;
    void configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) override;

protected:
    // 事件过滤器，用于捕获按钮上的鼠标右键和中键点击
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // "重新开始"按钮的槽函数
    void on_newGameButton_clicked();
    // "回到主页"按钮的槽函数
    void on_homeButton_clicked();

private:
    // 清空并删除棋盘上的所有按钮
    void clearBoard();
    void saveBestTime(GameMode mode, const GameDifficulty& difficulty, int time); // 声明保存最佳时间函数
    Ui::MainWindow *ui;  // 指向UI界面的指针
    IGameCommands* m_commands = nullptr;  // 指向命令接口的指针
    QVector<QVector<QPushButton*>> m_cellButtons;  // 存储所有格子按钮的二维向量
    GameMode m_currentMode;  // 保存当前游戏模式，用于“重新开始”
    GameDifficulty m_currentDifficulty;  // 保存当前难度，用于“重新开始”
    QSize m_boardSize;  // 记录当前棋盘尺寸，避免不必要的重建
    // 添加撤销按钮
    QPushButton* m_undoButton;
    // 添加计时器和时间标签
    QTimer* m_gameTimer;
    int m_elapsedSeconds;
    QLabel* m_timeLabel;
};

#endif // MAINWINDOW_H