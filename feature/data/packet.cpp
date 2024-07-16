#include "packet.h"
#include <QIODevice>

// 默认构造函数
Packet::Packet() :
    type(OTHER)
{

}

// 构造函数：只携带json数据
Packet::Packet(PacketType type, const QJsonObject &jsonData) :
    type(type),
    jsonData(jsonData)
{

}

// 构造函数：携带文件切片数据
Packet::Packet(PacketType type, const QJsonObject &jsonData, const QByteArray &fileData) :
    type(type),
    jsonData(jsonData),
    fileData(fileData)
{

}

// 将数据包转换为字节流
QByteArray Packet::toByteArray() const
{
    QByteArray byteArray;

    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << static_cast<quint32>(type);
    stream << QJsonDocument(jsonData).toJson(QJsonDocument::Compact);
    stream << fileData;

    return byteArray;
}

// 从字节流解析数据包
Packet Packet::fromByteArray(const QByteArray &byteArray)
{
    QDataStream stream(byteArray);
    quint32 typeInt;
    QByteArray jsonData;
    QByteArray fileData;

    stream >> typeInt;
    PacketType type = static_cast<PacketType>(typeInt);

    stream >> jsonData;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObject = doc.object();

    stream >> fileData;

    return Packet(type, jsonObject, fileData);
}

// 设置数据包类型
void Packet::setType(PacketType type)
{
    this->type = type;
}

// 获取数据包类型
PacketType Packet::getType() const
{
    return type;
}

// 设置json数据包
void Packet::setJsonData(const QJsonObject &jsonData)
{
    this->jsonData = jsonData;
}

// 获取json数据包
QJsonObject Packet::getJsonData() const
{
    return jsonData;
}

// 设置文件切片数据
void Packet::setFileData(const QByteArray &fileData)
{
    this->fileData = fileData;
}

// 获取文件切片数据
QByteArray Packet::getFileData() const
{
    return fileData;
}
