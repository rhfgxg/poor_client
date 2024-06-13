#ifndef TOOLS_H
#define TOOLS_H

#include "../../server/client_network.h"    // 传递套接字
#include <QWidget>
#include <QPushButton>

namespace Ui {
class Tools;
}

// 工具集页面
class Tools : public QWidget
{
    Q_OBJECT

public:
    explicit Tools(ClientNetwork *network_, QWidget *parent = nullptr);
    ~Tools();

private slots:
    void on_pushButton_to_mainWindow_clicked();

    void on_pushButton_to_convertor_clicked();

private:
    void layout();  // 界面初始化


    ClientNetwork *network; // 服务器通讯套接字

    QPushButton *pushButton_to_mainWindow;
    QPushButton *pushButton_to_convertor;
};

#endif // TOOLS_H
