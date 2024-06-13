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
        qDebug() << "文件打开失败:" << file_path;
        return "";
    }

    // 将文件转换为Base64编码的字符串
    QByteArray fileData = file.readAll();
    QString base64Data = fileData.toBase64();
    return base64Data;
}
