#src/CMakeLists.txt

#定义主应用程序的可执行文件目标
add_executable(MineSweeper
        main.cpp
        app/MinesweeperApp.cpp
        Model/GameModel.cpp
        ViewModel/GameViewModel.cpp
        View/MainWindow.cpp
        View/MainWindow.ui
)

#为MineSweeper目标链接它所依赖的Qt库
#PUBLIC关键字表示，如果将来有其他目标链接到MineSweeper，它们也会自动链接到这些Qt库
target_link_libraries(MineSweeper
        PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

#为MineSweeper目标添加头文件包含路径
#这确保了在src/内部，不同子目录（如app/，model/）之间的#include可以正常工作
#CMAKE_CURRENT_SOURCE_DIR变量指向当前正在处理的CMakeLists.txt所在的目录，即src/
target_include_directories(MineSweeper PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#--- Windows部署脚本，只针对本目录定义的目标(MineSweeper) ---
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    #获取Qt的bin目录路径
    get_target_property(QT_INSTALL_PATH Qt6::Core IMPORTED_LOCATION)
    get_filename_component(QT_BIN_PATH "${QT_INSTALL_PATH}" DIRECTORY)

    #复制平台插件qwindows.dll，这是Qt程序在Windows上运行所必需的
    add_custom_command(TARGET MineSweeper POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:MineSweeper>/plugins/platforms/")
    add_custom_command(TARGET MineSweeper POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_BIN_PATH}/../plugins/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:MineSweeper>/plugins/platforms/")

    #复制主程序所需的其他核心DLL
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET MineSweeper POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_BIN_PATH}/Qt6${QT_LIB}.dll"
                "$<TARGET_FILE_DIR:MineSweeper>")
    endforeach (QT_LIB)
endif()