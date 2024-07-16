#include "useruploadsmanager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include "../../feature/data/packet.h"  // 自定义数据包

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

    // json子数据包
    QJsonObject request;
    request["file_name"] = file_name;    // 文件名
    request["file_format"] = file_format;   // 文件拓展名
    request["file_size"] = file_size;  // 总大小
    request["account"] = account;    // 用户账号
    request["client_id"] = client_id;    // 客户端ID
    request["file_path"] = file_path;   // 文件路径，通过服务器间接传递给具体执行上传的函数

    // 使用数据头和json数据包，打包进自定义数据包
    Packet message(PacketType::INITIAL_UPLOAD, request);
    network->sendMessage(message.toByteArray());  // 序列化后，由通信管理发送给服务端
}

// 文件切片上传
void UserUploadsManager::uploadFileInChunks(const QString file_id, const QString& file_path, qint64 offset)
{/* 参数：文件路径，偏移量（已发送文件大小）
  * 当偏移量为0，表示文件第一次发送
  * 传入偏移量时，表示已发送部分文件（断点续传功能）从偏移量区域开始切片并传输
  */
    QFile file(file_path);  // 文件路径
    const qint64 chunk_size = 1024 * 1024; // 每个切片1MB
    QByteArray file_data;    // 文件数据

    Packet request;  // 上传的数据
    request.setType(PacketType::UPLOAD_CHUNK);   // 设置数据头
    QJsonObject request_json;    // 上传的子数据包
    request_json["file_id"] = file_id; // 文件id

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
    {// todo：防止上传时源文件被修改
        file_data = file.read(chunk_size);  // 每次读取一片的大小
        request.setFileData(file_data); // 将文件块加入数据包

        request_json["offset"] = offset; // 文件偏移地址（文件块下标）
        request.setJsonData(request_json);  // 添加json子数据包

        // 序列化数据，发送数据给服务器
        network->sendMessage(request.toByteArray());

        // 更新文件偏移地址（文件块下标）offset，加上当前读取的数据块大小。
        offset += file_data.size();
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

        uploadFileInChunks(file_id, file_path);   // 初始化成功：开始执行上传任务

        // todo：将任务加入任务列表（配置文件）
    }
    else
    {
        qDebug() << "初始上传请求失败";
    }
}
