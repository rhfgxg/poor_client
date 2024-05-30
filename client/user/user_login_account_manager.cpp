#include "./user_login_account_manager.h"
#include <QJsonObject>
#include <QJsonDocument>

UserLoginAccountManager::UserLoginAccountManager(QObject *parent)
    : QObject(parent),
    network(new ClientNetwork(this))
{
    connucts();
}

UserLoginAccountManager::~UserLoginAccountManager()
{
    delete network;
}

// 账号登录按钮调用此函数，传入账号和密码
void UserLoginAccountManager::login(const QString &username, const QString &password)
{
    QJsonObject request;
    request["username"] = username;
    request["password"] = password;

    QJsonDocument messageDoc(request);
    QByteArray message = messageDoc.toJson();

    // QByteArray是一个灵活的字节数组类，它既可以用来存储原始的二进制数据，也可以存储文本数据。在网络通信中，它常被用作传输数据的基本单位
    // QDataStream 提供了一种类型安全的方式来读写C++的数据结构，包括基本类型、Qt的容器类以及自定义类型。
    // 通过QDataStream，开发者可以方便地将复杂的数据结构序列化为字节流（写入到QByteArray或其他设备），
    // 或者从字节流中反序列化出原始的数据结构（从QByteArray或其他设备读取）
//    QByteArray message;
//    QDataStream out(&message, QIODevice::WriteOnly);    // 绑定message和out
//    out << QString("LOGIN") << username << password;    // 将数据写入流

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

void UserLoginAccountManager::connucts()
{
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::loginResponse, this, &UserLoginAccountManager::handleLoginResponse);

}
