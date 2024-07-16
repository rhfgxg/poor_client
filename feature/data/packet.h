#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

enum PacketType // 数据头类型
{
    CLIENT_CONNECT,    // 客户端链接
    LOGIN,  // 登录
    RESISTER,   // 注册
    INITIAL_UPLOAD, // 初始化文件切片上传任务
    UPLOAD_CHUNK, // 文件切片上传
    OTHER   // 其他数据
};


class Packet
{
public:
    Packet();   // 默认构造函数
    // 构造函数：只携带json数据
    Packet(PacketType type, const QJsonObject &jsonData);
    // 构造函数：携带文件切片数据
    Packet(PacketType type, const QJsonObject &jsonData, const QByteArray &fileData);

    // 将数据包转换为字节流
    QByteArray toByteArray() const;
    // 从字节流解析数据包
    static Packet fromByteArray(const QByteArray &byteArray);

    void setType(PacketType type); // 设置数据包类型
    PacketType getType() const; // 获取数据包类型

    void setJsonData(const QJsonObject &jsonData); // 设置json数据包
    QJsonObject getJsonData() const;    // 获取json数据包

    void setFileData(const QByteArray &fileData); // 设置数据包类型
    QByteArray getFileData() const; // 获取文件切片数据


private:
    PacketType type;    // 数据包类型
    QJsonObject jsonData;   // json数据包：携带其他文件附加信息 或 携带登录信息等
    QByteArray fileData;    // 文件切片数据
};

#endif // PACKET_H
