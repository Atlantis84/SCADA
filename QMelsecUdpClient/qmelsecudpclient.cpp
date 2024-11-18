#include "qmelsecudpclient.h"
#include "melsec_3e.h"
#include "QsLog.h"
#include <QMessageBox>

using namespace QsLogging;

QMelsecUdpClient::QMelsecUdpClient()
{
    m_pRemoteIP = "127.0.0.1";
    m_pRemotePort = 60000;
    m_pInitSign = false;

    m_pUdpSocket = new QUdpSocket(this);

    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    connect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    qRegisterMetaType<QByteArray>("QByteArray");

    map_melsec_values.insert("X",0x9C);//输入
    map_melsec_values.insert("Y",0x9D);//输入
    map_melsec_values.insert("M",0x90);
    map_melsec_values.insert("L",0x92);
    map_melsec_values.insert("TS",0xC1);
    map_melsec_values.insert("R",0xAF);
    map_melsec_values.insert("D",0xA8);
    map_melsec_values.insert("DT",0xA8);
}

void QMelsecUdpClient::init_ip_port(const QString remoteip, const int remotport, const quint16 local_port)
{
    if(m_pInitSign)
        return;

    m_pInitSign = true;
    m_pRemoteIP = remoteip;
    m_pRemotePort = remotport;

    if(local_port!=0)
        m_pUdpSocket->bind(local_port);
}

void QMelsecUdpClient::send_message_to_plc(uint16_t code, QString addr, uint16_t length)
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

    uint8_t caddr = map_melsec_values.value(addr);

    m_mutex.lock();

    CommandFrame_0401_ASCII frame;
    QByteArray cmd = frame.readBytes(code, caddr, length);
    QLOG_INFO()<<"Send Msg To PLC:"<<bytes_to_str(cmd);
    QHostAddress hAddr(m_pRemoteIP);
    int hPort = m_pRemotePort;
    QLOG_INFO()<< hAddr<<m_pRemotePort;
    m_pUdpSocket->writeDatagram(cmd,cmd.length(),hAddr,static_cast<quint16>(hPort));
    m_pUdpSocket->flush();
    m_mutex.unlock();
    sendDataLength = length;//global sendDataLength
}

void QMelsecUdpClient::slot_receive_message()
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

void QMelsecUdpClient::slot_proc_plc_msg(const QByteArray data_array)
{
    QLOG_INFO() << "slot_proc_plc_msg data_array is "<<data_array;
    QByteArray array;
    ResponseFrame_MC_3E_ASCII resp(data_array);
    uint16_t ret = resp.errorCode();
    if(ret){
        QLOG_WARN()<<"PLC read back message is ABNORMAL!" << ret;
        return;//return error code
    }

    if(data_array.size() != 11 + 2 * sendDataLength){
        QLOG_WARN() << "PLC read back message lenght is error!";
        return;
    }

    array.resize(2*sendDataLength);

    int nFindSize = data_array.size() - 2*sendDataLength;

    QLOG_INFO()<<"sendDataLength"<<sendDataLength<<nFindSize<<data_array.size();

    array = data_array.mid(nFindSize, 2*sendDataLength);

//    for(int i = 0; i < 2*sendDataLength; i++)
//    {
//         array[i] = data_array[nFindSize + i];
//    }
    emit signal_proc_real_data(array);
    //receive array data
}

QString QMelsecUdpClient::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]);
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}

QString QMelsecUdpClient::QByteArrayToValue(QByteArray array, int flags, int WordLength)
{
    bool ok;
    QString Res;

    if(flags == RESULT_TRANS_DEC){
        std::reverse(array.begin(), array.end());
        //quint32 values = *(reinterpret_cast<const quint32 *>(array.constData()));
        int values = array.toHex().toInt(&ok, 16);
        Res = QString::number(values, 10);
    }else{
        for(int i=0;i<array.size();i++){
            if(i%2 == 0){
                QByteArray ab;
                ab.push_back(array[i+1]);
                ab.push_back(array[i]);
                int values = ab.toHex().toInt(&ok, 16);
                Res.append(QString::number(values, 2).rightJustified(16, '0'));
            }
        }
    }
    return  Res;
}
