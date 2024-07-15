#include "useruploadsmanager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <iostream>

UserUploadsManager::UserUploadsManager(ClientNetwork *network_, QObject *parent)  :
    QObject(parent),
    network(network_)
{
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::uploadsInitResponse, this, &UserUploadsManager::onReadyRead);
}

UserUploadsManager::~UserUploadsManager()
{

}

// 初始化文件上传任务，分配文件id，创建数据库数据，准备接收文件
void UserUploadsManager::sendInitialUploadRequest(const QString& file_path)
{
    QFile file(file_path);  // 文件路径
    QString file_name = QFileInfo(file_path).fileName();    // 文件原始名
    QString account = "123"; // 用户ID
    QString client_id = network->clientId(); // 客户端ID
    qint64 file_size = file.size(); // 文件总大小

    QString file_format = QFileInfo(file_path).suffix().toLower();    // 查看文件扩展名
//    QStringList allowedExtensions = {"jpg", "png", "txt", "pdf"};
//    if (!allowedExtensions.contains(file_format))
//    {// 禁止上传的文件格式
//        qDebug() << "不允许的文件扩展名:" << fileExtension;
//        return;
//    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传任务初始化：文件打开失败: " << file_path;
        return;
    }

    QJsonObject request;
    request["type"] = "INITIAL_UPLOAD"; // 第一次上传
    request["file_name"] = file_name;    // 文件名
    request["file_format"] = file_format;   // 文件拓展名
    request["file_size"] = file_size;  // 总大小
    request["account"] = account;    // 用户账号
    request["client_id"] = client_id;    // 客户端ID
    request["file_path"] = file_path;   // 文件路径，通过服务器间接传递给具体执行上传的函数

    QJsonDocument json_doc(request);
    QByteArray json_data = json_doc.toJson();

    network->sendMessage(json_data); // 发送数据给服务器
}

// 文件切片上传
void UserUploadsManager::uploadFileInChunks(const QString file_id, const QString& file_path, qint64 offset)
{/* 参数：文件路径，偏移量（已发送文件大小）
  * 当偏移量为0，表示文件第一次发送
  * 传入偏移量时，表示已发送部分文件（断点续传功能）从偏移量区域开始切片并传输
  */
    QFile file(file_path);  // 文件路径
    const qint64 chunk_size = 1024 * 30; // 每个切片30kb：经过测试，在目前，30kb可以保持一个稳定的传输质量，切片传输时不会出现json解析异常
    QByteArray file_data;    // 文件数据

    QJsonObject request;    // 上传的数据
    request["type"] = "UPLOAD_CHUNK";
    request["file_id"] = file_id; // 文件id

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传：文件打开失败: " << file_path;
        return;
    }

    // 将文件指针定位到偏移量，从此位置开始切片，默认为0,从开始位置切片
    if (!file.seek(offset))
    {
        qDebug() << "文件上传：文件定位失败: " << file_path;
        return;
    }
    while (!file.atEnd())   // 循环直到文件末尾，每次发送一个文件块
    {
        // todo：防止上传时源文件被修改
        file_data = file.read(chunk_size);    // 每次读取一片的大小，保存在fileData
        QString base64_data = file_data.toBase64();  // 转 base64编码

        request["file_data"] = base64_data;   // 切片后文件块
        request["offset"] = offset; // 已发送文件大小

        // 序列化数据，发送数据给服务器
        QJsonDocument json_doc(request);
        QByteArray json_data = json_doc.toJson();
        network->sendMessage(json_data);

        offset += file_data.size();   // 更新已发送的字节数 bytesSent，加上当前读取的数据块大小。
    }
    file.close();
}

// 接受服务器发回数据：发回的文件id
void UserUploadsManager::onReadyRead(const QJsonObject &request)
{
    QString status = request["status"].toString();

    if (status == "SUCCESS")
    {
        QString file_id  = request["file_id"].toString();    // 文件ID
        QString file_path = request["file_path"].toString();// 文件路径

        qDebug() << "初始上传请求成功";

        uploadFileInChunks(file_id, file_path);   // 初始化成功：开始执行上传任务

        // todo：将任务加入任务列表（配置文件）
    }
    else
    {
        qDebug() << "初始上传请求失败";
    }
}
