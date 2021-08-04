#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include <QWidget>
#include <QThread>
#include <QMessageBox>

#include <QDesktopServices>
#include <QUrl>
#include "def.h"
#include <QStandardItem>

#include "Error_Data.h"

int firstIndex = 0;
extern QTimer *timer;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Text_DBG->setText("启动成功\r\n");
    ui->COMX->clear();

    serial_L = new QSerialPort(this);
    //connect(ui->actionUart,&QAction::triggered,this,on_actionUart_triggered);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->COMX->addItem(serial.portName());
            serial.close();
        }
    }
    QStandardItem *town1 = new QStandardItem("电白镇");

    
    qDebug()<< "ceshi";
    ui->TCP_MODE->setChecked(true);
    tcpClient = new QTcpSocket(this);
    tcpClient->abort();                 //取消原有连接

    tcpServer = new QTcpServer(this);
    QList<QHostAddress> ipAddressesList  = QNetworkInterface().allAddresses();
    //ui->ipaddr->setText(QNetworkInterface().allAddresses().at(1).toIPv4Address());   //获取本地IP
    int nListSize = ipAddressesList.size();
        for (int i = 0; i < nListSize; ++i)
        {
               if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                   ipAddressesList.at(i).toIPv4Address()) {
                    //qDebug()<<"i= "<<i<<ipAddressesList.at(i).toString();
                    ui->ipaddr->setText(ipAddressesList.at(i).toString());
                   break;
               }
         }
    ui->serverip->setText("239.0.0.30");



    //ui->serverip->setText(QHostAddress(QHostAddress::LocalHost).toString());
    subthread = new QThread(this);

    mythread = new My_thread();
    mythread->moveToThread(subthread);

    udpSocket = new QUdpSocket (this);

    //建立信号连接
    connect(this,MainWindow::StartThread,mythread,My_thread::Mywork);
    connect(mythread,&My_thread::signal_back,this,&MainWindow::slot_handle_finish);

    subthread_1 = new QThread(this);

    time_thread = new pthread_time();
    time_thread->moveToThread(subthread_1);

    //建立信号连接
    connect(this,MainWindow::StartThread,time_thread,pthread_time::Mywork);
/*
    ui->stackedWidget->addWidget(ui->widget_2);
       ui->stackedWidget->addWidget(ui->widget);
    // 创建UDP套接字
       ui->stackedWidget->setCurrentWidget(ui->widget);
           //获取当前页面的序号
           firstIndex=ui->stackedWidget->currentIndex();
           qDebug()<<"firstIndex:"<<firstIndex;
*/
    time = QDateTime::currentDateTime();

    subthread->start();
    subthread_1->start();
        //启动了线程，但是并没有进入线程
        //必须通过信号/槽的方式进入子线程
        //直接通过m_MyThread->Mywork()是不行的，这样MyWork()中的线程就是主线程
    emit StartThread();     //发送信号


/*
    const QUrl url("http://www.baidu.com");
    const QUrl url2("C:");
        qDebug() << url.scheme();
        qDebug() << url.port();
        QDesktopServices::openUrl(url);
        QDesktopServices::openUrl(url2);
        //QUrl("file:///C:/Documents and Settings/All Users", QUrl::TolerantMode));
*/
    connect(ui->qiehuang,SIGNAL(clicked()),this,SLOT(test()));
    connect(ui->a_test,SIGNAL(clicked(bool)),this,SLOT(aa_test(bool)));



    QDateTime d1(QDate(2021,3,16),QTime(9,36,56));
    ui->timeEdit->setDateTime(d1);
    ui->dateTimeEdit->setDateTime(d1);
}
void MainWindow::aa_test(bool fale)
{
     qDebug("123465777 %d",fale);
}
void MainWindow::test()
{

    qDebug("123465 ");
}
//槽函数-退出子界面处理函数
void MainWindow::close_uart_w(int arg)
{
    this->show();       //展示主界面
    if(arg == UART_SET_NUM)
    {
        disconnect(uart_w,uart_set::open_uart_set,0,0 );    //取消槽连接
        this->ui->Text_DBG->append("clsoe 1");
        delete(uart_w);     //删除子界面
    }
    else if(arg == DATE_TIME_NUM)
    {
        disconnect(datatime,Date_Time::date_time_close,0,0);
        connect(mythread,SIGNAL(signal_1s()),0,0);
        this->ui->Text_DBG->append("clsoe 2");
        delete(datatime);

    }

}
void MainWindow::on_actionShijian_triggered()
{
    qDebug("时间设置");
    datatime = new Date_Time();
    datatime->show();
    connect(datatime,Date_Time::date_time_close,this,MainWindow::close_uart_w);
    connect(time_thread,SIGNAL(signal_1s()),datatime,SLOT(time_1s_show()),Qt::QueuedConnection);
    this->hide();
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slot_handle_finish()
{
qDebug()<<"this is signal blak";
}

void MainWindow::on_CLOSE_clicked()
{
   close();
}
//打开串口参数配置的新界面
void MainWindow::on_actionUart_triggered()
{
    qDebug()<<"串口参数设置";
    uart_w = new uart_set();    //初始化窗口指针
    uart_w->show();             //显示新窗口
    connect(uart_w,uart_set::open_uart_set,this,MainWindow::close_uart_w);//建立信号连接
    this->hide();               //隐藏主窗口
}

void MainWindow::on_DBG_clear_clicked()
{
    ui->Text_DBG->clear();
    mythread->isStop = true;
}
void MainWindow::text_apped(unsigned char a)
{
    ui->Text_DBG->append(QString::number(a,16));
}

















void MainWindow::on_ipaddr_selectionChanged()
{
    qDebug("on_ipaddr_selectionChanged\r\n");
}

void MainWindow::on_ipaddr_cursorPositionChanged(int arg1, int arg2)
{
    /*if(arg1 == 1 && arg2 == 2)
    {
        ui->ipaddr->setText(ui->ipaddr->text()+":");
    }
*/
    qDebug()<<ui->timeEdit->date().toString("yyyy.MM.dd");
    qDebug()<<ui->timeEdit->dateTime().toString("yyyy.MM.dd hh:mm:ss");
}

void MainWindow::on_commandLinkButton_clicked()
{
    QDesktopServices::openUrl(QUrl("C:") );
}


