#include <QTest>
#include "../src/Model/GameModel.h"
#include "../src/ViewModel/GameViewModel.h"
#include "../src/common/IGameUI.h"  //包含UI接口，因为需要Mock它

//Mock（模拟）对象：一个IGameUI接口的模拟实现
class MockGameUI : public IGameUI {
public:
    int boardSizeChangedCount = 0;
    QSize lastBoardSize;
    int cellUpdatedCount = 0;
    int gameOverDialogCount = 0;
    QString lastGameOverMessage;
    int flagsLabelCount = 0;
    int lastFlagCount = 0;
    int statusLabelCount = 0;
    QString lastStatusText;
    int tempMessageCount = 0;
    QString lastTempMessage;

    //重写接口中的所有纯虚函数
    void onBoardSizeChanged(const QSize& newSize) override { boardSizeChangedCount++; lastBoardSize = newSize; }
    void onCellUpdated(const CellUpdateInfo&) override { cellUpdatedCount++; }
    void onShowGameOverDialog(const QString& message) override { gameOverDialogCount++; lastGameOverMessage = message; }
    void updateFlagsLabel(int flags) override { flagsLabelCount++; lastFlagCount = flags; }
    void updateStatusLabel(const QString& text) override { statusLabelCount++; lastStatusText = text; }
    void showTemporaryMessage(const QString& message, int) override { tempMessageCount++; lastTempMessage = message; }

    //一个辅助函数，用于在每个测试用例开始前清空所有记录
    void reset() {
        boardSizeChangedCount = 0;
        cellUpdatedCount = 0;
        gameOverDialogCount = 0;
        flagsLabelCount = 0;
        statusLabelCount = 0;
        tempMessageCount = 0;
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
    void testFlagLimitMessage();          //测试当旗帜用完时，是否会触发临时消息提示
    void testQuestionMarkDisplay();       //测试问号标记是否能被正确地翻译为UI更新
};

//测试用例：验证startNewGame命令
void TestGameViewModel::testStartGameCommand() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    viewModel.startNewGame(8, 12, 10);

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

    model.startGame(5, 5, 1);
    mockUI.reset();

    model.revealCell(2, 2);

    QCOMPARE(mockUI.cellUpdatedCount, 25);
    QCOMPARE(mockUI.flagsLabelCount, 1);
}

//测试用例：验证胜利场景的翻译
void TestGameViewModel::testGameOverWinTranslation() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    model.startGame(2, 2, 1);
    mockUI.reset();

    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 2; ++c) {
            if (model.getGameState() == GameState::Won) break;
            if (!model.getCell(r, c).isMine) {
                 model.revealCell(r, c);
            }
        }
        if (model.getGameState() == GameState::Won) break;
    }

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

    model.revealCell(0,0);
    mockUI.reset();

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

    QCOMPARE(mockUI.gameOverDialogCount, 1);
    QVERIFY(mockUI.lastGameOverMessage.contains("砰"));
    QCOMPARE(mockUI.statusLabelCount, 1);
    QCOMPARE(mockUI.lastStatusText, "你输了! :(");
}

//测试用例：验证当旗帜用完时，ViewModel是否会调用显示临时消息的UI接口
void TestGameViewModel::testFlagLimitMessage() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    viewModel.startNewGame(5, 5, 1); //只有1个旗帜可用
    viewModel.toggleFlagRequest(0, 0); //使用掉这个旗帜
    mockUI.reset(); //重置mock，只关心下一次的调用

    viewModel.toggleFlagRequest(0, 1); //尝试在另一个格子上插旗

    QCOMPARE(mockUI.tempMessageCount, 1); //验证显示临时消息的方法被调用了1次
    QVERIFY(mockUI.lastTempMessage.contains("旗帜已用完")); //验证消息内容是否正确
}

//测试用例：验证ViewModel是否能正确处理问号标记并更新UI
void TestGameViewModel::testQuestionMarkDisplay() {
    GameModel model;
    GameViewModel viewModel(model);
    MockGameUI mockUI;
    viewModel.setUI(&mockUI);

    viewModel.startNewGame(5, 5, 5);
    mockUI.reset();

    viewModel.cycleMarkRequest(1, 1); //通过ViewModel发出标记问号的命令

    //cellUpdatedCount会被调用25次，因为modelChanged会触发全盘刷新
    QCOMPARE(mockUI.cellUpdatedCount, 25);
    //虽然我们不能直接检查某个特定格子的更新信息，但可以确认model的状态是正确的
    QVERIFY(model.getCell(1, 1).isQuestionMark);
}


QTEST_MAIN(TestGameViewModel)
#include "TestGameViewModel.moc"