#ifndef USERUPLOADSMANAGER_H
#define USERUPLOADSMANAGER_H

#include <QObject>
#include "../client_network.h"

// 文件上传等任务，在账号登录期间保存在客户端(使用配置文件)
// 账号退出时，结束所有相关任务

// 使用文件上传任务，先调用sendInitialUploadRequest，初始化上传任务
// 根据发回的数据，决定是否开始正式执行上传任务
class UserUploadsManager : public QObject
{
public:
    explicit UserUploadsManager(ClientNetwork *network_, QObject *parent = nullptr);
    ~UserUploadsManager();

    // 初始化文件上传任务，分配文件id，创建数据库数据，准备接收文件
    void sendInitialUploadRequest(const QString& file_path);

    // 参数：文件路径，偏移量（已发送文件大小，当偏移量为0，表示文件第一次发送）
    // 传入偏移量时，表示已发送部分文件（断点续传功能）从偏移量区域开始切片并传输
    void uploadFileInChunks(const QString file_id, const QString& file_path, QVector<bool> missing_parts);

private slots:
    void onReadyReadUploadsInit(const QJsonObject &request); // 处理返回信息，调用 uploadFileInChunks函数 返回信息：第一次上传时返回文件ID
    void onReadyReadUploadsComplete(const QJsonObject &request);    // 处理文件上传结果(是否完整上传)

private:
    ClientNetwork *network;    // 服务器通信套接字，用于接受信号
};

#endif // USERUPLOADSMANAGER_H
