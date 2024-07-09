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

/*
// 上传文件
// 传入文件地址：绝对地址
void UserUploadsManager::uploads(QString file_path)
{
// 将文件转为 base64格式，然后上传服务器
// 在服务器解析回文件
    // 读取文件
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传：文件打开失败: " << file_path;
        return;
    }
    file.close();

    QString base64Data = {Base64().toBase64(file_path)};    // 将文件转换为Base64编码的字符串

    QJsonObject jsonObj;    // 创建JSON对象
    jsonObj["type"] = "UPLOADS";    // 数据包类型
    jsonObj["filename"] = QFileInfo(file_path).fileName();   // 文件名
    jsonObj["filedata"] = base64Data;   // Base64编码的文件数据

    // 序列化JSON对象，转为二进制数据用于传输
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    // 发送数据到服务器
    network->sendMessage(jsonData);
}
*/

// 参数：文件路径，偏移量（已发送文件大小）
// 当偏移量为0，表示文件第一次发送
// 传入偏移量时，表示已发送部分文件（断点续传功能）从偏移量区域开始切片并传输
void UserUploadsManager::uploadFileInChunks(QString file_path, qint64 offset)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传：文件打开失败: " << file_path;
        return;
    }

    if (offset == 0) // 只在首次上传时发送初始请求
    {
        QString userId = "user123"; // 假设从用户登录信息获取用户ID
        QString clientId = "client123"; // 假设从客户端配置获取客户端ID
        sendInitialUploadRequest(QFileInfo(file_path).fileName(), file.size(), userId, clientId);
    }

    // 将文件指针定位到偏移量
    if (!file.seek(offset))
    {
        qDebug() << "文件上传：文件定位失败: " << file_path;
        return;
    }

    const qint64 chunkSize = 1024 * 1024; // 每个切片1MB
    QByteArray fileData;    // 文件数据
    qint64 totalSize = file.size(); // 文件总大小
    qint64 bytesSent = offset;  // 已发送字节

    while (!file.atEnd())   // 循环直到文件末尾
    {
        fileData = file.read(chunkSize);    // 每次读取一片的大小，保存在fileData
        sendChunk(fileData, QFileInfo(file_path).fileName(), bytesSent, totalSize); // 调用 sendChunk 函数，发送给服务端
        bytesSent += fileData.size();   // 更新已发送的字节数 bytesSent，加上当前读取的数据块大小。
    }

    file.close();
}

void UserUploadsManager::sendInitialUploadRequest(const QString &fileName, qint64 totalSize, const QString &userId, const QString &clientId)
{
    QJsonObject jsonObj;
    jsonObj["type"] = "INITIAL_UPLOAD"; // 第一次上传
    jsonObj["filename"] = fileName;
    jsonObj["totalSize"] = totalSize;
    jsonObj["userId"] = userId;
    jsonObj["clientId"] = clientId;

    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    network->sendMessage(jsonData); // 发送数据给服务器
}

// 将切片的函数发送给服务器
// 参数：读取的文件数据块、文件名、当前已发送的字节数，文件总大小
void UserUploadsManager::sendChunk(const QByteArray &chunkData, const QString &fileName, qint64 offset, qint64 totalSize)
{
    QString base64Data = Base64().toBase64(chunkData);  // 转 base64编码

    // json数据包
    QJsonObject jsonObj;
    jsonObj["type"] = "UPLOAD_CHUNK";
    jsonObj["filename"] = fileName; // 文件名
    jsonObj["filedata"] = base64Data;   // 切片后文件块
    jsonObj["offset"] = offset; // 已发送文件大小
    jsonObj["totalSize"] = totalSize;   // 文件总大小

    // 序列化数据
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    network->sendMessage(jsonData); // 发送数据给服务器
}

void UserUploadsManager::onReadyRead(const QJsonObject &request)
{
    QString status = request["status"].toString();

    if (status == "ok")
    {
        current_file_id = request["file_id"].toString();
        qDebug() << "初始上传请求成功，文件ID：" << current_file_id;
    }
    else
    {
        qDebug() << "初始上传请求失败";
    }

}
