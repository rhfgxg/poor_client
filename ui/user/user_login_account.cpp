#include "user_login_account.h"
#include "user_login_phone.h"   // 手机号登录界面
#include "../../mainwindow.h"  // 主界面

#include <QFont>    // 字体
// 使用正则
#include "QRegularExpression"
#include <QRegularExpressionValidator>
#include <QCryptographicHash>   // 哈希
#include <QMessageBox>  // 弹窗
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../../feature/tools/encipher/md5.h"   // md5加密

// 账号密码登录
UserLoginAccount::UserLoginAccount(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    user_manager(new UserManager(network_, this))
{
    layout();   // 界面格式初始化

    // 账号和密码输入框输入内容后，显示登录按钮
    connect(this->lineEdit_account, &QLineEdit::textChanged, this, &UserLoginAccount::on_show_pushButton_login);
    connect(this->lineEdit_password, &QLineEdit::textChanged, this, &UserLoginAccount::on_show_pushButton_login);

    // 登录成功信号
    connect(user_manager, &UserManager::loginSuccess, this, &UserLoginAccount::on_login_response_success);
    // 登录失败信号
    connect(user_manager, &UserManager::loginFailed, this, &UserLoginAccount::on_login_response_failed);

    connect(pushButton_back, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_back_clicked);
    connect(pushButton_help, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_help_clicked);
    connect(pushButton_look_password, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_look_password_clicked);
    connect(pushButton_login, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_login_clicked);
    connect(pushButton_login_phone, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_login_phone_clicked);
    connect(pushButton_wechat, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_wechat_clicked);
    connect(pushButton_qq, &QPushButton::clicked, this, &UserLoginAccount::on_pushButton_qq_clicked);
}


UserLoginAccount::~UserLoginAccount()
{
    delete user_manager;

    delete pushButton_back;   // 返回个人界面
    delete pushButton_help;   // 帮助
    delete pushButton_look_password;  // 显示密码
    delete pushButton_login;  // 登录
    delete pushButton_login_phone;    // 验证码登录
    delete pushButton_wechat; // 微信登录
    delete pushButton_qq; // qq登录

    delete lineEdit_account;    // 账号输入框
    delete lineEdit_password;   // 密码输入框

    delete checkBox_protocol;   // 同意协议
}

// 槽函数
void UserLoginAccount::on_show_pushButton_login() // 显示登录按钮
{
    if (!this->lineEdit_account->text().isEmpty() && !this->lineEdit_password->text().isEmpty())
    {
        this->pushButton_login->setEnabled(true); // 启用按钮
        this->pushButton_login->setText("登录（可以点击）"); // 设置按钮文本
    }
    else
    {
        this->pushButton_login->setEnabled(false); // 禁用按钮
        this->pushButton_login->setText("登录（不能点击）"); // 设置按钮文本为初始文本
    }
}

void UserLoginAccount::on_login_response_success(const QString &tooken) // 登录成功
{
    // 登录成功，打开主界面
    this->close();
    MainWindow *w = new MainWindow(network);
    w->show();
}

void UserLoginAccount::on_login_response_failed(const QString &reason)    // 登录失败
{
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("登录失败！");
    msgBox->setText("登录失败" + reason);
    msgBox->setIcon(QMessageBox::Information);
    // 添加弹窗内按钮
    QPushButton *closeButton = msgBox->addButton(QMessageBox::Yes);   // 关闭提示
    closeButton->setText("好的");
    // 显示消息框并等待用户响应
    msgBox->exec();
    if (msgBox->clickedButton() == closeButton)
    {
        this->checkBox_protocol->setChecked(true);
        msgBox->close(); // 关闭弹窗
    }
}

void UserLoginAccount::on_pushButton_back_clicked()   // 按钮：返回上一级
{
    this->close();
    // 打开验证码登录界面

    UserLoginPhone* w = new UserLoginPhone(network);
    w->show();
}

void UserLoginAccount::on_pushButton_help_clicked()   // 按钮：帮助
{
    // 弹出一个弹窗有两个按钮
    // 找回密码：跳转到手机号找回
    // 人工申诉找回账号：跳转到申述
}

void UserLoginAccount::on_pushButton_look_password_clicked()  // 显示密码按钮
{
    if(this->pushButton_look_password->text() == "显示密码")
    {
        this->lineEdit_password->setEchoMode(QLineEdit::Normal);    // 设置显示模式为文本
        this->pushButton_look_password->setText("隐藏密码");  // 修改显示内容
    }
    else
    {
        this->lineEdit_password->setEchoMode(QLineEdit::Password);    // 设置显示模式为密码
        this->pushButton_look_password->setText("隐藏密码");
    }
}

void UserLoginAccount::on_pushButton_login_clicked()  // 登录按钮
{
    if (!this->checkBox_protocol->isChecked())
    {// 未勾选协议：弹窗：提示需要同意协议
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
            this->checkBox_protocol->setChecked(true);
            msgBox->close(); // 关闭弹窗
        }
        else if (msgBox->clickedButton() == noButton)
        {
            msgBox->close(); // 关闭弹窗
            return;
        }
    }
    // 获取数据
    QString connect = this->lineEdit_account->text();
    QString password = this->lineEdit_password->text();

    // 密码加密
    QString password_hash = Md5().md5_encipher(password);

    user_manager->user_login(connect, password_hash);   // 调用登录管理的登录部分，向服务器发送信息
}

