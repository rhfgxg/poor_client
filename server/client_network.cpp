#include "client_network.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QUuid>
#include <QFile>

ClientNetwork::ClientNetwork(QObject *parent)
    : QObject(parent),
    tcpSocket(new QTcpSocket(this))
{
    initClientId();

    // QTcpSocket::readyRead 服务器发送了数据，数据被成功接收并存储在QTcpSocket的内部缓冲区中，Qt就会发出这个信号
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientNetwork::readData);

}

ClientNetwork::~ClientNetwork()
{
    tcpSocket->close();
}

// 连接服务器
// 参数：地址，端口
void ClientNetwork::connectToServer(const QString &host, quint16 port)
{
    // 连接到服务器
    tcpSocket->connectToHost(host, port);

    // 连接成功后，通常会连接信号槽以响应连接成功或失败
    connect(tcpSocket, &QTcpSocket::connected, this, &ClientNetwork::onConnected);
    // 选择QAbstractSocket::errorOccurred信号的一个特定重载版本，该版本接受一个QAbstractSocket::SocketError作为参数。
    // QAbstractSocket::errorOccurred信号在QAbstractSocket发生错误时发射，
    // 它有多个重载形式，但使用QOverload可以帮助你精确地绑定到带有SocketError参数的版本
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &ClientNetwork::onError);

}

// 链接服务器成功
void ClientNetwork::onConnected()
{
    qDebug() << "成功连接到服务器";
    // 使用json对象，打包和发送数据
    QJsonObject request;
    request["type"] = "CONNECT";
    request["client_id"] = client_id;   // 传递客户端id

    QJsonDocument messageDoc(request);
    QByteArray message = messageDoc.toJson();

    sendMessage(message);  // 发送消息
}

// 链接服务器失败
void ClientNetwork::onError()
{
    qDebug() << "连接错误：" << tcpSocket->errorString();
}

// 发送信息
void ClientNetwork::sendMessage(const QByteArray &message)
{
    if (tcpSocket->state() != QAbstractSocket::ConnectedState)
    {
        qDebug() << "套接字未连接";
        return;
    }

    // 写入数据到套接字, 然后发送
    qint64 bytesWritten = tcpSocket->write(message);

    // 检查是否有数据实际被写入
    if(bytesWritten > 0)
    {
        qDebug() << "成功写入" << bytesWritten << "字节数据";
    }
    else
    {
        qDebug() << "写入数据失败：" << tcpSocket->errorString();

    }
}

// 读取服务器响应数据
void ClientNetwork::readData()
{
    // 获取发回的数据
    // 然后检查结果类型，然后发送对应信号

    QByteArray data = tcpSocket->readAll();
    QJsonDocument requestDoc = QJsonDocument::fromJson(data);
    QJsonObject request = requestDoc.object();

    QString type = request["type"].toString();  // 获取返回类型

    if (type == "LOGIN")
    {
        emit loginResponse(request);   // 发射信号，并携带 response
    }


}

// 初始化客户端标记
void ClientNetwork::initClientId()
{
    // 检测本地登录记录的逻辑
    // 例如，读取一个文件或配置项
    QFile file("./data/terminal/.client_message.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "客户端信息打开失败: ";
    }

    // 读取客户端标记
    // 读取第一行：客户端id保存在第一行
    QString line = QTextStream(&file).readLine();

    // 使用 "=" 分割字符串
    QStringList parts = line.split("=", Qt::SkipEmptyParts);

    // 长度小于2或常量不等于client_id
    if (parts.size() < 2 || parts[0] != "CLIENT_ID")
    {
        // 如果本地没有客户端标记
        // 生成一个UUID
        QUuid uuid = QUuid::createUuid();
        client_id = uuid.toString(QUuid::WithoutBraces);    // 转换为无连字符的格式

        // 保存到文件
        file.seek(file.size()); // 移动到文件末尾
        QTextStream out(&file); // 使用QTextStream进行文本读写
        out << "CLIENT_ID=" + client_id << '\n';   // 写入客户端id

        // 关闭文件
        file.close();
        return;
    }

    // 关闭文件
    file.close();

    // 获取 "=" 后面的部分：客户端id
    client_id = parts[1];
}

// 让外界获取client_id
const QString ClientNetwork::returnClientId()
{
    return client_id;
}
