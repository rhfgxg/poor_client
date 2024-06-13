#include "convertor.h"
#include "./timestamp/timestamp_ui.h"
#include "../tools.h"
#include <QVBoxLayout>

Convertor::Convertor(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    network(network_)
{
    layout();
    connect(pushButton_to_tools, &QPushButton::clicked, this, &Convertor::on_pushButton_to_tools_clicked);
    connect(pushButton_to_timestamp, &QPushButton::clicked, this, &Convertor::on_pushButton_to_timestamp_clicked);

}

Convertor::~Convertor()
{
    delete network;
    delete pushButton_to_tools;
    delete pushButton_to_timestamp;
}

void Convertor::on_pushButton_to_tools_clicked()    // 返回工具集页面
{
    this->close();
    Tools* w = new Tools(network);
    w->show();
}

void Convertor::on_pushButton_to_timestamp_clicked()    // 打开时间戳转换器页面
{
    this->close();  // 关闭当前页面
    TimestampUi* w = new TimestampUi(network);
    w->show();
}

void Convertor::layout()
{
    setWindowTitle("转换器");

    pushButton_to_tools = new QPushButton("返回工具集");
    pushButton_to_timestamp = new QPushButton("时间戳转换");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pushButton_to_tools);
    layout->addWidget(pushButton_to_timestamp);
}


