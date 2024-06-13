#ifndef TIMESTAMP_UI_H
#define TIMESTAMP_UI_H

#include "../../../../feature/tools/convertor/timestamp/timestamp.h"
#include "../../../../server/client_network.h"    // 传递套接字
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

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
    Timestamp timestamp;    // 功能类
    ClientNetwork *network; // 服务器通讯套接字

    QPushButton *pushButton_to_convertor;   // 返回转换器界面
    QPushButton *pushButton_timestampToTime_execute;    // 时间戳转时间
    QPushButton *pushButton_timeToTimestamp_execute;    // 时间转时间戳

    QComboBox *comboBox_timestampToTime_value_type; // 时间戳转时间，需要转换的值类型
    QComboBox *comboBox_timestampToTime_result_type;    // 时间戳转时间，结果类型
    QComboBox *comboBox_timeToTimestamp_value_type; // 时间转时间戳，需要转换的值类型
    QComboBox *comboBox_timeToTimestamp_result_type;    // 时间转时间戳，结果类型

    QLineEdit *lineEdit_timestampToTime_value;  // 时间戳转时间，需要转换的值
    QLineEdit *lineEdit_timestampToTime_result; // 时间戳转时间，结果
    QLineEdit *lineEdit_timeToTimestamp_value;  // 时间转时间戳，需要转换的值
    QLineEdit *lineEdit_timeToTimestamp_result; // 时间转时间戳，结果

    QLabel *label_title;    // 界面提示
    QLabel *label_now_time; // 显示当前时间
    QLabel *label_tetle_timeConvertor;  // 时间戳转换
    QLabel *label_timestampToTime;    // 时间戳转时间
    QLabel *label_timeToTimestamp;   // 时间转时间戳

};

#endif // TIMESTAMP_UI_H
