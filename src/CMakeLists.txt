# src/CMakeLists.txt

# 定义主应用程序的可执行文件目标
add_executable(MineSweeper
        main.cpp
        app/MinesweeperApp.cpp
        Model/GameModel.cpp
        ViewModel/GameViewModel.cpp
        View/StartWindow.cpp
        View/CustomGameDialog.cpp
        View/MainWindow.cpp
        View/StartWindow.ui
        View/CustomGameDialog.ui
        View/MainWindow.ui
        View/HighScoreDialog.h
        View/HighScoreDialog.cpp
)

# 将可执行文件链接到所需的Qt库
target_link_libraries(MineSweeper
        PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# 为目标添加头文件包含目录，使其能找到项目内的头文件
target_include_directories(MineSweeper PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)