#ifndef BASE64_H
#define BASE64_H

#include <QObject>

// 文件转base64编码
// 方便进行网络传输
class Base64
{
public:
    Base64();
    QString toBase64(QString file_path);    // 转base64格式，参数：文件路径

};

#endif // BASE64_H
