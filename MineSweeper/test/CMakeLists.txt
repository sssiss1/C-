#test/CMakeLists.txt

#为本目录下的所有目标添加头文件包含路径，以便找到src/下的头文件
include_directories(${CMAKE_SOURCE_DIR}/src)

#--- 目标1: Model测试 ---
add_executable(TestModel
        TestGameModel.cpp
        #测试代码需要链接被测试的源代码实现，所以要包含其.cpp文件
        ${CMAKE_SOURCE_DIR}/src/Model/GameModel.cpp
)
target_link_libraries(TestModel Qt::Core Qt::Test)

#--- 目标2: ViewModel测试 ---
add_executable(TestViewModel
        TestGameViewModel.cpp
        ${CMAKE_SOURCE_DIR}/src/Model/GameModel.cpp
        ${CMAKE_SOURCE_DIR}/src/ViewModel/GameViewModel.cpp
)
target_link_libraries(TestViewModel Qt::Core Qt::Test)

#--- 关键修正点：手动注册每个测试用例 ---
#我们定义一个CMake宏(macro)来简化为每个测试用例添加测试的过程，避免代码重复
macro(ADD_QTEST_CASE target test_case_name)
    #add_test命令将一个测试注册到CTest中
    #NAME: 在CTest中显示的测试名称，我们用 "目标名::测试用例名" 的格式，使其非常清晰
    #COMMAND: 运行该测试需要执行的命令
    #  - 第一个参数是要运行的可执行文件名 (${target})
    #  - 第二个参数是传递给该可执行文件的命令行参数，这里是具体的测试用例函数名 (${test_case_name})
    #  - QTest程序支持接收一个函数名作为参数，只运行该函数对应的测试
    add_test(NAME ${target}::${test_case_name} COMMAND ${target} ${test_case_name})
endmacro()

#--- 为TestModel的每个测试用例调用宏来注册 ---
ADD_QTEST_CASE(TestModel testInitialState)
ADD_QTEST_CASE(TestModel testStartGame)
ADD_QTEST_CASE(TestModel testMinePlacement)
ADD_QTEST_CASE(TestModel testFlagging)
ADD_QTEST_CASE(TestModel testWinCondition)
ADD_QTEST_CASE(TestModel testLoseCondition)
ADD_QTEST_CASE(TestModel testFlaggingDoesNotStartGame)
ADD_QTEST_CASE(TestModel testQuestionMarkCycle)
ADD_QTEST_CASE(TestModel testFlagLimit)
ADD_QTEST_CASE(TestModel testChordAction)
ADD_QTEST_CASE(TestModel testChordActionOnWrongFlags)


#--- 为TestViewModel的每个测试用例调用宏来注册 ---
ADD_QTEST_CASE(TestViewModel testStartGameCommand)
ADD_QTEST_CASE(TestViewModel testRevealTranslatesToUIUpdate)
ADD_QTEST_CASE(TestViewModel testGameOverWinTranslation)
ADD_QTEST_CASE(TestViewModel testGameOverLoseTranslation)
ADD_QTEST_CASE(TestViewModel testFlagLimitMessage)
ADD_QTEST_CASE(TestViewModel testQuestionMarkDisplay)


#--- Windows部署脚本 ---
#为测试目标自动复制运行所需的Qt DLL文件
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    #从已找到的Qt6::Core目标中获取其路径属性，这是查找Qt安装位置的可靠方法
    get_target_property(QT_INSTALL_PATH Qt6::Core IMPORTED_LOCATION)
    #从完整的库文件路径中提取出其所在的目录，即bin目录
    get_filename_component(QT_BIN_PATH "${QT_INSTALL_PATH}" DIRECTORY)

    #定义一个列表，包含所有测试目标
    set(TEST_TARGETS TestModel TestViewModel)
    #遍历每个测试目标
    foreach(TARGET_NAME ${TEST_TARGETS})
        #遍历每个目标所需的Qt库
        foreach (QT_LIB Core Test)
            #为目标添加一个构建后命令，用于复制DLL
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_BIN_PATH}/Qt6${QT_LIB}.dll"
                    #$<TARGET_FILE_DIR:...> 是一个生成器表达式，表示目标文件所在的目录
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>")
        endforeach (QT_LIB)
    endforeach()
endif()