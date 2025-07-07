
#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <QMessageBox>

#include <QMouseEvent>

#include <QPushButton>

#include <QFont>

// MainWindow的构造函数实现。
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), // 调用基类QMainWindow的构造函数。
      ui(new Ui::MainWindow), // 创建UI类的实例。
      m_boardSize(0,0) // 使用初始化列表，将棋盘尺寸m_boardSize初始化为0x0。
{
    // 调用ui指针的setupUi方法，并传入`this`。
    // 这个方法会读取.ui文件的设计，创建所有UI控件，并将它们布局在当前主窗口上。
    ui->setupUi(this);
}

// MainWindow的析构函数实现。
MainWindow::~MainWindow() {
    // 调用私有辅助函数，清空并删除棋盘上的所有按钮，释放内存。
    clearBoard();
    // 释放ui指针指向的内存，防止内存泄漏。
    delete ui;
}

// 私有辅助函数clearBoard的实现。
void MainWindow::clearBoard() {
    // 使用范围-for循环遍历存储按钮指针的二维向量m_cellButtons。
    // auto& row 表示对每一行（QVector<QPushButton*>）取引用，避免不必要的拷贝。
    for (auto& row : m_cellButtons) {
        // 遍历当前行中的每一个按钮指针。
        // auto* button 表示将元素（QPushButton*）直接作为指针使用。
        for (auto* button : row) {
            // 检查指针是否有效（非空）。
            if(button) {
                // 从gridLayout布局中移除该按钮，但此时按钮对象本身还存在。
                ui->gridLayout->removeWidget(button);
                // 彻底删除按钮对象，释放其占用的内存。
                delete button;
            }
        }
    }
    // 清空二维向量，移除所有（现在是悬挂的）指针。
    m_cellButtons.clear();
}

// 公共槽函数onBoardSizeChanged的实现，当ViewModel通知棋盘尺寸改变时调用。
void MainWindow::onBoardSizeChanged(const QSize& newSize) {
    // 优化：只有当请求的新尺寸与当前尺寸不同时，才执行耗时的棋盘重建操作。
    if (m_boardSize == newSize) return;
    // 更新成员变量，记录新的棋盘尺寸。
    m_boardSize = newSize;

    // 调用clearBoard清空旧的棋盘。
    clearBoard();
    // 从QSize中获取新的行数和列数。
    const int rows = newSize.height();
    const int cols = newSize.width();
    // 重新分配m_cellButtons向量，使其具有新的尺寸，并用nullptr初始化所有元素。
    m_cellButtons.assign(rows, QVector<QPushButton*>(cols, nullptr));

    // 双重循环，创建新的棋盘格按钮。
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // 创建一个新的QPushButton实例，并将其父控件设置为ui->grids，以便正确管理。
            QPushButton *button = new QPushButton(ui->grids);
            // 设置按钮的固定大小，使其成为一个30x30像素的正方形。
            button->setFixedSize(30, 30);
            // 设置按钮上文本的字体（如果显示数字的话）。
            button->setFont(QFont("Arial", 12, QFont::Bold));

            // 连接按钮的clicked()信号（左键点击）到一个Lambda表达式。
            // Lambda表达式捕获了this指针和当前的行列值[this, r, c]。
            // 当按钮被点击时，Lambda会发射revealCellRequested信号，并携带正确的行列坐标。
            connect(button, &QPushButton::clicked, this, [this, r, c]() {
                emit revealCellRequested(r, c);
            });

            // 为按钮安装事件过滤器，`this`（MainWindow）将成为其事件的过滤器。
            // 这使得MainWindow的eventFilter方法可以拦截并处理此按钮的事件（如右键点击）。
            button->installEventFilter(this);
            // 使用Qt的属性系统，将行(r)和列(c)坐标存储为按钮的属性。
            // 这样在eventFilter中可以方便地从按钮对象反向获取其在棋盘上的位置。
            button->setProperty("row", r);
            button->setProperty("col", c);

            // 将新创建的按钮添加到gridLayout布局中的指定位置(r, c)。
            ui->gridLayout->addWidget(button, r, c);
            // 将按钮的指针存储到我们的二维向量中，以便后续通过坐标访问。
            m_cellButtons[r][c] = button;
        }
    }

    // 使用QTimer::singleShot(0, ...)实现一个非阻塞的延迟调用。
    // 延迟为0意味着它会在当前事件处理完成后，立即被放入事件队列执行。
    // 这样做是为了确保在所有UI元素都创建和布局完毕后，再调整窗口大小。
    // 如果不延迟，可能因为布局尚未完全更新而得到错误的尺寸。
    QTimer::singleShot(0, this, [this]() {
        // adjustSize()会根据布局和内容自动调整窗口到最合适的尺寸。
        this->adjustSize();
        // setFixedSize()将窗口的尺寸固定下来，防止用户手动拖拽改变大小。
        this->setFixedSize(this->size());
    });
}

// 公共槽函数onCellUpdated的实现，当ViewModel通知单个单元格状态改变时调用。
void MainWindow::onCellUpdated(const CellUpdateInfo &info) {
    // 健壮性检查：确保传入的行列坐标在当前棋盘范围内。
    if (info.row >= 0 && info.row < m_cellButtons.size() &&
        info.col >= 0 && info.col < m_cellButtons[info.row].size()) {
        // 通过坐标从二维向量中获取对应的按钮指针。
        QPushButton* button = m_cellButtons[info.row][info.col];
        // 再次确认指针有效。
        if(button) {
            // 根据info中的信息更新按钮的UI状态。
            button->setText(info.text); // 设置按钮显示的文本（如数字、"🚩", "❓"）。
            button->setStyleSheet(info.styleSheet); // 设置按钮的样式表（如背景色、文字颜色）。
            button->setEnabled(info.enabled); // 设置按钮是否可点击。
        }
    }
}

