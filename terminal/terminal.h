#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include "./module/terminaltextedit.h"  // 重写的QPlainTextEdit
#include "../server/client_network.h"

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
    void on_command_execute_result(const QString& command_first, const QStringList& tokens);    // 指令执行
    void on_command_complete_list(const QStringList& completionSuggestion);  // 指令补全
private:
    void layout();  // 界面格式初始化
    void connects();    // 信号槽关联

private:
    Ui::Terminal *ui;
    TerminalTextEdit *plainTextEdit_terminal;   // 使用自定义的TerminalTextEdit：重写的QPlainTextEdit
    ClientNetwork *network; // 服务器通讯套接字

};

#endif // TERMINAL_H
