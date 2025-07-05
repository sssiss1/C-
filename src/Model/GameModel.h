#ifndef MINESWEEPER_GAMEMODEL_H
#define MINESWEEPER_GAMEMODEL_H

/*
Model是整个应用的核心，封装了所有的数据和业务逻辑
它与界面（View）完全无关，只负责管理游戏规则和状态
*/

#include <QObject>  // 包含Qt的核心基类，以使用信号/槽机制
#include <QVector>  // 包含Qt的动态数组容器，用于存储二维网格数据

// 定义了单个格子的所有状态信息
struct Cell {
    bool isMine = false;  // 标记这个格子是否是地雷
    bool isRevealed = false;  // 标记这个格子是否已被玩家翻开
    bool isFlagged = false;  // 标记这个格子是否已被玩家插上旗帜
    bool isQuestionMark = false;  // 标记这个格子是否为问号
    int adjacentMines = 0;  // 存储该格子周围8个相邻格子中的地雷总数
};

// 定义了游戏可能处于的几种状态
enum class GameState {
    Ready,  // 准备状态：游戏已初始化，但玩家还未进行第一次点击
    Playing,  // 进行中状态：玩家已开始点击，游戏正在进行中
    Won,  // 胜利状态：玩家成功翻开所有非地雷格子
    Lost  // 失败状态：玩家点到了地雷
};

// GameModel类是游戏的核心逻辑和数据中心
// 它继承自QObject，以便能发出信号，通知外界（ViewModel）其内部状态发生了变化
class GameModel : public QObject {
    Q_OBJECT  // Qt宏，必须包含在使用信号/槽的类中，以启用元对象系统

public:
    // 构造函数，`explicit`关键字防止意外的隐式类型转换
    explicit GameModel(QObject *parent = nullptr);

    // 仅供测试用的构造函数，允许直接传入一个布局来初始化棋盘
    explicit GameModel(const QVector<QVector<int>>& layout, QObject *parent = nullptr);

    // --- 公共接口 (Public API) ---
    // 这些是ViewModel可以调用的方法，用于驱动游戏逻辑

    // 开始一局新游戏，并根据指定的参数初始化棋盘
    void startGame(int rows, int cols, int mines);

    // 处理玩家翻开一个格子的逻辑
    void revealCell(int row, int col);

    // 处理玩家标记/取消标记一个格子的逻辑
    void flagCell(int row, int col);

    // 处理玩家循环标记一个格子的逻辑（空白 -> 问号 -> 空白）
    void cycleCellMark(int row, int col);

    // 添加撤销上一步标旗操作的方法
    void undoLastFlag();





    // --- Getters (访问器) ---
    // 提供对内部状态的只读访问(`const`表示这些函数不会修改类成员)
    int getRows() const { return m_rows; }  // 返回棋盘的行数
    int getCols() const { return m_cols; }  // 返回棋盘的列数
    int getMineCount() const { return m_mineCount; }  // 返回总地雷数
    int getFlagCount() const;  // 返回当前已标记旗帜的数量
    const Cell& getCell(int row, int col) const;  // 返回指定位置格子的只读引用(避免数据拷贝)
    GameState getGameState() const { return m_gameState; }  // 返回当前的游戏状态

signals:
    // --- 信号 ---
    // 当模型的状态发生改变时，会发出这些信号,ViewModel可以连接到这些信号来接收通知

    // 当棋盘上的任何数据发生变化时发出，通知监听者需要刷新
    void modelChanged();

    // 当游戏结束时发出，参数`victory`指明是胜利还是失败
    void gameOver(bool victory);

private:
    // --- 私有辅助函数 ---
    // 这些函数封装了内部逻辑，不直接暴露给外部

    // 在玩家首次点击后，根据点击位置安全地随机布置地雷
    void placeMines(int firstClickRow, int firstClickCol);

    // 计算并更新棋盘上每个非地雷格子周围的地雷数量
    void calculateAdjacentMines();

    // 当玩家点开一个空白格时，递归地自动翻开所有相邻的空白格
    void revealEmptyAdjacentCells(int row, int col);

    // 检查是否满足胜利条件
    void checkWinCondition();

    // 检查给定的坐标是否在棋盘的有效范围内
    bool isValid(int row, int col) const;

    // 添加记录最后一次标记操作的数据结构
    struct LastFlagAction {
        int row = -1;
        int col = -1;
        bool wasFlagged = false; // 操作前的状态
        bool wasQuestionMark = false; // 操作前的状态
    };
    LastFlagAction m_lastFlagAction; // 记录最后一次标记操作
    bool m_undoUsed = false; // 标记本局是否已使用过撤销功能



    // --- 核心数据成员 ---
    int m_rows = 0;  // 棋盘的行数
    int m_cols = 0;  // 棋盘的列数
    int m_mineCount = 0;  // 游戏设定的地雷总数
    QVector<QVector<Cell>> m_grid;  // 存储整个棋盘状态的二维动态数组
    GameState m_gameState = GameState::Ready;  // 当前游戏所处的状态
    int m_revealedCount = 0;  // 已翻开的非地雷格子计数，用于快速判断胜利条件
};

#endif //MINESWEEPER_GAMEMODEL_H