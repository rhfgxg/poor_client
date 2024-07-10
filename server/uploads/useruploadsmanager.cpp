#include "useruploadsmanager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include "../../feature/tools/encipher/base64.h"

UserUploadsManager::UserUploadsManager(ClientNetwork *network_, QObject *parent) :
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
    QString user_id = "user123"; // 用户ID
    QString client_id = network->clientId(); // 客户端ID
    qint64 total_size = file.size(); // 文件总大小

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传：文件打开失败: " << file_path;
        return;
    }

    QJsonObject request;
    request["type"] = "INITIAL_UPLOAD"; // 第一次上传
    request["file_name"] = file_name;    // 文件名
    request["file_path"] = file_path;   // 文件路径，通过服务器间接传递给具体执行上传的函数
    request["total_size"] = total_size;  // 总大小
    request["user_id"] = user_id;    // 用户ID
    request["client_id"] = client_id;    // 客户端ID

    QJsonDocument json_doc(request);
    QByteArray json_data = json_doc.toJson();

    network->sendMessage(json_data); // 发送数据给服务器
}

// 参数：文件路径，偏移量（已发送文件大小）
// 当偏移量为0，表示文件第一次发送
// 传入偏移量时，表示已发送部分文件（断点续传功能）从偏移量区域开始切片并传输
void UserUploadsManager::uploadFileInChunks(const QString& file_path, qint64 offset)
{
    QFile file(file_path);  // 文件路径
    const qint64 chunkSize = 1024 * 1024; // 每个切片1MB
    QByteArray fileData;    // 文件数据
    qint64 bytes_sent = offset;  // 已发送字节

    QJsonObject request;
    request["type"] = "UPLOAD_CHUNK";
    request["file_id"] = current_file_id; // 文件id
    request["totalSize"] = file.size();   // 文件总大小

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传：文件打开失败: " << file_path;
        return;
    }

    // 将文件指针定位到偏移量
    if (!file.seek(offset))
    {
        qDebug() << "文件上传：文件定位失败: " << file_path;
        return;
    }

    while (!file.atEnd())   // 循环直到文件末尾
    {
        // todo：防止上传时源文件被修改

        fileData = file.read(chunkSize);    // 每次读取一片的大小，保存在fileData
        QString base64Data = Base64().toBase64(fileData);  // 转 base64编码
        // 更新json数据包
        request["filedata"] = base64Data;   // 切片后文件块
        request["offset"] = offset; // 已发送文件大小

        // 序列化数据
        QJsonDocument json_doc(request);
        QByteArray json_data = json_doc.toJson();
        network->sendMessage(json_data); // 发送数据给服务器

        bytes_sent += fileData.size();   // 更新已发送的字节数 bytesSent，加上当前读取的数据块大小。
    }

    file.close();
}

// 接受服务器发回数据：发回的文件id
void UserUploadsManager::onReadyRead(const QJsonObject &request)
{
    QString status = request["status"].toString();

    if (status == "ok")
    {
        current_file_id = request["file_id"].toString();
        qDebug() << "初始上传请求成功，文件ID：" << current_file_id;
        uploadFileInChunks(request["file_path"].toString());   // 初始化成功：开始执行上传任务
        // todo：将任务加入任务列表（配置文件）
    }
    else
    {
        qDebug() << "初始上传请求失败";
    }

}
