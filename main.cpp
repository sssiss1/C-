#include <QApplication>
#include "View/MainWindow.h"
#include "Model/GameModel.h"
#include "ViewModel/GameViewModel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // --- Dependency Injection in action ---

    // 1. 创建 Model
    GameModel model;

    // 2. 创建 ViewModel，并将 Model 注入
    GameViewModel viewModel(model);

    // 3. 创建 View，并将 ViewModel 注入
    MainWindow w(viewModel);

    w.show();
    return a.exec();
}