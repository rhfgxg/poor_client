#ifndef USER_LOGIN_PHONE_H
#define USER_LOGIN_PHONE_H

#include "../../server/client_network.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

// 用户验证码登录界面
namespace Ui {
class UserLoginPhone;
}

class UserLoginPhone : public QWidget
{
    Q_OBJECT

public:
    explicit UserLoginPhone(ClientNetwork *network_, QWidget *parent = nullptr);
    ~UserLoginPhone();

private slots:
    void on_pushButton_back_clicked();  // 返回上一级

    void on_pushButton_send_code_clicked();     // 发送验证码

    void on_pushButton_login_account_clicked();    // 使用密码登录

    void on_pushButton_help_clicked();  // 帮助界面

    void on_pushButton_wechat_clicked();    // 跳转微信登录

    void on_pushButton_qq_clicked();    // 跳转qq


private:
    void layout();  // 界面格式初始化
    void connects();    // 信号槽关联初始化


private:
    ClientNetwork *network;

    QPushButton *pushButton_back;   // 返回个人界面
    QPushButton *pushButton_help;   // 帮助按钮
    QPushButton *pushButton_send_code;  // 发送验证码界面
    QPushButton *pushButton_login_account;  // 账号登陆界面
    QPushButton *pushButton_wechat; // 微信登录
    QPushButton *pushButton_qq; // qq登录

    QLabel *label_tip;  // 提示
    QLabel *label_phone;    // 提示输入手机号

    QLineEdit *lineEdit_phone;  // 手机号输入框
    QCheckBox *checkBox_protocol;   // 同意协议

};

#endif // USER_LOGIN_PHONE_H
