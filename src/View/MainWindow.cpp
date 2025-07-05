#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QSettings> // 用于访问 QSettings 类
#include <climits>   // 用于访问 INT_MAX 常量
// MainWindow.cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_boardSize(0,0),
      m_undoButton(nullptr), m_gameTimer(nullptr), m_elapsedSeconds(0), m_timeLabel(nullptr) {
    ui->setupUi(this);

    // 创建撤销按钮
    m_undoButton = new QPushButton("撤销标旗", this);
    m_undoButton->setEnabled(false);
    ui->horizontalLayout->insertWidget(2, m_undoButton);

    // 连接按钮点击事件
    connect(m_undoButton, &QPushButton::clicked, this, [this]() {
        if (m_commands) {
            m_commands->undoLastFlagRequest();
            m_undoButton->setEnabled(false);
        }
    });

    // 添加时间标签
    m_timeLabel = new QLabel("时间: 0", this);
    ui->horizontalLayout->insertWidget(0, m_timeLabel);

    // 初始化计时器
    m_gameTimer = new QTimer(this);
    m_elapsedSeconds = 0;
    connect(m_gameTimer, &QTimer::timeout, this, [this]() {
        m_elapsedSeconds++;
        m_timeLabel->setText(QString("时间: %1").arg(m_elapsedSeconds));
    });
}
MainWindow::~MainWindow() {
    clearBoard();
    delete ui;
}

void MainWindow::setCommands(IGameCommands* commands) {
    m_commands = commands;
}

void MainWindow::saveBestTime(GameMode mode, const GameDifficulty& difficulty, int time) {
    // 实现保存最佳时间逻辑
    QSettings settings("MyCompany", "MineSweeper");

    QString key;
    switch (mode) {
        case GameMode::Simple: key = "bestTime/Simple"; break;
        case GameMode::Normal: key = "bestTime/Normal"; break;
        case GameMode::Hard: key = "bestTime/Hard"; break;
        case GameMode::Custom:
            key = QString("bestTime/Custom_%1x%2_%3")
                  .arg(difficulty.rows)
                  .arg(difficulty.cols)
                  .arg(difficulty.mines);
            break;
        case GameMode::Campaign: key = "bestTime/Campaign"; break;
        default: key = "bestTime/Unknown"; break; // 添加默认情况
    }

    int currentBest = settings.value(key, INT_MAX).toInt();
    if (time < currentBest) {
        settings.setValue(key, time);
    }
}
// 清理棋盘，删除所有按钮并清空容器
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

// --- IGameUI 接口的实现 ---

// 当棋盘尺寸变化时，重建整个按钮网格
void MainWindow::onBoardSizeChanged(const QSize& newSize) {
    clearBoard();  // 清理旧的棋盘
    const int rows = newSize.height();
    const int cols = newSize.width();
    m_cellButtons.assign(rows, QVector<QPushButton*>(cols, nullptr));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = new QPushButton(ui->grids);
            button->setFixedSize(30, 30);
            button->setFont(QFont("Arial", 12, QFont::Bold));
            // 连接按钮的左键点击信号
            connect(button, &QPushButton::clicked, this, [this, r, c]() {
                if (m_commands) m_commands->revealCellRequest(r, c);
            });
            button->installEventFilter(this);  // 安装事件过滤器以捕获右键点击
            button->setProperty("row", r);  // 存储行和列信息到按钮属性中
            button->setProperty("col", c);
            ui->gridLayout->addWidget(button, r, c);
            m_cellButtons[r][c] = button;
        }
    }
    // 延迟调整窗口大小，确保布局计算完成
    QTimer::singleShot(0, this, [this]() {
        this->adjustSize();
        this->setFixedSize(this->size());
    });
}

// 更新单个格子的外观
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

