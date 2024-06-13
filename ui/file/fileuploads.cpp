#include "fileuploads.h"
#include "ui_fileuploads.h"
#include "../../mainwindow.h"  // 主界面
#include "../../server/uploads/useruploadsmanager.h"  // 文件上传
#include <QFileDialog>

FileUploads::FileUploads(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileUploads),
    network(network_)
{
    ui->setupUi(this);
}

FileUploads::~FileUploads()
{
    delete ui;
    delete network;
}

// 返回主界面
void FileUploads::on_pushButton_to_mainWindows_clicked()
{
    this->close();
    MainWindow* w = new MainWindow(network);
    w->show();
}

// 选择要上传的文件
void FileUploads::on_pushButton_filePath_clicked()
{
    // QFileDialog::getOpenFileName 方法用于获取用户选择的文件名
    // 第一个参数是你希望文件对话框作为父窗口的 QWidget 指针，
    // 第二个参数是对话框的标题，
    // 第三个参数是默认打开的目录，
    // 第四个参数是文件过滤器，用于限制可选的文件类型。在这个例子中，我们允许用户选择所有类型的文件，但特别指出了文本文件作为选项
    QString file_path = QFileDialog::getOpenFileName(this, tr("选择文件"), "", tr("所有文件 (*);;文本文件 (*.zip)"));

    if (!file_path.isEmpty())    // 如果打开了文件
    {
        ui->lineEdit_filePath->setText(file_path);
    }
    else
    {
        return;
    }
}

// 上传
void FileUploads::on_pushButton_uploads_clicked()
{
    QString file_path = ui->lineEdit_filePath->text();

    UserUploadsManager uploads(network);    // 创建管理对象
    uploads.uploads(file_path);   // 上传文件
}

