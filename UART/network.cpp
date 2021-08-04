#include "network.h"


void MainWindow::on_Multicasr_toggled(bool checked)
{
    if(checked)
    {
        ui->label_5->setText("组播地址");
    }
}
void MainWindow::on_Unicast_toggled(bool checked)
{
    if(checked)
    {
        ui->label_5->setText("远程IP");
    }
}

void MainWindow::on_broadcast_toggled(bool checked)
{
    if(checked)
    {
        ui->label_5->setText("广播地址");
    }
}

void MainWindow::on_Jion_Multicast_clicked()
{
    static bool Multicast_sta = false;
    if(Multicast_sta)//加入状态
    {
        ui->serverip->setEnabled(true);
        ui->port_num->setEnabled(true);
        qDebug("leave");
        udpSocket->leaveMulticastGroup(QHostAddress(ui->serverip->text()));
        Multicast_sta = false;
        ui->connect_ip->setEnabled(true);
        ui->Jion_Multicast->setText("加入组播");
    }
    else    //未加入组播
    {
        if(ui->port_num->text() != ui->port_num_2->text())
        {
            QMessageBox::information(NULL, "Title", "端口号不匹配");
            return;
        }
        ui->serverip->setEnabled(false);
        ui->port_num->setEnabled(false);
        qDebug("join");
        //设置缓冲区

        udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);
        udpSocket->joinMulticastGroup(QHostAddress(ui->serverip->text()));
        Multicast_sta = true;

        ui->connect_ip->setEnabled(false);
        ui->Jion_Multicast->setText("退出组播");
    }

}



//UDP读数据槽函数
void MainWindow::readPendingDatagrams ( ) {
    QString IP_Port;
    // 判断当前套接字是否有数据包到来
    ui->Text_DBG->append("**recv ---");
    qDebug("recv data \r\n");
    while (udpSocket->hasPendingDatagrams ( )) {
        // 数据缓冲区，类似 char [ ]
        QByteArray datagram;
        // 获取当前数据包的大小，从到来的套接字数据包
        datagram.resize (udpSocket->pendingDatagramSize ( ));
        QHostAddress sender;  // 保存客户端的IP地址
        quint16 senderPort;  // 保存客户端的端口号
        // 读取数据，.data ( )完成的是数据类型转换为 char *
        udpSocket->readDatagram (datagram.data ( ), datagram.size ( ), &sender, &senderPort);
        if(sender.toString() == NULL)       //空连接导致故障
        {     ui->Text_DBG->append("**recv ---err");ui->Text_DBG->append(datagram);}
        else
        {
            //IP端口信息获取及打印
            IP_Port = tr("[%1]:[%2]:").arg(sender.toString()).arg(senderPort);

            ui->Text_DBG->append(IP_Port);
            //打印收到的数据
            ui->Text_DBG->append(datagram);
            // 解析和处理...
            // processTheDatagram (datagram);
        }
    }
    qDebug("recv data over********* \r\n");
}


