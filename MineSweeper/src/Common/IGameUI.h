/*
* IGameUI.h
 *
 * 文件用途：
 * 定义IGameUI接口（纯虚基类）
 * 这个接口代表了所有从ViewModel层发往View层的UI更新指令
 * ViewModel通过这个接口来驱动View的显示，实现了ViewModel与View具体实现的解耦
 */

#ifndef IGAMEUI_H
#define IGAMEUI_H

#include <QString>
#include <QSize>
#include <QVector>
#include "GameModes.h"

//用于更新单个单元格显示信息的数据结构
struct CellUpdateInfo {
    int row, col; //单元格的坐标
    QString text; //要显示的文本（如数字、旗帜、雷等）
    QString styleSheet; //单元格的Qt样式表（用于设置颜色等）
    bool enabled; //单元格是否可点击
};

//用于在排行榜中显示一条记录的数据结构
struct HighScoreItem {
    QString mode; //游戏模式
    QString difficulty; //游戏难度描述
    QString time; //最佳时间
};

//定义了所有ViewModel可以对View执行的更新操作的接口
class IGameUI {
public:
    //虚析构函数
    virtual ~IGameUI() = default;

    //通知UI棋盘尺寸已改变，UI需要重建棋盘
    virtual void onBoardSizeChanged(const QSize& newSize) = 0;
    //通知UI更新指定单元格的显示
    virtual void onCellUpdated(const CellUpdateInfo& info) = 0;
    //通知UI显示游戏结束对话框
    virtual void onShowGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel) = 0;
    //更新旗帜数量的标签
    virtual void updateFlagsLabel(int flags) = 0;
    //更新游戏状态的标签（如"游戏中..."、"你赢了"）
    virtual void updateStatusLabel(const QString& text) = 0;
    //在状态栏显示一条临时消息
    virtual void showTemporaryMessage(const QString& message, int durationMs) = 0;
    //根据当前游戏模式配置UI（如显示/隐藏特定按钮、设置窗口标题）
    virtual void configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) = 0;
    //更新计时器的显示
    virtual void updateTimerDisplay(int seconds) = 0;
    //设置提示按钮是否可用
    virtual void setHintButtonEnabled(bool enabled) = 0;
};

#endif // IGAMEUI_H