#ifndef UART_SET_H
#define UART_SET_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class uart_set;
}

class uart_set : public QWidget
{
    Q_OBJECT

public:
    explicit uart_set(QWidget *parent = 0);
    ~uart_set();

private:
    Ui::uart_set *ui;
    void closeEvent( QCloseEvent * event);
signals:
    void open_uart_set(int);
private slots:
    void on_CLOSE_clicked();
};

#endif // UART_SET_H
