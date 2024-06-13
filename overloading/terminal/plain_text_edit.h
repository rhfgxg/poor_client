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

signals:
    // 自定义信号，携带指令字符串作为信息
    void commandEntered(const QString& command_first, const QStringList& now_args);
    void commandTab(const QStringList& completionSuggestion);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QString prompt;
    ClientNetwork *network; // 服务器通讯套接字

};

#endif // PLAIN_TEXT_EDIT_H
