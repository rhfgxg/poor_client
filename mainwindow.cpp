#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./tools/tools.h"
#include "./terminal/terminal.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_to_tools_clicked()   // 打开工具集页面
{
    this->close();
    Tools* w = new Tools;
    w->show();
}


void MainWindow::on_pushButton_terminal_clicked()   // 打开终端界面
{
    this->close();
    Terminal* w = new Terminal;
    w->show();
}

