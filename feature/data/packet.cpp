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
    QByteArray jsonDataBytes = QJsonDocument(json_data).toJson(QJsonDocument::Compact);

    // 计算数据包大小：数据包size，数据包类型size，json数据包size，文件数据size
    quint32 packetSize = sizeof(quint32) + sizeof(quint32) + jsonDataBytes.size() + file_data.size();
    /* 原计算方式
     * quint32 packetSize = sizeof(quint32) + sizeof(quint32) + (jsonDataBytes.size() + sizeof(quint32)) + (fileData.size() + sizeof(quint32));
     * 由于在写入数据流的时候，
     * 添加json数据和file_data前都会添加4字节来作为标记，
     * 所以这里，添加对应大小来使大小相同
     */

    /* 防止 QDataStream 在序列化时插入额外字符
     * QDataStream在序列化时，会在数据前插入额外的4字节用来保存下一段数据的长度
     * 例如原数据长度为 0，json数据长度为30
     * 在序列化后，数据包长度= 34 = 4(保存长度) + 30(实际数据)
     * 使用 QDataStream的 writeRawData(char*, int len) 进行序列化可以避免插入额外字符
     * 用法：QDataStream().writeRawData(实际数据转char*, 实际数据的长度)
     *
     * QByteArray转char*：QByteArray.constData()
     *
     * quint32转char*：先转QString，再转QByteArray，然后转char*
     * QString::number(packetSize)：转QString
     * QString.toUtf8()：转QByteArray
     * QByteArray.constData()转char*
     */

    // 写入数据包大小
    data_stream.writeRawData(QString::number(packetSize).toUtf8().constData(), sizeof(packetSize));
    // 写入数据包类型
    data_stream.writeRawData(QString::number(static_cast<quint32>(type)).toUtf8().constData(), sizeof(static_cast<quint32>(type)));
    // 写入 JSON 数据
    data_stream.writeRawData(jsonDataBytes.constData(), jsonDataBytes.size());
    // 写入文件切片数据
    data_stream.writeRawData(file_data.constData(), file_data.size());

    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);

        quint32 packet_size; // 数据包大小

        // 读取数据包大小
        stream >> packet_size;

        qDebug() << "发送数据" << byte_array;
        qDebug() << packet_size;
    }

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
    stream >> packet_size;
    // 读取数据包类型
    stream >> type_int;
    // 读取 JSON 数据大小和数据
    quint32 json_data_size;
    {
        stream >> json_data_size;   // 读取 JSON 数据的大小：在序列化时额外插入了4字节的长度信息
        json_data.resize(json_data_size);   // 调整json_data 的长度，确保 json_data 有足够的空间来存储即将从数据流中读取的数据。
        stream.readRawData(json_data.data(), json_data_size);   // 从 stream 中读取 json_data_size 字节的数据，并将其存储在 json_data 中
        /* json_data.data() 返回一个指向 json_data 内部数据的指针，readRawData 方法会将读取的数据直接写入这个指针所指向的内存区域 */
    }
    // 读取文件切片数据大小和数据
    quint32 file_data_size;
    {
        stream >> file_data_size;
        file_data.resize(file_data_size);
        stream.readRawData(file_data.data(), file_data_size);
    }

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
