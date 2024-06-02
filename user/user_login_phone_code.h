#ifndef USER_LOGIN_PHONE_CODE_H
#define USER_LOGIN_PHONE_CODE_H

#include "../client/client_network.h"
#include <QWidget>

// 输入验证码界面
// 输入六位验证码自动登录
// 验证码错误进行弹窗提示，清空输入
namespace Ui {
class UserLoginPhoneCode;
}

class UserLoginPhoneCode : public QWidget
{
    Q_OBJECT

public:
    explicit UserLoginPhoneCode(ClientNetwork *network_, QString phone, QWidget *parent = nullptr);
    ~UserLoginPhoneCode();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_help_clicked();

    void on_pushButton_send_code_clicked();

    void on_pushButton_voice_code_clicked();

private:
    void layout();  // 界面格式初始化
    void label_send_code_again_time();  // 再次发送验证码的倒计时
private:
    Ui::UserLoginPhoneCode *ui;
    ClientNetwork *network;
};

#endif // USER_LOGIN_PHONE_CODE_H
