#include <QTest>
#include "../src/Model/GameModel.h"
#include "../src/ViewModel/GameViewModel.h"
#include "../src/common/IGameUI.h"  //包含UI接口，因为需要Mock它

//Mock（模拟）对象：一个IGameUI接口的模拟实现
//它不执行任何真正的UI操作，而是记录ViewModel调用了它的哪些方法、以及传入了什么参数
//这使得我们可以验证ViewModel的行为是否符合预期，而无需一个真实的UI窗口
class MockGameUI : public IGameUI {
public:
    //用于记录方法调用次数和最后一次调用参数的成员变量
    int boardSizeChangedCount = 0;
    QSize lastBoardSize;
    int cellUpdatedCount = 0;
    int gameOverDialogCount = 0;
    QString lastGameOverMessage;
    int flagsLabelCount = 0;
    int lastFlagCount = 0;
    int statusLabelCount = 0;
    QString lastStatusText;

    //重写接口中的所有纯虚函数
    void onBoardSizeChanged(const QSize& newSize) override { boardSizeChangedCount++; lastBoardSize = newSize; }
    void onCellUpdated(const CellUpdateInfo&) override { cellUpdatedCount++; }
    void onShowGameOverDialog(const QString& message) override { gameOverDialogCount++; lastGameOverMessage = message; }
    void updateFlagsLabel(int flags) override { flagsLabelCount++; lastFlagCount = flags; }
    void updateStatusLabel(const QString& text) override { statusLabelCount++; lastStatusText = text; }

    //一个辅助函数，用于在每个测试用例开始前清空所有记录，确保测试的独立性
    void reset() {
        boardSizeChangedCount = 0;
        cellUpdatedCount = 0;
        gameOverDialogCount = 0;
        flagsLabelCount = 0;
        statusLabelCount = 0;
    }
};

//ViewModel的测试类
class TestGameViewModel : public QObject {
    Q_OBJECT

private slots:
    //每个测试用例都完全自包含，在函数内部创建所需的所有对象，以保证100%的隔离性
    void testStartGameCommand();        //测试startNewGame命令是否正确驱动了UI
    void testRevealTranslatesToUIUpdate();  //测试当Model数据变化时，ViewModel是否正确地将其翻译为UI更新
    void testGameOverWinTranslation();    //测试游戏胜利时，ViewModel是否发送了正确的UI指令
    void testGameOverLoseTranslation();   //测试游戏失败时，ViewModel是否发送了正确的UI指令
};

//测试用例：验证startNewGame命令
void TestGameViewModel::testStartGameCommand() {
    //Arrange：在测试函数内部创建所有需要的对象
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    //Act：执行要测试的动作
    viewModel.startNewGame(8, 12, 10);

    //Assert：验证结果是否符合预期
    //在正确的ViewModel逻辑下，startGame会触发一次modelChanged，从而调用一次updateFlagsLabel
    QCOMPARE(mockUI.flagsLabelCount, 1);
    QCOMPARE(mockUI.lastFlagCount, 10);
    QCOMPARE(mockUI.boardSizeChangedCount, 1);
    QCOMPARE(mockUI.lastBoardSize, QSize(12, 8));
    QCOMPARE(mockUI.statusLabelCount, 1);
    QCOMPARE(mockUI.lastStatusText, "游戏中...");
}

//测试用例：验证翻开格子后UI的更新
void TestGameViewModel::testRevealTranslatesToUIUpdate() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    model.startGame(5, 5, 1);  //开始一个5x5的游戏
    mockUI.reset();  //清空mockUI的记录，忽略startGame带来的影响，只关注后续动作

    model.revealCell(2, 2);  //直接操作model来触发信号，模拟玩家点击

    //onModelChanged会被触发，它会为所有25个格子调用onCellUpdated
    QCOMPARE(mockUI.cellUpdatedCount, 25);
    //同时，onModelChanged也会更新一次旗帜数量
    QCOMPARE(mockUI.flagsLabelCount, 1);
}

//测试用例：验证胜利场景的翻译
void TestGameViewModel::testGameOverWinTranslation() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    model.startGame(2, 2, 1);  //开始一个可以轻易获胜的游戏
    mockUI.reset();

    //通过实际操作达到胜利条件，以触发gameOver(true)信号
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 2; ++c) {
            if (model.getGameState() == GameState::Won) break;
            if (!model.getCell(r, c).isMine) {
                 model.revealCell(r, c);
            }
        }
        if (model.getGameState() == GameState::Won) break;
    }

    //验证ViewModel是否向UI发送了正确的胜利指令
    QCOMPARE(mockUI.gameOverDialogCount, 1);
    QVERIFY(mockUI.lastGameOverMessage.contains("恭喜"));
    QCOMPARE(mockUI.statusLabelCount, 1);
    QCOMPARE(mockUI.lastStatusText, "你赢了! :)");
}

//测试用例：验证失败场景的翻译
void TestGameViewModel::testGameOverLoseTranslation() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    model.startGame(10, 10, 10);

    model.revealCell(0,0);  //首次点击以布置地雷
    mockUI.reset();  //重置mock，只关心踩雷的动作

    //找到一个地雷并点击它
    int mineRow = -1, mineCol = -1;
    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            if (model.getCell(r, c).isMine) {
                mineRow = r; mineCol = c; break;
            }
        }
        if (mineRow != -1) break;
    }

    if (mineRow != -1) {
        model.revealCell(mineRow, mineCol);
    } else {
        QFAIL("Could not find a mine to test.");
    }

    //验证ViewModel是否向UI发送了正确的失败指令
    QCOMPARE(mockUI.gameOverDialogCount, 1);
    QVERIFY(mockUI.lastGameOverMessage.contains("砰"));
    QCOMPARE(mockUI.statusLabelCount, 1);
    QCOMPARE(mockUI.lastStatusText, "你输了! :(");
}


QTEST_MAIN(TestGameViewModel)  //为该测试文件生成独立的main函数
#include "TestGameViewModel.moc"  //包含MOC生成的代码