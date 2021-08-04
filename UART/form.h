#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "uart.h"
#include "ui_form.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
    Ui::Form *ui;
private slots:
    void setbuard(const QString &arg);
    void COMBOX(const QString &arg);

    void on_SET_UART_clicked();
};


#endif // FORM_H


