//
// Created by eyx on 25-7-3.
//
#ifndef IGAMEUI_H
#define IGAMEUI_H

/*
抽象接口IGameUI，是ViewModel->View的单向通信契约，定义了ViewModel可以对View下达的所有“渲染指令”
任何想要在屏幕上展示游戏界面的类（如MainWindow）必须完成该接口中规定的所有任务
ViewModel通过该接口指挥View，不需要知道View的具体类型
*/

#include <QString>  //包含Qt的字符串类
#include <QSize>   //包含Qt的尺寸类（宽度和高度）

//定义一个数据传输对象（Data Transfer Object，DTO），把多个相关的数据打包成一个独立的结构体，方便在不同层之间一次性传递
//这里，该对象封装了更新单个格子UI所需的所有信息
//它是ViewModel和View之间通信契约的一部分，所以定义在Common层
struct CellUpdateInfo {
    int row, col;  //格子的位置（行、列）
    QString text;  //格子上需要显示的文本（如数字、"🚩"、"💣"）
    QString styleSheet;  //控制格子外观的Qt样式表（CSS），用于改变颜色等
    bool enabled;  //格子是否可点击（已翻开的格子应被禁用）
};

//IGameUI是一个纯虚类（接口），定义了UI层必须对外提供的能力
//ViewModel通过一个指向IGameUI的指针来与View通信，从而实现对具体View类的解耦
//任何实现了这个接口的类，都可以被ViewModel所驱动
class IGameUI {
public:
    //虚析构函数，使用编译器生成的默认析构函数，确保当通过基类指针删除派生类对象时，派生类的析构函数能被正确调用，防止内存泄漏
    virtual ~IGameUI() = default;

    //--- 以下是纯虚函数，构成了接口的“合同” ---
    //“= 0”表明这是一个纯虚函数，意味着这个类本身不能被实例化，并且任何继承自IGameUI的子类都必须提供这个函数的具体实现

    //当游戏棋盘的尺寸发生变化时，ViewModel会调用此方法
    //View需要根据新的尺寸重建其内部的按钮网格
    virtual void onBoardSizeChanged(const QSize& newSize) = 0;

    //当单个格子的状态需要更新时，ViewModel会调用此方法
    //View需要根据传入的CellUpdateInfo更新对应格子的外观
    virtual void onCellUpdated(const CellUpdateInfo& info) = 0;

    //当游戏结束时（胜利或失败），ViewModel会调用此方法
    //View需要弹出一个对话框，向用户显示游戏结果
    virtual void onShowGameOverDialog(const QString& message) = 0;

    //当剩余旗帜数量变化时，ViewModel会调用此方法
    //View需要更新界面上显示旗帜数量的标签
    virtual void updateFlagsLabel(int flags) = 0;

    //当游戏状态文本（如 "进行中"、"胜利"）变化时，ViewModel会调用此方法
    //View需要更新界面上显示状态的标签
    virtual void updateStatusLabel(const QString& text) = 0;
};

#endif // IGAMEUI_H
