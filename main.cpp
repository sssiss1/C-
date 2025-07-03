/*
main函数是应用程序的入口点
在MVVM架构中，它扮演着一个至关重要的角色——组合根（Composition Root）
这是唯一一个知道所有层具体实现的地方，负责创建所有对象并将它们通过接口“粘合”在一起
*/

#include <QApplication>  //包含Qt应用程序类，管理GUI应用程序的控制流和主要设置
#include "View/MainWindow.h"
#include "Model/GameModel.h"
#include "ViewModel/GameViewModel.h"

//C++程序的入口函数
int main(int argc, char *argv[]) {
    //1.创建QApplication实例，这是所有Qt GUI应用程序的第一步，它负责事件循环、窗口管理等
    QApplication application(argc, argv);

    //--- 组合根 (Composition Root) ---
    //在这里，我们创建并组装应用程序的所有部分，这段代码是整个架构的“粘合剂”

    //2.创建各个层的具体实例
    //按照依赖关系，先创建最核心的Model，然后是ViewModel，最后是View
    GameModel model;  //创建Model实例
    GameViewModel viewModel(model);  //创建ViewModel实例，并将Model的引用“注入”到其构造函数中
    MainWindow window;  //创建View（MainWindow）实例，此时它是一个孤立的窗口

    //3.执行依赖注入，将各个层通过接口连接起来
    //这是解耦架构的核心步骤（我们在这里建立“合同”的双方）

    //将View（window）的指针向上转型为IGameUI*，设置给ViewModel
    //这样ViewModel就知道该向谁发送UI更新指令了
    viewModel.setUI(&window);

    //将ViewModel（viewModel）的指针向上转型为IGameCommands*设置给View
    //这样View就知道该向谁发送用户操作命令了
    window.setCommands(&viewModel);

    //4.显示主窗口
    window.show();

    //5.启动Qt的事件循环
    //程序将在这里等待并处理事件（如鼠标点击、键盘输入等），直到应用程序退出，`application.exec()` 的返回值是退出码
    return application.exec();
}