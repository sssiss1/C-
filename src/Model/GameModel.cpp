/*
 * GameModel.cpp
 *
 * 文件用途：
 * 实现GameModel类的所有功能
 * 包含游戏的核心逻辑，如布雷、揭开、连锁揭开、胜负判断等
 * 所有操作都只针对内部数据，不涉及任何UI交互
 */

#include "GameModel.h"
#include <QRandomGenerator>
#include <QDebug>

//默认构造函数
GameModel::GameModel(QObject *parent) : QObject(parent), m_gameState(GameState::Ready){}

//用于测试的构造函数，根据传入的布局初始化棋盘
GameModel::GameModel(const QVector<QVector<int>>& layout, QObject *parent) : QObject(parent) {
    m_rows = layout.size();
    m_cols = m_rows > 0 ? layout[0].size() : 0;
    m_mineCount = 0;
    m_revealedCount = 0;
    m_gameState = GameState::Playing;

    m_grid.assign(m_rows, QVector<Cell>(m_cols));
    for(int r = 0; r < m_rows; ++r) {
        for(int c = 0; c < m_cols; ++c) {
            if(layout[r][c] == -1) { //布局中-1代表地雷
                m_grid[r][c].isMine = true;
                m_mineCount++;
            }
        }
    }
    calculateAdjacentMines(); //计算邻近地雷数
}

//初始化新游戏
void GameModel::startGame(int rows, int cols, int mines) {
    m_rows = rows;
    m_cols = cols;
    m_mineCount = mines;
    m_revealedCount = 0;
    m_gameState = GameState::Ready; //游戏状态设置为准备就绪
    m_grid.assign(m_rows, QVector<Cell>(m_cols)); //重置棋盘网格
    emit modelChanged(); //发出模型变化信号，通知外部（ViewModel）更新
}

//放置地雷，确保第一次点击的位置不是雷
void GameModel::placeMines(int firstClickRow, int firstClickCol) {
    int minesToPlace = m_mineCount;
    QRandomGenerator *rand = QRandomGenerator::global(); //获取全局随机数生成器

    while (minesToPlace > 0) {
        int row = rand->bounded(m_rows); //随机生成行
        int col = rand->bounded(m_cols); //随机生成列
        //如果该位置不是雷，并且不是第一次点击的位置，则放置一个雷
        if (!m_grid[row][col].isMine && (row != firstClickRow || col != firstClickCol)) {
            m_grid[row][col].isMine = true;
            minesToPlace--;
        }
    }
    calculateAdjacentMines(); //地雷放置完毕后，计算所有格子的邻近地雷数
}

//计算所有格子的邻近地雷数
void GameModel::calculateAdjacentMines() {
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isMine) continue; //地雷格本身不需要计算
            int count = 0;
            //遍历周围8个格子
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue; //跳过自身
                    //如果邻近格子有效且是雷，则计数加一
                    if (isValid(r + dr, c + dc) && m_grid[r + dr][c + dc].isMine) {
                        count++;
                    }
                }
            }
            m_grid[r][c].adjacentMines = count;
        }
    }
}

//揭开一个格子
void GameModel::revealCell(int row, int col) {
    if (!isValid(row, col)) return; //坐标无效则返回

    //如果点击一个已揭开且有数字的格子，执行"双击"或"中键"逻辑（Chord）
    if (m_grid[row][col].isRevealed && m_grid[row][col].adjacentMines > 0) {
        int adjacentFlags = 0;
        //计算周围旗帜数量
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                if (isValid(row + dr, col + dc) && m_grid[row + dr][col + dc].isFlagged) {
                    adjacentFlags++;
                }
            }
        }
        //如果旗帜数等于数字，则揭开周围未揭开的格子
        if (adjacentFlags == m_grid[row][col].adjacentMines) {
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int newRow = row + dr;
                    int newCol = col + dc;
                    //递归调用revealCell来揭开周围格子
                    if (isValid(newRow, newCol) && !m_grid[newRow][newCol].isRevealed) {
                        revealCell(newRow, newCol);
                    }
                }
            }
        }
        return;
    }

    //如果格子已揭开、被标记或游戏已结束，则不执行任何操作
    if (m_grid[row][col].isRevealed || m_grid[row][col].isFlagged || m_gameState == GameState::Won || m_gameState == GameState::Lost) return;

    //如果是第一次点击
    if (m_gameState == GameState::Ready) {
        placeMines(row, col); //安全地放置地雷
        m_gameState = GameState::Playing; //游戏状态变为进行中
    }

    //揭开格子，并清除其上的标记
    m_grid[row][col].isQuestionMark = false;
    m_grid[row][col].isFlagged = false;
    m_grid[row][col].isRevealed = true;

    //如果点到地雷
    if (m_grid[row][col].isMine) {
        m_gameState = GameState::Lost; //游戏失败
        emit modelChanged(); //通知模型变化（以显示所有雷）
        emit gameOver(false); //通知游戏结束，参数为false表示失败
        return;
    }

    //成功揭开一个安全的格子
    m_revealedCount++;
    //如果揭开的是空白格（周围没有雷），则自动揭开相邻的格子
    if (m_grid[row][col].adjacentMines == 0) {
        revealEmptyAdjacentCells(row, col);
    }

    checkWinCondition(); //每次揭开后检查是否胜利
    emit modelChanged(); //通知模型变化
}

