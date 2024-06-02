#include "./user_login_account_manager.h"
#include <QJsonObject>
#include <QJsonDocument>


UserLoginAccountManager::UserLoginAccountManager(ClientNetwork *network_, QObject *parent)
    : QObject(parent),
    network(network_)
{
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::loginResponse, this, &UserLoginAccountManager::handleLoginResponse);
}

UserLoginAccountManager::~UserLoginAccountManager()
{
    delete network;
}

// 账号登录按钮调用此函数，传入账号和密码
void UserLoginAccountManager::login(const QString &username, const QString &password)
{

    QJsonObject request;
    request["type"] = "LOGIN";

    request["username"] = username;
    request["password"] = password;

    QJsonDocument messageDoc(request);
    QByteArray message = messageDoc.toJson();

    network->sendMessage(message);  // 使用服务器通信类，发送消息
}

// 根据服务器发回的消息 message，处理登录结果
void UserLoginAccountManager::handleLoginResponse(const QString &message)
{
    if (message == "SUCCESS")
    {
        emit loginSuccess();    // 发送登录成功信号，被账号登录类接收
    }
    else
    {
        emit loginFailed(message);  // 如果登录失败，发回的消息会携带失败内容
    }
}

