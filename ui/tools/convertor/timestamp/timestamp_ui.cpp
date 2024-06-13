#include "./timestamp_ui.h"
#include "../convertor.h"

#include <QTimer>
#include <QDateTime>    // 使用时间戳
#include <QRegularExpression>   // 使用正则
#include <QRegularExpressionValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>

TimestampUi::TimestampUi(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    network(network_)
{
//    timestamp;  // 初始化功能对象

    layout();   // 界面格式初始化
    connect(pushButton_to_convertor, &QPushButton::clicked, this, &TimestampUi::on_pushButton_to_convertor_clicked);

}

TimestampUi::~TimestampUi()
{
    delete network;

    delete pushButton_to_convertor;   // 返回转换器界面

    delete label_title;    // 界面提示
    delete label_now_time; // 显示当前时间

    // 时间戳转时间
    delete label_timestampToTime;    // 时间戳转时间
    delete lineEdit_timestampToTime_value;  // 时间戳转时间，需要转换的值
    delete comboBox_timestampToTime_value_type; // 时间戳转时间，需要转换的值类型
    delete pushButton_timestampToTime_execute;    // 时间戳转时间
    delete lineEdit_timestampToTime_result; // 时间戳转时间，结果
    delete comboBox_timestampToTime_result_type;    // 时间戳转时间，结果类型

    // 时间转时间戳
    delete label_timeToTimestamp;   // 时间转时间戳
    delete lineEdit_timeToTimestamp_value;  // 时间转时间戳，需要转换的值
    delete comboBox_timeToTimestamp_value_type; // 时间转时间戳，需要转换的值类型
    delete pushButton_timeToTimestamp_execute;    // 时间转时间戳
    delete lineEdit_timeToTimestamp_result; // 时间转时间戳，结果
    delete comboBox_timeToTimestamp_result_type;    // 时间转时间戳，结果类型
}


void TimestampUi::on_pushButton_to_convertor_clicked()    // 返回上一级：转换器总览
{
    this->close();
    Convertor* w = new Convertor(network);
    w->show();
}

void TimestampUi::on_pushButton_timestampToTime_execute_clicked()  // 时间戳转时间
{
    QString value = this->lineEdit_timestampToTime_value->text();

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

    QDateTime timestamp_result = timestamp.timestamp_to_time(timestamp_value, this->comboBox_timestampToTime_value_type->currentText(), this->comboBox_timestampToTime_result_type->currentText());
    this->lineEdit_timestampToTime_result->setText(timestamp_result.toString("yyyy-MM-dd HH:mm:ss.zzz"));   // 转换成字符串显示
}

void TimestampUi::on_pushButton_timeToTimestamp_execute_clicked() // 时间转时间戳
{
    QString value = this->lineEdit_timeToTimestamp_value->text();

    if (value.isEmpty())
    {// 如果没有输入，直接结束函数
        return;
    }

    // 将字符串转换为 QDateTime 对象
    // 参数：时间字符串QString，时间格式
    QDateTime time_value = QDateTime::fromString(value, "yyyy-MM-dd HH:mm:ss.zzz");

    // 将 QDateTime 对象转换为 Unix 时间戳
    qint64 timestamp_result = timestamp.time_to_timestamp(time_value, this->comboBox_timeToTimestamp_value_type->currentText(), this->comboBox_timeToTimestamp_result_type->currentText());
    this->lineEdit_timeToTimestamp_result->setText(QString::number(timestamp_result));   // 转换成字符串显示

    if (this->comboBox_timeToTimestamp_result_type->currentText() != "秒(s)" && this->comboBox_timeToTimestamp_result_type->currentText() != "毫秒(ms)")
    {
        qDebug("时间转时间戳 结果类型异常");
    }
}


void TimestampUi::layout()   // 界面格式初始化
{
    pushButton_to_convertor = new QPushButton("返回转换器界面");   // 返回转换器界面

    label_title = new QLabel("获取本机时间，然后计算自1970年1月1日到当前的时间");    // 界面提示
    label_now_time = new QLabel("当前时间："); // 显示当前时间
    QVBoxLayout *vBoxLayout_titel = new QVBoxLayout;
    vBoxLayout_titel->addWidget(label_title);
    vBoxLayout_titel->addWidget(label_now_time);

    // 时间戳转时间
    label_timestampToTime = new QLabel("时间戳>时间");    // 时间戳转时间
    lineEdit_timestampToTime_value = new QLineEdit;  // 时间戳转时间，需要转换的值
    comboBox_timestampToTime_value_type = new QComboBox; // 时间戳转时间，需要转换的值类型
        comboBox_timestampToTime_value_type->addItem("毫秒(ms)");
        comboBox_timestampToTime_value_type->addItem("秒(s)");
    pushButton_timestampToTime_execute = new QPushButton("转换>");    // 时间戳转时间
    lineEdit_timestampToTime_result = new QLineEdit; // 时间戳转时间，结果
    comboBox_timestampToTime_result_type = new QComboBox;    // 时间戳转时间，结果类型
        comboBox_timestampToTime_result_type->addItem("Asia/Shanghai");
    QHBoxLayout *hBoxLayout_timestamp_to_time = new QHBoxLayout;
    hBoxLayout_timestamp_to_time->addWidget(label_timestampToTime);
    hBoxLayout_timestamp_to_time->addWidget(lineEdit_timestampToTime_value);
    hBoxLayout_timestamp_to_time->addWidget(comboBox_timestampToTime_value_type);
    hBoxLayout_timestamp_to_time->addWidget(pushButton_timestampToTime_execute);
    hBoxLayout_timestamp_to_time->addWidget(lineEdit_timestampToTime_result);
    hBoxLayout_timestamp_to_time->addWidget(comboBox_timestampToTime_result_type);


    // 时间转时间戳
    label_timeToTimestamp = new QLabel("时间>时间戳");   // 时间转时间戳
    lineEdit_timeToTimestamp_value = new QLineEdit;  // 时间转时间戳，需要转换的值
    comboBox_timeToTimestamp_value_type = new QComboBox; // 时间转时间戳，需要转换的值类型
        comboBox_timeToTimestamp_value_type->addItem("Asia/Shanghai");
    pushButton_timeToTimestamp_execute = new QPushButton("转换>");    // 时间转时间戳
    lineEdit_timeToTimestamp_result = new QLineEdit; // 时间转时间戳，结果
    comboBox_timeToTimestamp_result_type = new QComboBox;    // 时间转时间戳，结果类型
        comboBox_timeToTimestamp_result_type->addItem("毫秒(ms)");
        comboBox_timeToTimestamp_result_type->addItem("秒(s)");
    QHBoxLayout *hBoxLayout_time_to_timestamp = new QHBoxLayout;
    hBoxLayout_time_to_timestamp->addWidget(label_timeToTimestamp);
    hBoxLayout_time_to_timestamp->addWidget(lineEdit_timeToTimestamp_value);
    hBoxLayout_time_to_timestamp->addWidget(comboBox_timeToTimestamp_value_type);
    hBoxLayout_time_to_timestamp->addWidget(pushButton_timeToTimestamp_execute);
    hBoxLayout_time_to_timestamp->addWidget(lineEdit_timeToTimestamp_result);
    hBoxLayout_time_to_timestamp->addWidget(comboBox_timeToTimestamp_result_type);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addWidget(pushButton_to_convertor);
    vBoxLayout->addLayout(vBoxLayout_titel);
    vBoxLayout->addLayout(hBoxLayout_timestamp_to_time);
    vBoxLayout->addLayout(hBoxLayout_time_to_timestamp);

// 获取当前时间，转换成时间戳，显示在副标题，每秒更新一次
    QDateTime date_time = QDateTime::currentDateTime();   // 获取当前日期和时间的 QDateTime 对象
    //    qint64 currentTimestamp = currentDateTime.toSecsSinceEpoch();   // 转换为秒
    qint64 current_timestamp_millis = date_time.toMSecsSinceEpoch();    // 转换为毫秒
    this->label_now_time->setText("当前时间：" + QString::number(current_timestamp_millis) + "  " + date_time.toString("yyyy-MM-dd HH:mm:ss.zzz"));   // 将时间戳转为字符串显示

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
                        this->label_now_time->setText("当前时间：" + QString::number(currentTimestampMillis) + "  " + currentDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz"));   // 将时间戳转为字符串显示
                    });


// 限制对应输入框只能输入日期
    // 定义一个正则表达式 timeRegex，匹配带毫秒级的时间格式 yyyy-MM-dd HH:mm:ss.zzz
    QRegularExpression timeRegex(R"(^(\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]) (?:[01]\d|2[0-3]):[0-5]\d:[0-5]\d\.\d{3}$)");
    // 时间转时间戳的 值输入框
    QRegularExpressionValidator *timeValidator_timeToTimestamp = new QRegularExpressionValidator(timeRegex, this->lineEdit_timeToTimestamp_value);
    this->lineEdit_timeToTimestamp_value->setValidator(timeValidator_timeToTimestamp);    // 设置 QLineEdit 的验证器
    // 时间戳转时间的 结果输入框
    QRegularExpressionValidator *timeValidator_timestampTotime = new QRegularExpressionValidator(timeRegex, this->lineEdit_timestampToTime_result);
    this->lineEdit_timestampToTime_result->setValidator(timeValidator_timestampTotime);
//    // 时间计算器的被减数
//    QRegularExpressionValidator *timeValidator_time_minuend = new QRegularExpressionValidator(timeRegex, ui->lineEdit_time_minuend);
//    this->lineEdit_timestampToTime_result->setValidator(timeValidator_time_minuend);
//    // 时间计算器的减数
//    QRegularExpressionValidator *timeValidator_time_subtrahend = new QRegularExpressionValidator(timeRegex, ui->lineEdit_time_subtrahend);
//    this->lineEdit_time_subtrahend->setValidator(timeValidator_time_subtrahend);

}




