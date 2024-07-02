#ifndef PLAIN_TEXT_EDIT_H
#define PLAIN_TEXT_EDIT_H

#include "../../server/client_network.h"    // 传递套接字
#include <QPlainTextEdit>
#include <QKeyEvent>

// 重写QPlainTextEdit
class PlainTextEdit: public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(ClientNetwork *network_, QWidget *parent = nullptr);
    void sl();

signals:
    void commandEntered(const QString& command_first, const QStringList& now_args); // 回车键信号，特殊处理窗口相关指令

protected:
    void keyPressEvent(QKeyEvent *event) override;  // 处理键盘事件

private:
    QString prompt;
    ClientNetwork *network; // 服务器通讯套接字

};

#endif // PLAIN_TEXT_EDIT_H
