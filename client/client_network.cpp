#include "client_network.h"
#include <QDataStream>
#include <QDebug>

ClientNetwork::ClientNetwork(QObject *parent)
    : QObject(parent),
    tcpSocket(new QTcpSocket(this))
{
    // QTcpSocket::readyRead 服务器发送了数据，数据被成功接收并存储在QTcpSocket的内部缓冲区中，Qt就会发出这个信号
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientNetwork::readData);
    // 选择QAbstractSocket::errorOccurred信号的一个特定重载版本，该版本接受一个QAbstractSocket::SocketError作为参数。
    // QAbstractSocket::errorOccurred信号在QAbstractSocket发生错误时发射，
    // 它有多个重载形式，但使用QOverload可以帮助你精确地绑定到带有SocketError参数的版本
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &ClientNetwork::handleError);

}

ClientNetwork::~ClientNetwork()
{
    tcpSocket->close();
}

// 连接服务器
// 参数：地址，端口
void ClientNetwork::connectToServer(const QString &host, quint16 port)
{
    tcpSocket->connectToHost(host, port);   // 连接服务器
    if (tcpSocket->waitForConnected(3000))  // 等待3000毫秒，再次确定是否连接到服务器
    {
        qDebug() << "成功连接到服务器";
    }
    else
    {
        qDebug() << "连接服务器失败";
    }
}

// 发送信息
void ClientNetwork::sendMessage(const QByteArray &message)
{
    tcpSocket->write(message);
}

// 读取服务器响应数据
void ClientNetwork::readData()
{
    QByteArray data = tcpSocket->readAll(); // 读取缓冲区数据，保存到data
    QDataStream in(&data, QIODevice::ReadOnly); // 读取data，写入到in
    QString response;
    // 从数据流中读取数据并存储到变量response中，
    // 如果response是一个基本数据类型（如int、double、QString等），这行代码会尝试从数据流中读取相应类型的数据并赋值给response
    // 如果是自定义类型，需要重载 operator>>以便与QDataStream兼容
    in >> response;
    emit loginResponse(response);   // 发射信号，并携带 response
}

// 处理通信异常
void ClientNetwork::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "套接字异常:" << tcpSocket->errorString();
}
