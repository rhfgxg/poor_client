#ifndef TERMINALTEXTEDIT_H
#define TERMINALTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

// 重写QPlainTextEdit
class TerminalTextEdit: public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TerminalTextEdit(QWidget *parent = nullptr);

signals:
    // 自定义信号，携带指令字符串作为信息
    void commandEntered(const QString& command_first, const QStringList& now_args);
    void commandTab(const QStringList& completionSuggestion);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QString prompt;
};

#endif // TERMINALTEXTEDIT_H
