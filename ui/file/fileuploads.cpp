#include "fileuploads.h"
#include "../../mainwindow.h"  // 主界面
#include "../../server/uploads/useruploadsmanager.h"  // 文件上传
#include <QFileDialog>
#include <QVBoxLayout>
FileUploads::FileUploads(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    network(network_),
{
    layout();
    connect(pushButton_to_mainWindows, &QPushButton::clicked, this, &FileUploads::on_pushButton_to_mainWindows_clicked);
    connect(pushButton_filePath, &QPushButton::clicked, this, &FileUploads::on_pushButton_filePath_clicked);
    connect(pushButton_uploads, &QPushButton::clicked, this, &FileUploads::on_pushButton_uploads_clicked);

}

FileUploads::~FileUploads()
{
    delete pushButton_to_mainWindows;
    delete pushButton_filePath;
    delete pushButton_uploads;
    delete lineEdit_filePath;
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
        this->lineEdit_filePath->setText(file_path);
    }
    else
    {
        return;
    }
}

// 上传
void FileUploads::on_pushButton_uploads_clicked()
{
    QString file_path = this->lineEdit_filePath->text();

    UserUploadsManager uploads(network);    // 创建管理对象
    uploads.sendInitialUploadRequest(file_path);   // 创建上传文件任务
}

// 界面初始化
void FileUploads::layout()
{
    setWindowTitle("文件上传");

    pushButton_to_mainWindows = new QPushButton("返回主界面");
    pushButton_filePath = new QPushButton("选择文件");
    pushButton_uploads = new QPushButton("上传文件");

    lineEdit_filePath = new QLineEdit;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pushButton_to_mainWindows);
    layout->addWidget(lineEdit_filePath);
    layout->addWidget(pushButton_filePath);
    layout->addWidget(pushButton_uploads);


}