// 公共槽函数configureForMode的实现，用于根据游戏模式调整UI。
void MainWindow::configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) {
    // 将当前模式和难度保存到成员变量中，主要为了“重新开始”功能使用。
    m_currentMode = mode;
    m_currentDifficulty = difficulty;

    // 判断是否为闯关模式。
    if (mode == GameMode::Campaign) {
        // 在闯关模式下，隐藏“重新开始”按钮（因为逻辑是“下一关”）。
        ui->newGameButton->hide();
        // 设置窗口标题。
        setWindowTitle("扫雷 - 闯关模式");
        // 设置并显示关卡状态标签。
        ui->campaignStatusLabel->setText(QString("第 %1 / %2 关").arg(currentLevel).arg(totalLevels));
        ui->campaignStatusLabel->show();
    } else { // 如果是其他模式
        // 显示“重新开始”按钮。
        ui->newGameButton->show();
        // 设置默认窗口标题。
        setWindowTitle("扫雷");
        // 隐藏关卡状态标签。
        ui->campaignStatusLabel->hide();
    }
}

// 公共槽函数onShowGameOverDialog的实现，当ViewModel通知游戏结束时调用。
void MainWindow::onShowGameOverDialog(const QString &message, bool isCampaign, bool isVictory, bool isLastLevel) {
    // 创建一个QMessageBox实例，父窗口为当前主窗口。
    QMessageBox msgBox(this);
    // 设置对话框的标题和主要文本信息。
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(message);

    // 为对话框动态添加按钮。
    QPushButton *restartButton = nullptr;
    QPushButton *nextLevelButton = nullptr;
    // 添加“回到主页”按钮，并设置其角色为RejectRole。
    QPushButton *homeButton = msgBox.addButton("回到主页", QMessageBox::RejectRole);

    // 根据游戏模式和结果，决定添加“重新开始”还是“下一关”按钮。
    if (isCampaign) { // 如果是闯关模式
        // 并且胜利了，且不是最后一关
        if (isVictory && !isLastLevel) {
            // 添加“下一关”按钮，并设置其角色为AcceptRole（通常是默认按钮）。
            nextLevelButton = msgBox.addButton("下一关", QMessageBox::AcceptRole);
        }
    } else { // 如果是其他模式
        // 添加“重新开始”按钮，并设置为AcceptRole。
        restartButton = msgBox.addButton("重新开始", QMessageBox::AcceptRole);
    }

    // 以模态方式执行（显示）对话框，程序会在此阻塞，直到用户关闭对话框。
    msgBox.exec();

    // 在对话框关闭后，检查用户点击了哪个按钮。
    if (msgBox.clickedButton() == restartButton) {
        // 如果点击了“重新开始”，使用singleShot延迟调用on_newGameButton_clicked槽，模拟用户点击。
        QTimer::singleShot(0, this, &MainWindow::on_newGameButton_clicked);
    } else if (msgBox.clickedButton() == nextLevelButton) {
        // 如果点击了“下一关”，延迟发射nextCampaignLevelRequested信号。
        QTimer::singleShot(0, this, [this](){ emit nextCampaignLevelRequested(); });
    } else if (msgBox.clickedButton() == homeButton) {
        // 如果点击了“回到主页”，延迟调用on_homeButton_clicked槽。
        QTimer::singleShot(0, this, &MainWindow::on_homeButton_clicked);
    }
}

// ------ 以下是一些简单的UI更新槽函数 ------
void MainWindow::updateFlagsLabel(int flags) { ui->flagsLabel->setText(QString("旗帜: %1").arg(flags)); }
void MainWindow::updateStatusLabel(const QString& text) { ui->statusLabel->setText(text); }
void MainWindow::showTemporaryMessage(const QString& message, int durationMs) { statusBar()->showMessage(message, durationMs); }
void MainWindow::updateTimerDisplay(int seconds) { ui->timeLabel->setText(QString("时间: %1").arg(seconds)); }
void MainWindow::setHintButtonEnabled(bool enabled) { ui->hintButton->setEnabled(enabled); }

// ------ 以下是响应UI按钮点击的私有槽函数（自动连接）------
void MainWindow::on_newGameButton_clicked(){ emit newGameRequested(m_currentMode, m_currentDifficulty); }
void MainWindow::on_homeButton_clicked() { emit returnToHomeRequested(); }
void MainWindow::on_hintButton_clicked() { emit hintRequested(); }

// 事件过滤器的实现，用于捕捉棋盘格按钮的非左键点击事件。
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    // 检查事件类型是否为鼠标按下。
    if (event->type() == QEvent::MouseButtonPress) {
        // 尝试将事件源（watched）动态转换为QPushButton指针。
        if (auto *button = qobject_cast<QPushButton*>(watched)) {
            // 如果转换成功（说明是棋盘格按钮的事件），将通用QEvent转换为具体的QMouseEvent。
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            // 从按钮的属性中恢复其行列坐标。
            int row = button->property("row").toInt();
            int col = button->property("col").toInt();

            // 判断是哪个鼠标按钮被按下。
            if (mouseEvent->button() == Qt::RightButton) {
                // 如果是右键，发射toggleFlagRequested信号。
                emit toggleFlagRequested(row, col);
                // 返回true表示该事件已被处理，不需要再继续传递。
                return true;
            } else if (mouseEvent->button() == Qt::MiddleButton) {
                // 如果是中键，发射cycleMarkRequested信号。
                emit cycleMarkRequested(row, col);
                // 返回true表示该事件已被处理。
                return true;
            }
        }
    }
    // 对于所有其他事件，调用基类的eventFilter方法进行默认处理。
    return QMainWindow::eventFilter(watched, event);
}