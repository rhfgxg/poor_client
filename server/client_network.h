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

    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QByteArray &message);

signals:
    // 使用信号传递发回的json数据给对应接口
    void loginResponse(const QJsonObject &request);     // 登录结果
    void uploadsResponse(const QJsonObject &request);   // 文件上传结果

private slots:
    void onConnected(); // 如果链接成功
    void onError(); // 如果连接失败
    void readData();    // 接受服务器发来的数据

private:
    QTcpSocket *tcpSocket;
};

#endif // CLIENT_NETWORK_H
