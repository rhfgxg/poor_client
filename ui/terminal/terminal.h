#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include "../../overloading/terminal/plain_text_edit.h"  // 重写的QPlainTextEdit
#include "../../server/client_network.h"

namespace Ui {
class Terminal;
}
// 终端页面
class Terminal : public QWidget
{
    Q_OBJECT

public:
    explicit Terminal(ClientNetwork *network_, QWidget *parent = nullptr);
    ~Terminal();

private slots:
    void on_command_execute_result(const QString& command_first, const QStringList& tokens);    // 指令执行

private:
    void layout();  // 界面格式初始化

    ClientNetwork *network; // 服务器通讯套接字
    PlainTextEdit *plainTextEdit_terminal;   // 使用自定义的TerminalTextEdit：重写的QPlainTextEdit
};

#endif // TERMINAL_H
