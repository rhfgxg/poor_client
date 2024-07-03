#include "terminal.h"
#include "../../mainwindow.h"
#include <QGridLayout>
#include "../../feature/terminal/bin/sl.h"  // 小火车动画


// 终端界面类
Terminal::Terminal(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    network(network_)
{
    layout();   // 界面布局初始化
    connect(plainTextEdit_terminal, &PlainTextEdit::commandEntered, this, &Terminal::on_command_execute_result);    // 回车键

}

Terminal::~Terminal()
{
    delete plainTextEdit_terminal;
}


// 自定义槽函数
// 接收 重写类的自定义commandEntered(回车)信号，在输入相关指令时发出
// 信号传递指令和参数列表
void Terminal::on_command_execute_result(const QString& command_first, const QStringList& tokens)
{
// 对终端界面进行修改的指令白名单：例如终端颜色，退出终端等
    if (command_first == "exit!")   // 退出终端
    {
        this->close();
        MainWindow* w = new MainWindow(network);
        w->show();
    }
    else if (command_first == "max_window")   // 窗口最大化
    {
        this->setWindowState(Qt::WindowMaximized);
    }
    else if (command_first == "sl")   // 小火车彩蛋
    {
        Sl *sl = new Sl(this->plainTextEdit_terminal, this);  // 创建 Sl 类实例
        sl->start();
    }
}

// 自定义私有函数
void Terminal::layout() // 界面格式初始化
{
    setWindowTitle("终端");

    plainTextEdit_terminal = new PlainTextEdit(network, this);  // 添加一个输入界面
    plainTextEdit_terminal->resize(1000, 1000);
    plainTextEdit_terminal->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 禁用滚动条
    plainTextEdit_terminal->setStyleSheet("QPlainTextEdit {"
                                        "background-color: #300924;"    // 背景颜色, 紫色
                                        "color: white;" // 文字颜色
                                        "}");

// 创建一个栅格布局
// 使输入框布满窗口，随父窗口改变大小
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(plainTextEdit_terminal);  // 添加控件
    layout->setContentsMargins(0, 0, 0, 0); // 移除边距
    layout->setSpacing(0); // 移除间距
    plainTextEdit_terminal->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  // 设置QPlainTextEdit的大小策略，使其能够扩展
    this->setLayout(layout);
}



