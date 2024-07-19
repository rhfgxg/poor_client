#include "packet.h"
#include <QIODevice>

// 默认构造函数
Packet::Packet() :
    type(OTHER)
{

}

// 构造函数：只携带json数据
Packet::Packet(PacketType type, const QJsonObject &json_data) :
    type(type),
    json_data(json_data)
{

}

// 构造函数：携带文件切片数据
Packet::Packet(PacketType type, const QJsonObject &json_data, const QByteArray &file_data) :
    type(type),
    json_data(json_data),
    file_data(file_data)
{

}

// 将数据包转换为字节流
QByteArray Packet::toByteArray() const
{
    QByteArray byte_array;
    QDataStream data_stream(&byte_array, QIODevice::WriteOnly);   // 用来序列化数据

    // 序列化json子包
    QByteArray json_data_bytes = QJsonDocument(json_data).toJson(QJsonDocument::Compact);

    // 计算数据包大小：数据包size，数据包类型size，json数据包size，文件数据size
    quint32 packet_size = sizeof(quint32) + sizeof(quint32) + (sizeof(quint32) + json_data_bytes.size()) + (sizeof(quint32)+file_data.size());
    /* 原计算方式
     * quint32 packetSize = sizeof(quint32) + sizeof(quint32) + (jsonDataBytes.size() + sizeof(quint32)) + (fileData.size() + sizeof(quint32));
     * 由于在写入数据流的时候，
     * 添加json数据和file_data前都会添加4字节来作为标记，
     * 所以这里，添加对应大小来使大小相同
     */

    // 写入数据包大小
    data_stream.writeRawData(reinterpret_cast<const char*>(&packet_size), sizeof(packet_size));
    // 写入数据包类型
    data_stream.writeRawData(reinterpret_cast<const char*>(&type), sizeof(static_cast<quint32>(type)));
    // 写入 JSON 数据
    data_stream << json_data_bytes;
    // 写入文件切片数据
    data_stream << file_data;

    return byte_array;
}

// 从字节流解析数据包
Packet Packet::fromByteArray(const QByteArray &byte_array)
{
    QDataStream stream(byte_array);

    quint32 packet_size; // 数据包大小
    quint32 type_int;    // 数据包类型枚举整形值
    QByteArray json_data;// json子包
    QByteArray file_data;// 文件切片数据子包

    // 读取数据包大小
    stream.readRawData(reinterpret_cast<char*>(&packet_size), sizeof(packet_size));
    // 读取数据包类型
    stream.readRawData(reinterpret_cast<char*>(&type_int), sizeof(type_int));
    // 读取 JSON 数据大小和数据
    stream >> json_data;
    // 读取文件切片数据大小和数据
    stream >> file_data;

    PacketType type = static_cast<PacketType>(type_int); // 枚举值 转 文件类型枚举
    QJsonObject json_object = QJsonDocument::fromJson(json_data).object();    // 解析json子包

    return Packet(type, json_object, file_data);
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
void Packet::setJsonData(const QJsonObject &json_data)
{
    this->json_data = json_data;
}

// 获取json数据包
QJsonObject Packet::getJsonData() const
{
    return json_data;
}

// 设置文件切片数据
void Packet::setFileData(const QByteArray &file_data)
{
    this->file_data = file_data;
}

// 获取文件切片数据
QByteArray Packet::getFileData() const
{
    return file_data;
}
