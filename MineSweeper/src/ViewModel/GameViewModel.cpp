#include "GameViewModel.h"

//GameViewModel的构造函数实现
GameViewModel::GameViewModel(GameModel& model, QObject *parent)
    : QObject(parent), m_model(model) {
    //--- 核心连接逻辑 ---
    //在构造函数中，建立ViewModel和Model之间的信号/槽连接，这样，一旦ViewModel被创建，它就会自动开始“监听”Model

    //将Model的modelChanged信号连接到ViewModel的onModelChanged槽
    //当Model的数据发生任何变化时，onModelChanged函数就会被调用
    connect(&m_model, &GameModel::modelChanged, this, &GameViewModel::onModelChanged);

    //将Model的gameOver信号连接到ViewModel的onGameOver槽
    //当Model判断游戏结束时，onGameOver函数就会被调用
    connect(&m_model, &GameModel::gameOver, this, &GameViewModel::onGameOver);
}

//setUI方法的实现
//这个方法由main.cpp在程序启动时调用，用于将具体的View实例（如MainWindow）与ViewModel关联起来
void GameViewModel::setUI(IGameUI* ui) {
    m_ui = ui;
}

//--- IGameCommands 接口的实现 ---

//startNewGame命令的实现
void GameViewModel::startNewGame(int rows, int cols, int mines) {
    //ViewModel将业务逻辑委托给Model处理
    m_model.startGame(rows, cols, mines);

    //在 Model初始化后，ViewModel主动向UI发送初始化的渲染指令
    if (m_ui) {
        m_ui->updateStatusLabel("游戏中...");
        //QSize的构造(宽度, 高度)对应(列数, 行数)
        m_ui->onBoardSizeChanged(QSize(cols, rows));
    }
}

//revealCellRequest命令的实现
void GameViewModel::revealCellRequest(int row, int col) {
    //这是一个简单的“直通”命令：直接将View的请求转发给Model的相应方法
    m_model.revealCell(row, col);
}

//toggleFlagRequest命令的实现
////修改
void GameViewModel::toggleFlagRequest(int row, int col) {
    //在调用Model的flagCell之前，检查旗帜是否已用完
    const int currentFlags = m_model.getFlagCount();
    const int mineCount = m_model.getMineCount();
    const bool isFlagged = m_model.getCell(row, col).isFlagged;

    if (!isFlagged && currentFlags >= mineCount) {
        //如果想新插旗但旗帜已用完，则通过UI接口显示临时消息
        if (m_ui) {
            m_ui->showTemporaryMessage("旗帜已用完! 没有更多地雷了!", 2000); // 显示2秒
        }
        return;  //不再调用model.flagCell
    }

    m_model.flagCell(row, col);
}

//cycleMarkRequest的实现
////新增
void GameViewModel::cycleMarkRequest(int row, int col) {
    m_model.cycleCellMark(row, col);
}

//--- 槽函数的实现 ---

//onModelChanged槽的实现
////修改
void GameViewModel::onModelChanged() {
    //如果没有关联的UI，则不执行任何操作
    if (!m_ui) return;

    //从Model获取摘要信息（剩余旗帜数）
    const int flags = m_model.getMineCount() - m_model.getFlagCount();
    //通过UI接口更新对应的标签
    m_ui->updateFlagsLabel(flags);

    //遍历Model中的每一个格子，将其状态“翻译”成UI更新指令
    for (int r = 0; r < m_model.getRows(); ++r) {
        for (int c = 0; c < m_model.getCols(); ++c) {
            //从Model获取格子数据
            const Cell& cell = m_model.getCell(r, c);

            //创建一个DTO对象来打包所有UI更新信息，先为其设置一个默认值（未翻开的灰色格子）
            CellUpdateInfo info{r, c, "", "background-color: #c0c0c0;", true};

            //根据Model的状态，决定格子的具体外观（ViewModel的“翻译”工作）
            if (m_model.getGameState() == GameState::Lost && cell.isMine) {
                info.text = "💣";
                info.styleSheet = "background-color: red;";
            } else if (cell.isFlagged) {
                info.text = "🚩";
            } else if (cell.isQuestionMark) {  //对问号状态的判断
                info.text = "❓";
            } else if (cell.isRevealed) {
                //这里不再令已翻开的格子不可被点击
                info.styleSheet = "background-color: #e0e0e0; border: 1px solid #808080;";
                if (cell.adjacentMines > 0) {
                    info.text = QString::number(cell.adjacentMines);
                    //根据数字设置不同的颜色
                    switch (cell.adjacentMines) {
                        case 1: info.styleSheet += "color: blue;"; break;
                        case 2: info.styleSheet += "color: green;"; break;
                        case 3: info.styleSheet += "color: red;"; break;
                        case 4: info.styleSheet += "color: darkblue;"; break;
                        case 5: info.styleSheet += "color: brown;"; break;
                        default: info.styleSheet += "color: black;"; break;
                    }
                }
            }
            //为每个格子都通过UI接口发送一个更新指令
            m_ui->onCellUpdated(info);
        }
    }
}

//onGameOver槽的实现
void GameViewModel::onGameOver(bool victory) {
    if (!m_ui) return;

    //根据Model传递过来的胜利/失败结果，准备不同的提示信息
    if (victory) {
        m_ui->updateStatusLabel("你赢了! :)");
        m_ui->onShowGameOverDialog("恭喜! 你成功清除了所有地雷!");
    }
    else {
        m_ui->updateStatusLabel("你输了! :(");
        m_ui->onShowGameOverDialog("砰! 你踩到地雷了!");
    }
}