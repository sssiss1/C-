#顶层CMakeLists.txt

#指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.22)
#定义整个项目的名称
project(MineSweeper)

#在顶层启用测试，这样CTest才能发现所有子目录中通过add_test注册的测试
enable_testing()

#全局设置，这些设置会自动传递给所有通过add_subdirectory包含的子目录
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#全局设置Qt的安装路径，子目录会继承这个设置
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/mingw_64")

#在顶层一次性查找所有可能用到的Qt模块
#这样做可以确保所有模块都被找到，避免在子目录中重复查找
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Test
        REQUIRED)

#包含子目录，CMake会暂停当前文件的处理，进入这些目录并执行其中的CMakeLists.txt文件
#处理完子目录后，再回到这里继续执行后续命令（如果有的话）
add_subdirectory(src)
add_subdirectory(test)