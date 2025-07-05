# CMakeLists.txt (顶层)
#
# 文件用途：
# 这是项目顶层的CMake配置文件
# 它负责设置整个项目的基本构建环境，如CMake版本、项目名称、C++标准
# 以及查找并配置像Qt这样的外部依赖库
# 最后，它通过add_subdirectory指令将构建过程引导到源文件所在的子目录

#设置CMake最低版本要求
cmake_minimum_required(VERSION 3.16)

#定义项目名称和支持的语言
project(MineSweeper LANGUAGES CXX)

#设置C++标准为C++17，并要求编译器必须支持该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#--- Qt6 设置 ---
#寻找Qt6开发包，并指定需要的组件 (Core, Gui, Widgets)
#REQUIRED关键字表示如果找不到Qt6，CMake会立即报错并停止
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

#启用Qt的元对象编译器(MOC)、资源编译器(RCC)和UI编译器(UIC)的自动运行功能
#这会让CMake自动处理Q_OBJECT宏、qrc资源文件和.ui界面文件，无需手动配置规则
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#添加src子目录
#CMake会进入src目录，并处理那里的CMakeLists.txt文件
add_subdirectory(src)