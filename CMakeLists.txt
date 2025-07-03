cmake_minimum_required(VERSION 3.31)
project(untitled5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/eyx/Qt/6.9.1/macos")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(untitled5 main.cpp
        Model/GameModel.cpp
        Model/GameModel.h
        ViewModel/GameViewModel.cpp
        ViewModel/GameViewModel.h
        View/mainwindow.cpp
        View/mainwindow.h
        View/MainWindow.ui
        common/IGameUI.h
        common/IGameCommands.h
)
target_link_libraries(untitled5
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