//连接
void MainWindow::on_connect_ip_clicked()
{
    bool ret;
    if(ui->UDP_MODE->isChecked())               //UDP 模式
    {
        qDebug("UDP cc");
       /* if(udpSocket->state()== QAbstractSocket::ConnectingState \
                || udpSocket->waitForConnected(1000)) //连接状态)*/
        if(ui->connect_ip->text() == "断开")
        {
            disconnect(udpSocket, SIGNAL (readyRead( )),0,0);
           //
            udpSocket->disconnectFromHost();
            ui->connect_ip->setText("绑定");
            qDebug("UDP cno");
            ui->TCP_UDP->setEnabled(true);
            ui->SEND_IP->setEnabled(false);
            ui->Jion_Multicast->setEnabled(false);
            ui->UDP_MODE_2->setEnabled(true);
        }
        else
        {

         // 绑定
            //ret = udpSocket->bind(QHostAddress::AnyIPv4,ui->port_num_2->text().toInt(),QUdpSocket::ShareAddress);
            ret = udpSocket->bind(QHostAddress("192.168.1.214"), ui->port_num_2->text().toInt(), QUdpSocket::ShareAddress);
            if(!ret)
            {
                qDebug("UDP_Bind error");
            }
            QString ip = ui->serverip->text();
            qint16 port = ui->port_num->text().toInt();
            //获取编辑区内容
            QString contentStr = ui->sen_text->toPlainText();
            //给指定的IP发送数据
            //ret = udpSocket->writeDatagram(contentStr.toUtf8(), QHostAddress(ip), port);

            ui->connect_ip->setText("断开");
            ui->TCP_UDP->setEnabled(false);
            ui->SEND_IP->setEnabled(true);
            if(ui->Multicasr->isChecked())
                ui->Jion_Multicast->setEnabled(true);
            ui->UDP_MODE_2->setEnabled(false);
         // 当套接字有数据时会发送 readyRead ( )
            connect (udpSocket, SIGNAL (readyRead( )), this, SLOT (readPendingDatagrams()));

            qDebug("UDP cyes");
        }
    }
    else
    {
        if(ui->CLIENT_MODE->isChecked())        //作为TCP客户端
        {
            if (tcpClient->state() == QAbstractSocket::ConnectingState \
                            || tcpClient->waitForConnected(1000)) //连接状态
            {
                tcpClient->disconnectFromHost();
                if (tcpClient->state() == QAbstractSocket::UnconnectedState \
                                || tcpClient->waitForDisconnected(1000))//关闭连接成功
                {
                    ui->connect_ip->setText(tr("连接"));
                    ui->SEND_IP->setEnabled(false);
                    //关闭连接
                    qDebug()<<"手动关闭TCP链接";
                    //取消槽函数连接
                    disconnect(tcpClient, SIGNAL(readyRead()), 0, 0);
                    disconnect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), 0, 0);
                    ui->TCP_UDP->setEnabled(true);
                }
            }
            else
            {

                //连接到服务器端
                tcpClient->connectToHost(ui->serverip->text(), ui->port_num->text().toInt());
                ret = tcpClient->waitForConnected(1000);
                if (ret)  // TCP连接成功则进入if{}
                {
                    ui->connect_ip->setText("断开");
                    ui->SEND_IP->setEnabled(true);//读取数据以及报错信号连接
                    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
                    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
                            this, SLOT(ReadError(QAbstractSocket::SocketError)));
                    ui->TCP_UDP->setEnabled(false);
                }

            }



       }
        else if(ui->SERVER_MODE->isChecked())    //作为TCP服务端
        {
            int len = tcpClient_s.length();
            int j = 0;
            if(tcpServer->isListening())    //监听状态
            {
                for(int i=0; i<len; i++)//断开所有连接
                {
                    // 删除存储在combox中的客户端信息
                    ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")\
                                          .arg(tcpClient_s[0]->peerAddress().toString().split("::ffff:")[1])\
                                          .arg(tcpClient_s[0]->peerPort())));

                    disconnect(tcpClient_s[0], SIGNAL(readyRead()), 0, 0);
                    disconnect(tcpClient_s[0], SIGNAL(disconnected()), 0,0);
                    tcpClient_s[0]->disconnectFromHost();
                    bool ok = tcpClient_s[0]->state() == QAbstractSocket::UnconnectedState ||
                              tcpClient_s[0]->waitForDisconnected(-1);
                    if(!ok)
                    {
                     // 处理异常
                     qDebug()<<"断开连接发生异常"<<i<<tcpClient_s.length();
                    }
                    else
                    {
                        qDebug()<<"断开连接正常";
                        // 删除存储在tcpClient列表中的客户端信息
                        tcpClient_s[0]->destroyed();
                        tcpClient_s.removeAt(0);
                    }



                }
                disconnect(tcpServer, SIGNAL(newConnection()), 0, 0);
                ui->SEND_IP->setEnabled(false);
                tcpServer->close();     //不再监听端口
                ui->connect_ip->setText("监听");
                qDebug()<<"关闭监听TCP链接";
                ui->TCP_UDP->setEnabled(true);
            }
            else//未监听
            {
                ui->connect_ip->setEnabled(true);
                ui->SEND_IP->setEnabled(false);
                //有新连接的时候触发信号槽函数
                connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
                bool ok = tcpServer->listen(QHostAddress::Any, ui->port_num->text().toInt());
                     if(ok)
                     {
                         ui->connect_ip->setText("取消");
                         ui->SEND_IP->setEnabled(true);
                         ui->TCP_UDP->setEnabled(false);
                     }
                     qDebug()<<"监听TCP链接"<<ok;

            }
        }
        else
            qDebug()<<"未选择服务端客户端";
    }
}
//新连接-槽函数
void MainWindow::NewConnectionSlot()
{
    qDebug()<<"有新连接";
    currentClient = tcpServer->nextPendingConnection();
        tcpClient_s.append(currentClient);//添加到连接的队列
        //展示客户端IP信息
        ui->cbxConnection->addItem(tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
                                              .arg(currentClient->peerPort()));

        connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadData_S()));
        connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
}
//断开连接-槽函数
void MainWindow::disconnectedSlot()
{
    for(int i=0; i<tcpClient_s.length(); i++)
   {
       if(tcpClient_s[i]->state() == QAbstractSocket::UnconnectedState)
       {

           // 删除存储在combox中的客户端信息
           ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")\
                                 .arg(tcpClient_s[i]->peerAddress().toString().split("::ffff:")[1])\
                                 .arg(tcpClient_s[i]->peerPort())));
           disconnect(tcpClient_s[i], SIGNAL(readyRead()), 0, 0);
           disconnect(tcpClient_s[i], SIGNAL(disconnected()), 0,0);
           // 删除存储在tcpClient列表中的客户端信息
           qDebug()<<(tcpClient_s[i]->peerPort())<<i;
            tcpClient_s[i]->destroyed();
            tcpClient_s.removeAt(i);
            qDebug()<<"数据长度"<<tcpClient_s.length();
       }
   }
}
//发送网络数据
void MainWindow::on_SEND_IP_clicked()
{
    bool ret;
    if(ui->UDP_MODE->isChecked())
    {
        if(udpSocket == NULL){
            return;
        }
        //先获取对方的IP和端口
        QString ip = ui->serverip->text();
        qint16 port = ui->port_num->text().toInt();
        //获取编辑区内容
        QString contentStr = ui->sen_text->toPlainText();
        //给指定的IP发送数据
        ret = udpSocket->writeDatagram(contentStr.toUtf8(), QHostAddress(ip), port);
        if(ret)
            qDebug("ret os ture");
        else
            qDebug("ret os false");

    }
    else if(ui->CLIENT_MODE->isChecked())//判断是服务器还是客户端
    {
        QString data = ui->sen_text->toPlainText();
            if(data != "")
            {
                tcpClient->write(data.toLatin1()); //qt5去除了.toAscii()
            }
    }
    else if(ui->SERVER_MODE->isChecked())
    {


        QString clientIP = ui->cbxConnection->currentText().split(":")[0];
            int clientPort = ui->cbxConnection->currentText().split(":")[1].toInt();
            for(int i=0; i<tcpClient_s.length(); i++)
            {
                if(tcpClient_s[i]->peerAddress().toString().split("::ffff:")[1]==clientIP\
                                && tcpClient_s[i]->peerPort()==clientPort)
                {
                    tcpClient_s[i]->write(ui->sen_text->toPlainText().toLatin1());
                    return; //ip:port唯一，无需继续检索
                }
            }
    }

}
//读取客户端数据
void MainWindow::ReadData_S()
    {
        // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
        for(int i=0; i<tcpClient_s.length(); i++)
        {
            QByteArray buffer = tcpClient_s[i]->readAll();
            if(buffer.isEmpty())    continue;

            static QString IP_Port, IP_Port_Pre;
            IP_Port = tr("[%1:%2]:").arg(tcpClient_s[i]->peerAddress().toString().split("::ffff:")[1])\
                                         .arg(tcpClient_s[i]->peerPort());

            // 若此次消息的地址与上次不同，则需显示此次消息的客户端地址
            if(IP_Port != IP_Port_Pre)
                ui->recv_text->append(IP_Port);

            ui->recv_text->append(buffer);

            //更新ip_port
            IP_Port_Pre = IP_Port;
        }
    }
