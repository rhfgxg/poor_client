#include "sl.h"

Sl::Sl(QPlainTextEdit *terminal, QObject *parent)
    : QObject(parent), textEdit(terminal), position(0), verticalPosition(0), timer(new QTimer(this)) {
    train << "        ====        ________                ___________ "
          << "    _D _|  |_______/        \\__I_I_____===__|_________| "
          << "     |(_)---  |   H\\________/ |   |        =|___ ___|   "
          << "     /     |  |   H  |  |     |   |         ||_| |_||   "
          << "    |      |  |   H  |__--------------------| [___] |   "
          << "    | ________|___H__/__|_____/[][]~\\_______|       |   "
          << "    |/ |   |-----------I_____I [][] []  D   |=======|__ "
          << "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__|__ "
          << " |/-=|___|=    ||    ||    ||    |_____/~\\___/        |  "
          << "  \\_/      \\O=====O=====O=====O_/      \\_/            \\_ ";

    connect(timer, &QTimer::timeout, this, &Sl::updateTrainPosition);
}

void Sl::start() {
    // 保存原来的显示内容
    originalContent = textEdit->toPlainText();
    textEdit->clear();
    position = textEdit->width() / 8;  // 设置初始位置为最右边

    // 计算垂直位置，使火车显示在窗口中间
    int totalLines = textEdit->height() / textEdit->fontMetrics().height();
    verticalPosition = (totalLines - train.size()) / 2;

    timer->start(100);  // 100ms 刷新一次
}

void Sl::updateTrainPosition() {
    position--;
    if (position < -train.at(0).size()) {  // 火车移动到最左边后停止显示
        timer->stop();
        textEdit->clear();
        textEdit->setPlainText(originalContent + "\nusername-windows > ");
        // 将光标移动到文本末尾
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        textEdit->setTextCursor(cursor);
    } else {
        displayTrain();
    }
}

void Sl::displayTrain() {
    QString output;
    for (int i = 0; i < verticalPosition; ++i) {
        output += "\n";
    }
    for (const QString &line : train) {
        output += QString(position, ' ') + line + "\n";
    }
    textEdit->setPlainText(output);
}
