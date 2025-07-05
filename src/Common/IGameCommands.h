#ifndef IGAMECOMMANDS_H
#define IGAMECOMMANDS_H

#include "GameModes.h"

// IGameCommands接口定义了所有用户可以发出的命令
// View通过这个接口向ViewModel或App发送请求，实现了UI与逻辑的解耦
class IGameCommands {
public:
    virtual ~IGameCommands() = default;  // 虚析构函数确保正确销毁派生类对象

    // 根据游戏模式和难度开始新游戏
    virtual void startNewGame(GameMode mode, GameDifficulty difficulty) = 0;

    // 闯关模式：开始下一关
    virtual void startNextCampaignLevel() = 0;

    // 请求翻开一个格子
    virtual void revealCellRequest(int row, int col) = 0;
    // 请求切换旗帜标记
    virtual void toggleFlagRequest(int row, int col) = 0;
    // 请求循环切换标记（问号）
    virtual void cycleMarkRequest(int row, int col) = 0;

    // 请求返回主页
    virtual void returnToHomeRequest() = 0;

    // 添加撤销上一步标旗操作的新命令
    virtual void undoLastFlagRequest() = 0;
};
#endif // IGAMECOMMANDS_H