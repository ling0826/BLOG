#include "thread.h"
#include <QDebug>
#include <uart.h>
My_thread::My_thread(QObject *parent):QObject(parent)
{
    qDebug()<<"My_thread::My_thread";
}
void My_thread::Mywork()
{
    while(!isStop)
       {
        if(RX_End!=RX_Begin)
        {
            qDebug() <<QString::number( RX_BUF[RX_Begin], 16);
            RX_Begin = ((RX_Begin++)==RX_LENGTH)?0:RX_Begin;

        }
        else
           QThread::sleep(1);
           /*emit signal_back();    //发送返回信号
           qDebug() << "the child thread number:" << QThread::currentThread();
      */ }
}
