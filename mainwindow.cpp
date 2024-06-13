#include "mainwindow.h"
#include "./ui/tools/tools.h"  // 工具包页面
#include "./ui/terminal/terminal.h"    // 终端页面
#include "./ui/file/fileuploads.h" // 文件上传页面
#include <QVBoxLayout>

MainWindow::MainWindow(ClientNetwork *network_, QWidget *parent):
    QMainWindow(parent),
    network(network_)
{
    layout();

    connect(pushButton_to_tools, &QPushButton::clicked, this, &MainWindow::on_pushButton_to_tools_clicked);
    connect(pushButton_terminal, &QPushButton::clicked, this, &MainWindow::on_pushButton_terminal_clicked);
    connect(pushButton_uploads, &QPushButton::clicked, this, &MainWindow::on_pushButton_uploads_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete pushButton_to_tools;
    delete pushButton_terminal;
    delete pushButton_uploads;

    delete network;
}

// 打开工具集页面
void MainWindow::on_pushButton_to_tools_clicked()
{
    this->close();
    Tools* w = new Tools(network);
    w->show();
}

// 打开终端界面
void MainWindow::on_pushButton_terminal_clicked()
{
    this->close();
    Terminal* w = new Terminal(network);
    w->show();
}

// 打开图片上传界面
void MainWindow::on_pushButton_uploads_clicked()
{
    this->close();
    FileUploads* w = new FileUploads(network);
    w->show();
}

// 自定义私有函数
// 界面初始化
void MainWindow::layout()
{
    ui = new QWidget;
    resize(400, 300);
    setWindowTitle("MainWindow");
    setCentralWidget(ui);   // 用于设置QMainWindow的中心小部件, 如果有之前已经设置的中心小部件，它会被自动移除并由新的小部件替换

    QVBoxLayout *layout_vb = new QVBoxLayout(ui);

    pushButton_to_tools = new QPushButton("工具集", ui);
    pushButton_terminal = new QPushButton("终端", ui);
    pushButton_uploads = new QPushButton("文件上传", ui);
    layout_vb->addWidget(pushButton_to_tools);
    layout_vb->addWidget(pushButton_terminal);
    layout_vb->addWidget(pushButton_uploads);
}
