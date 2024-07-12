#include "user_login_phone.h"
#include "./user_login_account.h"   // 账号登陆界面
#include "./user_login_phone_code.h"    // 验证码登录界面
#include <QFont>    // 字体
#include <QMessageBox>  // 弹窗
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>

// 手机号登录
UserLoginPhone::UserLoginPhone(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    network(network_)
{
    layout();   // 界面格式初始化
    connect(pushButton_back, &QPushButton::clicked, this, &UserLoginPhone::on_pushButton_back_clicked);
    connect(pushButton_help, &QPushButton::clicked, this, &UserLoginPhone::on_pushButton_help_clicked);
    connect(pushButton_send_code, &QPushButton::clicked, this, &UserLoginPhone::on_pushButton_send_code_clicked);
    connect(pushButton_login_account, &QPushButton::clicked, this, &UserLoginPhone::on_pushButton_login_account_clicked);
    connect(pushButton_wechat, &QPushButton::clicked, this, &UserLoginPhone::on_pushButton_wechat_clicked);
    connect(pushButton_qq, &QPushButton::clicked, this, &UserLoginPhone::on_pushButton_qq_clicked);
}

UserLoginPhone::~UserLoginPhone()
{
    delete pushButton_back;     // 返回个人界面
    delete pushButton_help;  // 帮助按钮
    delete pushButton_send_code; // 发送验证码界面
    delete pushButton_login_account; // 账号登陆界面
    delete pushButton_wechat;    // 微信登录
    delete pushButton_qq;    // qq登录

    delete label_tip; // 提示
    delete label_phone;   // 提示输入手机号

    delete lineEdit_phone; // 手机号输入框
    delete checkBox_protocol;  // 同意协议
}

// 槽函数
void UserLoginPhone::on_pushButton_back_clicked() // 按钮：返回上一级
{
    // 返回个人界面
}

void UserLoginPhone::on_pushButton_help_clicked() // 按钮：帮助界面
{
    // 底部弹出悬浮窗两个按钮：
    // 手机号不可用，绑定新手机：跳转到账号申述
    // 其他常见账号问题：跳转到答疑界面
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(" ");
    msgBox->setText("请同意《软件协议》《个人信息保护指引》后登录");
    msgBox->setIcon(QMessageBox::Information);
    // 添加弹窗内按钮
    QPushButton *button_statement = msgBox->addButton(QMessageBox::Yes);
    button_statement->setText("手机号不可用，绑定新手机");
    QPushButton *button_other = msgBox->addButton(QMessageBox::No);
    button_other->setText("其他常见账号问题");
    // 显示消息框并等待用户响应
    msgBox->exec();

    // 根据用户选择执行相应操作
    if (msgBox->clickedButton() == button_statement)
    {
        msgBox->close(); // 关闭弹窗

    }
    else if (msgBox->clickedButton() == button_other)
    {
        msgBox->close(); // 关闭弹窗
    }

}

void UserLoginPhone::on_pushButton_send_code_clicked()    // 按钮：发送验证码
{
    if (!this->checkBox_protocol->isChecked())
    {
        // 未勾选协议：弹窗：提示需要同意协议
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle(" ");
        msgBox->setText("请同意《软件协议》《个人信息保护指引》后登录");
        msgBox->setIcon(QMessageBox::Information);
        // 添加弹窗内按钮
        QPushButton *yesButton = msgBox->addButton(QMessageBox::Yes);   // 同意协议
        yesButton->setText("同意相关协议");
        QPushButton *noButton = msgBox->addButton(QMessageBox::No); // 取消
        noButton->setText("取消");
        // 显示消息框并等待用户响应
        msgBox->exec();

        // 根据用户选择执行相应操作
        if (msgBox->clickedButton() == yesButton)
        {
            this->checkBox_protocol->setChecked(true);    // 勾选协议
            msgBox->close(); // 关闭弹窗
        }
        else if (msgBox->clickedButton() == noButton)
        {
            msgBox->close(); // 关闭弹窗
            return;
        }      
    }
    this->close();
    // 打开输入验证码界面
    QString phone = this->lineEdit_phone->text(); // 获取输入的手机号
    UserLoginPhoneCode* w = new UserLoginPhoneCode(network, phone);    // 打开验证码输入界面并传入手机号
    w->show();
}

void UserLoginPhone::on_pushButton_login_account_clicked()   // 按钮：使用密码登录
{
    this->close();
    // 打开密码登录界面
    UserLoginAccount* w = new UserLoginAccount(network);
    w->show();
}

void UserLoginPhone::on_pushButton_wechat_clicked()   // 跳转微信登录
{
    if (this->checkBox_protocol->isChecked())
    {// 勾选协议，正常跳转

    }
    else
    {// 未勾选协议，弹窗：提示需要同意协议
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle(" ");
        msgBox->setText("请同意《软件协议》《个人信息保护指引》后登录");
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setStandardButtons(QMessageBox::NoButton);  // 不显示同意按钮
        msgBox->show(); // 显示弹窗

        // 创建一个定时器来关闭弹窗
        QTimer::singleShot(3000, this,
                           [msgBox]()
                           {
                               if (msgBox != nullptr)  // 确保msgBox不是nullptr
                               {
                                   msgBox->close(); // 关闭弹窗
                               }
                           }
                           );
    }
}

void UserLoginPhone::on_pushButton_qq_clicked()   // 跳转qq登录
{
    if (this->checkBox_protocol->isChecked())
    {// 勾选协议，正常跳转

    }
    else
    {// 未勾选协议，弹窗：提示需要同意协议
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle(" ");
        msgBox->setText("请同意《软件协议》《个人信息保护指引》后登录");
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setStandardButtons(QMessageBox::NoButton);  // 不显示同意按钮
        msgBox->show(); // 显示弹窗

        // 创建一个定时器来关闭弹窗
        QTimer::singleShot(3000, this,
                           [msgBox]()
                           {
                               if (msgBox != nullptr)  // 确保msgBox不是nullptr
                               {
                                   msgBox->close(); // 关闭弹窗
                               }
                           }
                           );
    }
}

// 自定义私有函数
void UserLoginPhone::layout() // 界面格式初始化
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
        QHBoxLayout *hBpxLayout_help = new QHBoxLayout;
            pushButton_back = new QPushButton("返回");     // 返回个人界面
            pushButton_help = new QPushButton("帮助");  // 帮助按钮
        hBpxLayout_help->addWidget(pushButton_back);
        hBpxLayout_help->addWidget(pushButton_help);

        label_tip = new QLabel("未注册手机号验证后自动登录"); // 提示

        QHBoxLayout *hBpxLayout_phone = new QHBoxLayout;
            label_phone = new QLabel("手机号");   // 提示输入手机号
            lineEdit_phone = new QLineEdit; // 手机号输入框
                lineEdit_phone->setPlaceholderText("请输入11位手机号");
        hBpxLayout_phone->addWidget(label_phone);
        hBpxLayout_phone->addWidget(lineEdit_phone);

        QVBoxLayout *vBoxLayout_login = new QVBoxLayout;
            pushButton_send_code = new QPushButton("发送验证码"); // 发送验证码界面
            pushButton_login_account = new QPushButton("密码登录"); // 账号登陆界面
        vBoxLayout_login->addWidget(pushButton_send_code);
        vBoxLayout_login->addWidget(pushButton_login_account);

        checkBox_protocol = new QCheckBox("我已阅读并同意《软件协议》《个人信息保护指引》");  // 同意协议

        QHBoxLayout *hBpxLayout_wechat = new QHBoxLayout;
            pushButton_wechat = new QPushButton("微信");    // 微信登录
            pushButton_qq = new QPushButton("qq");    // qq登录
        hBpxLayout_wechat->addWidget(pushButton_wechat);
        hBpxLayout_wechat->addWidget(pushButton_qq);

    vBoxLayout->addLayout(hBpxLayout_help);
    vBoxLayout->addWidget(label_tip);
    vBoxLayout->addLayout(hBpxLayout_phone);
    vBoxLayout->addLayout(vBoxLayout_login);
    vBoxLayout->addWidget(checkBox_protocol);
    vBoxLayout->addLayout(hBpxLayout_wechat);

    // 手机号输入框提示字符

}
