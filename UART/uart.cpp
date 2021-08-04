#include "uart.h"
#include "cJSON/cJSON.h"
#include "Error_Data.h"
QSerialPort *serial_L;

unsigned char RX_BUF[RX_LENGTH];
unsigned char TX_BUF[TX_LENGTH];
unsigned int RX_Begin = 0;
unsigned int RX_End = 0;
void MainWindow::on_buard_currentIndexChanged(const QString &arg1)
{


    serial_L->setBaudRate(arg1.toInt());
    DBG("改变波特率:"+arg1);
    //DBG(QString::number(serial_L->baudRate(QSerialPort::AllDirections),10));
}

void MainWindow::on_COMX_currentIndexChanged(const QString &arg1)
{
    serial_L->setPortName(ui->COMX->currentText());
    DBG("改变串口号:"+arg1);
}

void MainWindow::on_OPNE_CLOSE_clicked()
{
    //open_flag = !open_flag;
    serial_L->setPortName(ui->COMX->currentText());
    if(!serial_L->isOpen()) //未打开 -》 打开串口
    {
        //设置串口参数
        serial_L->setBaudRate(ui->buard->currentText().toInt());
        serial_L->setDataBits(QSerialPort::Data8);
        serial_L->setParity(QSerialPort::NoParity);
        serial_L->setStopBits(QSerialPort::OneStop);
        if(serial_L->open(QIODevice::ReadWrite))
        {
            ui->Text_DBG->append("串口打开成功\r\n");
            ui->buard->setEnabled(false);
            ui->COMX->setEnabled(false);
            ui->GETCOMS->setEnabled(false);
            ui->OPNE_CLOSE->setText("关闭串口");

            connect(serial_L,SIGNAL(readyRead()),this,SLOT(readMyCom()));
        }
        else
        {
            ui->Text_DBG->append("串口打开失败\r\n");
        }

    }
    else
    {
        ui->buard->setEnabled(true);
        ui->COMX->setEnabled(true);
        ui->GETCOMS->setEnabled(true);
        ui->OPNE_CLOSE->setText("打开串口");
        serial_L->clear();
        serial_L->close();
        //serial_L->deleteLater();

    }
}
//数据读取回调函数
char RX_Getchar()
{
    char ch;
    ch = RX_BUF[RX_Begin];
    RX_Begin++;
    if(RX_Begin == RX_LENGTH)
    {
        RX_Begin = 0;
    }
    return ch;
}
unsigned char rx_stepcou = 0;
unsigned int rbuf_length = 0;
unsigned int rbuf_offset = 0;
unsigned char rbuf[1024] ;
void MainWindow::readMyCom()
{
    int ii;
    unsigned char ch;
    requestData = serial_L->readAll();
    if(requestData!=NULL)//如果读取到有数据，就显示出来。
    {

        qDebug() << requestData.toHex();
        for(ii = 0; ii < requestData.length();ii++)
        {
            RX_BUF[RX_End] = requestData[ii];
            //text_apped(RX_BUF[RX_End]);
            RX_End++;
            if(RX_End == RX_LENGTH)
                RX_End = 0;
        }
    }
    //数据处理
    /*json*/
    while(RX_End != RX_Begin)
    {
        qDebug()<<"ste:"<<rx_stepcou;
        ch = RX_Getchar();
        qDebug("ste:%d,ch:%#x",rx_stepcou,ch);
        switch(rx_stepcou)
        {
            case 0:
            if(ch == 0xFA) {rx_stepcou = 1;}
            break;
        case 1:
            if(ch == 0xFB) {rx_stepcou = 2;}
            else {rx_stepcou = 0;}
            break;
        case 2:
            rbuf_length = ch;rx_stepcou = 3;
            break;
        case 3:
            rbuf_length = (rbuf_length<<8)+ch;rx_stepcou = 4;rbuf_offset = 0;qDebug()<<"length:"<<rbuf_length;
            break;
        case 4:
            if(rbuf_offset != rbuf_length)
            {
                rbuf[rbuf_offset] = ch;
                rbuf_offset++;
                if(rbuf_offset == rbuf_length)
                   { rx_stepcou = 5;rbuf[rbuf_offset] = 0;}
            }
            break;
        case 5:
            if(ch == 0xEA) {rx_stepcou = 6;}
            else rx_stepcou = 0;
            break;
        case 6:
            if(ch == 0xE1)
            {
                qDebug()<<"接收成功";
                cJSON *json;
                char *out;
                json = cJSON_Parse((const char*)rbuf);
                out = cJSON_Print(json);
                qDebug()<<out;
                free(out);

            }
            rx_stepcou = 0;
            break;
        default:
            rx_stepcou = 0;


        }

    }
    //qDebug()<<"数据回调函数";

}
//输出调试信息
void MainWindow::DBG(const QString &text)
{
    if(DBG_F)
        ui->Text_DBG->append(text);
}


//配置调试信息
void MainWindow::on_DBG_clicked(bool checked)
{
    DBG_F = checked;
}
//发送文本框数据
void MainWindow::on_SEND_clicked()
{
    char *out;
    cJSON *test,*json;
    char sendbuf[16];
    int len  = 0;
        cJSON *arry;
        cJSON *people;
        cJSON *name;
        cJSON *arg;
        cJSON *math;
        test = cJSON_CreateObject();
        arry = cJSON_CreateArray();
        cJSON_AddItemToObject(test,"test",arry);
        people = cJSON_CreateObject();
        cJSON_AddItemToArray(arry,people);
        std::string tr;


        QString str="张三";

        tr=str.toStdString();

        const char *chr=tr.data();


        name = cJSON_CreateObject();
        cJSON_AddStringToObject(name,"name",chr);
        cJSON_AddItemToObject(people,"1",name);

        arg = cJSON_CreateObject();
        cJSON_AddStringToObject(arg,"arg","12");
        cJSON_AddItemToObject(people,"2",arg);

        math = cJSON_CreateObject();
        cJSON_AddStringToObject(math,"math","96.5");
        cJSON_AddItemToObject(people,"3",math);

        out = cJSON_Print(test);
        json = cJSON_GetObjectItem(test,"test");
        json = cJSON_GetArrayItem(json,0);
        json = cJSON_GetObjectItem(json,"1");
        json = cJSON_GetObjectItem(json,"name");
        //qDebug()<<json->valuestring;
        //out = json->valuestring;
        QString str1(out);

    if(serial_L->isOpen())
    {
        sendbuf[0] = 0xFA;
        sendbuf[1] = 0xFB;

        len = str1.toUtf8().length();
        sendbuf[2] = len>>8;
        sendbuf[3] = len&0xff;
        serial_L->write(sendbuf,4);
        serial_L->write(out,len);
        sendbuf[4] = 0xEA;
        sendbuf[5] = 0xE1;
        serial_L->write(&sendbuf[4],2);
        //qDebug()<<out;
    }
    qDebug()<<"size"<<str1.length()<<str1;

        //serial_L->write(ui->textEdit->toPlainText().toLatin1());
}
//获取可用串口
void MainWindow::on_GETCOMS_clicked()
{
    ui->COMX->clear();
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
    DBG("获取端口号成功\r\n");
}
