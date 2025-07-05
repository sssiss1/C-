/*
 * MainWindow.cpp
 *
 * 文件用途：
 * 实现MainWindow类的功能
 * 作为View层，它的职责是“被动”的：
 * 1. 实现IGameUI接口的方法，根据ViewModel的指令更新界面元素
 * 2. 捕获用户输入事件（如按钮点击），并将其转换为命令发送给ViewModel
 * 它不包含任何游戏逻辑
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>

//构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_boardSize(0,0)
{
    ui->setupUi(this);
    //Qt的命名约定会自动连接 on_<objectName>_<signalName> 槽函数
    //所以这里的on_newGameButton_clicked等函数无需手动connect
}

//析构函数
MainWindow::~MainWindow() {
    clearBoard(); //确保在窗口关闭时清理所有动态创建的按钮
    delete ui;
}

//依赖注入：设置命令接口
void MainWindow::setCommands(IGameCommands* commands) {
    m_commands = commands;
}

//清空棋盘上的所有按钮
void MainWindow::clearBoard() {
    for (auto& row : m_cellButtons) {
        for (auto* button : row) {
            if(button) {
                ui->gridLayout->removeWidget(button); //从布局中移除
                delete button; //删除按钮对象
            }
        }
    }
    m_cellButtons.clear(); //清空指针容器
}

//当ViewModel通知棋盘尺寸改变时调用
void MainWindow::onBoardSizeChanged(const QSize& newSize) {
    clearBoard(); //先清空旧棋盘
    const int rows = newSize.height();
    const int cols = newSize.width();
    m_cellButtons.assign(rows, QVector<QPushButton*>(cols, nullptr)); //重置按钮容器

    //循环创建新的按钮网格
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = new QPushButton(ui->grids); //创建按钮
            button->setFixedSize(30, 30); //设置固定大小
            button->setFont(QFont("Arial", 12, QFont::Bold)); //设置字体
            //连接按钮的左键点击信号，触发一个lambda表达式
            connect(button, &QPushButton::clicked, this, [this, r, c]() {
                //在lambda中，通过命令接口发送“揭开格子”的请求
                if (m_commands) m_commands->revealCellRequest(r, c);
            });
            button->installEventFilter(this); //安装事件过滤器以捕获右键和中键点击
            //将坐标作为属性附加到按钮上，便于在事件过滤器中识别
            button->setProperty("row", r);
            button->setProperty("col", c);
            ui->gridLayout->addWidget(button, r, c); //将按钮添加到网格布局
            m_cellButtons[r][c] = button; //保存按钮指针
        }
    }
    //使用QTimer::singleShot确保在所有UI事件处理完毕后调整窗口大小
    //这可以防止因布局变化导致的尺寸计算不准确
    QTimer::singleShot(0, this, [this]() {
        this->adjustSize(); //根据内容自动调整窗口大小
        this->setFixedSize(this->size()); //固定窗口大小，防止用户拖拽
    });
}

//当ViewModel通知更新某个单元格时调用
void MainWindow::onCellUpdated(const CellUpdateInfo &info) {
    //检查坐标是否有效
    if (info.row >= 0 && info.row < m_cellButtons.size() &&
        info.col >= 0 && info.col < m_cellButtons[info.row].size()) {
        QPushButton* button = m_cellButtons[info.row][info.col];
        if(button) {
            //根据ViewModel传来的信息更新按钮的属性
            button->setText(info.text);
            button->setStyleSheet(info.styleSheet);
            button->setEnabled(info.enabled);
        }
    }
}

//当ViewModel通知需要根据游戏模式配置UI时调用
void MainWindow::configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) {
    m_currentMode = mode; //保存当前模式
    m_currentDifficulty = difficulty; //保存当前难度
    QSize newSize(difficulty.cols, difficulty.rows);
    if (m_boardSize != newSize) { //如果尺寸变化了
        m_boardSize = newSize;
        onBoardSizeChanged(newSize); //才重建棋盘
    }
    if (mode == GameMode::Campaign) {
        ui->newGameButton->hide(); //闯关模式下隐藏“重新开始”按钮
        setWindowTitle("扫雷 - 闯关模式");
        ui->campaignStatusLabel->setText(QString("第 %1 / %2 关").arg(currentLevel).arg(totalLevels));
        ui->campaignStatusLabel->show(); //显示关卡信息
    } else {
        ui->newGameButton->show(); //其他模式显示“重新开始”按钮
        setWindowTitle("扫雷");
        ui->campaignStatusLabel->hide(); //隐藏关卡信息
    }
}

//当ViewModel通知显示游戏结束对话框时调用
void MainWindow::onShowGameOverDialog(const QString &message, bool isCampaign, bool isVictory, bool isLastLevel) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(message);
    QPushButton *restartButton = nullptr;
    QPushButton *nextLevelButton = nullptr;
    //所有模式都有“回到主页”按钮
    QPushButton *homeButton = msgBox.addButton("回到主页", QMessageBox::RejectRole);

    //根据ViewModel传来的参数，决定显示哪个按钮
    if (isCampaign) {
        //闯关模式胜利且不是最后一关，显示“下一关”
        if (isVictory && !isLastLevel) {
            nextLevelButton = msgBox.addButton("下一关", QMessageBox::AcceptRole);
        }
    } else {
        //非闯关模式显示“重新开始”
        restartButton = msgBox.addButton("重新开始", QMessageBox::AcceptRole);
    }

    msgBox.exec(); //显示对话框并等待用户选择

    //根据用户的选择，通过命令接口发送不同的请求
    if (msgBox.clickedButton() == restartButton) {
        //使用QTimer::singleShot确保在对话框关闭后执行，避免时序问题
        QTimer::singleShot(0, this, &MainWindow::on_newGameButton_clicked);
    } else if (msgBox.clickedButton() == nextLevelButton) {
        QTimer::singleShot(0, this, [this](){
            if (m_commands) m_commands->startNextCampaignLevel();
        });
    } else if (msgBox.clickedButton() == homeButton) {
        QTimer::singleShot(0, this, &MainWindow::on_homeButton_clicked);
    }
}

//更新旗帜标签
void MainWindow::updateFlagsLabel(int flags) { ui->flagsLabel->setText(QString("旗帜: %1").arg(flags)); }
//更新状态标签
void MainWindow::updateStatusLabel(const QString& text) { ui->statusLabel->setText(text); }
//在状态栏显示临时消息
void MainWindow::showTemporaryMessage(const QString& message, int durationMs) { statusBar()->showMessage(message, durationMs); }
//更新计时器标签
void MainWindow::updateTimerDisplay(int seconds) { ui->timeLabel->setText(QString("时间: %1").arg(seconds)); }

//设置提示按钮是否可用
void MainWindow::setHintButtonEnabled(bool enabled) {
    ui->hintButton->setEnabled(enabled);
}

//“重新开始”按钮点击槽函数
void MainWindow::on_newGameButton_clicked(){ if (m_commands) m_commands->startNewGame(m_currentMode, m_currentDifficulty); }
//“回到主页”按钮点击槽函数
void MainWindow::on_homeButton_clicked() { if (m_commands) m_commands->returnToHomeRequest(); }

//“提示”按钮点击槽函数
void MainWindow::on_hintButton_clicked() {
    if (m_commands) {
        m_commands->hintRequest(); //发送提示请求
    }
}

//事件过滤器，用于处理非左键点击事件
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) { //如果是鼠标按下事件
        if (auto *button = qobject_cast<QPushButton*>(watched)) { //如果事件源是一个QPushButton
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            //从按钮属性中获取其坐标
            int row = button->property("row").toInt();
            int col = button->property("col").toInt();
            if (mouseEvent->button() == Qt::RightButton) {
                //右键点击，发送“标记旗帜”请求
                if (m_commands) m_commands->toggleFlagRequest(row, col);
                return true; //返回true表示事件已处理，不再向下传递
            } else if (mouseEvent->button() == Qt::MiddleButton) {
                //中键点击，发送“循环标记”请求
                if (m_commands) m_commands->cycleMarkRequest(row, col);
                return true; //事件已处理
            }
        }
    }
    //对于其他事件，调用基类的默认处理
    return QMainWindow::eventFilter(watched, event);
}