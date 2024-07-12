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

    void login(const QString &account, const QString &password);   // 向服务器发送登录信息，参数：用户名、密码

signals:
    void loginSuccess();    // 登录成功信号
    void loginFailed(const QString &reason);    // 登录失败信号

private slots:
    void handleLoginResponse(const QJsonObject &request);   // 处理服务器发回的响应数据，发出登录成功或失败信号

private:
    ClientNetwork *network;    // 创建服务器通信对象，来接受他的信号
};


#endif // USER_MANAGER_H
