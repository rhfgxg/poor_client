#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>

namespace Ui {
class Tools;
}

// 工具集页面
class Tools : public QWidget
{
    Q_OBJECT

public:
    explicit Tools(QWidget *parent = nullptr);
    ~Tools();

private slots:
    void on_pushButton_to_mainWindow_clicked();

    void on_pushButton_to_convertor_clicked();

private:
    Ui::Tools *ui;
};

#endif // TOOLS_H
