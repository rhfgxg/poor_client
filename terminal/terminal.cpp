#include "terminal.h"
#include "ui_terminal.h"

#include "../mainwindow.h"
#include <QGridLayout>

// 终端界面类
Terminal::Terminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Terminal)
{
    ui->setupUi(this);

    layout();   // 界面布局初始化
    connects(); // 设置信号槽
}

Terminal::~Terminal()
{
    delete ui;
}


// 自定义槽函数
// 接收 重写类的自定义commandEntered信号，信号传递指令和参数列表
void Terminal::on_command_execute_result(const QString& command_first, const QStringList& tokens)
{

// 对终端界面进行修改的指令白名单：例如终端颜色，退出终端等
    if (command_first == "exit!")   // 退出终端
    {
        this->close();
        MainWindow* w = new MainWindow;
        w->show();
    }
    else if (command_first == "max_window")   // 窗口最大化
    {
        this->setWindowState(Qt::WindowMaximized);
    }

}

// 接收 重写类的自定义commandEntered信号，信号传递备选的所有补全指令
void Terminal::on_command_complete_list(const QStringList& completionSuggestion)
{

}

// 自定义私有函数
void Terminal::layout() // 界面格式初始化
{
    plainTextEdit_terminal = new TerminalTextEdit(this);  // 添加一个输入界面
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

void Terminal::connects()   // 信号槽关联
{
    connect(plainTextEdit_terminal, &TerminalTextEdit::commandEntered, this, &Terminal::on_command_execute_result);
    connect(plainTextEdit_terminal, &TerminalTextEdit::commandTab, this, &Terminal::on_command_complete_list);

}
