#include "./timestamp_ui.h"
#include "ui_timestamp_ui.h"
#include "../convertor.h"

#include <QTimer>
#include <QDateTime>    // 使用时间戳
#include <QRegularExpression>   // 使用正则
#include <QRegularExpressionValidator>

TimestampUi::TimestampUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimestampUi)
{
    ui->setupUi(this);
    layout();   // 界面格式初始化
//    timestamp;  // 初始化时间戳计算对象
}

TimestampUi::~TimestampUi()
{
    delete ui;
}


void TimestampUi::on_pushButton_to_convertor_clicked()    // 返回上一级：转换工具总览
{
    this->close();
    Convertor* w = new Convertor;
    w->show();
}

void TimestampUi::on_pushButton_timestampToTime_execute_clicked()  // 时间戳转时间
{
    QString value = ui->lineEdit_timestampToTime_value->text();

    if (value.isEmpty())
    {// 如果没有输入，直接结束函数
        return;
    }

    // 将字符串转换为 时间戳
    // 参数：时间字符串QString，时间格式
    bool conversionOk = false;  // 转换是否成功
    qint64 timestamp_value = value.toLongLong(&conversionOk);
    if (!conversionOk)
    {
        qDebug() << "转换失败，字符串不能转换为有效的qint64数值";
    }

    QDateTime timestamp_result = timestamp.timestamp_to_time(timestamp_value, ui->comboBox_timestampToTime_value_type->currentText(), ui->comboBox_timestampToTime_result_type->currentText());
    ui->lineEdit_timestampToTime_result->setText(timestamp_result.toString("yyyy-MM-dd HH:mm:ss.zzz"));   // 转换成字符串显示
}

void TimestampUi::on_pushButton_timeToTimestamp_execute_clicked() // 时间转时间戳
{
    QString value = ui->lineEdit_timeToTimestamp_value->text();

    if (value.isEmpty())
    {// 如果没有输入，直接结束函数
        return;
    }

    // 将字符串转换为 QDateTime 对象
    // 参数：时间字符串QString，时间格式
    QDateTime time_value = QDateTime::fromString(value, "yyyy-MM-dd HH:mm:ss.zzz");

    // 将 QDateTime 对象转换为 Unix 时间戳
    qint64 timestamp_result = timestamp.time_to_timestamp(time_value, ui->comboBox_timeToTimestamp_value_type->currentText(), ui->comboBox_timeToTimestamp_result_type->currentText());
    ui->lineEdit_timeToTimestamp_result->setText(QString::number(timestamp_result));   // 转换成字符串显示

    if (ui->comboBox_timeToTimestamp_result_type->currentText() != "秒(s)" && ui->comboBox_timeToTimestamp_result_type->currentText() != "毫秒(ms)")
    {
        qDebug("时间转时间戳 结果类型异常");
    }
}


void TimestampUi::layout()   // 界面格式初始化
{
// 获取当前时间，转换成时间戳，显示在副标题，每秒更新一次
    QDateTime date_time = QDateTime::currentDateTime();   // 获取当前日期和时间的 QDateTime 对象
    //    qint64 currentTimestamp = currentDateTime.toSecsSinceEpoch();   // 转换为秒
    qint64 current_timestamp_millis = date_time.toMSecsSinceEpoch();    // 转换为毫秒
    ui->label_now_time->setText("当前时间：" + QString::number(current_timestamp_millis) + "  " + date_time.toString("yyyy-MM-dd HH:mm:ss.zzz"));   // 将时间戳转为字符串显示

    // 创建一个 QTimer 定时器，每秒触发一次
    QTimer* timer = new QTimer(this);;
    timer->setInterval(1000); // 1000毫秒 = 1秒
    timer->start();
    // 设置定时器触发事件
    QObject::connect(timer, &QTimer::timeout, this, [&]()
                     {
                        // 在定时器触发时更新 QLabel 的文本为当前时间
                        QDateTime currentDateTime = QDateTime::currentDateTime();   // 获取当前日期和时间的 QDateTime 对象
                        //    qint64 currentTimestamp = currentDateTime.toSecsSinceEpoch();   // 转换为秒
                        qint64 currentTimestampMillis = currentDateTime.toMSecsSinceEpoch();    // 转换为毫秒
                        ui->label_now_time->setText("当前时间：" + QString::number(currentTimestampMillis) + "  " + currentDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz"));   // 将时间戳转为字符串显示
                    });


// 限制对应输入框只能输入日期
    // 定义一个正则表达式 timeRegex，匹配带毫秒级的时间格式 yyyy-MM-dd HH:mm:ss.zzz
    QRegularExpression timeRegex(R"(^(\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]) (?:[01]\d|2[0-3]):[0-5]\d:[0-5]\d\.\d{3}$)");
    // 时间转时间戳的 值输入框
    QRegularExpressionValidator *timeValidator_timeToTimestamp = new QRegularExpressionValidator(timeRegex, ui->lineEdit_timeToTimestamp_value);
    ui->lineEdit_timeToTimestamp_value->setValidator(timeValidator_timeToTimestamp);    // 设置 QLineEdit 的验证器
    // 时间戳转时间的 结果输入框
    QRegularExpressionValidator *timeValidator_timestampTotime = new QRegularExpressionValidator(timeRegex, ui->lineEdit_timestampToTime_result);
    ui->lineEdit_timestampToTime_result->setValidator(timeValidator_timestampTotime);
    // 时间计算器的被减数
    QRegularExpressionValidator *timeValidator_time_minuend = new QRegularExpressionValidator(timeRegex, ui->lineEdit_time_minuend);
    ui->lineEdit_timestampToTime_result->setValidator(timeValidator_time_minuend);
    // 时间计算器的减数
    QRegularExpressionValidator *timeValidator_time_subtrahend = new QRegularExpressionValidator(timeRegex, ui->lineEdit_time_subtrahend);
    ui->lineEdit_time_subtrahend->setValidator(timeValidator_time_subtrahend);

}




