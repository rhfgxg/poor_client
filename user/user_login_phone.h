#ifndef USER_LOGIN_PHONE_H
#define USER_LOGIN_PHONE_H

#include "../client/client_network.h"
#include <QWidget>

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
    Ui::UserLoginPhone *ui;
    ClientNetwork *network;
};

#endif // USER_LOGIN_PHONE_H
