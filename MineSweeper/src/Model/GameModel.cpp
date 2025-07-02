#include "GameModel.h"
#include <QRandomGenerator>  //包含Qt的随机数生成器，用于安全地随机放置地雷
#include <QDebug>  //包含Qt的调试输出工具

//GameModel的构造函数实现
//初始化列表 `: QObject(parent)` 调用基类的构造函数，`m_gameState(GameState::Ready)` 初始化游戏状态为准备就绪
GameModel::GameModel(QObject *parent) : QObject(parent), m_gameState(GameState::Ready){}

//开始新游戏的实现
void GameModel::startGame(int rows, int cols, int mines) {
    //初始化或重置游戏的核心数据
    m_rows = rows;
    m_cols = cols;
    m_mineCount = mines;  //设置行数、列数和地雷数
    m_revealedCount = 0;  //重置已翻开格子计数
    m_gameState = GameState::Ready;  //重新设置为准备状态

    //使用assign方法重置二维网格，创建一个新的rows x cols大小的棋盘，所有Cell都为默认值
    m_grid.assign(m_rows, QVector<Cell>(m_cols));

    //发出modelChanged信号，通知ViewModel游戏状态已重置，UI需要完全刷新
    emit modelChanged();
}

//放置地雷的实现
void GameModel::placeMines(int firstClickRow, int firstClickCol) {
    int minesToPlace = m_mineCount;  //需要放置的地雷数量，初始为地雷总数
    QRandomGenerator *rand = QRandomGenerator::global();  //获取全局随机数生成器实例

    //循环直到所有地雷都已放置
    while (minesToPlace > 0) {
        int row = rand->bounded(m_rows);  //生成一个0到m_rows-1之间的随机行号
        int col = rand->bounded(m_cols);  //生成一个0到m_cols-1之间的随机列号

        //检查该位置是否可以放置地雷：必须是空地，且不能是玩家首次点击的位置
        if (!m_grid[row][col].isMine && (row != firstClickRow || col != firstClickCol)) {
            m_grid[row][col].isMine = true;  //在该位置放置一个地雷
            minesToPlace--;  //待放置的地雷数减一
        }
    }
    //地雷放置完毕后，计算所有格子周围的地雷数
    calculateAdjacentMines();
}

//计算每个格子周围地雷数量的实现
void GameModel::calculateAdjacentMines() {
    //遍历每一个格子
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isMine) continue;  //如果当前格子是地雷，则跳过

            int count = 0;
            //遍历周围的8个格子
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;  //跳过中心格子（自身）
                    //如果相邻格子坐标有效且是地雷，则计数器加一
                    if (isValid(r + dr, c + dc) && m_grid[r + dr][c + dc].isMine) {
                        count++;
                    }
                }
            }
            m_grid[r][c].adjacentMines = count;  //更新当前格子的相邻地雷数
        }
    }
}

//翻开格子的实现
void GameModel::revealCell(int row, int col) {
    //边界检查和状态验证：如果坐标无效，或格子已翻开/已标记，或游戏已结束，则不执行任何操作
    if (!isValid(row, col) || m_grid[row][col].isRevealed || m_grid[row][col].isFlagged || m_gameState == GameState::Won || m_gameState == GameState::Lost) {
        return;
    }

    //如果这是第一次点击（游戏处于Ready状态）
    if (m_gameState == GameState::Ready) {
        placeMines(row, col);  //安全地放置地雷
        m_gameState = GameState::Playing;  //游戏状态变为“进行中”
    }

    m_grid[row][col].isRevealed = true;  //将当前格子标记为“已翻开”

    //检查是否踩到地雷
    if (m_grid[row][col].isMine) {
        m_gameState = GameState::Lost;  //游戏状态变为“失败”
        emit gameOver(false);  //发出游戏结束信号，参数false表示失败
        emit modelChanged();   //触发一次UI更新，以显示所有地雷的位置
        return;
    }

    m_revealedCount++;  //已翻开的非地雷格子数加一

    //如果翻开的是一个空白格（周围没有地雷）
    if (m_grid[row][col].adjacentMines == 0) {
        revealEmptyAdjacentCells(row, col);  //递归地翻开相邻的格子
    }

    checkWinCondition();  //每次成功翻开后都检查是否胜利
    emit modelChanged();  //发出信号，通知ViewModel更新UI
}

//标记/取消标记旗帜的实现
void GameModel::flagCell(int row, int col) {
    //边界检查：如果坐标无效，或格子已翻开，或游戏已结束，则不执行任何操作
    if (!isValid(row, col) || m_grid[row][col].isRevealed || m_gameState == GameState::Won || m_gameState == GameState::Lost) {
        return;
    }

    //切换标记状态
    m_grid[row][col].isFlagged = !m_grid[row][col].isFlagged;
    emit modelChanged();  //发出信号，通知ViewModel更新UI以显示/隐藏旗帜
}

//getCell的实现
const Cell& GameModel::getCell(int row, int col) const {
    //直接返回对应格子的只读引用
    return m_grid[row][col];
}

//getFlagCount的实现
int GameModel::getFlagCount() const {
    int count = 0;
    //遍历整个棋盘，统计被标记为旗帜的格子数量
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isFlagged) {
                count++;
            }
        }
    }
    return count;
}

//递归翻开空白区域的实现
void GameModel::revealEmptyAdjacentCells(int row, int col) {
    //遍历周围8个格子
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int newRow = row + dr;
            int newCol = col + dc;
            //如果相邻格子有效，且未被翻开也未被标记
            if (isValid(newRow, newCol) && !m_grid[newRow][newCol].isRevealed && !m_grid[newRow][newCol].isFlagged) {
                 if (!m_grid[newRow][newCol].isMine) {  //再次确认不是雷（虽然空白格周围肯定不是雷）
                    m_grid[newRow][newCol].isRevealed = true;  //翻开它
                    m_revealedCount++;
                    //如果新翻开的格子也是空白格，则以它为中心继续递归
                    if (m_grid[newRow][newCol].adjacentMines == 0) {
                        revealEmptyAdjacentCells(newRow, newCol);
                    }
                }
            }
        }
    }
}

//检查胜利条件的实现
void GameModel::checkWinCondition() {
    //胜利条件：已翻开的格子数等于总格子数减去地雷数
    if (m_revealedCount == (m_rows * m_cols - m_mineCount)) {
        m_gameState = GameState::Won;  //游戏状态变为“胜利”
        emit gameOver(true);  //发出游戏结束信号，参数true表示胜利
    }
}

//检查坐标是否有效的实现
bool GameModel::isValid(int row, int col) const {
    return row >= 0 && row < m_rows && col >= 0 && col < m_cols;
}