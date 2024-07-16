#include "./user_manager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkInterface>
#include "../../feature/data/packet.h"  // 自定义数据包

UserManager::UserManager(ClientNetwork *network_, QObject *parent) :
    QObject(parent),
    network(network_)
{
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::loginResponse, this, &UserManager::handleLoginResponse);
}

UserManager::~UserManager()
{

}

// 账号注册调用此函数
void UserManager::user_register(const QString &user_name, const QString &password_hash, const QString email, const QString phone_number)
{
    // json子数据包
    QJsonObject request;
    request["user_name"] = user_name;   // 用户名
    request["password_hash"] = password_hash; // 用户密码
    request["email"] = email;   // 邮箱
    request["phone_number"] = phone_number; // 手机号码
    request["client_id"] = network->clientId();   // 传递客户端id

    // 打包进自定义数据包
    Packet message(PacketType::RESISTER, request);

    network->sendMessage(message.toByteArray());  // 序列化后，由通信管理发送给服务端
}

// 账号登录调用此函数
void UserManager::user_login(const QString &account, const QString &password_hash)
{
    // json子数据包
    QJsonObject request;
    request["account"] = account; // 账号
    request["password_hash"] = password_hash; // 用户密码
    request["client_id"] = network->clientId();   // 传递客户端id

    // 使用数据头和json数据包，打包进自定义数据包
    Packet message(PacketType::LOGIN, request);
    network->sendMessage(message.toByteArray());  // 序列化后，由通信管理发送给服务端
}

// 根据服务器发回的消息 message，处理登录结果
void UserManager::handleLoginResponse(const QJsonObject &request)
{
    QString status = request["status"].toString();

    if (status == "SUCCESS")
    {
        QString tooken = request["tooken"].toString();
        emit loginSuccess(tooken);    // 发送登录成功信号，被账号登录类接收
    }
    else
    {
        QString message = request["message"].toString();
        emit loginFailed(message);  // 如果登录失败，发回的消息会携带失败内容
    }
}

