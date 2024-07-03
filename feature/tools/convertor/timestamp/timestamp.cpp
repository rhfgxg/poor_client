#include "timestamp.h"
#include <QTimeZone>    // 设置时区，只会影响单个time对象

Timestamp::Timestamp()
{

}

// 时间转时间戳（终端）
QString Timestamp::time_to_timestamp(QString time_value, QString time_zone, QString timestamp_format) // 时间转时间戳
{
    // 将字符串转换为 QDateTime 对象
    // 参数：时间字符串QString，时间格式
    QDateTime time;
    if (time_value.length() == 23)
    { // 长度为23时，包含毫秒部分
        time = QDateTime::fromString(time_value, "yyyy-MM-dd HH:mm:ss.zzz");
    }
    else if (time_value.length() == 19)
    { // 长度为19时，不包含毫秒部分
        time = QDateTime::fromString(time_value, "yyyy-MM-dd HH:mm:ss");
    }

    // 设置时区
    QTimeZone timeZone(time_zone.toUtf8());
    time.setTimeZone(timeZone);

    if (timestamp_format == "毫秒(ms)") // 如果结果类型是毫秒
    {
        qint64 timestamp = time.toMSecsSinceEpoch();

        return QString::number(timestamp);
    }
    else if (timestamp_format == "秒(s)")
    {
        qint64 timestamp = time.toSecsSinceEpoch();
        return QString::number(timestamp);
    }


    return 0;
}

// 时间戳转时间（终端）
QString Timestamp::timestamp_to_time(QString timestamp_value, QString timestamp_format, QString time_zone)
{
    // 将传入的字符串转换为时间戳
    bool conversionOk = false;  // 转换是否成功
    qint64 timestamp = timestamp_value.toLongLong(&conversionOk);
    if (!conversionOk)
    {
        qDebug() << "转换失败，时间戳不能转换为有效的qint64数值";
        return "1970-01-01 01:01:01.000";
    }

    // 创建 QDateTime 对象
    QDateTime time;
    if (timestamp_format == "毫秒(ms)")
    {
        time = QDateTime::fromMSecsSinceEpoch(timestamp);
    }
    else if (timestamp_format == "秒(s)")
    {
        time = QDateTime::fromSecsSinceEpoch(timestamp);
    }

    // 设置结果的时区
    QTimeZone timeZone(time_zone.toUtf8());

    // 将时间转换为指定时区
    time.setTimeZone(timeZone);

    // 返回格式化后的时间字符串
    return time.toString("yyyy-MM-dd HH:mm:ss.zzz");
}

