#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QObject>
#include <QDateTime>

// 时间戳类
class Timestamp
{
public:
    Timestamp();
    // 时间转时间戳, 默认使用 上海时间 转 毫秒
    // 参数：QString需要转换的时间，QString传入时间的类型，QString转换后结果的类型
    qint64 time_to_timestamp(QDateTime time_value, QString currentText_value = "Asia/Shanghai", QString currentText_result = "毫秒(ms)");
    QString time_to_timestamp(QString time_value, QString currentText_value = "Asia/Shanghai", QString currentText_result = "毫秒(ms)");

    // 时间戳转时间, 默认使用 毫秒 转 上海时间
    // 参数：QString需要转换的时间戳，QString传入时间戳的类型，QString转换后结果的类型
    QDateTime timestamp_to_time(qint64 timestamp_value, QString currentText_value = "毫秒(ms)", QString currentText_result = "Asia/Shanghai");
    QString timestamp_to_time(QString timestamp_value, QString currentText_value = "毫秒(ms)", QString currentText_result = "Asia/Shanghai");

};

#endif // TIMESTAMP_H

// 使用默认参数值时，只能在类的声明中指定默认参数值，而不应该在函数实现中再次指定
// 编译器一般会将声明与实现看成两个重载函数
// C++编译器会认为这是函数重载而不是默认参数的重复定义
