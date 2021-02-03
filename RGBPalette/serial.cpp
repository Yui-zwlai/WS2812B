#include "serial.h"


Serial::Serial()
{
    // 初始化串口
    m_serialPort = new QSerialPort;
}

QStringList Serial::scanSerial()
{
    QStringList serialStrList;
    // 读取串口信息
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        m_serialPort->setPort(info);
        serialStrList.append(m_serialPort->portName());
    }
    return serialStrList;
}

//打开串口
bool Serial::Open(QString serialName,QString baudRate,int parityBit,int dataBit,int stopBit,int controlFlow)
{
    m_serialPort->setPortName(serialName);// 设置串口名
    if(m_serialPort->open(QIODevice::ReadWrite))// 打开串口(以读写方式)
    {
        m_serialPort->setDataTerminalReady(true);
        switch(parityBit)
        {
            case NoParity:parityBit = QSerialPort::NoParity;break;
            case EvenParity:parityBit = QSerialPort::EvenParity;break;
            case OddParity:parityBit = QSerialPort::OddParity;break;
            case SpaceParity:parityBit = QSerialPort::SpaceParity;break;
            case MarkParity:parityBit = QSerialPort::MarkParity;break;
        }
        switch(dataBit)
        {
            case Data5:dataBit = QSerialPort::Data5;break;
            case Data6:dataBit = QSerialPort::Data6;break;
            case Data7:dataBit = QSerialPort::Data7;break;
            case Data8:dataBit = QSerialPort::Data8;break;
        }
        switch(stopBit)
        {
            case OneStop:stopBit = QSerialPort::OneStop;break;
            case OneAndHalfStop:stopBit = QSerialPort::OneAndHalfStop;break;
            case TwoStop:stopBit = QSerialPort::TwoStop;break;
        }
        switch(controlFlow)
        {
            case NoFlowControl:controlFlow = QSerialPort::NoFlowControl;break;
            case HardwareControl:controlFlow = QSerialPort::HardwareControl;break;
            case SoftwareControl:controlFlow = QSerialPort::SoftwareControl;break;
        }

        m_serialPort->setBaudRate(baudRate.toInt()); // 设置波特率(默认为115200)
        m_serialPort->setParity( (QSerialPort::Parity)parityBit ); // 设置校验位(无校验位)
        m_serialPort->setDataBits( (QSerialPort::DataBits)dataBit ); // 设置数据位(数据位为8位)
        m_serialPort->setStopBits( (QSerialPort::StopBits)stopBit ); // 设置停止位(停止位为1)
        m_serialPort->setFlowControl( (QSerialPort::FlowControl)controlFlow ); // 设置流控制(无数据流控制)
        // 当下位机中有数据发送过来时就会响应这个槽函数
        connect(m_serialPort, SIGNAL(readyRead()), this,SLOT(ReadData()));
        // 当下位机中有数据发送过来时就会触发这个信号，以提示其它类对象
        connect(m_serialPort, SIGNAL(readyRead()), this, SIGNAL(readSignal()));
        return true;
    }
    return false;
}

// 关闭串口
void Serial::Close()
{
    m_serialPort->clear();
    m_serialPort->close();
}

// 发送数据给下位机
void Serial::SendData(QByteArray sendData)
{

    qDebug() << sendData;
    m_serialPort->write(sendData);// 发送数据帧
}

// 读取下位机发来数据
void Serial::ReadData()
{
    m_readBuf = m_serialPort->readAll();// 将下位机发来数据存储在数据缓冲区
}

// 获得读取数据缓冲区
QByteArray Serial::GetReadBuf()
{
    return m_readBuf;
}

// 清除读取数据缓冲区
void Serial::ClearReadBuf()
{
    m_readBuf.clear();
}


