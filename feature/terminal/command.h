#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QStringList>
#include "../../server/client_network.h"    // 传递服务器通讯套接字

/*
 * 指令处理逻辑：
 * 用户在键盘输入指令
 * 按下回车，激发键盘事件，由重写的QPlainTextEdit类处理
 * 根据输入内容，
 * 普通指令：调用指令处理类，
 * 窗口相关：发射信号传给窗口类处理
*/

// 终端指令处理
class Command
{
public:
    Command(ClientNetwork *network_);
    const QStringList command_complete(const QString& now_command); // 指令补全
    const QString command_execute(const QString& command_first, const QStringList& tokens); // 执行指令，传入指令和参数列表，返回执行结果（报错等）
    void command_log(const QString& command); // 添加指令到记录

private:
    QVector<QString> command_vector;   // 指令列表，保存所有指令
    ClientNetwork* network;
};

#endif // COMMAND_H
