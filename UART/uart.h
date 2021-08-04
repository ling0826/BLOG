#ifndef UART_H
#define UART_H
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>



#define RX_LENGTH   4096
#define TX_LENGTH   4096
extern unsigned char RX_BUF[RX_LENGTH];
extern unsigned char TX_BUF[TX_LENGTH];
extern unsigned int RX_Begin;
extern unsigned int RX_End;

extern QSerialPort *serial_L;
#endif // UART_H
