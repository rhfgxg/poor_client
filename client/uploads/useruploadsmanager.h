#ifndef USERUPLOADSMANAGER_H
#define USERUPLOADSMANAGER_H

#include <QObject>
#include "../client_network.h"

class UserUploadsManager : public QObject
{
public:
    UserUploadsManager(ClientNetwork *network_, QObject *parent = nullptr); // 参数：服务器通信管理对象
    ~UserUploadsManager();

    void uploads(QString file_path); // 向服务器发送数据，参数：上传的文件名

private slots:
    void handleUploadsResponse(const QJsonObject &request);   // 处理服务器发回的响应数据，发出上传成功或失败信号

private:
    ClientNetwork *network;    // 创建服务器通信对象，来接受他的信号
};

#endif // USERUPLOADSMANAGER_H
