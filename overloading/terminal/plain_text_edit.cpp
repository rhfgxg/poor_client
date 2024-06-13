#include "plain_text_edit.h"
#include "../../feature/terminal/command.h" // 指令处理类：指令补全

#include <QStringList>
#include <QCoreApplication>
#include <QFile>
PlainTextEdit::PlainTextEdit(ClientNetwork *network_, QWidget *parent) :
    QPlainTextEdit(parent),
    prompt("username-windows > "),
    network(network_)
{
    appendPlainText(prompt + "Welcome to poor");    // 欢迎界面
    appendPlainText(prompt);    // 第一行指令
}

// 事件处理函数，用于处理键盘按键事件。
// 当用户在窗口中按下键盘上的任何按键时，Qt 会生成一个键盘事件，并将其传递给具有键盘焦点的窗口小部件（例如文本框、按钮等）。
// 键盘事件包含有关按下的键的信息，如键码和键的状态
// 重写此函数，在按下回车时处理内容
void PlainTextEdit::keyPressEvent(QKeyEvent *event)
{
// 处理回车键，执行指令
    // 键盘事件的枚举 回车：Qt::Key_Return，Qt::Key_Enter，Tab：Qt::Key_Tab，上下键：Qt::Key_Up，Qt::Key_Down
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // 获取当前行的文本
        QString command = toPlainText().split("\n").last();
        command = command.remove(0, prompt.length()).trimmed(); // 去掉提示符和前后空白

        Command new_command(network);
        new_command.command_log(command);    // 调用日志函数，传入指令

        QStringList tokens = command.split(' ', Qt::SkipEmptyParts); // 将指令分隔成字符串列表

        if (!tokens.isEmpty())
        {
            QString command_first = tokens.takeFirst(); // 获取第一个元素作为指令
        // 对终端界面进行修改的指令白名单：例如终端颜色，退出终端等
            if (command_first == "exit!" || command_first == "max_window")
            {
                // 发射信号，将对窗口修改的指令做特殊处理
                emit commandEntered(command_first, tokens);
            }
        // 处理其他指令
            else
            {
                // 实现命令处理逻辑
                QString result = new_command.command_execute(command_first, tokens); // 传入指令到执行函数
                appendPlainText(result);   // 显示执行结果
            }
        }
        insertPlainText("\n");  // 执行结果后换行

        // 在指令执行完后，插入新的命令提示符
        insertPlainText(prompt);
        moveCursor(QTextCursor::End);

        // 防止默认处理
        event->accept();
    }
// 处理Tab，实现命令补全
    else if (event->key() == Qt::Key_Tab)
    {
        // Tab键处理：实现命令补全
        QString currentLine = toPlainText().split("\n").last(); // 获取当前行文本
        QString commandPart = currentLine.remove(0, prompt.length()).trimmed(); // 去掉提示符和前后空白

        // 调用命令补全函数, 返回一个匹配链表
        Command new_command(network);
        QStringList completionSuggestion = new_command.command_complete(commandPart);

        if (completionSuggestion.size() == 1)
        {
        // 匹配到一个指令，或没有匹配到指令时
            // 从后向前查找最后一个匹配的命令部分
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
            cursor.select(QTextCursor::LineUnderCursor);
            cursor.removeSelectedText();    // 清除最后一行内容
            cursor.insertText(prompt+completionSuggestion.first()); // 将提示符号与指令一起显示

            // 移动光标到行末
            moveCursor(QTextCursor::End);
        }
        else if (completionSuggestion.size() > 1)
        {
        // 有多个匹配项，在窗口显示
            for (auto &commands : completionSuggestion)
            {
                appendPlainText(commands);
            }
            // 在新的一行显示指令
            insertPlainText("\n");  // 执行结果后换行

            // 在指令执行完后，插入新的命令提示符
            insertPlainText(prompt + commandPart);  // 显示提示字符和原来的指令
            moveCursor(QTextCursor::End);
        }

        event->accept(); // 阻止Tab键的默认行为
    }
// 键盘上下键，寻找上一个指令
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        // 获取当前行的文本, 作为第一条记录
        QString command_now = toPlainText().split("\n").last();
        command_now = command_now.remove(0, prompt.length()).trimmed(); // 去掉提示符和前后空白

        QString exePath = QCoreApplication::applicationDirPath();   // 获取可执行文件地址
        QString bash_history_path = exePath + "/data/terminal/home/root/.bash_history.txt"; // 指令记录的地址
        QFile file(bash_history_path);

        if (event->key() == Qt::Key_Up)
        {
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))   // 只读方式打开txt文件
            {

            }

        }
        else if (event->key() == Qt::Key_Down)
        {

            if (file.open(QIODevice::ReadOnly | QIODevice::Text))   // 只读方式打开txt文件
            {

            }
        }

    }
// 处理其他键
    else
    {
        // 防止用户在提示符之前编辑文本
        if (textCursor().position() < toPlainText().lastIndexOf(prompt) + prompt.length())
        {
            // 保证光标位置在提示符之后
            moveCursor(QTextCursor::End);
        }
        QPlainTextEdit::keyPressEvent(event);
    }
}
