#ifndef FILEUPLOADS_H
#define FILEUPLOADS_H

#include "../../server/client_network.h"    // 传递套接字
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class FileUploads;
}

class FileUploads : public QWidget
{
    Q_OBJECT

public:
    explicit FileUploads(ClientNetwork *network_, QWidget *parent = nullptr);   // 传入通信套接字
    ~FileUploads();

private slots:
    void on_pushButton_to_mainWindows_clicked();    // 按钮：返回主界面

    void on_pushButton_filePath_clicked();  // 按钮：选择要上传的文件

    void on_pushButton_uploads_clicked();   // 按钮：上传文件

private:
    void layout();  // 界面初始化
    ClientNetwork *network; // 服务器通讯套接字

    QPushButton *pushButton_to_mainWindows; // 返回主界面
    QPushButton *pushButton_filePath;   // 选择文件，添加文件路径
    QPushButton *pushButton_uploads;    // 文件上传

    QLineEdit *lineEdit_filePath;   // 文件路径输入框
};

#endif // FILEUPLOADS_H
