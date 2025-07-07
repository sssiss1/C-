/*
* GameConfig.h
 *
 * 文件用途：
 * 定义整个项目中使用的全局游戏配置常量
 * 将这些配置放在一个独立的文件中便于管理和修改
 */

#ifndef GAMECONFIG_H
#define GAMECONFIG_H

//自定义模式排行榜中允许保存的最大条目数
//用于限制在QSettings中保存的自定义模式成绩数量
const int MAX_CUSTOM_SCORES = 10;

#endif // GAMECONFIG_H