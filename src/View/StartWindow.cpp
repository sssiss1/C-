#include "StartWindow.h"
#include "ui_StartWindow.h"
#include "CustomGameDialog.h"
#include "HighScoreDialog.h"

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow),
    m_highScoreDialog(nullptr) {
    ui->setupUi(this);

    // 添加排行榜按钮
    QPushButton* highScoreButton = new QPushButton("排行榜", this);
    highScoreButton->setMinimumSize(0, 40);
    connect(highScoreButton, &QPushButton::clicked, this, &StartWindow::on_highScoreButton_clicked);
    ui->verticalLayout->insertWidget(6, highScoreButton);
}

void StartWindow::on_highScoreButton_clicked() {
    if (!m_highScoreDialog) {
        m_highScoreDialog = new HighScoreDialog(this);
    }
    m_highScoreDialog->exec();
}

StartWindow::~StartWindow() {
    delete ui;
    if (m_highScoreDialog) delete m_highScoreDialog;
}

// 接收并保存命令接口的指针
void StartWindow::setCommands(IGameCommands* commands) {
    m_commands = commands;
}

// "简单模式"按钮点击处理
void StartWindow::on_simpleButton_clicked() {
    if (m_commands) m_commands->startNewGame(GameMode::Simple, EASY_DIFFICULTY);
}

// "普通模式"按钮点击处理
void StartWindow::on_normalButton_clicked() {
    if (m_commands) m_commands->startNewGame(GameMode::Normal, NORMAL_DIFFICULTY);
}

// "困难模式"按钮点击处理
void StartWindow::on_hardButton_clicked() {
    if (m_commands) m_commands->startNewGame(GameMode::Hard, HARD_DIFFICULTY);
}

// "自定义模式"按钮点击处理
void StartWindow::on_customButton_clicked() {
    CustomGameDialog dialog(this);  // 创建自定义游戏对话框
    if (dialog.exec() == QDialog::Accepted) {  // 如果用户点击OK
        if (m_commands) m_commands->startNewGame(GameMode::Custom, dialog.getDifficulty());
    }
}

// "闯关模式"按钮点击处理
void StartWindow::on_campaignButton_clicked() {
    // 闯关模式的初始难度由ViewModel决定，这里传递一个空对象
    if (m_commands) m_commands->startNewGame(GameMode::Campaign, {});
}
