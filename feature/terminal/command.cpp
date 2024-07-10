#include "command.h"
#include "../../feature/tools/convertor/timestamp/timestamp.h" // 时间戳转换
//#include "../tools/convertor/timestamp/timestamp_ui.h"  // 时间戳界面
#include "../../server/uploads/useruploadsmanager.h"   // 文件上传
//#include "../../ui/file/fileuploads.h"    // 文件上传界面
#include <QDebug>
#include <QStringList>  // 参数列表
#include <QCoreApplication> // 获取可执行文件的地址（debug文件夹），从而获取指令记录文件的地址
#include <QDateTime>    // 获取写入指令运行的时间
#include <QFile>    // 使用指令记录文件

// 终端指令类
Command::Command(ClientNetwork *network_):
    network(network_)
{
    // 指令列表
    command_vector = {
        "echo", // 输出字符
        "exit!", // 退出终端
        "help", // 帮助
        "max_window",   // 窗口最大化
        "open_window",  // 打开功能的ui界面
        "sl",   // 显示小火车
        "timestamp", // 时间戳转换
        "uploads"   // 上传文件
    };
}

// 命令补全
// 参数：当前输入的指令
// 返回执行结果：所有匹配的指令链表
const QStringList Command::command_complete(const QString& now_commend)
{
    QStringList completions;

    // 精确匹配优先
    for (const QString &cmd : command_vector) {
        if (cmd == now_commend) {
            completions << cmd; // 精确匹配，直接添加
            return completions;
        }
    }

    // 收集所有前缀匹配项
    for (const QString &cmd : command_vector) {
        if (cmd.startsWith(now_commend)) {
            completions << cmd;
        }
    }

    // 处理结果
    if (completions.isEmpty()) {
        completions << now_commend; // 没有匹配项，返回用户输入
    }

    return completions; // 返回匹配项列表，可能是单个匹配、多个匹配或用户输入
}

// 执行指令
// 参数：指令，参数链表
// 返回执行结果
const QString Command::command_execute(const QString& command_first, const QStringList& tokens)
{
    QString result;
// 基本输出指令
    if (command_first == "echo")
    {
        result = tokens.join(' ');  // join：将字符串列表中的所有字符串连接成一个单独的字符串，接受一个分隔符作为参数，使用这个分隔符将列表中的字符串连接
    }
// 时间戳转换
    else if (command_first == "timestamp")
    {
        Timestamp timestamp;    // 时间戳转换指令 执行类

        if (tokens.isEmpty())    // 参数链表为空
        {
            result = "选项不足，目前选项有--time_to_timestamp、--timestamp_to_time，详情请参考手册";
        }
        // 时间戳转时间
        else if (tokens.at(0) == "--timestamp_to_time")
        {
            if (tokens.size() < 2)    // 第二个参数：需要转换的时间戳
            {
                result = "参数不足，请输入一个时间戳，详情请参考手册";
            }
            else
            {
                QString time = timestamp.timestamp_to_time(tokens.at(1));   // 第三个元素：需要转换的时间戳
                result = "传入的时间戳：" + tokens.at(1) + "，转换后的时间：" + time;
            }
        }
        // 时间转时间戳
        else if (tokens.at(0) == "--time_to_timestamp")
        {
            if (tokens.size() < 2)    // 第二个参数：目标单位：ms，s
            {
                result = "选项不足，目前选项有--ms、--s";
            }
            else if(tokens.at(1) == "--ms")
            {
                if (tokens.size() < 3)    // 第三四个元素：需要转换的时间
                {
                    result = "参数不足，请输入一个时间：yyyy-MM-dd HH:mm:ss.zzz";
                }
                else
                {
                    QString time = timestamp.time_to_timestamp(tokens.at(2), "Asia/Shanghai", "毫秒(ms)");   // 第四个元素：需要转换的时间戳
                    result = "传入的时间：" + tokens.at(2) + "，转换后的时间戳：" + time;
                }
            }
            else if(tokens.at(1) == "--s")
            {
                if (tokens.size() < 3)    // 第三个元素：需要转换的时间
                {
                    result = "参数不足，请输入一个时间：yyyy-MM-dd HH:mm:ss.zzz";
                }
                else
                {
                    QString time = timestamp.time_to_timestamp(tokens.at(2), "Asia/Shanghai", "秒(s)");   // 第四个元素：需要转换的时间戳
                    result = "传入的时间：" + tokens.at(2) + "，转换后的时间戳：" + time;
                }
            }
        }
        else
        {
            result = "无法识别选项，目前选项有--time_to_timestamp、--timestamp_to_time，详情请参考手册";
        }


    }
// 文件上传
    else if (command_first == "uploads")
    {
        // 参数列表
        // 第一个参数：要上传的文件
        if (tokens.isEmpty())    // 参数链表为空
        {
            result = "参数不足，参考：uploads 需要上传的文件路径";
        }
        else if (tokens.at(0) == "--uploads")
        {
            QString file_path = tokens.at(0);

            UserUploadsManager uploads(network);    // 创建管理对象
            uploads.sendInitialUploadRequest(file_path);   // 初始化上传文件任务
            result = "开始上传";
        }

    }
// 功能实现
    else if (command_first == "指令")
    {
        // 下标访问参数链表：tokens.at(0)
        // tokens.size<2：元素小于两个
        result = "指令结果";
    }
// 未标识指令
    else
    {
        result = "Unknown command: " + command_first;
    }

    return result;
}

// 指令加入记录
// 参数：指令+参数的字符串
void Command::command_log(const QString& command_str)
{
    QString exePath = QCoreApplication::applicationDirPath();   // 获取可执行文件地址
    QString bash_history_path = exePath + "/data/terminal/home/root/.bash_history.txt"; // 指令记录的地址

    QString user_name_str = "user_name";
    // 获取当前时间，转换成时间戳
    QDateTime date_time = QDateTime::currentDateTime();   // 获取当前日期和时间的 QDateTime 对象
    QString date_time_str = date_time.toString("yyyy-MM-dd HH:mm:ss");  // 时间转换成字符串

    QString log_new = user_name_str + "-" + date_time_str + ": " + command_str + "\n";  // 拼接成完整的记录

    // 使用QFile对象打开文件，QIODevice::ReadWrite表示读写模式，QIODevice::Text指定文件是文本文件，QIODevice::Append指定每次写入文件时，数据都将追加到文件的当前结尾，而不是覆盖现有内容
    QFile file(bash_history_path);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        // 移动到文件末尾
        file.seek(file.size());

        // 使用QTextStream进行文本读写操作更为方便
        QTextStream out(&file);

        // 检查是否已经是文件的最后一行，避免在空文件或已有内容后直接添加换行符
        if (!file.atEnd()) {
            out << "\n"; // 如果不是空文件且不在最后一行，先添加一个换行符
        }

        // 写入新的字符串
        out << log_new;

        // 关闭文件
        file.close();
    }
    else
    {
        qDebug() << "指令日志打开失败: " << bash_history_path;
    }

}
