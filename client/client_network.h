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
    void loginResponse(const QString &message);

private slots:
    void readData();
    void handleError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket;
};

#endif // CLIENT_NETWORK_H
