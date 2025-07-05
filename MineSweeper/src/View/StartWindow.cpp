/*
 * StartWindow.cpp
 *
 * 文件用途：
 * 实现StartWindow类的功能
 * 作为View层，它将用户的按钮点击事件转换为对IGameCommands接口的调用
 */

#include "StartWindow.h"
#include "ui_StartWindow.h"
#include "CustomGameDialog.h"
#include "HighScoreDialog.h"
#include "../ViewModel/GameViewModel.h" //此处包含完整定义，因为需要连接其信号

//构造函数
StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

//析构函数
StartWindow::~StartWindow() {
    delete ui;
}

//依赖注入：设置命令接口和ViewModel指针
void StartWindow::setCommands(IGameCommands* appCommands, GameViewModel* vm) {
    m_appCommands = appCommands;
    m_viewModel = vm;
}

//“排行榜”按钮点击槽函数
void StartWindow::on_highScoreButton_clicked() {
    if (!m_viewModel) return;

    //懒加载：如果排行榜对话框尚未创建，则创建它
    if (!m_highScoreDialog) {
        //创建时将ViewModel作为命令接口传入（用于清空操作）
        m_highScoreDialog = new HighScoreDialog(m_viewModel, this);
        //连接ViewModel的highScoresReady信号到对话框的onHighScoresReady槽
        //这样当ViewModel准备好数据后，对话框会自动更新
        connect(m_viewModel, &GameViewModel::highScoresReady, m_highScoreDialog, &HighScoreDialog::onHighScoresReady);
    }

    //每次点击都请求加载最新的排行榜数据
    m_viewModel->loadHighScoresRequest();
    //以模态方式显示对话框
    m_highScoreDialog->exec();
}

//简单模式按钮，通过App命令接口开始一个新游戏
void StartWindow::on_simpleButton_clicked() { if (m_appCommands) m_appCommands->startNewGame(GameMode::Simple, EASY_DIFFICULTY); }
//普通模式按钮
void StartWindow::on_normalButton_clicked() { if (m_appCommands) m_appCommands->startNewGame(GameMode::Normal, NORMAL_DIFFICULTY); }
//困难模式按钮
void StartWindow::on_hardButton_clicked() { if (m_appCommands) m_appCommands->startNewGame(GameMode::Hard, HARD_DIFFICULTY); }
//自定义模式按钮
void StartWindow::on_customButton_clicked() {
    CustomGameDialog dialog(this); //创建自定义设置对话框
    //如果用户点击了“OK”
    if (dialog.exec() == QDialog::Accepted) {
        //获取用户设置的难度，并通过App命令接口开始新游戏
        if (m_appCommands) m_appCommands->startNewGame(GameMode::Custom, dialog.getDifficulty());
    }
}
//闯关模式按钮，难度参数为空结构体，由ViewModel内部处理
void StartWindow::on_campaignButton_clicked() { if (m_appCommands) m_appCommands->startNewGame(GameMode::Campaign, {}); }