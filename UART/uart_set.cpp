#include "uart_set.h"
#include "ui_uart_set.h"
#include "def.h"
uart_set::uart_set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uart_set)
{
    ui->setupUi(this);
}

uart_set::~uart_set()
{
    delete ui;
}
//点击“X”关闭窗口触发信号
void uart_set::closeEvent( QCloseEvent * event)
{
    emit open_uart_set(UART_SET_NUM);
}
//点击退出按钮触发信号
void uart_set::on_CLOSE_clicked()
{
    emit open_uart_set(UART_SET_NUM);
}
