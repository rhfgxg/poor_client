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
    connect(network, &ClientNetwork::uploadsResponse, this, &UserUploadsManager::handleUploadsResponse);

}

UserUploadsManager::~UserUploadsManager()
{
    delete network;
}

// 上传文件
// 传入文件地址：绝对地址
void UserUploadsManager::uploads(QString file_path)
{
// 将文件转为 base64格式，然后上传服务器
// 在服务器解析回文件
    // 读取文件
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件打开失败: " << file_path;
        return;
    }
    // 将文件转换为Base64编码的字符串
    QString base64Data = {Base64().toBase64(file_path)};
//    QByteArray fileData = file.readAll();
//    QString base64Data = fileData.toBase64();

    // 创建JSON对象
    QJsonObject jsonObj;
    jsonObj["type"] = "UPLOADS";    // 数据类型为上传数据
    jsonObj["filename"] = QFileInfo(file).fileName(); // 文件名
    jsonObj["filedata"] = base64Data; // Base64编码的文件数据

    // 序列化JSON对象
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    // 发送数据到服务器
    network->sendMessage(jsonData);
}

void UserUploadsManager::handleUploadsResponse(const QJsonObject &request)  // 处理服务器响应数据
{

}
