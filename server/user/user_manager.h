#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <QObject>
#include "../client_network.h"

// 用户登录管理类
class UserManager : public QObject
{
    Q_OBJECT

public:
    explicit UserManager(ClientNetwork *network_, QObject *parent = nullptr);   // 参数：服务器通信管理对象
    ~UserManager();

    void user_login(const QString &account, const QString &password);   // 向服务器发送登录信息，参数：账号、密码
    void user_register(const QString &user_name, const QString &password, const QString email, const QString phone_number);// 向服务器发送注册信息

signals:
    void loginSuccess(const QString tooken);    // 登录成功信号：携带本次的tooken
    void loginFailed(const QString &reason);    // 登录失败信号

private slots:
    void handleLoginResponse(const QJsonObject &request);   // 处理服务器发回的响应数据，发出登录成功或失败信号

private:
    ClientNetwork *network;    // 创建服务器通信对象，来接受他的信号
};


#endif // USER_MANAGER_H
