/*
* MineSweeperApp.h
 *
 * 文件用途：
 * 定义MinesweeperApp类，这是整个应用程序的顶层控制器（Coordinator）
 * 它不属于MVVM中的任何一层，但负责创建和组装Model、View和ViewModel
 * 它还负责处理最高级别的应用流程，例如窗口之间的切换
 * 它部分实现了IGameCommands接口，专门处理那些需要改变应用顶层状态的命令（如开始新游戏、返回主页）
 */

#ifndef MINESWEEPERAPP_H
#define MINESWEEPERAPP_H

#include "../Model/GameModel.h"
#include "../ViewModel/GameViewModel.h"
#include "../View/StartWindow.h"
#include "../View/MainWindow.h"

//MinesweeperApp类作为应用的总控制器，并实现了部分游戏命令接口
class MinesweeperApp : public IGameCommands {
public:
    //构造函数，负责初始化和组装所有组件
    MinesweeperApp();
    //析构函数
    ~MinesweeperApp();
    //启动应用程序的公共方法
    void run();

    //--- IGameCommands 接口实现 ---
    //这些是App自身需要处理的命令，因为它们涉及到窗口的显示和隐藏

    //开始一个新游戏，这会隐藏开始窗口并显示游戏主窗口
    void startNewGame(GameMode mode, GameDifficulty difficulty) override;
    //请求返回主菜单，这会隐藏游戏主窗口并显示开始窗口
    void returnToHomeRequest() override;

    //--- 其他命令不归App管，提供空实现 ---
    //这些命令由ViewModel处理，App层面不需要关心其具体逻辑，因此提供空实现以满足接口要求
    void startNextCampaignLevel() override {}
    void revealCellRequest(int, int) override {}
    void toggleFlagRequest(int, int) override {}
    void cycleMarkRequest(int, int) override {}
    void hintRequest() override {} //为新增的提示功能提供空实现
    void loadHighScoresRequest() override {}
    void clearHighScoresRequest() override {}

private:
    //M - 游戏核心数据和逻辑模型
    GameModel m_model;
    //VM - 连接Model和View的视图模型
    GameViewModel m_viewModel;
    //V - 开始窗口视图
    StartWindow m_startWindow;
    //V - 游戏主窗口视图
    MainWindow m_gameWindow;
};

#endif //MINESWEEPERAPP_H