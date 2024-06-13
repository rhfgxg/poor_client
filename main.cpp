#include "mainwindow.h"
#include "./ui/user/user_login_phone.h"
#include "./server/client_network.h" // 与服务器通信

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

    ClientNetwork *network = new ClientNetwork();
    network->connectToServer("127.0.0.1", 1234);

//    if (!hasLocalLoginRecord())
//    {// 如果本地没有登录记录，进行登录活动
//        UserLoginPhone *w = new UserLoginPhone(network);
//        w->show();
//    }
//    else
    {
        // 进入软件
        MainWindow *w = new MainWindow(network);
        w->show();
    }

    return a.exec();
}



