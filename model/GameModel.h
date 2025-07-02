#ifndef MINESWEEPER_GAMEMODEL_H
#define MINESWEEPER_GAMEMODEL_H

#include <QObject>  //Qt框架的核心基类，提供信号/槽机制、对象树管理等功能
#include <QVector>  //Qt的动态数组容器，用于存储二维网格数据

//单个格子的状态
struct Cell {
    bool isMine = false;  //是否是地雷
    bool isRevealed = false;  //是否已翻开
    bool isFlagged = false;  //是否被标记（插旗）
    int adjacentMines = 0;  //8个相邻格子中的地雷数量
};

//游戏状态：Ready → Playing → (Won/Lost)，其中雷区生成在首次点击时触发，并且游戏结束后禁止操作
enum class GameState {
    Ready,  //准备状态（首次点击前）
    Playing,  //游戏进行中
    Won,  //胜利状态
    Lost  //失败状态
};

class GameModel : public QObject {  //继承自 QObject，启用Qt的元对象系统（信号/槽、属性系统等）
    Q_OBJECT  //Q_OBJECT 宏是使用信号/槽机制的必要声明

public:
    explicit GameModel(QObject *parent = nullptr);  //参数 QObject *parent：Qt对象树的父对象指针，默认为空

    //公共接口
    void startGame(int rows, int cols, int mines);  //初始化游戏，参数为行数、列数和地雷数
    void revealCell(int row, int col);  //处理玩家左键点击事件，翻开指定格子
    void flagCell(int row, int col);  //处理玩家右键点击事件，标记或取消标记指定格子

    //Getters
    int getRows() const { return m_rows; }  //返回网格的行数
    int getCols() const { return m_cols; }  //返回网格的列数
    int getMineCount() const { return m_mineCount; }  //返回总地雷数
    int getFlagCount() const;  //返回当前标记的地雷数量
    const Cell& getCell(int row, int col) const;  //返回指定坐标格子的引用
    GameState getGameState() const { return m_gameState; }  //返回当前游戏状态


    signals:
        //信号：当模型数据改变时发出
        void modelChanged();  //数据变化信号：当网格数据（如格子状态、地雷数等）发生变化时发出，通知视图更新
        void gameOver(bool victory);  //游戏结束信号：游戏结束时发出，携带 victory 参数标识胜利或失败

private:
    void placeMines(int firstClickRow, int firstClickCol);  //雷区生成：传入首次点击的位置，确保该位置无雷
    void calculateAdjacentMines();  //数字计算：遍历所有格子，计算并更新每个格子周围的雷数
    void revealEmptyAdjacentCells(int row, int col);  //连锁翻开指定格子周围的空白格子
    void checkWinCondition();  //胜负判定：非雷格子全部翻开=胜利
    bool isValid(int row, int col) const;  //辅助函数：检查行列索引是否在合法范围内

    //核心数据成员
    int m_rows;  //行数
    int m_cols;  //列数
    int m_mineCount;  //地雷总数
    QVector<QVector<Cell>> m_grid;  //二维网格
    GameState m_gameState;  //当前游戏状态
    int m_revealedCount;  //已揭开格子计数
};

#endif //MINESWEEPER_GAMEMODEL_H