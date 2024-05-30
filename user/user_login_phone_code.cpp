#include "user_login_phone_code.h"
#include "ui_user_login_phone_code.h"

#include "./user_login_phone.h" // 手机号登录界面

#include <QTimer>   // 定时器
#include <QPushButton>
#include <QLabel>

// 手机号登录_验证码界面
UserLoginPhoneCode::UserLoginPhoneCode(QString phone, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLoginPhoneCode)
{
    ui->setupUi(this);
    layout();   // 界面格式初始化
    ui->label_tip->setText("短信已发送至：" + phone);  // 修改手机号显示内容
}


UserLoginPhoneCode::~UserLoginPhoneCode()
{
    delete ui;
}


// 自定义私有函数
void UserLoginPhoneCode::layout()    // 界面格式初始化
{
    // label_title格式
    QFont font_title("Arial", 20, QFont::Bold); // Arial字体，20号字，粗体
    ui->label_title->setFont(font_title);   // 主标题字体
    label_send_code_again_time();   // 再次发送验证码的倒计时

    // 验证码输入框提示字符
    ui->lineEdit_code->setPlaceholderText("请输入六位验证码");

    // 界面创建60秒后显示按钮：“重新发送验证码”和”使用短信验证码“
    // 创建定时器
    QTimer* timer_button = new QTimer(this);
    timer_button->setSingleShot(true); // 设置定时器仅触发一次
    timer_button->start(60000); // 延迟60秒
    // 连接定时器的超时信号到槽函数
    QObject::connect(timer_button, &QTimer::timeout, this,
                     [this]()   // lambda表达式，捕获到定时器超时信号后，显示两个控件
                     {
                         QPushButton *pushButton_send_code_again = new QPushButton("再次发送验证码", this);
                         QPushButton *pushButton_voice_code = new QPushButton("发送语音验证码", this);
                         pushButton_send_code_again->show();
                         pushButton_voice_code->show();
                     }
                     );    // 在定时器超时后显示标签
}

void UserLoginPhoneCode::label_send_code_again_time()       // 再次发送验证码的倒计时
{
    // 每次发送验证码都需要重新倒计时，所以单独列出一个函数
    // 创建一个提示标签
    QLabel *label_send_code_again_time = new QLabel("60秒后重新发送验证码", this);
    label_send_code_again_time->show();
    // 创建定时器，每次超时都触发connect
    QTimer* timer_label = new QTimer(this);
    timer_label->start(1000); // 延迟1秒

    int counter = 60;
    QObject::connect(timer_label, &QTimer::timeout, this,
                        [&counter, label_send_code_again_time, timer_label] ()mutable    // lambda表达式，捕获到定时器超时信号后，显示两个控件
                        {
                            if (counter == 0)   // 如果倒计时为0，结束计时器，关闭倒计时显示
                            {
                                timer_label->stop();
                                label_send_code_again_time->close();
                            }
                            else
                            {
                                label_send_code_again_time->setText(QString::number(--counter) + "秒后重新发送验证码");
                            }
                        }
                    );    // 在定时器超时后显示标签
}


// 槽函数
void UserLoginPhoneCode::on_pushButton_back_clicked()    // 返回上一级
{
    this->close();
    UserLoginPhone* w = new UserLoginPhone;
    w->show();
}

void UserLoginPhoneCode::on_pushButton_help_clicked()    // 帮助
{
    // 与手机号登录界面相同
    // 底部弹出悬浮窗两个按钮：
    // 手机号不可用，绑定新手机：跳转到账号申述
    // 其他常见账号问题：跳转到答疑界面
}

void UserLoginPhoneCode::on_pushButton_send_code_clicked()   // 重新发送验证码
{
    // 重新发送验证码请求
}

void UserLoginPhoneCode::on_pushButton_voice_code_clicked()  // 使用语音验证码
{
    // 使用电话发送验证码
    // 弹窗提示注意接听电话
}

