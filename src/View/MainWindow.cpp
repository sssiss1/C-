#include "MainWindow.h"
#include "ui_MainWindow.h"  //必须包含由uic从.ui文件生成的头文件，它定义了`Ui::MainWindow`类
#include <QMessageBox>  //包含Qt的消息框类，用于显示游戏结束对话框
#include <QMouseEvent>  //包含Qt的鼠标事件类，用于在事件过滤器中判断鼠标按键
#include <QPushButton>  //包含Qt的按钮类

//构造函数的实现
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  //调用基类QMainWindow的构造函数
    , ui(new Ui::MainWindow)  //创建UI定义类的实例
{
    //`setupUi`是Qt UI系统的核心方法，它会读取.ui文件的内容，并将其中定义的所有控件（按钮、标签等）实例化，以及设置好布局和父子关系
    ui->setupUi(this);
    setWindowTitle("扫雷");  //设置窗口标题

    //View是一个被动的接收者，其更新完全由IGameUI接口的方法驱动
}

//析构函数的实现
MainWindow::~MainWindow() {
    clearBoard();  //清理棋盘上的按钮
    delete ui;  //删除ui对象，释放其管理的Designer创建的所有控件
}

//setCommands方法的实现
//这个方法由main.cpp在程序启动时调用，用于将具体的命令处理者（ViewModel）与View关联起来
void MainWindow::setCommands(IGameCommands* commands) {
    m_commands = commands;
    //在命令接口被设置好之后，立即自动开始第一局游戏
    //这通过模拟点击“New Game”按钮来实现
    on_newGameButton_clicked();
}

//清理棋盘的实现
void MainWindow::clearBoard() {
    for (auto& row : m_cellButtons) {
        for (auto* button : row) {
            //从布局中移除控件（如果不移除，控件在视觉上会消失，但仍在布局的管理之下）
            ui->gridLayout->removeWidget(button);
            //显式删除动态创建的QPushButton对象，释放内存
            delete button;
        }
    }
    //清空存储指针的向量
    m_cellButtons.clear();
}

//--- IGameUI 接口的实现 ---

//当棋盘尺寸变化时的实现
void MainWindow::onBoardSizeChanged(const QSize& newSize) {
    clearBoard();  //先清除旧的棋盘
    const int rows = newSize.height();
    const int cols = newSize.width();
    m_cellButtons.resize(rows);  //调整行数

    for (int r = 0; r < rows; ++r) {
        m_cellButtons[r].resize(cols);  //调整列数
        for (int c = 0; c < cols; ++c) {
            //为每个格子动态创建一个新的QPushButton
            //将this作为父对象，这样当MainWindow被销毁时，这些按钮也会被Qt的对象树机制自动清理（作为安全保障）
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(30, 30);  //设置格子大小为固定30×30像素
            button->setFont(QFont("Arial", 12, QFont::Bold));  //设置字体、字号、粗体

            //将按钮的左键点击信号连接到一个Lambda表达式
            //Lambda捕获this指针和当前的行列号(r, c)，当按钮被点击时，它会通过m_commands接口发出“翻开格子”的命令
            connect(button, &QPushButton::clicked, this, [this, r, c]() {
                if (m_commands) m_commands->revealCellRequest(r, c);
            });

            //为按钮安装事件过滤器，让MainWindow可以“监视”它的事件，从而捕获右键点击
            button->installEventFilter(this);
            //使用QObject的动态属性来将行列信息“附加”到按钮上，方便在事件过滤器中读取
            button->setProperty("row", r);
            button->setProperty("col", c);

            //将新创建的按钮添加到.ui文件中定义的网格布局中
            ui->gridLayout->addWidget(button, r, c);
            //将按钮指针存入我们的二维向量中，以便后续通过行列号访问
            m_cellButtons[r][c] = button;
        }
    }
    //在所有按钮都添加到布局后，让窗口自适应到最优尺寸
    adjustSize();
    //然后将这个最优尺寸设为固定大小
    setFixedSize(size());
}

//更新单个格子外观的实现
void MainWindow::onCellUpdated(const CellUpdateInfo &info) {
    //边界检查，确保行列号有效
    if (info.row < m_cellButtons.size() && info.col < m_cellButtons[info.row].size()) {
        QPushButton* button = m_cellButtons[info.row][info.col];
        //根据ViewModel准备好的信息，直接更新按钮的文本、样式和启用状态
        //View在这里只做最简单的“执行”工作，不包含任何逻辑判断
        button->setText(info.text);
        button->setStyleSheet(info.styleSheet);
        button->setEnabled(info.enabled);
    }
}

//显示游戏结束对话框的实现
void MainWindow::onShowGameOverDialog(const QString &message) {
    //使用Qt的静态方法弹出一个标准的信息对话框
    QMessageBox::information(this, "游戏结束", message);
}

//更新旗帜数量标签的实现
void MainWindow::updateFlagsLabel(int flags) {
    ui->flagsLabel->setText(QString("旗帜: %1").arg(flags));
}

//更新状态标签的实现
void MainWindow::updateStatusLabel(const QString& text) {
    ui->statusLabel->setText(text);
}

//--- UI 槽函数的实现 ---

//“New Game”按钮点击事件的槽函数
void MainWindow::on_newGameButton_clicked(){
    //如果命令接口指针有效，则通过它发出“开始新游戏”的命令
    if (m_commands) {
        m_commands->startNewGame(10, 10, 15); // 使用默认难度。
    }
}

//事件过滤器的实现
//标准的QPushButton只提供了一个clicked()信号。这个信号通常由鼠标左键点击触发，它并没有提供专用于右键点击的内置信号
//为响应右键点击，可以子类化QPushButton（继承），但这样会增加类的数量
//也可以使用事件过滤器，不用修改QPushButton，而是让另一个对象（通常是父窗口，即这里的MainWindow）来监视按钮的事件，如果发现是右键点击，则进行拦截与处理（先于button自己的事件处理函数被调用）
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    //检查事件类型是否是鼠标按下
    if (event->type() == QEvent::MouseButtonPress) {
        //检查事件源对象是否是一个QPushButton
        if (auto *button = qobject_cast<QPushButton*>(watched)) {
            //将通用事件安全地转换为鼠标事件
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            //检查是否是右键
            if (mouseEvent->button() == Qt::RightButton) {
                //从按钮的动态属性中读回行列信息
                int row = button->property("row").toInt();
                int col = button->property("col").toInt();
                //通过命令接口发出“插旗”的命令
                if (m_commands) m_commands->toggleFlagRequest(row, col);
                //返回true，表示事件已被处理，不需要再传递给按钮自身，这可以防止右键点击时按钮出现“按下”的视觉效果
                return true;
            }
        }
    }
    //对于所有其他我们不关心的事件，调用基类的实现，让Qt按默认方式处理
    //事件会继续被传递给它原本的目标——button，然后button会按照它自己的标准流程来处理这个事件
    return QMainWindow::eventFilter(watched, event);
}