/*
 * MainWindow.h
 *
 * 文件用途：
 * 定义MainWindow类，这是游戏的主界面View
 * 它负责显示游戏棋盘、状态信息和控制按钮
 * 它实现了IGameUI接口，使其可以被ViewModel驱动来更新显示
 * 它将所有用户交互（点击、右击等）转发给IGameCommands接口处理
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSize>
#include <QTimer>
#include "../common/IGameUI.h"
#include "../common/IGameCommands.h"

//前向声明QPushButton和UI类
class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//主窗口类，继承自QMainWindow，并实现了IGameUI接口
class MainWindow : public QMainWindow, public IGameUI {
    Q_OBJECT

public:
    //构造函数
    explicit MainWindow(QWidget *parent = nullptr);
    //析构函数
    ~MainWindow();

    //设置命令接口，由App在初始化时调用（依赖注入）
    void setCommands(IGameCommands* commands);

    //--- IGameUI 接口实现 ---
    //这些方法由ViewModel调用，用于更新UI

    void onBoardSizeChanged(const QSize& newSize) override;
    void onCellUpdated(const CellUpdateInfo& info) override;
    void onShowGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel) override;
    void updateFlagsLabel(int flags) override;
    void updateStatusLabel(const QString& text) override;
    void showTemporaryMessage(const QString& message, int durationMs) override;
    void configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) override;
    void updateTimerDisplay(int seconds) override;
    void setHintButtonEnabled(bool enabled) override;

protected:
    //重写事件过滤器，用于捕获棋盘格子的右键和中键点击
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    //Qt自动连接的槽函数，处理UI上按钮的点击事件
    void on_newGameButton_clicked();
    void on_homeButton_clicked();
    void on_hintButton_clicked();

private:
    //私有辅助函数，用于清空并删除棋盘上所有的QPushButton
    void clearBoard();

    //指向由UIC生成的UI类的指针
    Ui::MainWindow *ui;
    //持有IGameCommands接口的指针，用于发送命令
    IGameCommands* m_commands = nullptr;
    //二维向量，用于存储指向棋盘上所有QPushButton的指针
    QVector<QVector<QPushButton*>> m_cellButtons;
    //存储当前游戏模式和难度，主要用于“重新开始”按钮
    GameMode m_currentMode;
    GameDifficulty m_currentDifficulty;
    //存储当前棋盘大小，用于避免不必要的重建
    QSize m_boardSize;
};

#endif // MAINWINDOW_H