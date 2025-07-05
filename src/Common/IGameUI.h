#ifndef IGAMEUI_H
#define IGAMEUI_H

#include <QString>
#include <QSize>
#include "GameModes.h"

// 用于向UI传递单个格子更新所需的所有信息
struct CellUpdateInfo {
    int row, col;  // 格子坐标
    QString text;  // 显示的文本 (数字, 💣, 🚩, ❓)
    QString styleSheet;  // QSS样式表，用于控制颜色等
    bool enabled;  // 格子是否可点击
};

// IGameUI接口定义了ViewModel可以对View进行的所有操作
// ViewModel通过此接口更新UI，实现了ViewModel与具体UI技术的解耦
class IGameUI {
public:
    virtual ~IGameUI() = default;  // 虚析构函数

    // 当棋盘尺寸改变时调用，UI需要重建格子
    virtual void onBoardSizeChanged(const QSize& newSize) = 0;
    // 当单个格子状态改变时调用
    virtual void onCellUpdated(const CellUpdateInfo& info) = 0;
    // 当游戏结束时调用，以显示包含结果信息的对话框
    virtual void onShowGameOverDialog(const QString& message, bool isCampaign, bool isVictory, bool isLastLevel) = 0;
    // 更新剩余旗帜数量的标签
    virtual void updateFlagsLabel(int flags) = 0;
    // 更新游戏状态的标签 (如 "游戏中...")
    virtual void updateStatusLabel(const QString& text) = 0;
    // 在状态栏显示临时消息 (如 "旗帜已用完")
    virtual void showTemporaryMessage(const QString& message, int durationMs) = 0;
    // 根据当前游戏模式配置UI，传递难度和关卡信息
    virtual void configureForMode(GameMode mode, const GameDifficulty& difficulty, int currentLevel, int totalLevels) = 0;
};

#endif // IGAMEUI_H