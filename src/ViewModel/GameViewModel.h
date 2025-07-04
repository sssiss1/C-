#ifndef MINESWEEPER_GAMEVIEWMODEL_H
#define MINESWEEPER_GAMEVIEWMODEL_H

/*
ViewModel是MVVM架构的“中枢”，是Model和View之间的桥梁和协调者
对Model而言：ViewModel是一个观察者，它监听Model的信号，并在Model数据变化时做出反应
对View而言：ViewModel是一个指令发布者和命令响应者，它通过IGameUI接口向View发送更新指令，并通过IGameCommands接口接收来自View的用户操作请求
*/

#include <QObject>  //包含Qt的核心基类，以使用信号/槽机制来监听Model
#include <QSize>  //包含QSize，这是Model和View之间传递棋盘尺寸的数据类型
#include "../Model/GameModel.h"  //ViewModel需要知道Model的公共接口和信号定义才能与之交互
#include "../common/IGameCommands.h"  //ViewModel需要实现IGameCommands接口，以响应来自View的请求
#include "../common/IGameUI.h"  //ViewModel需要通过IGameUI接口向View发送指令

//GameViewModel类是连接Model和View的桥梁
//它从QObject继承，以使用信号/槽机制连接到Model
//它从IGameCommands继承（并实现），以接收来自View的命令
class GameViewModel : public QObject, public IGameCommands {
    Q_OBJECT  //Qt宏，必须包含，用于启用元对象系统

public:
    //构造函数接收一个对GameModel的引用，是一种“依赖注入”的形式
    //ViewModel不负责创建Model，而是由外部（main.cpp）创建并“注入”进来
    explicit GameViewModel(GameModel& model, QObject *parent = nullptr);

    //提供一个方法来设置ViewModel要与之通信的UI对象
    //参数是一个指向IGameUI接口的指针，这使得ViewModel只知道它在和一个“UI契约”对话，而不知道具体的UI类是什么（如MainWindow）
    void setUI(IGameUI* ui);

    //--- IGameCommands 接口的实现声明 ---
    //override关键字告诉编译器，这些函数意在覆盖基类（IGameCommands）中的纯虚函数
    void startNewGame(int rows, int cols, int mines) override;
    void revealCellRequest(int row, int col) override;
    void toggleFlagRequest(int row, int col) override;

private slots:
    //--- 槽函数 ---
    //这些是私有的槽函数，专门用于响应来自GameModel的信号
    //当GameModel发出相应的信号时，Qt的信号/槽机制会自动调用这些函数
    void onModelChanged();  //连接到GameModel::modelChanged()信号
    void onGameOver(bool victory);  //连接到GameModel::gameOver(bool)信号

private:
    //--- 私有成员变量 ---
    GameModel& m_model;  //存储对注入的Model的引用，使用引用可以确保总有一个有效的Model对象
    IGameUI* m_ui = nullptr;  //存储一个指向UI接口的指针，初始化为nullptr以确保安全
};

#endif //MINESWEEPER_GAMEVIEWMODEL_H