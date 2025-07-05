#include "GameModel.h"
#include <QRandomGenerator>  // 包含Qt的随机数生成器，用于安全地随机放置地雷
#include <QDebug>  // 包含Qt的调试输出工具

// GameModel的构造函数实现
GameModel::GameModel(QObject *parent) : QObject(parent), m_gameState(GameState::Ready){}

// 仅供测试使用的构造函数实现
GameModel::GameModel(const QVector<QVector<int>>& layout, QObject *parent) : QObject(parent) {
    m_rows = layout.size();
    m_cols = m_rows > 0 ? layout[0].size() : 0;
    m_mineCount = 0;
    m_revealedCount = 0;
    m_gameState = GameState::Playing;  // 为方便测试，直接将游戏状态设置为Playing

    m_grid.assign(m_rows, QVector<Cell>(m_cols));
    for(int r = 0; r < m_rows; ++r) {
        for(int c = 0; c < m_cols; ++c) {
            if(layout[r][c] == -1) {  // 根据传入的布局设置地雷
                m_grid[r][c].isMine = true;
                m_mineCount++;
            }
        }
    }
    calculateAdjacentMines();  // 根据地雷位置计算所有格子的数字
}

// 开始新游戏的实现
void GameModel::startGame(int rows, int cols, int mines) {
    m_rows = rows;
    m_cols = cols;
    m_mineCount = mines;  // 设置行数、列数和地雷数
    m_revealedCount = 0;  // 重置已翻开格子计数
    m_gameState = GameState::Ready;  // 重新设置为准备状态
    // 重置撤销状态
    m_undoUsed = false;
    m_lastFlagAction = LastFlagAction(); // 重置操作记录
    // 使用assign方法重置二维网格，创建一个新的棋盘
    m_grid.assign(m_rows, QVector<Cell>(m_cols));

    // 发出modelChanged信号，通知ViewModel游戏状态已重置
    emit modelChanged();
}

// 放置地雷的实现
void GameModel::placeMines(int firstClickRow, int firstClickCol) {
    int minesToPlace = m_mineCount;  // 需要放置的地雷数量
    QRandomGenerator *rand = QRandomGenerator::global();  // 获取全局随机数生成器实例

    while (minesToPlace > 0) {
        int row = rand->bounded(m_rows);  // 生成一个随机行号
        int col = rand->bounded(m_cols);  // 生成一个随机列号

        // 检查该位置是否可放雷：必须是空地，且不能是玩家首次点击的位置
        if (!m_grid[row][col].isMine && (row != firstClickRow || col != firstClickCol)) {
            m_grid[row][col].isMine = true;  // 在该位置放置一个地雷
            minesToPlace--;  // 待放置的地雷数减一
        }
    }
    calculateAdjacentMines();  // 地雷放置完毕后，计算所有格子周围的地雷数
}

// 计算每个格子周围地雷数量的实现
void GameModel::calculateAdjacentMines() {
    for (int r = 0; r < m_rows; ++r) {  // 遍历每一个格子
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isMine) continue;  // 如果当前格子是地雷，则跳过

            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {  // 遍历周围的8个格子
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;  // 跳过中心格子（自身）
                    if (isValid(r + dr, c + dc) && m_grid[r + dr][c + dc].isMine) {
                        count++;  // 如果相邻格子坐标有效且是地雷，则计数器加一
                    }
                }
            }
            m_grid[r][c].adjacentMines = count;  // 更新当前格子的相邻地雷数
        }
    }
}

// 翻开格子的实现
void GameModel::revealCell(int row, int col) {
    if (!isValid(row, col)) return;  // 边界检查，检查坐标是否有效

    // 处理点击已翻开格子的“清扫”逻辑(双击/中键点击)
    if (m_grid[row][col].isRevealed && m_grid[row][col].adjacentMines > 0) {
        int adjacentFlags = 0;
        for (int dr = -1; dr <= 1; ++dr) {  // 统计周围8个格子的旗帜数量
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                if (isValid(row + dr, col + dc) && m_grid[row + dr][col + dc].isFlagged) {
                    adjacentFlags++;
                }
            }
        }

        // 如果旗帜数等于格子上的数字，则翻开周围所有未标记旗帜的格子
        if (adjacentFlags == m_grid[row][col].adjacentMines) {
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int newRow = row + dr;
                    int newCol = col + dc;
                    // 对周围每个未翻开且未插旗的格子，递归地调用revealCell
                    if (isValid(newRow, newCol) && !m_grid[newRow][newCol].isRevealed) {
                        revealCell(newRow, newCol);
                    }
                }
            }
        }
        return;  // 直接返回，不再执行下面的常规翻开逻辑
    }

    // 常规翻开逻辑：检查格子是否能被翻开
    if (m_grid[row][col].isRevealed || m_grid[row][col].isFlagged || m_gameState == GameState::Won || m_gameState == GameState::Lost) {
        return;
    }

    // 如果这是第一次点击（游戏处于Ready状态）
    if (m_gameState == GameState::Ready) {
        placeMines(row, col);  // 安全地放置地雷，避开第一次点击
        m_gameState = GameState::Playing;  // 游戏状态变为“进行中”
    }

    m_grid[row][col].isQuestionMark = false;  // 翻开格子会清除问号标记
    m_grid[row][col].isFlagged = false;  // 翻开格子会清除旗帜标记
    m_grid[row][col].isRevealed = true;  // 将当前格子标记为“已翻开”

    // 检查是否踩到地雷
    if (m_grid[row][col].isMine) {
        m_gameState = GameState::Lost;  // 游戏状态变为“失败”
        emit modelChanged();  // 触发UI更新，以显示所有地雷
        emit gameOver(false);  // 发出游戏结束信号，参数false表示失败
        return;
    }

    m_revealedCount++;  // 已翻开的非地雷格子数加一

    // 如果翻开的是一个空白格（周围没有地雷），则连锁翻开
    if (m_grid[row][col].adjacentMines == 0) {
        revealEmptyAdjacentCells(row, col);  // 递归地翻开相邻的格子
    }

    checkWinCondition();  // 每次成功翻开后都检查是否胜利
    emit modelChanged();  // 发出信号，通知ViewModel更新UI
}

