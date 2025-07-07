# src/CMakeLists.txt
#
# 文件用途：
# 这是src子目录的CMake配置文件
# 它负责定义实际的可执行文件目标，并列出所有需要编译和链接的源文件、UI文件等
# 它还负责将可执行文件链接到Qt库，并设置头文件包含路径

#定义主应用程序的可执行文件目标，名为MineSweeper
#列出所有构成该可执行文件的源文件和UI文件
add_executable(MineSweeper
        main.cpp
        App/MineSweeperApp.cpp
        Model/GameModel.cpp
        ViewModel/GameViewModel.cpp
        View/StartWindow.cpp
        View/CustomGameDialog.cpp
        View/MainWindow.cpp
        View/HighScoreDialog.cpp

        View/StartWindow.ui
        View/CustomGameDialog.ui
        View/MainWindow.ui
        View/HighScoreDialog.ui
        Common/GameData.h
)

#将可执行文件链接到所需的Qt库
#Qt6使用命名空间的方式引入目标，例如Qt::Widgets
target_link_libraries(MineSweeper
        PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

#为目标添加头文件包含目录，使其能找到项目内的头文件
#${CMAKE_CURRENT_SOURCE_DIR} 指的是当前CMakeLists.txt所在的目录（即src目录）
#这样在代码中就可以使用像 #include "App/MineSweeperApp.h" 这样的相对路径
target_include_directories(MineSweeper PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)