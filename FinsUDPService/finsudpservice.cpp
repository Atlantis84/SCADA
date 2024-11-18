#include "finsudpservice.h"
#include <QMessageBox>
#include <QHostInfo>
#include "QsLog.h"
#include <QDateTime>
#include <QApplication>
#include <QDir>
using namespace QsLogging;
FinsUDPService::FinsUDPService()
{
    m_pRemoteIP = "127.0.0.1";
    m_pRemotePort = 60000;
    m_pInitSign = false;

    m_pUdpSocket = new QUdpSocket(this);
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    connect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    qRegisterMetaType<QByteArray>("QByteArray");
}

void FinsUDPService::send_message_to_plc(const uchar function_code, const short data_address, const short data_length, const QByteArray data_array)
{
    if(!m_pInitSign)
    {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
        msgBox->setWindowTitle(u8"警告");
        msgBox->setText(u8"FinsUDPService needs to initialize!");
        msgBox->exec();
        return;
    }

    m_mutex.lock();
    QHostAddress hAddr(m_pRemoteIP);
    int hPort = m_pRemotePort;
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To PLC:"<<bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_pUdpSocket->flush();
    m_mutex.unlock();
}

void FinsUDPService::send_message_to_plc(const uchar function_code, const uchar section_code, const short data_address, const short data_length, const QByteArray data_array)
{
    if(!m_pInitSign)
    {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
        msgBox->setWindowTitle(u8"警告");
        msgBox->setText(u8"FinsUDPService needs to initialize!");
        msgBox->exec();
        return;
    }

    m_mutex.lock();
    QHostAddress hAddr(m_pRemoteIP);
    int hPort = m_pRemotePort;
    QByteArray btArray = make_plc_message(function_code,section_code,data_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To PLC:"<<bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_pUdpSocket->flush();
    m_mutex.unlock();
}

void FinsUDPService::init_ip_port(const QString remoteip, const int remotport)
{
    if(m_pInitSign)
        return;

    m_pInitSign = true;
    m_pRemoteIP = remoteip;
    m_pRemotePort = remotport;
    QLOG_INFO()<<"init FINS UDP SUCCESS!";
}

quint8 FinsUDPService::process_ip_string(const QString ipstr)
{
    int nSize = ipstr.size();
    int nCount = 0;
    QString mData;
    mData.clear();
    QChar mChar;
    for(int idx = 0; idx<nSize; idx++)
    {
        mChar = ipstr[idx];
        if(mChar == '.')
        {
            nCount++;
        }
        else if(mChar != '.' && nCount == 3 && (mChar >= '0' || mChar <= '9'))
        {
            mData.append(mChar);
        }
    }
    return static_cast<quint8>(mData.toInt());
}

quint8 FinsUDPService::get_last_ip(const int id)
{
    if (id == 0)
    {
        QString localHostName = QHostInfo::localHostName();
        QString localIP = "";
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress addr,info.addresses())
            if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
                localIP = addr.toString();
            }
        return process_ip_string(localIP);
    }
    else
    {
        return process_ip_string(m_pRemoteIP);
    }
}

QByteArray FinsUDPService::make_plc_message(const uchar function_code, const short data_address, const short data_length, const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //起始码
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(0x82));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

QByteArray FinsUDPService::make_plc_message(const uchar function_code, const uchar section_code, const short data_address, const short data_length, const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //起始码
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(section_code));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

QString FinsUDPService::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]) ;
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}

void FinsUDPService::slot_receive_message()
{
    m_mutex.lock();
    if(m_pUdpSocket == nullptr)
        return;
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    }
    m_mutex.unlock();
    emit signal_proc_plc_msg(datagram);
}

void FinsUDPService::slot_proc_plc_msg(const QByteArray data_array)
{
    if(data_array.length() > 10)//check the header length
    {
        if((data_array[9] & 0xFF) == 0xFF)
        {
            QLOG_INFO()<<"REV PLC MSG:"<<bytes_to_str(data_array);
            if((data_array[10] == 0x01) && (data_array[11] == 0x01))
            {
//                if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
//                {
                QByteArray tmpArray;
                for(int i=14;i<data_array.length();i++)
                    tmpArray.append(data_array[i]);
                emit signal_proc_real_data(tmpArray);
//                }
//                else
//                {
//                    QLOG_WARN()<<"PLC read back message is ABNORMAL!";
//                    QByteArray tmpArray;
//                    emit signal_proc_real_data(tmpArray);
//                }
            }
            else if((data_array[10] == 0x01) && (data_array[11] == 0x02))
            {
                if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
                {
                    ;
                }
                else
                {
//                    QLOG_WARN()<<"PLC write back message is ABNORMAL!";
                    ;
                }
            }
        }
    }
    else
    {
        QLOG_WARN()<<"PLC message header lenght is WRONG!";
    }
}

