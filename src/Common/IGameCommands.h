#ifndef IGAMECOMMANDS_H
#define IGAMECOMMANDS_H

/*
抽象接口IGameCommands，是View->ViewModel的单向通信契约，定义了View可以向ViewModel发出的所有“用户操作命令”
任何处理游戏逻辑的类（这里的GameViewModel）必须能够相应该接口中规定的所有命令
View通过这个接口来向ViewModel发出指令，不需要知道ViewModel的具体类型
*/

//IGameCommands 是一个纯虚类（接口），定义了View可以向逻辑层发出的所有“命令”
//View通过一个指向IGameCommands的指针来调用ViewModel的功能，从而实现对具体ViewModel类的解耦
//任何实现了这个接口的类，都可以接收并处理来自View的用户操作请求
class IGameCommands {
public:
    //虚析构函数，使用编译器生成的默认析构函数，确保当通过基类指针删除派生类对象时，派生类的析构函数能被正确调用，防止内存泄漏
    virtual ~IGameCommands() = default;

    //--- 以下是纯虚函数，构成了命令接口的“合同” ---

    //View调用此命令来请求开始一局新游戏
    //参数定义了新游戏的难度（行数、列数、地雷数）
    virtual void startNewGame(int rows, int cols, int mines) = 0;

    //当用户左键点击一个格子时，View调用此命令，请求翻开该格子
    //参数是用户点击的格子的坐标
    virtual void revealCellRequest(int row, int col) = 0;

    //当用户右键点击一个格子时，View调用此命令，请求在该格子上标记/取消标记旗帜
    //参数是用户点击的格子的坐标
    virtual void toggleFlagRequest(int row, int col) = 0;
};

#endif // IGAMECOMMANDS_H