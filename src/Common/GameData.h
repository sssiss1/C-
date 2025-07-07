#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QString>
#include <QVector>

// 用于更新单个单元格显示信息的数据结构
struct CellUpdateInfo {
    int row, col;
    QString text;
    QString styleSheet;
    bool enabled;
};

// 用于在排行榜中显示一条记录的数据结构
struct HighScoreItem {
    QString mode;
    QString difficulty;
    QString time;
};

#endif // GAMEDATA_H