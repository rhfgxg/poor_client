#include "convertor.h"
#include "ui_convertor.h"
#include "./timestamp/timestamp_ui.h"
#include "../tools.h"
Convertor::Convertor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Convertor)
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
    Tools* w = new Tools;
    w->show();
}

void Convertor::on_pushButton_to_timestamp_clicked()    // 打开时间戳转换器页面
{
    this->close();  // 关闭当前页面
    TimestampUi* w = new TimestampUi;
    w->show();
}




