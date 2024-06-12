#ifndef CONVERTOR_H
#define CONVERTOR_H

#include "../../client/client_network.h"
#include <QWidget>

namespace Ui {
class Convertor;
}

// 转换工具集页面
class Convertor : public QWidget
{
    Q_OBJECT
public:
    explicit Convertor(ClientNetwork *network_, QWidget *parent = nullptr);
    ~Convertor();


private slots:
    void on_pushButton_to_tools_clicked();  // 返回 工具集页面

    void on_pushButton_to_timestamp_clicked();  // 打开 时间戳转换器页面


private:
    Ui::Convertor *ui;
    ClientNetwork *network; // 服务器通讯套接字
};

#endif // CONVERTOR_H
