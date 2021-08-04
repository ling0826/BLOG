#include "pthread_time.h"
#include "def.h"
#include "mainwindow.h"
#include <QApplication>

QTimer *timer;

pthread_time::pthread_time(QObject *parent):QObject(parent)
{

}
void pthread_time::Mywork()
{

     qDebug()<<"******1s ";
     timer = new QTimer();
         connect(timer,SIGNAL(timeout()),this,SLOT(timer_1s()),Qt::DirectConnection);
     timer->start(1500);


  //timer->start(1500);
    while(!isStop)
       {
           QThread::sleep(1);
           

           /*emit signal_back();    //发送返回信号
           qDebug() << "the child thread number:" << QThread::currentThread();*/
       }

}

void pthread_time::timer_1s()
{
qDebug()<<"+++1s**** ";
    timer->start(1000);
    time = time.addSecs(1);
    emit signal_1s();
    //ui->dateTimeEdit_2->setDateTime(time);
}
