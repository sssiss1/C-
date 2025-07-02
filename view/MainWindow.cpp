#include "MainWindow.h"
#include "ui_MainWindow.h" // 关键：必须包含由 uic 生成的头文件
#include "../ViewModel/GameViewModel.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>

MainWindow::MainWindow(GameViewModel& viewModel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 创建 UI 类的实例
    , m_viewModel(viewModel)
{
    // 关键：这一行代码会读取 MainWindow.ui 文件并构建其中定义的所有界面控件
    ui->setupUi(this);

    // 虽然 .ui 文件已设置标题，但在代码中设置是好习惯，便于未来进行动态修改或国际化
    setWindowTitle("Minesweeper");

    // --- 将 ViewModel 的信号连接到 View 的槽函数或控件属性 ---
    connect(&m_viewModel, &GameViewModel::statusTextChanged, ui->statusLabel, &QLabel::setText);
    connect(&m_viewModel, &GameViewModel::flagsRemainingChanged, this, &MainWindow::updateFlagsLabel);
    connect(&m_viewModel, &GameViewModel::boardSizeChanged, this, &MainWindow::onBoardSizeChanged);
    connect(&m_viewModel, &GameViewModel::cellUpdated, this, &MainWindow::onCellUpdated);
    connect(&m_viewModel, &GameViewModel::showGameOverDialog, this, &MainWindow::onShowGameOverDialog);

    // newGameButton 的 clicked 信号会自动连接到 on_newGameButton_clicked() 槽函数，无需手动 connect

    // 开始第一局游戏
    on_newGameButton_clicked();
}

MainWindow::~MainWindow()
{
    clearBoard(); // 清理动态分配的按钮
    delete ui;    // 释放 UI 对象
}

void MainWindow::clearBoard() {
    for (auto& row : m_cellButtons) {
        for (auto* button : row) {
            // 从布局中移除并删除按钮对象，防止内存泄漏
            ui->gridLayout->removeWidget(button);
            delete button;
        }
    }
    m_cellButtons.clear();
}

void MainWindow::onBoardSizeChanged(const QSize& newSize) {
    clearBoard();
    const int rows = newSize.height();
    const int cols = newSize.width();
    m_cellButtons.resize(rows);

    for (int r = 0; r < rows; ++r) {
        m_cellButtons[r].resize(cols);
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = new QPushButton();
            button->setFixedSize(30, 30);
            button->setFont(QFont("Arial", 12, QFont::Bold));

            // 将按钮的点击事件连接到 ViewModel 的命令
            connect(button, &QPushButton::clicked, this, [this, r, c]() {
                m_viewModel.revealCellRequest(r, c);
            });

            // 安装事件过滤器以捕获右键点击
            button->installEventFilter(this);
            button->setProperty("row", r); // 存储行列信息，供事件过滤器使用
            button->setProperty("col", c);

            // 将新按钮添加到 .ui 文件中定义的网格布局里
            ui->gridLayout->addWidget(button, r, c);
            m_cellButtons[r][c] = button;
        }
    }
}

void MainWindow::onCellUpdated(const CellUpdateInfo &info) {
    if (info.row < m_cellButtons.size() && info.col < m_cellButtons[info.row].size()) {
        QPushButton* button = m_cellButtons[info.row][info.col];
        button->setText(info.text);
        button->setStyleSheet(info.styleSheet);
        button->setEnabled(info.enabled);
    }
}

void MainWindow::onShowGameOverDialog(const QString &message) {
    QMessageBox::information(this, "Game Over", message);
}

void MainWindow::updateFlagsLabel(int flags) {
    ui->flagsLabel->setText(QString("Flags: %1").arg(flags));
}

// 这个槽函数由 Qt 自动连接到 ui->newGameButton 的 clicked() 信号
void MainWindow::on_newGameButton_clicked()
{
    // 调用 ViewModel 的命令来开始新游戏
    m_viewModel.startNewGame(10, 10, 15); // 默认难度
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        // 确保事件源是一个 QPushButton
        if (auto *button = qobject_cast<QPushButton*>(watched)) {
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::RightButton) {
                // 从按钮属性中获取行列信息
                int row = button->property("row").toInt();
                int col = button->property("col").toInt();
                // 调用 ViewModel 的命令来插旗
                m_viewModel.toggleFlagRequest(row, col);
                return true; // 表示事件已处理，不再向后传递
            }
        }
    }
    // 对于其他事件，调用基类的实现
    return QMainWindow::eventFilter(watched, event);
}