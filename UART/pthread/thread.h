#ifndef THREAD_H
#define THREAD_H
#include <QObject>

#include <QThread>


class My_thread: public QObject
{
    Q_OBJECT
public:
    explicit My_thread(QObject *parent = 0);
    void Mywork();  //线程工作函数

signals:
    void signal_back();
    public:
        bool isStop = false;

};


#endif // THREAD_H
