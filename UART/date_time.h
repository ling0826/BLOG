#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Date_Time;
}

class Date_Time : public QWidget
{
    Q_OBJECT

public:
    explicit Date_Time(QWidget *parent = 0);
    void closeEvent( QCloseEvent * event);
    ~Date_Time();

private slots:
    void time_1s_show();
    void on_close_clicked();
    void on_Set_datatime_clicked();

    void on_datetime_check_clicked();

signals:
    date_time_close(int);



private:
    Ui::Date_Time *ui;



};

#endif // DATE_TIME_H
