#ifndef PTHREAD_TIME_H
#define PTHREAD_TIME_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QDebug>


class pthread_time: public QObject
{
    Q_OBJECT
public:
    explicit pthread_time(QObject *parent = 0);
    void Mywork();  //线程工作函数
signals:
    void signal_back();
    void signal_1s();
    public:
        bool isStop = false;
private    slots:
    void timer_1s();
public:

};



#endif // PTHREAD_TIME_H
