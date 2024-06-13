#include "./user_manager.h"
#include <QJsonObject>
#include <QJsonDocument>

UserManager::UserManager(ClientNetwork *network_, QObject *parent) :
    QObject(parent),
    network(network_)
{
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::loginResponse, this, &UserManager::handleLoginResponse);
}

UserManager::~UserManager()
{
    delete network;
}

// 账号登录按钮调用此函数，传入账号和密码
void UserManager::login(const QString &username, const QString &password)
{
    // 使用json对象，打包和发送数据
    QJsonObject request;
    request["type"] = "LOGIN";
    request["username"] = username; // 用户名
    request["password"] = password; // 用户密码

    QJsonDocument messageDoc(request);
    QByteArray message = messageDoc.toJson();

    network->sendMessage(message);  // 使用服务器通信类，发送消息
}

// 根据服务器发回的消息 message，处理登录结果
void UserManager::handleLoginResponse(const QJsonObject &request)
{
    qDebug("客户端收到消息");
    QString status = request["status"].toString();

    if (status == "SUCCESS")
    {
        emit loginSuccess();    // 发送登录成功信号，被账号登录类接收
    }
    else
    {
        QString message = request["message"].toString();
        emit loginFailed(message);  // 如果登录失败，发回的消息会携带失败内容
    }
}

