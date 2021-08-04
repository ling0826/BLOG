#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>
#include <QAbstractSocket>
#include "uart.h"
#include "network.h"
#include "pthread/thread.h"
#include "pthread/pthread_time.h"
#include "mainwindow.h"
#include "uart_set.h"
#include "date_time.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();   

    void slot_handle_finish();
    void text_apped(unsigned char a);

signals:
    void StartThread();
private slots:
    void test();
    void aa_test(bool fale);

    void readMyCom();//用于读取数据
    void DBG(const QString &text);

    void on_GETCOMS_clicked();

    void on_CLOSE_clicked();

    void on_buard_currentIndexChanged(const QString &arg1);

    void on_COMX_currentIndexChanged(const QString &arg1);

    void on_OPNE_CLOSE_clicked();

    void on_DBG_clicked(bool checked);

    void on_SEND_clicked();

    void on_actionUart_triggered();

    void on_DBG_clear_clicked();

    void on_TCP_MODE_toggled(bool checked);

    void on_SERVER_MODE_toggled(bool checked);

    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    void NewConnectionSlot();
    void disconnectedSlot();
    void ReadData_S();
    void on_SEND_IP_clicked();

    void on_connect_ip_clicked();

    void close_uart_w(int arg);

    void readPendingDatagrams();


    void on_CLIENT_MODE_toggled(bool checked);



    void on_Jion_Multicast_clicked();

    void on_Unicast_toggled(bool checked);

    void on_broadcast_toggled(bool checked);

    void on_Multicasr_toggled(bool checked);





    void on_ipaddr_selectionChanged();

    void on_ipaddr_cursorPositionChanged(int arg1, int arg2);

    void on_actionShijian_triggered();

    void on_commandLinkButton_clicked();

private:
    Ui::MainWindow *ui;
    bool DBG_F = false;
    QByteArray requestData;
    QTcpSocket *tcpClient;
    QUdpSocket *udpSocket;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> tcpClient_s;
    QTcpSocket *currentClient;

    My_thread * mythread;
    pthread_time* time_thread;
    QThread *subthread;
    QThread *subthread_1;
    uart_set* uart_w;
    Date_Time * datatime;

    friend class pthread_time;

};

#endif // MAINWINDOW_H


