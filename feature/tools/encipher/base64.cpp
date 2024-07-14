#include "base64.h"
#include <QFile>
#include <QDebug>

Base64::Base64()
{

}

QString Base64::toBase64(QString file_path)
{
    QFile file(file_path);  // 创建文件对象
    if (!file.open(QIODevice::ReadOnly))    // 打开文件
    {// 如果打开失败
        qDebug() << "base64：文件打开失败:" << file_path;
        return "";
    }

    // 将文件转换为Base64编码的字符串
    QString base64Data = file.readAll().toBase64();

    // 如果是切片后的文件块，可以直接使用切片后的 QByteArrays对象调用toBase64()
    // 读取文件的一个切片
//    QFile file(file_path);  // 文件对象
//    const qint64 chunkSize = 1024 * 1024; // 每个切片1MB
//    QByteArray fileData;    // 文件数据
//    fileData = file.read(chunkSize);    // 每次读取一个切片的大小，保存在fileData
//    // 转换为 Base64 编码
//    QString base64_data = fileData.toBase64();

    return base64Data;
}
