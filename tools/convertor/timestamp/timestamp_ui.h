#ifndef TIMESTAMP_UI_H
#define TIMESTAMP_UI_H

#include "./timestamp.h"
#include <QWidget>

namespace Ui {
class TimestampUi;
}

// 时间戳转换器页面
class TimestampUi : public QWidget
{
    Q_OBJECT

public:
    explicit TimestampUi(QWidget *parent = nullptr);
    ~TimestampUi();

private:
    void layout();   // 界面格式初始化


private slots:
    // 时间转时间戳
    void on_pushButton_timeToTimestamp_execute_clicked();   // 时间转时间戳

    void on_pushButton_to_convertor_clicked();

    void on_pushButton_timestampToTime_execute_clicked();

private:
    Ui::TimestampUi *ui;
    Timestamp timestamp;
};

#endif // TIMESTAMP_UI_H
