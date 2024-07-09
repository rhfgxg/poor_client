#ifndef USERUPLOADSMANAGER_H
#define USERUPLOADSMANAGER_H

#include <QObject>
#include "../client_network.h"

class UserUploadsManager : public QObject
{
public:
    explicit UserUploadsManager(ClientNetwork *network_, QObject *parent = nullptr);
    ~UserUploadsManager();

//    void uploads(QString file_path); // 向服务器发送数据
    void sendChunk(const QByteArray &chunkData, const QString &fileName, qint64 offset, qint64 totalSize);  // 将文件块发送到服务端

    // 参数：文件路径，偏移量（已发送文件大小）
    // 当偏移量为0，表示文件第一次发送
    // 传入偏移量时，表示已发送部分文件（断点续传功能）从偏移量区域开始切片并传输
    void uploadFileInChunks(QString file_path, qint64 offset = 0);

private slots:
    void handleUploadsResponse(const QJsonObject &request);   // 处理服务器发回的响应数据
    void onReadyRead(const QJsonObject &request); // 服务端返回信息：第一次上传时返回文件ID

private:
    // 第一次上传文件，发送请求
    void sendInitialUploadRequest(const QString &fileName, qint64 totalSize, const QString &userId, const QString &clientId);

    ClientNetwork *network;    // 服务器通信套接字，用于接受信号
    QString current_file_id; // 存储当前上传文件的文件ID
};

#endif // USERUPLOADSMANAGER_H
