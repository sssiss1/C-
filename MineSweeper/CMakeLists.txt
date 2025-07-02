# 指定所需的CMake最低版本，这确保了我们使用的CMake命令是可用的
cmake_minimum_required(VERSION 3.22)

# 定义项目名称，这个名字会用在生成的可执行文件和解决方案文件中
project(MineSweeper)

# --- C++ 和 Qt 的基本设置 ---

# 设置C++标准为C++20
set(CMAKE_CXX_STANDARD 20)

# 启用Qt的自动化工具，这是使用Qt与CMake集成的关键
set(CMAKE_AUTOMOC ON)   # 自动运行MOC(Meta-Object Compiler)，处理Q_OBJECT宏，生成信号/槽等所需的代码
set(CMAKE_AUTORCC ON)   # 自动运行RCC(Resource Compiler)，将.qrc资源文件编译进程序
set(CMAKE_AUTOUIC ON)   # 自动运行UIC(User Interface Compiler)，将.ui界面文件编译成C++头文件

# --- 查找 Qt6 库 ---

# 设置Qt6的安装路径，CMake会在这个路径下查找Qt的库和工具
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/mingw_64")

# 查找Qt6的核心组件
# `find_package`是CMake用于查找外部库的命令
# `REQUIRED`表示如果找不到这些组件，CMake将会报错并停止构建
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Test
        REQUIRED)

# --- 定义可执行文件及其源文件 ---

# `add_executable`命令创建一个名为MineSweeper的可执行文件目标
# 它后面的列表是构建这个可执行文件所需的所有源文件(.cpp)和需要特殊处理的文件(.ui)
add_executable(MineSweeper
        src/main.cpp
        src/Model/GameModel.cpp
        src/ViewModel/GameViewModel.cpp
        src/View/MainWindow.cpp
        src/View/MainWindow.ui  # .ui文件也需要在这里列出，以便CMAKE_AUTOUIC能够找到并处理它
)
# 头文件(.h)通常不需要在这里列出，因为编译器会通过源文件中的#include指令找到它们

# --- 链接库 ---

# `target_link_libraries`命令将指定的目标（MineSweeper）与所需的库链接起来
# `Qt::Core`, `Qt::Gui`, `Qt::Widgets`是CMake在`find_package(Qt6)`后提供的导入目标，它们包含了链接到相应Qt模块所需的所有信息（库路径、头文件路径等）
target_link_libraries(MineSweeper
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# --- 单元测试目标 ---
# 目标1：Model测试
add_executable(TestModel
        test/TestGameModel.cpp
        src/Model/GameModel.cpp # Model 测试需要链接 Model 的实现
)
target_link_libraries(TestModel Qt::Core Qt::Test)
add_test(NAME GameModelTests COMMAND TestModel) # 添加到 CTest

# 目标 2: ViewModel 测试
add_executable(TestViewModel
        test/TestGameViewModel.cpp
        src/Model/GameModel.cpp # ViewModel 测试需要 Model
        src/ViewModel/GameViewModel.cpp # ViewModel 测试需要链接 ViewModel 的实现
)
target_link_libraries(TestViewModel Qt::Core Qt::Test)
add_test(NAME GameViewModelTests COMMAND TestViewModel) # 添加到 CTest

# --- Windows 平台部署脚本 (可选但推荐) ---
# 这部分脚本用于在构建完成后，自动将Qt的动态链接库(.dll)复制到可执行文件所在的目录
# 这使得你可以直接从构建目录运行程序，而无需手动复制DLL或配置系统路径
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # 根据构建类型（Debug/Release）确定DLL的后缀（例如Qt6Cored.dll vs Qt6Core.dll）
    # 这个脚本目前只处理了MSVC的情况，对于MinGW通常没有'd'后缀
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    #确定Qt的安装根目录
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    # --- 新增：为所有需要运行的目标定义一个函数来复制DLL ---
    function(add_qt_deployment target_name)
        # 复制 Qt 平台插件
        if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${target_name} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${target_name}>/plugins/platforms/")
            add_custom_command(TARGET ${target_name} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${target_name}>/plugins/platforms/")
        endif ()

        # 复制核心 DLL
        foreach (QT_LIB Core Gui Widgets Test) # Test 也需要 Test.dll
            # 检查 DLL 是否存在，避免因缺少某些DLL（如Test目标不需要Gui）而报错
            if(EXISTS "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll")
                add_custom_command(TARGET ${target_name} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                        "$<TARGET_FILE_DIR:${target_name}>")
            endif()
        endforeach (QT_LIB)
    endfunction()

    # 为每个可执行目标调用部署函数
    add_qt_deployment(MineSweeper)
    add_qt_deployment(TestModel)
    add_qt_deployment(TestViewModel)

endif()