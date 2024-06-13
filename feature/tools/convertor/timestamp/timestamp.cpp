#include "timestamp.h"

Timestamp::Timestamp()
{

}

// 时间转时间戳（窗口）
qint64 Timestamp::time_to_timestamp(QDateTime time_value, QString currentText_value, QString currentText_result) // 时间转时间戳
{
    if (currentText_value == "Asia/Shanghai")   // 如果时区是上海
    {
        if (currentText_result == "毫秒(ms)") // 如果结果类型是毫秒
        {
            qint64 timestamp = time_value.toMSecsSinceEpoch();
            return timestamp;
        }
        else if (currentText_result == "秒(s)")
        {
            qint64 timestamp = time_value.toSecsSinceEpoch();
            return timestamp;
        }
    }

    return 0;
}

// 时间转时间戳（终端）
QString Timestamp::time_to_timestamp(QString time_value, QString currentText_value, QString currentText_result) // 时间转时间戳
{
    // 将字符串转换为 QDateTime 对象
    // 参数：时间字符串QString，时间格式
    QDateTime time = QDateTime::fromString(time_value, "yyyy-MM-dd HH:mm:ss.zzz");

    if (currentText_value == "Asia/Shanghai")   // 如果时区是上海
    {
        if (currentText_result == "毫秒(ms)") // 如果结果类型是毫秒
        {
            qint64 timestamp = time.toMSecsSinceEpoch();
            return QString::number(timestamp);
        }
        else if (currentText_result == "秒(s)")
        {
            qint64 timestamp = time.toSecsSinceEpoch();
            return QString::number(timestamp);
        }
    }

    return 0;
}

// 时间戳转时间（窗口）
QDateTime Timestamp::timestamp_to_time(qint64 timestamp_value, QString currentText_value, QString currentText_result)  // 时间戳转时间
{
    if(currentText_value == "毫秒(ms)")
    {
        QDateTime time = QDateTime::fromMSecsSinceEpoch(timestamp_value);
        return time;
    }
    else if (currentText_value == "秒(s)")
    {
        QDateTime time = QDateTime::fromSecsSinceEpoch(timestamp_value);
        return time;
    }

    QDateTime nul = QDateTime::fromString("1970-01-01 01:01:01.000", "yyyy-MM-dd HH:mm:ss.zzz");
    return nul;
}

// 时间戳转时间（终端）
QString Timestamp::timestamp_to_time(QString timestamp_value, QString currentText_value, QString currentText_result)
{
// 传入的字符串转时间戳
    bool conversionOk = false;  // 转换是否成功
    qint64 timestamp = timestamp_value.toLongLong(&conversionOk);
    if (!conversionOk)
    {
        qDebug() << "转换失败，字符串不能转换为有效的qint64数值";
    }
// 时间戳转时间，然后转字符串返回
    if(currentText_value == "毫秒(ms)")
    {
        QDateTime time = QDateTime::fromMSecsSinceEpoch(timestamp);
        return time.toString("yyyy-MM-dd HH:mm:ss.zzz");
    }
    else if (currentText_value == "秒(s)")
    {
        QDateTime time = QDateTime::fromSecsSinceEpoch(timestamp);
        return time.toString("yyyy-MM-dd HH:mm:ss.zzz");
    }

    return "1970-01-01 01:01:01.000";
}
