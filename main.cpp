#include "mainwindow.h"
#include "./user/user_login_phone.h"
#include "./client/client_network.h" // 与服务器通信

#include <QApplication>
#include <QFile>

bool hasLocalLoginRecord()
{
    // 检测本地登录记录的逻辑
    // 例如，读取一个文件或配置项
    return QFile::exists("login_record.txt");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!hasLocalLoginRecord())
    {// 如果本地没有登录记录，进行登录活动
        ClientNetwork *network = new ClientNetwork();
        network->connectToServer("127.0.0.1", 49674);
        
        UserLoginPhone *w = new UserLoginPhone();
        w->show();
    }
    else
    {
        MainWindow *w = new MainWindow;
        w->show();
    }

    return a.exec();
}



