/*
 * GameModel.h
 *
 * 文件用途：
 * 定义GameModel类，这是MVVM中的Model层。
 * 它封装了扫雷游戏的核心数据（如棋盘网格、地雷位置）和纯粹的游戏逻辑（如揭开格子、胜负判断）。
 * GameModel完全不了解UI的存在，是独立且可重用的游戏引擎。
 */


#ifndef MINESWEEPER_GAMEMODEL_H
#define MINESWEEPER_GAMEMODEL_H


#include <QObject>

#include <QVector>

#include <QPoint>

// 代表棋盘上一个单元格的数据结构。
// 这是一个纯数据结构（POD - Plain Old Data），只包含数据成员，没有复杂的逻辑。
struct Cell {
    bool isMine = false;         // 标记该单元格是否是地雷。C++11 的类内成员初始化。
    bool isRevealed = false;     // 标记该单元格是否已被揭开。
    bool isFlagged = false;      // 标记该单元格是否被插上旗帜。
    bool isQuestionMark = false; // 标记该单元格是否是问号。
    int adjacentMines = 0;       // 存储该单元格周围8个格子中的地雷总数。
};

// 游戏状态的枚举。
// 使用 `enum class` (强类型枚举) 是现代C++的推荐做法，可以避免命名冲突和意外的整数转换。
enum class GameState {
    Ready,    // 准备开始，游戏已初始化但等待玩家第一次点击。
    Playing,  // 游戏中，计时器正在运行。
    Won,      // 游戏胜利。
    Lost      // 游戏失败。
};

// 游戏模型类，继承自QObject以使用信号和槽。
class GameModel : public QObject {
    // Q_OBJECT 是一个必须包含的宏，用于任何定义了自定义信号或槽的 QObject 子类。
    Q_OBJECT

public: // 声明公共接口部分
    // 默认构造函数。
    explicit GameModel(QObject *parent = nullptr);
    // 另一个构造函数，主要用于单元测试。它允许直接传入一个预设的棋盘布局，方便测试特定场景。
    explicit GameModel(const QVector<QVector<int>>& layout, QObject *parent = nullptr);

    // --- 公共的“命令”接口 ---
    // 这些函数由 ViewModel 调用，以改变 Model 的状态。

    // 开始一个新游戏，根据指定的行、列和雷数重置游戏状态。
    void startGame(int rows, int cols, int mines);
    // 揭开指定坐标(row, col)的单元格，包含核心的游戏逻辑。
    void revealCell(int row, int col);
    // 标记或取消标记指定坐标(row, col)的单元格为旗帜。
    void flagCell(int row, int col);
    // 循环切换指定坐标单元格的标记（空白 -> 问号 -> 空白）。
    void cycleCellMark(int row, int col);
    // 处理提示请求。这是一个更复杂的业务逻辑，它会揭开或标记一个单元格。
    bool resolveHint(int row, int col);

    // --- 公共的“查询”接口 (Getters) ---
    // 这些函数用于让外部（ViewModel）安全地获取 Model 的当前状态，而不暴露内部实现。
    // `const` 关键字保证这些函数不会修改类的任何成员变量。

    int getRows() const { return m_rows; }          // 获取棋盘行数。
    int getCols() const { return m_cols; }          // 获取棋盘列数。
    int getMineCount() const { return m_mineCount; } // 获取总地雷数。
    int getFlagCount() const;                        // 获取当前已标记的旗帜数量。
    const Cell& getCell(int row, int col) const;     // 获取指定坐标单元格的只读引用。`const&` 避免了数据拷贝，同时保证了只读。
    GameState getGameState() const { return m_gameState; } // 获取当前游戏状态。
    int getQuestionMarkCount() const;                // 获取当前问号标记的数量。
    QPoint findFirstQuestionMark() const;            // 查找并返回第一个问号标记的坐标。

signals: // 声明信号部分
    // 当模型内部的任何数据发生变化时，都应该发射此信号。这是一个通用的“状态已更新”通知。
    void modelChanged();
    // 当游戏结束（无论是胜利还是失败）时，发射此信号。bool参数用于指明是胜利(true)还是失败(false)。
    void gameOver(bool victory);

private: // 声明私有成员部分，这些是类的内部实现细节，外部不可访问。
    // --- 私有的辅助函数 ---

    // 在玩家第一次点击后，安全地放置地雷（确保第一次点击的位置不是雷）。
    void placeMines(int firstClickRow, int firstClickCol);
    // 计算每个非地雷单元格周围的地雷数量。
    void calculateAdjacentMines();
    // 当揭开一个空白（周围0个雷）单元格时，递归地揭开其所有相邻的单元格。
    void revealEmptyAdjacentCells(int row, int col);
    // 检查是否满足胜利条件（所有非地雷格子都已被揭开）。
    void checkWinCondition();
    // 检查给定的坐标是否在棋盘的有效范围内。
    bool isValid(int row, int col) const;

    // --- 私有成员变量，存储游戏的核心数据 ---
    int m_rows = 0;                        // 棋盘行数。
    int m_cols = 0;                        // 棋盘列数。
    int m_mineCount = 0;                   // 地雷总数。
    QVector<QVector<Cell>> m_grid;       // 存储所有单元格状态的二维向量，即游戏棋盘。
    GameState m_gameState = GameState::Ready; // 当前游戏状态，默认为准备开始。
    int m_revealedCount = 0;               // 已揭开的非地雷单元格数量，用于快速判断胜利条件。
};

#endif //MINESWEEPER_GAMEMODEL_H