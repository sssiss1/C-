/*
* GameModes.h
 *
 * 文件用途：
 * 定义游戏模式、难度等核心枚举和结构体
 * 这些定义是类型安全的，并在整个项目中共享，确保各层之间对游戏配置的理解一致
 */

#ifndef GAMEMODES_H
#define GAMEMODES_H

#include <vector>

//游戏模式枚举，定义了所有可玩的模式
enum class GameMode {
    Simple,  //简单模式
    Normal,  //普通模式
    Hard,  //困难模式
    Custom,  //自定义模式
    Campaign  //闯关模式
};

//游戏难度设置结构体，封装了棋盘的各项参数
struct GameDifficulty {
    int rows;  //棋盘行数
    int cols;  //棋盘列数
    int mines;  //地雷总数
};

//预定义的难度常量，方便直接使用
const GameDifficulty EASY_DIFFICULTY = {9, 9, 10};
const GameDifficulty NORMAL_DIFFICULTY = {16, 16, 40};
const GameDifficulty HARD_DIFFICULTY = {16, 30, 99};

//闯关模式的关卡定义
//使用std::vector存储一系列GameDifficulty，代表每个关卡的配置
const std::vector<GameDifficulty> CAMPAIGN_LEVELS = {
    {8, 8, 1},  //Level 1
    {9, 9, 2},  //Level 2
    {10, 10, 3},  //Level 3
    {11, 11, 4},  //Level 4
    {12, 12, 5}  //Level 5
};

#endif // GAMEMODES_H