#include <QApplication>
#include "app/MinesweeperApp.h"  //现在main函数只需要包含App层的头文件

int main(int argc, char *argv[]) {
    //1.创建QApplication实例，这是所有Qt GUI应用程序的入口点
    QApplication application(argc, argv);

    //2.创建App实例，所有复杂的对象创建和组装工作都在App的构造函数中被封装和隐藏了
    MinesweeperApp app;

    //3.调用App的run方法来启动应用逻辑，这里是显示主窗口
    app.run();

    //4.启动Qt的事件循环，程序将在此等待和处理用户事件，直到退出
    return application.exec();
}