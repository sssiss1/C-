#include <QTest>  //包含Qt测试框架的核心头文件
#include "../src/Model/GameModel.h"  //包含被测试的GameModel类

//测试类必须继承自QObject以使用QTest的特性
class TestGameModel : public QObject {
    Q_OBJECT  //启用元对象系统的宏，对于QTest中的槽函数是必需的

private slots:
    //这里的每个private slot都是一个独立的测试用例，QTest会自动发现并执行它们
    void testInitialState();              //测试模型默认构造后的初始状态
    void testStartGame();                 //测试startGame函数是否正确初始化游戏
    void testMinePlacement();             //测试地雷是否在首次点击后被正确且安全地布置
    void testFlagging();                  //测试标记和取消标记旗帜的功能
    void testWinCondition();              //测试胜利条件的触发
    void testLoseCondition();             //测试失败条件的触发
    void testFlaggingDoesNotStartGame();  //测试右键点击不应更改游戏状态
};

//测试用例：验证模型在默认构造函数调用后，其内部状态是否符合预期
void TestGameModel::testInitialState() {
    GameModel model;  //创建一个GameModel实例
    QCOMPARE(model.getGameState(), GameState::Ready);  //QCOMPARE是一个断言宏，验证实际值(左)与期望值(右)是否相等
    QCOMPARE(model.getRows(), 0);  //验证初始行数应为0
    QCOMPARE(model.getCols(), 0);  //验证初始列数应为0
}

//测试用例：验证调用startGame后，模型的属性是否被正确设置
void TestGameModel::testStartGame() {
    GameModel model;  //创建一个GameModel实例
    model.startGame(10, 8, 12);  //开始一个10x8，12个雷的游戏
    QCOMPARE(model.getRows(), 10);  //验证行数是否为10
    QCOMPARE(model.getCols(), 8);  //验证列数是否为8
    QCOMPARE(model.getMineCount(), 12);  //验证地雷数是否为12
    QCOMPARE(model.getGameState(), GameState::Ready);  //验证游戏状态是否重置为Ready
}

//测试用例：验证地雷布置逻辑的正确性
void TestGameModel::testMinePlacement() {
    GameModel model;
    model.startGame(10, 10, 15);  //开始一个10x10，15个雷的游戏
    model.revealCell(0, 0);  //模拟玩家首次点击(0,0)位置，这会触发地雷布置

    QVERIFY(!model.getCell(0, 0).isMine);  //QVERIFY是一个断言宏，验证表达式是否为true，此处确保首次点击的位置没有雷
    QCOMPARE(model.getGameState(), GameState::Playing);  //验证首次点击后游戏状态切换为Playing

    int mineCount = 0;  //初始化地雷计数器
    for (int r = 0; r < model.getRows(); ++r) {
        for (int c = 0; c < model.getCols(); ++c) {
            if (model.getCell(r, c).isMine) {
                mineCount++;  //遍历整个棋盘，统计实际布置的地雷数量
            }
        }
    }
    QCOMPARE(mineCount, 15);  //验证实际地雷数与设定的地雷数是否一致
}

//测试用例：验证标记和取消标记旗帜的逻辑
void TestGameModel::testFlagging() {
    GameModel model;
    model.startGame(5, 5, 5);

    QCOMPARE(model.getFlagCount(), 0);  //验证初始时旗帜数为0

    model.flagCell(1, 1);  //在(1,1)位置标记旗帜
    QVERIFY(model.getCell(1, 1).isFlagged);  //验证该格子确实被标记
    QCOMPARE(model.getFlagCount(), 1);  //验证旗帜总数变为1

    model.flagCell(1, 1);  //在同一位置再次调用，应为取消标记
    QVERIFY(!model.getCell(1, 1).isFlagged);  //验证该格子的旗帜已被移除
    QCOMPARE(model.getFlagCount(), 0);  //验证旗帜总数变回0

    model.revealCell(2, 2);  //先翻开一个格子
    model.flagCell(2, 2);  //尝试在已翻开的格子上标记
    QVERIFY(!model.getCell(2, 2).isFlagged);  //验证标记失败，已翻开的格子不能插旗
}

