#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QStringList>
#include <QByteArray>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>

#include <QTextCodec>

class Serial:public QObject // 要继承QObject，才能使用 connect() 函数
{
    Q_OBJECT // 有这个才能使用信号和槽

public:
    Serial();
    QStringList scanSerial();
    bool Open(QString serialName,QString baudRate,int parityBit,int dataBit,int stopBit,int controlFlow);
    void Close();
    void SendData(QByteArray sendData);
    QByteArray GetReadBuf();
    void ClearReadBuf();

signals:
    void readSignal(); // 当下位机中有数据发送过来时就会触发这个信号，以提示其它类对象


public slots:
    void ReadData(); // 读取下位机发来数据


private:
    QSerialPort *m_serialPort;// 实例化一个指向串口的指针，可以用于访问串口
    QByteArray m_readBuf; // 存储下位机发来数据的缓冲区
    enum Parity
    {
        NoParity,
        EvenParity,
        OddParity,
        SpaceParity,
        MarkParity
    };

    enum DataBits
    {
        Data5,
        Data6,
        Data7,
        Data8,
    };

    enum StopBits
    {
        OneStop,
        OneAndHalfStop,
        TwoStop,
    };

    enum FlowControl
    {
        NoFlowControl,
        HardwareControl,
        SoftwareControl,
    };

};



#endif // SERIAL_H
