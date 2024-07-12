#include "md5.h"
#include <QCryptographicHash>  //md5加密的库

Md5::Md5()
{

}

QString Md5::md5_encipher(QString str) // md5加密
{
    QByteArray MD5 = QCryptographicHash::hash(str.toLatin1(),QCryptographicHash::Md5);  // 加密
    QString result = MD5.toHex();   // 转为QString

    return result;
}

bool Md5::md5_verify(QString str, QString md5)   // md5校验，传入字符串与md5值，比较两个值是否相同
{
    QString result;
    QByteArray MD5;
    MD5 = QCryptographicHash::hash(str.toLatin1(),QCryptographicHash::Md5);
    result.append(MD5.toHex());

    if (result == str)
    {
        return true;
    }
    else
    {
        return false;
    }

}