// 标记/取消标记旗帜的实现
void GameModel::flagCell(int row, int col) {
    // 如果坐标无效，或格子已翻开，或游戏已结束，则不执行任何操作
    if (!isValid(row, col) || m_grid[row][col].isRevealed || m_gameState == GameState::Won || m_gameState == GameState::Lost) {
        return;
    }

    // 记录操作前的状态
    m_lastFlagAction.row = row;
    m_lastFlagAction.col = col;
    m_lastFlagAction.wasFlagged = m_grid[row][col].isFlagged;
    m_lastFlagAction.wasQuestionMark = m_grid[row][col].isQuestionMark;



    if (m_grid[row][col].isFlagged) {  // 如果格子当前已被标记为旗帜，则取消标记
        m_grid[row][col].isFlagged = false;
    } else {  // 否则，尝试标记旗帜
        if (getFlagCount() >= m_mineCount) {  // 如果旗帜已用完，则不作操作
            return;
        }
        m_grid[row][col].isFlagged = true;
        m_grid[row][col].isQuestionMark = false;  // 插旗会覆盖问号
    }

    emit modelChanged();  // 发出信号，通知ViewModel更新UI以显示/隐藏旗帜
}

// 循环切换标记的实现
void GameModel::cycleCellMark(int row, int col) {
    if (!isValid(row, col) || m_grid[row][col].isRevealed || m_gameState == GameState::Won || m_gameState == GameState::Lost) {
        return;
    }

    // 状态循环：空白 -> 问号 -> 空白 (如果原来是旗帜，也会变成问号)
    if (m_grid[row][col].isQuestionMark) {
        m_grid[row][col].isQuestionMark = false;
    } else {
        m_grid[row][col].isQuestionMark = true;
        m_grid[row][col].isFlagged = false;  // 标记问号会覆盖旗帜
    }

    emit modelChanged();  // 发出信号更新UI
}

// 获取指定格子的状态
const Cell& GameModel::getCell(int row, int col) const {
    return m_grid[row][col];  // 直接返回对应格子的只读引用
}

// 获取当前已用旗帜数量的实现
int GameModel::getFlagCount() const {
    int count = 0;
    for (int r = 0; r < m_rows; ++r) {  // 遍历整个棋盘
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isFlagged) {
                count++;  // 统计被标记为旗帜的格子数量
            }
        }
    }
    return count;
}

// 递归翻开空白区域的实现
void GameModel::revealEmptyAdjacentCells(int row, int col) {
    for (int dr = -1; dr <= 1; ++dr) {  // 遍历周围8个格子
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int newRow = row + dr;
            int newCol = col + dc;
            // 如果相邻格子有效，且未被翻开也未被标记
            if (isValid(newRow, newCol) && !m_grid[newRow][newCol].isRevealed && !m_grid[newRow][newCol].isFlagged) {
                 if (!m_grid[newRow][newCol].isMine) {  // 再次确认不是雷（安全检查）
                    m_grid[newRow][newCol].isRevealed = true;  // 翻开它
                    m_revealedCount++;
                    if (m_grid[newRow][newCol].adjacentMines == 0) {
                        revealEmptyAdjacentCells(newRow, newCol);  // 如果新翻开的格子也是空白格，则以它为中心继续递归
                    }
                }
            }
        }
    }
}

// 检查胜利条件的实现
void GameModel::checkWinCondition() {
    // 胜利条件：已翻开的格子数等于总格子数减去地雷数
    if (m_revealedCount == (m_rows * m_cols - m_mineCount)) {
        m_gameState = GameState::Won;  // 游戏状态变为“胜利”
        emit gameOver(true);  // 发出游戏结束信号，参数true表示胜利
    }
}


void GameModel::undoLastFlag() {
    // 检查是否已使用过撤销功能或没有可撤销的操作
    if (m_undoUsed || m_lastFlagAction.row == -1) return;

    // 检查位置是否有效
    if (!isValid(m_lastFlagAction.row, m_lastFlagAction.col)) return;

    Cell& cell = m_grid[m_lastFlagAction.row][m_lastFlagAction.col];

    // 恢复操作前的状态
    cell.isFlagged = m_lastFlagAction.wasFlagged;
    cell.isQuestionMark = m_lastFlagAction.wasQuestionMark;

    // 标记已使用撤销功能
    m_undoUsed = true;

    // 通知视图更新
    emit modelChanged();
}





// 检查坐标是否有效的实现
bool GameModel::isValid(int row, int col) const {
    return row >= 0 && row < m_rows && col >= 0 && col < m_cols;
}