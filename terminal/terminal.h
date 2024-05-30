#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include "./module/terminaltextedit.h"  // 重写的QPlainTextEdit

namespace Ui {
class Terminal;
}
// 终端页面
class Terminal : public QWidget
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = nullptr);
    ~Terminal();
    void on_command_execute_result(const QString& command_first, const QStringList& tokens);    // 指令执行
    void on_command_complete_list(const QStringList& completionSuggestion);  // 指令补全
private:
    void layout();  // 界面格式初始化
    void connects();    // 信号槽关联

private:
    Ui::Terminal *ui;
    // 使用自定义的TerminalTextEdit
    TerminalTextEdit *plainTextEdit_terminal;
};

#endif // TERMINAL_H
