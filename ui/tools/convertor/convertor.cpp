#include "convertor.h"
#include "ui_convertor.h"
#include "./timestamp/timestamp_ui.h"
#include "../tools.h"

Convertor::Convertor(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Convertor),
    network(network_)
{
    ui->setupUi(this);
}

Convertor::~Convertor()
{
    delete ui;
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




