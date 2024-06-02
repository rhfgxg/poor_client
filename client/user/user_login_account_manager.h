#ifndef USER_LOGIN_ACCOUNT_MANAGER_H
#define USER_LOGIN_ACCOUNT_MANAGER_H

#include "../client_network.h"
#include <QObject>

// 用户登录管理类
class UserLoginAccountManager : public QObject
{
    Q_OBJECT

public:
    explicit UserLoginAccountManager(ClientNetwork *network_, QObject *parent = nullptr);
    ~UserLoginAccountManager();

    void login(const QString &username, const QString &password);

signals:
    void loginSuccess();
    void loginFailed(const QString &reason);

private slots:
    void handleLoginResponse(const QString &message);

private:
    ClientNetwork *network;    // 创建服务器通信对象，来接受他的信号
};


#endif // USER_LOGIN_ACCOUNT_MANAGER_H