void UserLoginAccount::on_pushButton_login_phone_clicked()    // 跳转短信验证码登录界面
{
    this->close();

    // 打开验证码登录界面
    UserLoginPhone* w = new UserLoginPhone(network);
    w->show();
}

void UserLoginAccount::on_pushButton_wechat_clicked() // 跳转微信登录
{
    if (this->checkBox_protocol->isChecked())
    {
        // 复选框被选中，正常跳转
    }
    else
    {
        // 复选框未被选中，弹窗：提示需要同意协议
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

void UserLoginAccount::on_pushButton_qq_clicked() // 跳转qq登录
{
    if (this->checkBox_protocol->isChecked())
    {// 已勾选协议，正常跳转

    }
    else
    {// 未勾选协议：弹窗：提示需要同意协议
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
void UserLoginAccount::layout()   // 界面格式初始化
{
    setWindowTitle("账号密码登录");

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);

        QHBoxLayout *hBpxLayout_help = new QHBoxLayout;
            pushButton_back = new QPushButton("返回");   // 返回个人界面
            pushButton_help = new QPushButton("帮助");   // 帮助
        hBpxLayout_help->addWidget(pushButton_back);
        hBpxLayout_help->addWidget(pushButton_help);

        QVBoxLayout *vBoxLayout_line = new QVBoxLayout;
            lineEdit_account = new QLineEdit;    // 账号输入框
            QHBoxLayout *hBpxLayout_password = new QHBoxLayout;
                lineEdit_password = new QLineEdit;   // 密码输入框
                pushButton_look_password = new QPushButton("显示密码");  // 显示密码
            hBpxLayout_password->addWidget(lineEdit_password);
            hBpxLayout_password->addWidget(pushButton_look_password);
        vBoxLayout_line->addWidget(lineEdit_account);
        vBoxLayout_line->addLayout(hBpxLayout_password);

        QVBoxLayout *vBoxLayout_login = new QVBoxLayout;
            pushButton_login = new QPushButton("登录(不可点击)");  // 登录
            pushButton_login_phone = new QPushButton("短信验证码登录");    // 验证码登录
        vBoxLayout_login->addWidget(pushButton_login);
        vBoxLayout_login->addWidget(pushButton_login_phone);

        checkBox_protocol = new QCheckBox("我已阅读并同意《软件协议》《个人信息保护指引》");   // 同意协议

        QHBoxLayout *hBpxLayout_wechat = new QHBoxLayout;
            pushButton_wechat = new QPushButton("微信"); // 微信登录
            pushButton_qq = new QPushButton("qq"); // qq登录
        hBpxLayout_wechat->addWidget(pushButton_wechat);
        hBpxLayout_wechat->addWidget(pushButton_qq);


    vBoxLayout->addLayout(hBpxLayout_help);
    vBoxLayout->addLayout(vBoxLayout_line);
    vBoxLayout->addLayout(vBoxLayout_login);
    vBoxLayout->addWidget(checkBox_protocol);
    vBoxLayout->addLayout(hBpxLayout_wechat);


    // 账号和密码输入框提示字符
    this->lineEdit_account->setPlaceholderText("请输入手机号或邮箱");
    this->lineEdit_password->setPlaceholderText("请输入密码");
    // 限制密码输入框输入和显示内容
    this->lineEdit_password->setEchoMode(QLineEdit::Password);    // 设置显示模式为密码
    QRegularExpression regExp("[0-9a-zA-Z.@]{1,16}");   // 正则设置只能输入英文数字符号，长度：1-16位
    this->lineEdit_password->setValidator(new QRegularExpressionValidator(regExp, this));

    // 限制登录按钮：在账号和密码输入后才能点击
    this->pushButton_login->setEnabled(false); // 初始时禁用登录按钮
}
