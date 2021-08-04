#include "mainwindow.h"
#include <QApplication>
#include "form.h"
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*a.setFont(QFont("Microsoft Yahei", 9));
     QTextCodec *codec = QTextCodec::codecForName("UTF-8");*/
MainWindow w;
    w.show();

    return a.exec();
}