void MainWindow::ReadData()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        ui->recv_text->append(buffer);
    }
}


void MainWindow::ReadError(QAbstractSocket::SocketError)
{
    qDebug()<<"关闭";
    disconnect(tcpClient, SIGNAL(readyRead()), 0, 0);
    disconnect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), 0, 0);
    tcpClient->disconnectFromHost();
    ui->connect_ip->setText(tr("连接"));
    ui->SEND_IP->setEnabled(false);
    QMessageBox msgBox;
    msgBox.setText(tr("failed to connect server because %1").arg(tcpClient->errorString()));



}

void MainWindow::on_TCP_MODE_toggled(bool checked)
{
    qDebug()<<"on_TCP_MODE_toggled";
    if(ui->TCP_MODE->isChecked())//TCP模式默认服务端
    {
        ui->SERVER_MODE->setEnabled(true);
        ui->CLIENT_MODE->setEnabled(true);
        ui->SERVER_MODE->setChecked(true);
        ui->CLIENT_MODE->setChecked(false);
        ui->port_num_2->setEnabled(true);
        ui->serverip->setEnabled(false);
        ui->port_num->setEnabled(false);
        ui->Jion_Multicast->setEnabled(false);
        ui->connect_ip->setText("监听");
        ui->label_5->setText("服务器IP");
        ui->UDP_MODE_2->setEnabled(false);
    }
    else
    {
        ui->SERVER_MODE->setChecked(false);
        ui->CLIENT_MODE->setChecked(false);
        ui->SERVER_MODE->setEnabled(false);
        ui->CLIENT_MODE->setEnabled(false);
        ui->port_num_2->setEnabled(true);
        ui->serverip->setEnabled(true);
        ui->port_num->setEnabled(true);
        ui->Jion_Multicast->setEnabled(true);
        ui->connect_ip->setText("绑定");
        if(ui->Unicast->isChecked())
            ui->label_5->setText("远程IP");
        else if(ui->broadcast->isChecked())
            ui->label_5->setText("广播地址");
        else if(ui->Multicasr->isChecked())
            ui->label_5->setText("组播地址");

        ui->Jion_Multicast->setEnabled(false);
        ui->UDP_MODE_2->setEnabled(true);
    }
}

void MainWindow::on_SERVER_MODE_toggled(bool checked)
{
    qDebug()<<"on_SERVER_MODE_toggled";
    if(checked)
    {
        ui->connect_ip->setText("监听");
        ui->port_num_2->setEnabled(true);
        ui->serverip->setEnabled(false);
        ui->port_num->setEnabled(false);
    }
}

void MainWindow::on_CLIENT_MODE_toggled(bool checked)
{
    qDebug()<<"on_CLIENT_MODE_toggled";
    if(checked)
    {
        ui->connect_ip->setText("连接");
        ui->port_num_2->setEnabled(false);
        ui->serverip->setEnabled(true);
        ui->port_num->setEnabled(true);
    }
}


