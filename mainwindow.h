#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./server/client_network.h"    // 传递套接字
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 主界面类
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ClientNetwork *network_, QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_to_tools_clicked();  // 工具包界面
    void on_pushButton_terminal_clicked();  // 终端界面
    void on_pushButton_uploads_clicked();   // 文件上传测试

private:
    void layout();  // 界面初始化
    ClientNetwork *network; // 服务器通讯套接字
//    Ui::MainWindow *ui;
    QWidget *ui;
    QPushButton *pushButton_to_tools;   // 前往工具节目
    QPushButton *pushButton_terminal;   // 前往终端界面
    QPushButton *pushButton_uploads;    // 前往文件上传界面

};
#endif // MAINWINDOW_H
