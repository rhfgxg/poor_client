#ifndef TIMESTAMP_UI_H
#define TIMESTAMP_UI_H

#include "./timestamp.h"
#include "../../../client/client_network.h"    // 传递套接字
#include <QWidget>

namespace Ui {
class TimestampUi;
}

// 时间戳转换器页面
class TimestampUi : public QWidget
{
    Q_OBJECT

public:
    explicit TimestampUi(ClientNetwork *network_, QWidget *parent = nullptr);
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
    ClientNetwork *network; // 服务器通讯套接字
};

#endif // TIMESTAMP_UI_H
