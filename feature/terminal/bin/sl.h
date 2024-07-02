#ifndef SL_H
#define SL_H

#include <QObject>
#include <QPlainTextEdit>
#include <QTimer>
#include <QStringList>

class Sl : public QObject
{
    Q_OBJECT

public:
    Sl(QPlainTextEdit *terminal, QObject *parent = nullptr);
    void start();

private slots:
    void updateTrainPosition();

private:
    void displayTrain();

    QPlainTextEdit *textEdit;   // 终端界面
    QTimer *timer;  // 计时器
    QStringList train;  // 动画帧
    QString originalContent;    // 保留运行前的终端内容，在运行结束后恢复
    int position;
    int verticalPosition;   // 垂直高度
};

#endif // SL_H
