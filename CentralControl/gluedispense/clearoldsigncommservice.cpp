#include "clearoldsigncommservice.h"
#include "QsLog.h"

ClearOldSignCommService::ClearOldSignCommService(QObject *parent) : QObject(parent)
{
    m_serial = new QSerialPort(this);
}

void ClearOldSignCommService::connect_to_mcu_port(QString port_name)
{
    if(m_serial->isOpen()){
       m_serial->close();
    }

    qint32 val_baud = QSerialPort::Baud9600;
    val_baud = QSerialPort::Baud115200;
    if(m_serial != nullptr){

        m_serial->setPortName(port_name);
        if(!m_serial->open(QIODevice::ReadWrite))
        {
            QLOG_INFO()<<port_name<<u8"打开控制器失败!";
        }else{
            m_serial->setBaudRate(val_baud,QSerialPort::AllDirections);
            m_serial->setDataBits(QSerialPort::Data8);
            m_serial->setFlowControl(QSerialPort::NoFlowControl);
            m_serial->setParity(QSerialPort::NoParity);
            m_serial->setStopBits(QSerialPort::OneStop);
            QLOG_INFO()<<port_name<<u8"打开控制器成功!";
            connect(m_serial,&QSerialPort::readyRead, this, &ClearOldSignCommService::slot_rev_serial_number);
        }
    }
}

void ClearOldSignCommService::sendClearOldSignCommand()
{
    QList<QByteArray> datas;
    QByteArray aa;
    aa.append(static_cast<char>(0xDD));
    aa.append(static_cast<char>(0xFF));
    aa.append(static_cast<char>(0x00));
    aa.append(static_cast<char>(0x06));
    aa.append(static_cast<char>(0xA1));
    aa.append(static_cast<char>(0x02));
    aa.append(static_cast<char>(0x00));
    aa.append(static_cast<char>(0x00));
    aa.append(static_cast<char>(0x08));
    quint8 nCheckCode = 0;
    for(int i=2;i<9;i++)
    {
        nCheckCode ^= static_cast<uchar>(aa[i]);
    }
    aa.append(static_cast<char>(nCheckCode));
    aa.append(static_cast<char>(0xBB));
    aa.append(static_cast<char>(0xCC));
    datas.append(aa);
    QByteArray responseData;

    m_is_sync = true;

    if(!m_serial->isOpen()){
        QLOG_INFO() << "press serial port open fail";
    }else{
        if(m_serial!=nullptr&&m_serial->isOpen()){
            m_serial->write(aa);
            m_serial->flush();
            //m_serial->waitForBytesWritten(1000);
        }

        while (m_serial->waitForReadyRead(1000)){
            responseData += m_serial->readAll();

            if(responseData.right(2).compare(QByteArray::fromRawData((const char*)&CMD_END[0],2))==0
                    && !responseData.isEmpty()  ){
                break;
            }
        }

        if(checkClearOldSignResult(responseData)){

            QLOG_INFO() << "scanner check command is ok!";
        }


        m_is_sync = false;
    }
}

void ClearOldSignCommService::slot_rev_serial_number()
{
    if(!m_is_sync){

    }
}

bool ClearOldSignCommService::checkClearOldSignResult(QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0C) return false;
    //判断报文头是否正确
    if(0xAB != static_cast<uchar>(m_response_data[0])) return false;
    if(0xAB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x00 != static_cast<uchar>(m_response_data[2])) return false;
    if(0x06 != static_cast<uchar>(m_response_data[3])) return false;
    if(0xA1 != static_cast<uchar>(m_response_data[4])) return false;
    if(0x02 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x00 != static_cast<uchar>(m_response_data[6])) return false;
    if(0x00 != static_cast<uchar>(m_response_data[7])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<9;i++)
    {
        nCheckCode ^= static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[9])) return false;
    if(0xCD != static_cast<uchar>(m_response_data[10]))   return false;
    if(0xCD != static_cast<uchar>(m_response_data[11]))   return false;
    return true;
}


