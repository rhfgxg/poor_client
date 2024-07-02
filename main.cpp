#include "mainwindow.h"
#include "./ui/user/user_login_phone.h"
#include "./server/client_network.h" // 与服务器通信

#include <QApplication>
#include <QFile>    // 创建文件
#include <QDir> // 创建文件夹

void openClient(ClientNetwork *network);  // 打开客户端
void fileOk();  // 检查需要的本地文件是否被创建

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 程序启动检查
    fileOk();

    // 链接服务器
    ClientNetwork *network = new ClientNetwork();
    network->connectToServer("127.0.0.1", 1234);

    // 打开客户端
    openClient(network);

    return a.exec();
}

// 打开客户端
void openClient(ClientNetwork *network)
{
    // 检测本地登录记录的逻辑
    // 例如，读取一个文件或配置项
//    if (!QFile::exists("login_record.txt"))
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
}

void fileOk()
{
    // 终端指令记录
    if (!QFile::exists("./data/terminal/home/root/.bash_history.txt"))
    {// 如果文件无法打开，则创建文件及文件夹
        QDir dir;
        if (!dir.mkpath("./data/terminal/home/root/"))
        {
            qDebug() << "创建多级目录失败";
        }
        QFile file("./data/terminal/home/root/.bash_history.txt");
        bool ok = file.open(QIODevice::WriteOnly | QIODevice::Text); // 打开文件以写入
        if (!ok)
        {
            qDebug() << "创建文件失败";
        }
        else
        {
            file.close(); // 关闭文件
        }

    }
    // 客户端信息
    if (!QFile::exists("./data/terminal/.client_message.txt"))
    {// 如果文件无法打开，则创建文件及文件夹
     // 此文件保存客户端标记uuid，登录状态
        QDir dir;
        if (!dir.mkpath("./data/terminal/"))
        {
            qDebug() << "创建多级目录失败";
        }
        QFile file("./data/terminal/.client_message.txt");
        bool ok = file.open(QIODevice::WriteOnly | QIODevice::Text); // 打开文件以写入
        if (!ok)
        {
            qDebug() << "创建文件失败";
        }
        else
        {
            file.close(); // 关闭文件
        }

    }
}
