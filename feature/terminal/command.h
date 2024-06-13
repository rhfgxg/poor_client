#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QStringList>

// 终端指令处理
class Command
{
public:
    Command();
    const QStringList command_complete(const QString& now_command); // 指令补全
    const QString command_execute(const QString& command_first, const QStringList& tokens); // 执行指令，传入指令和参数列表，返回执行结果（报错等）
    void command_log(const QString& command); // 添加指令到记录

private:
    QVector<QString> command_vector;   // 指令列表，保存所有指令
};

#endif // COMMAND_H