//测试用例：验证胜利条件是否能被正确触发
void TestGameModel::testWinCondition() {
    GameModel model;
    model.startGame(2, 2, 1);  //开始一个极简的2x2游戏，只有1个雷，3个安全格

    bool gameWon = false;  //用于捕捉gameOver信号的标志位
    //使用Qt的信号/槽机制连接到模型的gameOver信号，当信号发出时，用Lambda表达式更新标志位
    QObject::connect(&model, &GameModel::gameOver, [&](bool victory) {
        if (victory) {
            gameWon = true;  //如果接收到的信号表示胜利，则设置标志位
        }
    });

    //通过翻开所有非地雷格子来稳定地触发胜利
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 2; ++c) {
            if (model.getGameState() == GameState::Won) break;  //如果已经胜利，则提前退出循环
            //首次点击是安全的，并且会布置地雷
            if (r == 0 && c == 0) {
                 model.revealCell(r, c);
                 continue;  //继续下一次循环
            }
            //翻开其他不是雷的格子
            if (!model.getCell(r, c).isMine) {
                 model.revealCell(r, c);
            }
        }
    }

    QVERIFY(gameWon);  //验证gameWon标志位是否已通过信号变为true
    QCOMPARE(model.getGameState(), GameState::Won);  //验证模型的内部状态是否也切换为Won
}

//测试用例：验证失败条件是否能被正确触发
void TestGameModel::testLoseCondition() {
    GameModel model;
    model.startGame(10, 10, 10);  //开始一个有10个雷的游戏

    bool gameLost = false;  //用于捕捉gameOver信号的标志位
    QObject::connect(&model, &GameModel::gameOver, [&](bool victory) {
        if (!victory) {
            gameLost = true;  //如果接收到的信号表示失败，则设置标志位
        }
    });

    model.revealCell(0, 0);  //首次点击是安全的，并完成地雷布置

    //地雷已布置，现在遍历棋盘找到一个地雷的位置
    int mineRow = -1, mineCol = -1;
    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            if (model.getCell(r, c).isMine) {
                mineRow = r;
                mineCol = c;
                break;  //找到第一个雷就退出内层循环
            }
        }
        if (mineRow != -1) break;  //找到第一个雷就退出外层循环
    }

    //如果确实找到了一个地雷
    if (mineRow != -1) {
        model.revealCell(mineRow, mineCol);  //主动点击这个地雷以触发失败
    } else {
        QFAIL("Could not find a mine to test the lose condition.");  //QFAIL使测试立即失败并打印消息，这在测试设置失败时很有用
    }

    QVERIFY(gameLost);  //验证gameLost标志位是否已通过信号变为true
    QCOMPARE(model.getGameState(), GameState::Lost);  //验证模型的内部状态是否也切换为Lost
}

//测试用例：验证右键插旗不应启动游戏，只有左键翻开才启动
void TestGameModel::testFlaggingDoesNotStartGame() {
    GameModel model;  //创建一个GameModel实例
    model.startGame(5, 5, 5);  //开始一个新游戏
    //Arrange: 验证初始游戏状态为Ready
    QCOMPARE(model.getGameState(), GameState::Ready);
    //Act: 模拟玩家首次交互为右键插旗
    model.flagCell(1, 1);
    //Assert: 验证游戏状态在插旗后并未改变
    QCOMPARE(model.getGameState(), GameState::Ready);
    //Assert: 同时验证旗帜被正确标记
    QVERIFY(model.getCell(1, 1).isFlagged);
    //Act: 现在模拟一次左键点击
    model.revealCell(2, 2);
    //Assert: 验证只有在左键点击后，游戏状态才切换为Playing
    QCOMPARE(model.getGameState(), GameState::Playing);
}

QTEST_MAIN(TestGameModel)  //这个宏为测试类自动生成一个main函数，使其可以独立运行
#include "TestGameModel.moc"  //必须包含由MOC（元对象编译器）为该文件生成的代码，以实现信号/槽和QTest的内部机制