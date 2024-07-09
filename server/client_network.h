#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include <QTcpSocket>
#include <QObject>

// 服务器通信管理类
class ClientNetwork : public QObject
{
    Q_OBJECT

public:
    explicit ClientNetwork(QObject *parent = nullptr);
    ~ClientNetwork();

    void connectToServer(const QString &host, quint16 port);    // 链接服务器
    void sendMessage(const QByteArray &message);    // 发送数据
    const QString returnClientId();   // 获取客户端标记

signals:
    // 使用信号传递发回的json数据给对应接口
    void loginResponse(const QJsonObject &request);     // 登录结果
    void uploadsInitResponse(const QJsonObject &request);   // 文件上传结果

private slots:
    void onConnected(); // 如果链接成功
    void onError(); // 如果连接失败
    void readData();    // 接受服务器发来的数据

private:
    void initClientId(); // 创建或在本地获取客户端id，然后初始化成员：client_id

    QTcpSocket *tcpSocket;  // 套接字
    QString client_id;   // 客户端标记ID，全球唯一的UUid
};

#endif // CLIENT_NETWORK_H
