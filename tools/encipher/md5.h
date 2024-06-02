#ifndef MD5_H
#define MD5_H

#include <QObject>

// Md5加密类
class Md5
{
public:
    Md5();

    QString md5_encipher(QString str); // md5加密
    bool md5_verify(QString str, QString md5);   // md5校验，传入字符串与md5值，如果两个值相同，返回true

};

#endif // MD5_H
