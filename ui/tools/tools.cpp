#include "tools.h"
#include "ui_tools.h"
#include "../../mainwindow.h"
#include "./convertor/convertor.h"
Tools::Tools(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tools),
    network(network_)
{
    ui->setupUi(this);
}

Tools::~Tools()
{
    delete ui;
}

void Tools::on_pushButton_to_mainWindow_clicked()   // 返回主页面
{
    this->close();
    MainWindow* w = new MainWindow(network);
    w->show();
}


void Tools::on_pushButton_to_convertor_clicked()    // 前往转换器总览页面
{
    this->close();
    Convertor* w = new Convertor(network);
    w->show();
}

