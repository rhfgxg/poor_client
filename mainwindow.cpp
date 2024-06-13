#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui/tools/tools.h"  // 工具包页面
#include "./terminal/terminal.h"    // 终端页面
#include "./ui/file/fileuploads.h" // 文件上传页面

MainWindow::MainWindow(ClientNetwork *network_, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    network(network_)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete network;
}


void MainWindow::on_pushButton_to_tools_clicked()   // 打开工具集页面
{
    this->close();
    Tools* w = new Tools(network);
    w->show();
}


void MainWindow::on_pushButton_terminal_clicked()   // 打开终端界面
{
    this->close();
    Terminal* w = new Terminal(network);
    w->show();
}


void MainWindow::on_pushButton_uploads_clicked()    // 打开图片上传界面
{
    this->close();
    FileUploads* w = new FileUploads(network);
    w->show();
}

