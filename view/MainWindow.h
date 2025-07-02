#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSize>

// 前向声明，以加速编译
class GameViewModel;
struct CellUpdateInfo;
class QPushButton;

// 这是使用 .ui 文件时的标准样板代码
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 构造函数依然通过依赖注入接收 ViewModel
    explicit MainWindow(GameViewModel& viewModel, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 用于捕获右键点击的事件过滤器
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // --- 响应 ViewModel 信号的槽函数 ---
    void onBoardSizeChanged(const QSize& newSize);
    void onCellUpdated(const CellUpdateInfo& info);
    void onShowGameOverDialog(const QString& message);
    void updateFlagsLabel(int flags);

    // --- 响应 UI 事件的槽函数 ---
    // 这个命名遵循 Qt 的自动连接约定 (on_<objectName>_<signalName>)
    void on_newGameButton_clicked();

private:
    // 清理动态创建的扫雷按钮
    void clearBoard();

    Ui::MainWindow *ui; // 指向由 Designer 生成的 UI 类的指针
    GameViewModel& m_viewModel; // 对 ViewModel 的引用
    QVector<QVector<QPushButton*>> m_cellButtons; // 存储动态生成的按钮
};
#endif // MAINWINDOW_H