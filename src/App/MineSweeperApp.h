#ifndef MINESWEEPERAPP_H
#define MINESWEEPERAPP_H

#include "../Model/GameModel.h"
#include "../ViewModel/GameViewModel.h"
#include "../View/MainWindow.h"

//MinesweeperApp类封装了整个应用程序的生命周期和核心组件
//它作为“组合根”，负责创建和组装Model, View, 和ViewModel三层
class MinesweeperApp {
public:
    //构造函数：在这里完成所有对象的创建和依赖注入
    MinesweeperApp();
    //析构函数
    ~MinesweeperApp();

    //对外暴露的接口，用于显示主窗口并启动应用程序
    void run();

private:
    //将三层核心对象作为类的成员变量
    //这确保了它们的生命周期与App对象绑定，当App对象被销毁时，它们也会被自动销毁
    GameModel m_model;  //数据模型实例
    GameViewModel m_viewModel;  //视图模型实例
    MainWindow m_mainWindow;  //主窗口视图实例
};

#endif //MINESWEEPERAPP_H