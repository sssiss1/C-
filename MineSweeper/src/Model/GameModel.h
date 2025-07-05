/*
 * GameModel.h
 *
 * 文件用途：
 * 定义GameModel类，这是MVVM中的Model层
 * 它封装了扫雷游戏的核心数据（如棋盘网格、地雷位置）和纯粹的游戏逻辑（如揭开格子、胜负判断）
 * GameModel完全不了解UI的存在，是独立且可重用的游戏引擎
 */

#ifndef MINESWEEPER_GAMEMODEL_H
#define MINESWEEPER_GAMEMODEL_H

#include <QObject>
#include <QVector>
#include <QPoint>

//代表棋盘上一个单元格的数据结构
struct Cell {
    bool isMine = false; //是否是地雷
    bool isRevealed = false; //是否已被揭开
    bool isFlagged = false; //是否被标记为旗帜
    bool isQuestionMark = false; //是否被标记为问号
    int adjacentMines = 0; //周围相邻的地雷数量
};

//游戏状态的枚举
enum class GameState {
    Ready,    //准备开始，等待第一次点击
    Playing,  //游戏中
    Won,      //游戏胜利
    Lost      //游戏失败
};

//游戏模型类，继承自QObject以使用信号和槽
class GameModel : public QObject {
    Q_OBJECT

public:
    //默认构造函数
    explicit GameModel(QObject *parent = nullptr);
    //用于测试的构造函数，可以传入一个预设的棋盘布局
    explicit GameModel(const QVector<QVector<int>>& layout, QObject *parent = nullptr);

    //开始一个新游戏，初始化棋盘状态
    void startGame(int rows, int cols, int mines);
    //揭开指定坐标的单元格
    void revealCell(int row, int col);
    //标记/取消标记指定坐标的单元格为旗帜
    void flagCell(int row, int col);
    //循环切换指定坐标单元格的标记（空白 -> 问号 -> 空白）
    void cycleCellMark(int row, int col);
    //处理提示请求，揭开或标记一个单元格，并返回它之前是否是雷
    bool resolveHint(int row, int col);

    //获取棋盘行数
    int getRows() const { return m_rows; }
    //获取棋盘列数
    int getCols() const { return m_cols; }
    //获取总地雷数
    int getMineCount() const { return m_mineCount; }
    //获取当前已标记的旗帜数量
    int getFlagCount() const;
    //获取指定坐标单元格的只读引用
    const Cell& getCell(int row, int col) const;
    //获取当前游戏状态
    GameState getGameState() const { return m_gameState; }
    //获取当前问号标记的数量
    int getQuestionMarkCount() const;
    //查找并返回第一个问号标记的坐标
    QPoint findFirstQuestionMark() const;

signals:
    //当模型数据发生任何变化时发射此信号
    void modelChanged();
    //当游戏结束（胜利或失败）时发射此信号
    void gameOver(bool victory);

private:
    //在第一次点击后，安全地放置地雷（确保第一次点击不是雷）
    void placeMines(int firstClickRow, int firstClickCol);
    //计算每个非地雷单元格周围的地雷数量
    void calculateAdjacentMines();
    //当揭开一个空白（周围0个雷）单元格时，递归地揭开其相邻的单元格
    void revealEmptyAdjacentCells(int row, int col);
    //检查是否满足胜利条件
    void checkWinCondition();
    //检查给定的坐标是否在棋盘范围内
    bool isValid(int row, int col) const;

    //私有成员变量，存储游戏的核心数据
    int m_rows = 0; //行数
    int m_cols = 0; //列数
    int m_mineCount = 0; //地雷总数
    QVector<QVector<Cell>> m_grid; //存储所有单元格状态的二维向量
    GameState m_gameState = GameState::Ready; //当前游戏状态
    int m_revealedCount = 0; //已揭开的非地雷单元格数量
};

#endif //MINESWEEPER_GAMEMODEL_H