#include "client_network.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QUuid>
#include <QFile>
#include "../feature/data/packet.h" // 自定义数据包

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

// 链接服务器
// 参数：地址，端口
void ClientNetwork::connectToServer(const QString &host, quint16 port)
{
    // 连接到服务器
    tcpSocket->connectToHost(host, port);

// 连接成功后，通常会连接信号槽以响应连接成功或失败
    connect(tcpSocket, &QTcpSocket::connected, this, &ClientNetwork::onConnected);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &ClientNetwork::onError);
    /* QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred)做信号
     * 选择QAbstractSocket::errorOccurred信号的一个特定重载版本，该版本接受一个QAbstractSocket::SocketError作为参数。
     * QAbstractSocket::errorOccurred信号在QAbstractSocket发生错误时发射，
     * 它有多个重载形式，但使用QOverload可以帮助你精确地绑定到带有SocketError参数的版本
     */
}

// 链接服务器成功
void ClientNetwork::onConnected()
{
    qDebug() << "成功连接到服务器";

    // 添加json 子数据包
    QJsonObject request;
    request["client_id"] = client_id;   // 传递客户端id，记录客户端链接日志

    Packet packet(PacketType::LOGIN, request);  // 添加数据头，打包数据

    // 序列化数据，发给服务端
    sendMessage(packet.toByteArray());
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
    /* write和flush区别
     * write 将数据写入到内部缓冲区。依赖于 Qt 框架的自动缓冲和发送机制，异步操作
     * flush 强制将内部缓冲区中的数据发送，同步操作，会阻塞线程，直到数据发送完毕
     */

    // 检查是否有数据实际被写入
    if(bytesWritten <= 0)
    {
        qDebug() << "写入数据失败：" << tcpSocket->errorString();
    }
    else
    {
//        qDebug() << "成功发送" << bytesWritten << "字节数据";
//        tcpSocket->flush(); // 确保数据立即发送，立刻清空缓冲区，发送数据
    }
}

// 读取服务器响应数据
void ClientNetwork::readData()
{
    qDebug("客户端收到消息");
    // 获取发回的数据
    // 然后检查结果类型，然后发送对应信号

    QByteArray data = tcpSocket->readAll();
    Packet request = Packet::fromByteArray(data);    // 反序列化数据包

    // 根据服务器返回结果，发射对应信号，通知对象
    if (request.getType() == PacketType::LOGIN)    // 登录结果
    {
        emit loginResponse(request.getJsonData());
    }
    else if (request.getType() == PacketType::INITIAL_UPLOAD) // 文件上传任务初始化结果
    {
        emit uploadsInitResponse(request.getJsonData());
    }
}

// 初始化客户端标记
void ClientNetwork::initClientId()
{
/* 读取本地配置文件，文件记录生成的客户端uuid
 * 假设uuid不会重复（理论上存在重复的可能）
 * 重复概率太低，客户端id重复产生的损失也不足以为此浪费性能每次都检测一次id
 */
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

    // 长度小于2或常量不等于client_id，生成id
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
const QString ClientNetwork::clientId()
{
    return client_id;
}