//标记旗帜
void GameModel::flagCell(int row, int col) {
    if (!isValid(row, col) || m_grid[row][col].isRevealed || m_gameState == GameState::Won || m_gameState == GameState::Lost) return;

    if (m_grid[row][col].isFlagged) {
        m_grid[row][col].isFlagged = false; //再次点击取消旗帜
    } else {
        if (getFlagCount() >= m_mineCount) return; //如果旗帜已用完，则不能再插
        m_grid[row][col].isFlagged = true; //插旗
        m_grid[row][col].isQuestionMark = false; //清除问号标记
    }
    emit modelChanged(); //通知模型变化
}

//循环标记（问号）
void GameModel::cycleCellMark(int row, int col) {
    if (!isValid(row, col) || m_grid[row][col].isRevealed || m_gameState == GameState::Won || m_gameState == GameState::Lost) return;

    if (m_grid[row][col].isQuestionMark) {
        m_grid[row][col].isQuestionMark = false; //问号 -> 空白
    } else {
        m_grid[row][col].isQuestionMark = true; //空白 -> 问号
        m_grid[row][col].isFlagged = false; //清除旗帜标记
    }
    emit modelChanged(); //通知模型变化
}

//获取指定格子的常量引用
const Cell& GameModel::getCell(int row, int col) const { return m_grid[row][col]; }

//计算当前旗帜数量
int GameModel::getFlagCount() const {
    int count = 0;
    for (const auto& row : m_grid) for (const auto& cell : row) if (cell.isFlagged) count++;
    return count;
}

//递归揭开空白格子周围的格子
void GameModel::revealEmptyAdjacentCells(int row, int col) {
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int newRow = row + dr;
            int newCol = col + dc;
            //如果邻近格子有效、未被揭开且未被标记
            if (isValid(newRow, newCol) && !m_grid[newRow][newCol].isRevealed && !m_grid[newRow][newCol].isFlagged) {
                 if (!m_grid[newRow][newCol].isMine) { //确保不会揭开雷（理论上空白格周围不会有）
                    m_grid[newRow][newCol].isRevealed = true;
                    m_revealedCount++;
                    //如果新揭开的格子也是空白格，则继续递归
                    if (m_grid[newRow][newCol].adjacentMines == 0) {
                        revealEmptyAdjacentCells(newRow, newCol);
                    }
                }
            }
        }
    }
}

//检查胜利条件
void GameModel::checkWinCondition() {
    //当揭开的格子数等于总格子数减去地雷数时，游戏胜利
    if (m_revealedCount == (m_rows * m_cols - m_mineCount)) {
        m_gameState = GameState::Won;
        emit gameOver(true); //通知游戏胜利
    }
}

//检查坐标是否有效
bool GameModel::isValid(int row, int col) const {
    return row >= 0 && row < m_rows && col >= 0 && col < m_cols;
}

//处理提示请求
bool GameModel::resolveHint(int row, int col) {
    //如果格子无效或已揭开，提示失败
    if (!isValid(row, col) || m_grid[row][col].isRevealed) {
        return false;
    }

    //记录这个格子原本是不是雷
    bool wasMine = m_grid[row][col].isMine;

    if (wasMine) {
        //如果是雷，且旗帜没用完，就帮忙插上旗
        if (getFlagCount() < m_mineCount) {
             m_grid[row][col].isFlagged = true;
             m_grid[row][col].isQuestionMark = false; //清除问号
        }
    } else {
        //如果不是雷，就直接揭开它
        revealCell(row, col);
    }

    emit modelChanged(); //通知模型变化
    return wasMine; //返回它是否是雷，用于ViewModel向UI显示不同提示信息
}

//获取问号标记的数量
int GameModel::getQuestionMarkCount() const {
    int count = 0;
    for (const auto& row : m_grid) {
        for (const auto& cell : row) {
            if (cell.isQuestionMark) {
                count++;
            }
        }
    }
    return count;
}

//查找第一个问号标记的位置
QPoint GameModel::findFirstQuestionMark() const {
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isQuestionMark) {
                return QPoint(c, r); //返回坐标
            }
        }
    }
    return QPoint(-1, -1); //没找到返回无效坐标
}