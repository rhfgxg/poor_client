#ifndef USER_LOGIN_H
#define USER_LOGIN_H

#include "../../server/user/user_manager.h"
#include <QWidget>

// 用户账号密码登录界面
namespace Ui {
class UserLoginAccount;
}

class UserLoginAccount : public QWidget
{
    Q_OBJECT

public:
    explicit UserLoginAccount(ClientNetwork *network_, QWidget *parent = nullptr);
    ~UserLoginAccount();

private slots:
// 自定义槽函数：
    void on_show_pushButton_login();    // 在账号和密码都输入内容后使登录按钮可以被点击

    void on_login_response_success();    // 登录成功
    void on_login_response_failed(const QString &reason);    // 登录失败

// 槽函数
    void on_pushButton_back_clicked();  // 返回上一级

    void on_pushButton_help_clicked();  // 帮助

    void on_pushButton_look_password_clicked(); // 显示密码按钮

    void on_pushButton_login_clicked(); // 登录按钮

    void on_pushButton_login_phone_clicked();   // 跳转短信验证码登录

    void on_pushButton_wechat_clicked();    // 跳转微信登录

    void on_pushButton_qq_clicked();    // 跳转qq登录

private:
    void layout();  // 界面格式初始化

private:
    Ui::UserLoginAccount *ui;
    UserManager *user_manager; // 创建一个用户管理对象，方便接收来自他的信号

    ClientNetwork *network;
};

#endif // USER_LOGIN_H
