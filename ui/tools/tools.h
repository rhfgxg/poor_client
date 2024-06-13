#ifndef TOOLS_H
#define TOOLS_H

#include "../../server/client_network.h"    // 传递套接字
#include <QWidget>

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
    Ui::Tools *ui;
    ClientNetwork *network; // 服务器通讯套接字

};

#endif // TOOLS_H
