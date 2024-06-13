#include "tools.h"
#include "../../mainwindow.h"
#include "./convertor/convertor.h"
#include <QVBoxLayout>

Tools::Tools(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    network(network_)
{
    layout();
    connect(pushButton_to_mainWindow, &QPushButton::clicked, this, &Tools::on_pushButton_to_mainWindow_clicked);
    connect(pushButton_to_convertor, &QPushButton::clicked, this, &Tools::on_pushButton_to_convertor_clicked);

}

Tools::~Tools()
{
    delete network;
    delete pushButton_to_mainWindow;
    delete pushButton_to_convertor;
}

// 返回主页面
void Tools::on_pushButton_to_mainWindow_clicked()
{
    this->close();
    MainWindow* w = new MainWindow(network);
    w->show();
}

// 前往转换器总览页面
void Tools::on_pushButton_to_convertor_clicked()
{
    this->close();
    Convertor* w = new Convertor(network);
    w->show();
}

// 界面初始化
void Tools::layout()
{
    setWindowTitle("工具集");

    pushButton_to_mainWindow = new QPushButton("返回主界面");
    pushButton_to_convertor = new QPushButton("转换器");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pushButton_to_mainWindow);
    layout->addWidget(pushButton_to_convertor);
}
