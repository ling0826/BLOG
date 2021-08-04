#include "date_time.h"
#include "ui_date_time.h"
#include "def.h"
#include "pthread/pthread_time.h"


QDateTime time_data;

Date_Time::Date_Time(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Date_Time)
{
    ui->setupUi(this);
    //emit ui->datetime_check->clicked();
    qDebug()<<"开始计时";

}

Date_Time::~Date_Time()
{
    delete ui;
}

void Date_Time::on_close_clicked()
{
    emit date_time_close(DATE_TIME_NUM);
}
//点击“X”关闭窗口触发信号
void Date_Time::closeEvent( QCloseEvent * event)
{

    emit date_time_close(DATE_TIME_NUM);
}

void Date_Time::on_Set_datatime_clicked()
{

}
//时间校验
void Date_Time::on_datetime_check_clicked()
{
    time_data = QDateTime::currentDateTime();
    ui->dateTimeEdit_2->setDateTime(time_data);
    ui->lcdNumber_h->display(time_data.time().hour());
    ui->lcdNumber_m->display(time_data.time().minute());
    ui->lcdNumber_s->display(time_data.time().second());
}
//时间显示
void Date_Time::time_1s_show()
{
    qDebug()<<"time_1s_show";
    ui->dateTimeEdit_2->setDateTime(time_data);
    ui->lcdNumber_h->display(time_data.time().hour());
    ui->lcdNumber_m->display(time_data.time().minute());
    ui->lcdNumber_s->display(time_data.time().second());
}
