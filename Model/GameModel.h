#ifndef MINESWEEPER_GAMEMODEL_H
#define MINESWEEPER_GAMEMODEL_H

/*
Model是整个应用的核心，封装了所有的数据和业务逻辑，并且与界面（View）完全无关
在扫雷游戏中，GameModel负责管理棋盘状态、地雷位置、胜负判断等所有核心规则
*/

#include <QObject>  //包含Qt的核心基类，GameModel继承自QObject以使用信号/槽机制
#include <QVector>  //包含Qt的动态数组容器，用于高效地存储二维网格数据

//定义了单个格子的所有状态信息，用于存储格子数据
struct Cell {
    bool isMine = false;  //标记这个格子是否是地雷
    bool isRevealed = false;  //标记这个格子是否已被玩家翻开
    bool isFlagged = false;  //标记这个格子是否已被玩家插上旗帜
    int adjacentMines = 0;  //存储该格子周围8个相邻格子中的地雷总数
};

//定义了游戏可能处于的几种状态
enum class GameState {
    Ready,  //准备状态：游戏已初始化，但玩家还未进行第一次点击
    Playing,  //进行中状态：玩家已开始点击，游戏正在进行中
    Won,  //胜利状态：玩家成功翻开所有非地雷格子，游戏胜利
    Lost  //失败状态玩家点到了地雷，游戏失败
};

//GameModel类是游戏的核心逻辑和数据中心
//它继承自QObject，以能够发出信号，通知外界（ViewModel）其内部状态发生了变化
class GameModel : public QObject {
    Q_OBJECT  //一个特殊的Qt宏，必须包含在使用信号/槽的类中，使得MOC（元对象编译器）能够处理这个类

public:
    //构造函数，`explicit` 关键字防止意外的隐式类型转换。
    //`QObject *parent = nullptr` 是Qt对象树机制的标准写法，用于自动内存管理
    explicit GameModel(QObject *parent = nullptr);

    //--- 公共接口 (Public API) ---
    //这些是ViewModel可以调用的方法，用于驱动游戏逻辑

    //开始一局新游戏，并根据指定的参数初始化棋盘
    void startGame(int rows, int cols, int mines);

    //处理玩家翻开一个格子的逻辑
    void revealCell(int row, int col);

    //处理玩家标记/取消标记一个格子的逻辑
    void flagCell(int row, int col);

    //--- Getters (访问器) ---
    //提供对内部状态的只读访问(`const` 关键字表示这些函数不会修改类的任何成员变量)
    int getRows() const { return m_rows; }  //返回棋盘的行数
    int getCols() const { return m_cols; }  //返回棋盘的列数
    int getMineCount() const { return m_mineCount; }  //返回总地雷数
    int getFlagCount() const;  //返回当前已标记旗帜的数量
    const Cell& getCell(int row, int col) const;  //返回指定位置格子的只读引用(避免数据拷贝)
    GameState getGameState() const { return m_gameState; }  //返回当前的游戏状态

signals:
    //--- 信号 ---
    //当模型的状态发生改变时，会发出这些信号,ViewModel可以连接到这些信号来接收通知

    //当棋盘上的任何数据（如格子状态、标记等）发生变化时发出
    //这是一个通用的“刷新”信号，通知监听者需要从Model重新获取数据来更新自己
    void modelChanged();

    //当游戏结束时发出
    //`bool victory` 参数明确告诉监听者游戏是以胜利（true）还是失败（false）结束
    void gameOver(bool victory);

private:
    //--- 私有辅助函数 ---
    //这些函数封装了内部逻辑，不直接暴露给外部

    //在玩家首次点击后，根据点击位置安全地随机布置地雷
    void placeMines(int firstClickRow, int firstClickCol);

    //计算并更新棋盘上每个非地雷格子周围的地雷数量
    void calculateAdjacentMines();

    //当玩家点开一个空白格（周围没有地雷）时，递归地自动翻开所有相邻的空白格
    void revealEmptyAdjacentCells(int row, int col);

    //检查是否满足胜利条件（所有非地雷格子都已被翻开）
    void checkWinCondition();

    //检查给定的坐标是否在棋盘的有效范围内
    bool isValid(int row, int col) const;

    //--- 核心数据成员 ---
    int m_rows = 0;  //棋盘的行数
    int m_cols = 0;  //棋盘的列数
    int m_mineCount = 0;  //游戏设定的地雷总数
    QVector<QVector<Cell>> m_grid;  //存储整个棋盘状态的二维动态数组
    GameState m_gameState = GameState::Ready;  //当前游戏所处的状态
    int m_revealedCount = 0;  //已经翻开的非地雷格子计数，用于快速判断胜利条件
};

#endif //MINESWEEPER_GAMEMODEL_H