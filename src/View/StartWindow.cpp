#include "StartWindow.h"
#include "ui_StartWindow.h"
#include "CustomGameDialog.h"

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

StartWindow::~StartWindow() {
    delete ui;
}

void StartWindow::on_highScoreButton_clicked() {
    emit highScoresRequested();
}

void StartWindow::on_simpleButton_clicked() { emit gameModeSelected(GameMode::Simple, EASY_DIFFICULTY); }
void StartWindow::on_normalButton_clicked() { emit gameModeSelected(GameMode::Normal, NORMAL_DIFFICULTY); }
void StartWindow::on_hardButton_clicked() { emit gameModeSelected(GameMode::Hard, HARD_DIFFICULTY); }
void StartWindow::on_campaignButton_clicked() { emit gameModeSelected(GameMode::Campaign, {}); }

void StartWindow::on_customButton_clicked() {
    CustomGameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        emit gameModeSelected(GameMode::Custom, dialog.getDifficulty());
    }
}