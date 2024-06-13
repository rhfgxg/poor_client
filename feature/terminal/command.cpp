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
        "timestamp" // 时间戳转换
        "uploads"   // 上传文件
    };
}

const QStringList Command::command_complete(const QString& now_commend) // 命令补全
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

const QString Command::command_execute(const QString& command_first, const QStringList& tokens) // 执行指令，传入指令，返回执行结果（报错等）
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
        // 参数列表（时间默认上海，时间戳默认毫秒）
        // 0 转换形式
        // 1 需要转换的值
        // 2 值的类型
        // 3 结果的类型
        Timestamp timestamp;

        if (tokens.isEmpty())    // 参数链表为空
        {
            result = "参数不足，参考：timestamp --timestamp_to_time 需要转换的时间戳 时间戳类型（默认毫秒） 转换后时间的时区（默认上海）";
        }
        else if (*tokens.begin() == "--timestamp_to_time")
        {
            if ((tokens.begin()+1) == tokens.end())    // 第二个参数指向尾部表示参数不足
            {
                result = "参数不足，参考：timestamp --timestamp_to_time 需要转换的时间戳 时间戳类型（默认毫秒） 转换后时间的时区（默认上海）";
            }
            QString time = timestamp.timestamp_to_time(*(tokens.begin()+1));   // 第二个元素：需要转换的时间戳
            result = "传入的时间戳：" + *(tokens.begin()+1) + "，转换后的时间：" + time;
        }
        else if (*tokens.begin() == "--time_to_timestamp")
        {
            if ((tokens.begin()+1) == tokens.end())    // 第二个参数指向尾部表示参数不足
            {
                result = "参数不足，参考：timestamp --time_to_timestamp 需要转换的时间 时间时区（默认上海） 转换后时间类型（默认毫秒）";
            }
            QString time = timestamp.time_to_timestamp(*(tokens.begin()+1));   // 第二个元素：需要转换的时间戳
            result = "传入的时间：" + *(tokens.begin()+1) + "，转换后的时间戳：" + time;
        }
        else
        {
            result = "参数1错误";
        }


    }
// 功能介绍
    else if (command_first == "uploads")
    {
        // 参数列表
        // 第一个参数：要上传的文件
        if (tokens.isEmpty())    // 参数链表为空
        {
            result = "参数不足，参考：uploads 需要上传的文件路径";
        }
        else
        {
            QString file_path = *(tokens.begin());

            UserUploadsManager uploads(network);    // 创建管理对象
            uploads.uploads(file_path);   // 上传文件
            result = "上传成功";
        }

    }
// 功能实现
    else if (command_first == "指令")
    {
        result = "指令结果";
    }
// 未标识指令
    else
    {
        result = "Unknown command: " + command_first;
    }

    return result;
}

void Command::command_log(const QString& command_str)   // 指令加入记录
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
        qDebug() << "Failed to open file: " << bash_history_path;
    }

}
