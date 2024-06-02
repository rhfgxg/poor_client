#include "user_login_account.h"
#include "ui_user_login_account.h"

#include "user_login_phone.h"   // 手机号登录界面
#include "../mainwindow.h"  // 主界面

#include <QFont>    // 字体
// 使用正则
#include "QRegularExpression"
#include <QRegularExpressionValidator>
#include <QCryptographicHash>   // 哈希
#include <QMessageBox>  // 弹窗
#include <QTimer>

// 账号密码登录
UserLoginAccount::UserLoginAccount(ClientNetwork *network_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLoginAccount),
    user_login_account_manager(new UserLoginAccountManager(network_, this))
{
    ui->setupUi(this);
    layout();   // 界面格式初始化
// 关联信号槽
    // 账号和密码输入框输入内容后，显示登录按钮
    connect(ui->lineEdit_account, &QLineEdit::textChanged, this, &UserLoginAccount::on_show_pushButton_login);
    connect(ui->lineEdit_password, &QLineEdit::textChanged, this, &UserLoginAccount::on_show_pushButton_login);

    // 登录成功信号
    connect(user_login_account_manager, &UserLoginAccountManager::loginSuccess, this, &UserLoginAccount::on_login_response_success);
    // 登录失败信号
    connect(user_login_account_manager, &UserLoginAccountManager::loginFailed, this, &UserLoginAccount::on_login_response_failed);
}


UserLoginAccount::~UserLoginAccount()
{
    delete ui;
    delete user_login_account_manager;
}


// 自定义私有函数
void UserLoginAccount::layout()   // 界面格式初始化
{
    // 创建一个字体对象
    QFont font_title("Arial", 20, QFont::Bold); // Arial字体，20号字，粗体
    ui->label_title->setFont(font_title);   // 主标题字体

    // 账号和密码输入框提示字符
    ui->lineEdit_account->setPlaceholderText("请输入手机号或邮箱");
    ui->lineEdit_password->setPlaceholderText("请输入密码");
    // 限制密码输入框输入和显示内容
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);    // 设置显示模式为密码
    QRegularExpression regExp("[0-9a-zA-Z.@]{1,16}");   // 正则设置只能输入英文数字符号，长度：1-16位
    ui->lineEdit_password->setValidator(new QRegularExpressionValidator(regExp, this));

    // 限制登录按钮：在账号和密码输入后才能点击
    ui->pushButton_login->setEnabled(false); // 初始时禁用登录按钮
}

// 自定义槽函数
void UserLoginAccount::on_show_pushButton_login() // 显示登录按钮
{
    if (!ui->lineEdit_account->text().isEmpty() && !ui->lineEdit_password->text().isEmpty())
    {
        ui->pushButton_login->setEnabled(true); // 启用按钮
        ui->pushButton_login->setText("登录（可以点击）"); // 设置按钮文本
    }
    else
    {
        ui->pushButton_login->setEnabled(false); // 禁用按钮
        ui->pushButton_login->setText("登录（不能点击）"); // 设置按钮文本为初始文本
    }
}

void UserLoginAccount::on_login_response_success() // 登录成功
{
    // 登录成功，打开主界面
    this->close();
    MainWindow *w = new MainWindow();
    w->show();
}

void UserLoginAccount::on_login_response_failed(const QString &reason)    // 登录失败
{
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("登录失败！");
    msgBox->setText("登录失败，请检查输入的账号或密码是否正确。");
    msgBox->setIcon(QMessageBox::Information);
    // 添加弹窗内按钮
    QPushButton *yesButton = msgBox->addButton(QMessageBox::Yes);   // 关闭提示
    yesButton->setText("好的");
}

// 槽函数
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
    if(ui->pushButton_look_password->text() == "显示密码")
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);    // 设置显示模式为文本
        ui->pushButton_look_password->setText("隐藏密码");  // 修改显示内容
    }
    else
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);    // 设置显示模式为密码
        ui->pushButton_look_password->setText("隐藏密码");
    }
}

void UserLoginAccount::on_pushButton_login_clicked()  // 登录按钮
{
    if (!ui->checkBox_protocol->isChecked())
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

            ui->checkBox_protocol->setChecked(true);
            msgBox->close(); // 关闭弹窗
        }
        else if (msgBox->clickedButton() == noButton)
        {
            msgBox->close(); // 关闭弹窗
            return;
        }
    }
    // 获取数据
    QString connect = ui->lineEdit_account->text();
    QString password = ui->lineEdit_password->text();
    user_login_account_manager->login(connect, password);   // 调用登录管理的登录部分，向服务器发送信息
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
    if (ui->checkBox_protocol->isChecked())
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
    if (ui->checkBox_protocol->isChecked())
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

