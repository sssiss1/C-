/*
* IGameCommands.h
 *
 * 文件用途：
 * 定义IGameCommands接口（纯虚基类）
 * 这个接口代表了所有从View层发往ViewModel层或App层的用户意图或命令
 * View层通过这个接口与上层通信，实现了View与ViewModel/App的具体实现解耦
 */

#ifndef IGAMECOMMANDS_H
#define IGAMECOMMANDS_H

#include "GameModes.h"

//定义了所有View可以发出的命令的接口
class IGameCommands {
public:
    //虚析构函数，确保派生类对象能被正确销毁
    virtual ~IGameCommands() = default;

    //--- 游戏流程命令 ---
    //请求开始一个新游戏，并指定模式和难度
    virtual void startNewGame(GameMode mode, GameDifficulty difficulty) = 0;
    //请求开始闯关模式的下一关
    virtual void startNextCampaignLevel() = 0;
    //请求返回主菜单界面
    virtual void returnToHomeRequest() = 0;

    //--- 游戏操作命令 ---
    //请求揭开一个单元格
    virtual void revealCellRequest(int row, int col) = 0;
    //请求切换一个单元格的旗帜状态
    virtual void toggleFlagRequest(int row, int col) = 0;
    //请求循环切换一个单元格的标记（正常 -> 问号 -> 正常）
    virtual void cycleMarkRequest(int row, int col) = 0;
    //请求使用提示功能
    virtual void hintRequest() = 0;

    //--- 排行榜命令 ---
    //请求加载排行榜数据
    virtual void loadHighScoresRequest() = 0;
    //请求清空排行榜数据
    virtual void clearHighScoresRequest() = 0;
};
#endif // IGAMECOMMANDS_H