
#include "HighScoreDialog.h"
#include "ui_HighScoreDialog.h"

#include <QMessageBox>

// HighScoreDialog 的构造函数实现。
HighScoreDialog::HighScoreDialog(QWidget *parent) :
    QDialog(parent), // 调用基类QDialog的构造函数，传递父窗口指针。
    ui(new Ui::HighScoreDialog) // 创建一个Ui::HighScoreDialog类的实例，并将其指针赋值给成员变量ui。
{
    // 调用ui指针的setupUi方法，并传入`this`（即HighScoreDialog实例本身）。
    // 这个方法会读取.ui文件的设计，创建所有UI控件，并将它们布局在当前对话框上。
    ui->setupUi(this);

    // ----------------- UI美化与配置 -----------------
    // 下面的代码用于配置排行榜表格（QTableWidget）的列宽，使其显示效果更佳。

    // 获取表格的水平表头（horizontalHeader），并设置第0列（"模式"列）的调整大小模式。
    // QHeaderView::ResizeToContents 表示该列的宽度将自动调整以适应其内容。
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    // 设置第1列（"难度"列）的调整大小模式。
    // QHeaderView::Stretch 表示该列将自动拉伸以填满表格剩余的可用宽度。
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // 设置第2列（"最佳时间"列）的调整大小模式。
    // 同样设置为根据内容自动调整宽度。
    ui->highScoreTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

// HighScoreDialog 的析构函数实现。
HighScoreDialog::~HighScoreDialog() {
    // 释放ui指针指向的内存。
    // 因为ui对象是在构造函数中用`new`动态分配的，所以必须在析构函数中用`delete`来释放，以防止内存泄漏。
    delete ui;
}

// 公共槽函数 onHighScoresReady 的实现。
// 这个槽函数在接收到ViewModel传来的排行榜数据后被调用。
void HighScoreDialog::onHighScoresReady(const QVector<HighScoreItem>& scores) {
    // 首先将表格的行数设置为0，这会清空表格中所有已存在的内容。
    ui->highScoreTable->setRowCount(0);

    // 使用C++11的范围-for循环（range-based for loop）遍历传入的scores向量。
    // `const auto& score` 表示以只读引用的方式遍历每一个HighScoreItem对象，这样既安全又高效。
    for (const auto& score : scores) {
        // 获取当前表格的总行数，这将作为新插入行的索引。
        int row = ui->highScoreTable->rowCount();

        // 在表格的末尾插入一个新行。
        ui->highScoreTable->insertRow(row);

        // 在新创建的行(row)的第0列，设置一个新的表格项(QTableWidgetItem)。
        // `new QTableWidgetItem(...)` 创建了一个新的项，并用score.mode的内容初始化它。
        // 表格会获得这个新创建项的所有权，并在需要时自动删除它。
        ui->highScoreTable->setItem(row, 0, new QTableWidgetItem(score.mode));

        // 在新创建的行(row)的第1列，设置显示难度信息的表格项。
        ui->highScoreTable->setItem(row, 1, new QTableWidgetItem(score.difficulty));

        // 在新创建的行(row)的第2列，设置显示时间的表格项。
        ui->highScoreTable->setItem(row, 2, new QTableWidgetItem(score.time));
    }
}

// 私有槽函数 on_clearButton_clicked 的实现。
// 这个函数由Qt的自动连接机制在用户点击名为 "clearButton" 的按钮时调用。
void HighScoreDialog::on_clearButton_clicked() {
    // 声明一个变量来存储用户的选择结果。
    QMessageBox::StandardButton reply;

    // 弹出一个标准的“问题”对话框，向用户进行二次确认。
    // `this`: 对话框的父窗口是当前HighScoreDialog实例。
    // "确认": 对话框的标题。
    // "确定要清空...": 对话框中显示的主要问题文本。
    // QMessageBox::Yes | QMessageBox::No: 指定对话框上显示“是”和“否”两个按钮。
    reply = QMessageBox::question(this, "确认", "确定要清空所有排行榜记录吗？此操作不可恢复",
                                  QMessageBox::Yes|QMessageBox::No);

    // 检查用户的选择。
    if (reply == QMessageBox::Yes) {
        // 如果用户点击了“是”按钮，则发射 clearHighScoresRequested 信号。
        // 这个信号将通知外部（App Controller），用户请求执行清空排行榜的操作。
        // 对话框本身不执行清空逻辑，只是发出请求，这体现了职责分离。
        emit clearHighScoresRequested();
    }
}