#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./tools/tools.h"  // 工具包
#include "./terminal/terminal.h"    // 终端
#include "client/uploads/useruploadsmanager.h"  // 文件上传

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


void MainWindow::on_pushButton_uploads_clicked()    // 图片上传
{
    UserUploadsManager uploads(network);    // 创建管理对象
    uploads.uploads("D:/Project/qt/aaa.txt");
}