// 显示游戏结束对话框
void MainWindow::onShowGameOverDialog(const QString &message, bool isCampaign, bool isVictory, bool isLastLevel) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(message);
    QPushButton *restartButton = nullptr;
    QPushButton *nextLevelButton = nullptr;
    QPushButton *homeButton = msgBox.addButton("回到主页", QMessageBox::RejectRole);

    if (isCampaign) {  // 如果是闯关模式
        if (isVictory && !isLastLevel) {
            nextLevelButton = msgBox.addButton("下一关", QMessageBox::AcceptRole);
        }
    } else {  // 如果是普通模式
        restartButton = msgBox.addButton("重新开始", QMessageBox::AcceptRole);
    }

    msgBox.exec();  // 显示对话框并等待用户操作
    // 根据用户的点击，发出相应的命令
    if (msgBox.clickedButton() == restartButton) {
        QTimer::singleShot(0, this, &MainWindow::on_newGameButton_clicked);
    } else if (msgBox.clickedButton() == nextLevelButton) {
        QTimer::singleShot(0, this, [this](){
            if (m_commands) m_commands->startNextCampaignLevel();
        });
    } else if (msgBox.clickedButton() == homeButton) {
        QTimer::singleShot(0, this, &MainWindow::on_homeButton_clicked);
    }
    // 停止计时器
    m_gameTimer->stop();

    // 如果是胜利，保存记录
    if (isVictory) {
        saveBestTime(m_currentMode, m_currentDifficulty, m_elapsedSeconds);
    }
}

// 根据游戏模式配置窗口的UI元素
void MainWindow::configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) {
    m_currentMode = mode;
    m_currentDifficulty = difficulty;

    QSize newSize(difficulty.cols, difficulty.rows);
    if (m_boardSize != newSize) {  // 只有在尺寸变化时才重建棋盘
        m_boardSize = newSize;
        onBoardSizeChanged(newSize);
    }

    if (mode == GameMode::Campaign) {
        ui->newGameButton->hide();  // 闯关模式下隐藏“重新开始”按钮
        setWindowTitle("扫雷 - 闯关模式");
        // 显示并设置关卡状态标签
        ui->campaignStatusLabel->setText(QString("第 %1 / %2 关").arg(currentLevel).arg(totalLevels));
        ui->campaignStatusLabel->show();
    } else {
        ui->newGameButton->show();  // 其他模式显示“重新开始”按钮
        setWindowTitle("扫雷");
        ui->campaignStatusLabel->hide();  // 隐藏关卡状态标签
    }

    // 新游戏开始时启用撤销按钮
    m_undoButton->setEnabled(true);
    // 开始计时
    m_elapsedSeconds = 0;
    m_timeLabel->setText("时间: 0");
    m_gameTimer->start(1000);
}

// 更新旗帜标签
void MainWindow::updateFlagsLabel(int flags) {
    ui->flagsLabel->setText(QString("旗帜: %1").arg(flags));
}

// 更新状态标签
void MainWindow::updateStatusLabel(const QString& text) {
    ui->statusLabel->setText(text);
}

// 在状态栏显示临时消息
void MainWindow::showTemporaryMessage(const QString& message, int durationMs) {
    statusBar()->showMessage(message, durationMs);
}

// "重新开始"按钮点击处理
void MainWindow::on_newGameButton_clicked(){
    if (m_commands) {
        // 使用保存的当前模式和难度开始新游戏
        m_commands->startNewGame(m_currentMode, m_currentDifficulty);
    }
}

// "回到主页"按钮点击处理
void MainWindow::on_homeButton_clicked() {
    if (m_commands) m_commands->returnToHomeRequest();
}

// 事件过滤器，用于处理非左键的鼠标点击
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (auto *button = qobject_cast<QPushButton*>(watched)) {
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            int row = button->property("row").toInt();
            int col = button->property("col").toInt();

            if (mouseEvent->button() == Qt::RightButton) {
                if (m_commands) m_commands->toggleFlagRequest(row, col);
                return true;  // 返回true表示事件已被处理，不再向后传递
            } else if (mouseEvent->button() == Qt::MiddleButton) {
                if (m_commands) m_commands->cycleMarkRequest(row, col);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event); // 其他事件交由基类处理
}


