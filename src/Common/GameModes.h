#ifndef GAMEMODES_H
#define GAMEMODES_H

#include <vector>

// 游戏模式枚举，定义了所有可玩的模式
enum class GameMode {
    Simple,  // 简单
    Normal,  // 普通
    Hard,  // 困难
    Custom,  // 自定义
    Campaign  // 闯关
};

// 游戏难度设置结构体，封装了棋盘的各项参数
struct GameDifficulty {
    int rows;  // 行数
    int cols;  // 列数
    int mines;  // 地雷数
};

// 预定义的难度常量
const GameDifficulty EASY_DIFFICULTY = {9, 9, 10};
const GameDifficulty NORMAL_DIFFICULTY = {16, 16, 40};
const GameDifficulty HARD_DIFFICULTY = {16, 30, 99};

// 闯关模式的关卡定义
const std::vector<GameDifficulty> CAMPAIGN_LEVELS = {
    {8, 8, 1},  // Level 1
    {9, 9, 2},  // Level 2
    {10, 10, 3},  // Level 3
    {11, 11, 4},  // Level 4
    {12, 12, 5}  // Level 5
};

#endif // GAMEMODES_H