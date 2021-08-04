#include "form.h"





Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    QString xx = "123";
    ui->setupUi(this);

    if( QString::compare( serial_L->portName(),"NULL") == 0 )
        qDebug()<<"没有串口打开";
    else
    {
        qDebug()<<"关闭串口"<<serial_L->portName();
        serial_L->clear();
        serial_L->close();
    }
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comBox->addItem(serial.portName());
            serial.close();
        }

    }
    connect(ui->comBox,SIGNAL(currentIndexChanged( QString)),this,SLOT(COMBOX(QString)));
    connect(ui->Buard,SIGNAL(currentIndexChanged(QString)),this,SLOT(setbuard(QString)));


}
void Form::COMBOX(const QString &arg)
{
    qDebug()<<"123456\r\n"<<arg;
}
void Form::setbuard(const QString &arg)
{
    qDebug()<<"1234567\r\n"<<arg;
}

Form::~Form()
{
    delete ui;
}
void Form::on_SET_UART_clicked()
{
    //设置串口参数
   /* serial_L->setBaudRate(ui->Buard->currentText().toInt());
    serial_L->setPortName(ui->comBox->currentText());
    serial_L->setDataBits(QSerialPort::Data8);
    serial_L->setParity(QSerialPort::NoParity);
    serial_L->setStopBits(QSerialPort::OneStop);
    if(serial_L->open(QIODevice::ReadWrite))
    {
        connect(serial_L,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    }
    else
    {
    }
*/
    this->ui->TEST->append("this is form");

    //parentWidget()->show();
    delete this;
}

