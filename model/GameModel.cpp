#include "GameModel.h"
#include <QRandomGenerator>  //Qt的伪随机数生成器：用于安全生成随机地雷位置
#include <QDebug>  //调试输出工具，用于输出日志信息

//构造函数：初始化基类 QObject，并将父对象设置为传入的 parent，将初始游戏状态设为 GameState::Ready（准备状态）
GameModel::GameModel(QObject *parent) : QObject(parent), m_gameState(GameState::Ready) {}

void GameModel::startGame(int rows, int cols, int mines) {  //初始化游戏，参数为行数、列数和地雷数
    //初始化成员变量
    m_rows = rows;
    m_cols = cols;
    m_mineCount = mines;  //m_rows、m_cols、m_mineCount 分别存储游戏配置
    m_revealedCount = 0;  //m_revealedCount 初始化为 0（未翻开任何格子）
    m_gameState = GameState::Ready;  //m_gameState 设为 Ready（准备状态，首次点击前）
    //初始化二维网格
    m_grid.assign(m_rows, QVector<Cell>(m_cols));  //创建 rows × cols 的二维数组，每个元素为默认构造的 Cell（所有状态为 false 或 0）
    //发射信号
    emit modelChanged();  //modelChanged() 通知视图重新渲染整个网格
}

void GameModel::placeMines(int firstClickRow, int firstClickCol) {  //雷区生成：传入首次点击的位置，确保该位置无雷
    int minesToPlace = m_mineCount;  //总共要放置的地雷数量，初始为地雷总量
    QRandomGenerator *rand = QRandomGenerator::global();  //从 Qt 框架中获取一个全局的随机数生成器实例
    //循环在网格中随机放置地雷
    while (minesToPlace > 0) {
        //使用QRandomGenerator生成随机坐标
        int row = rand->bounded(m_rows);  //生成一个 0 ≤ row < m_rows 的随机整数
        int col = rand->bounded(m_cols);  //生成一个 0 ≤ col < m_cols 的随机整数
        //确保不在第一次点击的位置放雷
        if (!m_grid[row][col].isMine && (row != firstClickRow || col != firstClickCol)) {  //选择的格子未放置地雷，且不是首次点击的位置
            m_grid[row][col].isMine = true;  //在选择的格子放置地雷
            minesToPlace--;  //减少要放置的地雷数量
        }
    }
    calculateAdjacentMines();  //重新计算所有格子的相邻地雷数
}

void GameModel::calculateAdjacentMines() {  //数字计算：遍历所有格子，计算并更新每个格子周围的雷数
    //遍历所有格子
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isMine) continue;  //跳过地雷格
            int count = 0;  //初始计数为0
            //检查周围8个相邻格子
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;  //跳过自身
                    if (isValid(r + dr, c + dc) && m_grid[r + dr][c + dc].isMine) {  //位置合法且有地雷
                        count++;  //计数增加
                    }
                }
            }
            m_grid[r][c].adjacentMines = count;  //更新该格的相邻地雷数
        }
    }
}

void GameModel::revealCell(int row, int col) {  //处理玩家左键点击事件，翻开指定格子
    //边界检查与状态验证
    if (!isValid(row, col) ||  //位置不合法
        m_grid[row][col].isRevealed ||  //已翻开
        m_grid[row][col].isFlagged ||  //已标记
        m_gameState == GameState::Won || m_gameState == GameState::Lost  //游戏已结束
       ) {
        return;
    }

    //首次点击处理：初始化雷区
    if (m_gameState == GameState::Ready) {  //当前游戏处于准备状态
        placeMines(row, col);  //进行雷区生成
        m_gameState = GameState::Playing;  //将游戏状态切换为进行中
    }

    //翻开当前格子
    m_grid[row][col].isRevealed = true;  //将当前格子状态设置为已翻开

    //踩雷：游戏失败
    if (m_grid[row][col].isMine) {
        m_gameState = GameState::Lost;  //将游戏状态切换为失败
        emit gameOver(false);  //发射游戏失败信号
        emit modelChanged();  //通知视图更新（显示所有地雷）
        return;  //直接返回
    }

    m_revealedCount++;  //增加已翻开格子数量

    //连锁自动翻开空白区域
    if (m_grid[row][col].adjacentMines == 0) {  //当前格子相邻位置没有地雷
        revealEmptyAdjacentCells(row, col);  //递归翻开相邻空白格
    }

    checkWinCondition();  //检查胜利条件
    emit modelChanged();  //通知视图更新
}

void GameModel::flagCell(int row, int col) {  //处理玩家右键点击事件，标记或取消标记指定格子
    //边界检查与状态验证
    if (!isValid(row, col) ||  //位置不合法
        m_grid[row][col].isRevealed ||  //已翻开
        m_gameState == GameState::Won || m_gameState == GameState::Lost  //游戏已结束
       ) {
        return;
    }
    if (m_gameState == GameState::Ready) {  //若当前游戏状态为准备状态
        m_gameState = GameState::Playing;  //切换到进行中状态
    }

    //切换格子的标记状态
    m_grid[row][col].isFlagged = !m_grid[row][col].isFlagged;  //将格子的标记状态置为与当前相反的状态
    emit modelChanged();  //通知视图更新标记状态
}

const Cell& GameModel::getCell(int row, int col) const {  //返回指定坐标格子的引用
    return m_grid[row][col];
}

int GameModel::getFlagCount() const {  //返回当前标记的地雷数量
    int count = 0;  //初始计数为0
    //遍历所有格子，统计被标记的格子数量
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c].isFlagged) {
                count++;
            }
        }
    }
    return count;  //返回计数
}


void GameModel::revealEmptyAdjacentCells(int row, int col) {  //连锁翻开指定格子周围的空白格子
    //遍历当前格子的8个相邻方向
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;  //跳过自身
            int newRow = row + dr;
            int newCol = col + dc;
            if (isValid(newRow, newCol) &&  //坐标合法
                !m_grid[newRow][newCol].isRevealed &&  //未翻开
                !m_grid[newRow][newCol].isFlagged  //未标记
               ) {
                 if (!m_grid[newRow][newCol].isMine) {  //当前格子不是雷
                    m_grid[newRow][newCol].isRevealed = true;  //翻开当前格子
                    m_revealedCount++;  //增加已翻开格子计数
                    if (m_grid[newRow][newCol].adjacentMines == 0) {  //当前格子也为空白格
                        revealEmptyAdjacentCells(newRow, newCol);  //递归处理
                    }
                }
            }
        }
    }
}

void GameModel::checkWinCondition() {  //胜负判定：非雷格子全部翻开=胜利
    if (m_revealedCount == (m_rows * m_cols - m_mineCount)) {  //所有非雷格子均被翻开
        m_gameState = GameState::Won;  //将游戏状态切换为胜利
        emit gameOver(true);  //发射游戏胜利信号
    }
}


bool GameModel::isValid(int row, int col) const {  //辅助函数：检查行列索引是否在合法范围内
    return row >= 0 && row < m_rows &&
           col >= 0 && col < m_cols;  //边界判断
}