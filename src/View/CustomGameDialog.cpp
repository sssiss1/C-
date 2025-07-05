#include "CustomGameDialog.h"
#include "ui_CustomGameDialog.h"
#include <QDebug> // 用于调试输出

CustomGameDialog::CustomGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomGameDialog)
{
    ui->setupUi(this);

    // 将行、列输入框的值变化信号连接到更新地雷数范围的槽函数
    connect(ui->rowsSpinBox, &QSpinBox::valueChanged, this, &CustomGameDialog::updateMineCountRange);
    connect(ui->colsSpinBox, &QSpinBox::valueChanged, this, &CustomGameDialog::updateMineCountRange);

    // 初始化地雷数范围
    updateMineCountRange();
}

CustomGameDialog::~CustomGameDialog() {
    delete ui;
}

// 从UI控件中获取值并返回GameDifficulty结构体
GameDifficulty CustomGameDialog::getDifficulty() const {
    return {
        ui->rowsSpinBox->value(),
        ui->colsSpinBox->value(),
        ui->minesSpinBox->value()
    };
}

// 根据当前行数和列数动态更新地雷数的最大值
void CustomGameDialog::updateMineCountRange() {
    int rows = ui->rowsSpinBox->value();
    int cols = ui->colsSpinBox->value();

    // 地雷数上限为总格子数减一，确保至少为首次点击留出一个安全空位
    int maxMines = rows * cols - 1;

    // 检查计算出的maxMines是否小于设定的最小值，防止设置无效范围
    if (maxMines < ui->minesSpinBox->minimum()) {
        maxMines = ui->minesSpinBox->minimum();
    }

    // 更新地雷数输入框的最大值
    ui->minesSpinBox->setMaximum(maxMines);

    // 同时更新标签以提示用户当前的最大值
    ui->label_3->setText(QString("雷数 (10-%1):").arg(maxMines));
}