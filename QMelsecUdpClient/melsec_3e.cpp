#include "melsec_3e.h"
#include <QString>
#include <QDebug>
#include "QsLog.h"
#include <QEventLoop>
#include <QTimer>
#include <QObject>

Melsec_3E::Melsec_3E()
{

}

int Melsec_Comm_3E_ASCII::CPUtype(QTcpSocket *socket, uint32_t timeout, QString &cpu_string, uint16_t &cpu_code)
{
    CommandFrame_0101_ASCII frame;
    QByteArray cmd = frame.build();
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();

    ResponseFrame_MC_3E_ASCII resp(data);
    uint16_t ret = resp.errorCode();
    if(ret == 0)
    {
        QByteArray text = resp.responseData();
        cpu_string = text.mid(0, 16);
        cpu_string.trimmed();

        QString num(text.mid(16, 2)) ;
        cpu_code = num.toUInt(nullptr, 16);
    }
    return ret;
}

int Melsec_Comm_3E_ASCII::run(QTcpSocket *socket, uint32_t timeout, int mode, int clear_mode)
{
    CommandFrame_1001_ASCII frame;
    QByteArray cmd = frame.build(mode, clear_mode);
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);
    return resp.errorCode();
}

int Melsec_Comm_3E_ASCII::stop(QTcpSocket *socket, uint32_t timeout, int mode)
{
    CommandFrame_1002_ASCII frame;
    QByteArray cmd = frame.build(mode);
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);
    return resp.errorCode();
}

int Melsec_Comm_3E_ASCII::reset(QTcpSocket *socket, uint32_t timeout, int mode)
{
    CommandFrame_1006_ASCII frame;
    QByteArray cmd = frame.build(mode);
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);
    return resp.errorCode();
}

int Melsec_Comm_3E_ASCII::pause(QTcpSocket *socket, uint32_t timeout, int mode)
{
    CommandFrame_1003_ASCII frame;
    QByteArray cmd = frame.build(mode);
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);
    return resp.errorCode();
}

int Melsec_Comm_3E_ASCII::loopback(QTcpSocket *socket, uint32_t timeout, const QByteArray &in_data, QByteArray &out_data)
{
    CommandFrame_0619_ASCII frame;
    QByteArray cmd = frame.build(in_data);
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);
    uint16_t ret = resp.errorCode();
    if(ret) return ret;

    out_data = resp.responseData().mid(4);
    return ret;
}

QBitArray Melsec_Comm_3E_ASCII::readBits(QTcpSocket *socket, uint32_t timeout, char code[], uint32_t addr, uint16_t N)
{
    CommandFrame_0401_ASCII frame;
    QByteArray cmd = frame.readBits(code, addr, N);
    socket->write(cmd);
    socket->waitForReadyRead(timeout);
    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);

    QBitArray array;
    uint16_t ret = resp.errorCode();
    if(ret) return array;

    data = resp.responseData().mid(4);

    if(data.size() != N) return array;

    array.resize(N);
    for(int i = 0; i < N; i++)
    {
        if( data.at(i) == '1') array.setBit(i);
    }
    return array;
}

QByteArray Melsec_Comm_3E_ASCII::readBytes(QTcpSocket *socket, uint32_t timeout, uint16_t code, uint8_t addr, uint16_t N)
{
    CommandFrame_0401_ASCII frame;
    QByteArray cmd = frame.readBytes(code, addr, N);
    qDebug() << "send cmd" <<cmd;
    socket->write(cmd);
    //socket->flush();
    QEventLoop eventloop;
    QObject::connect(socket,SIGNAL(readyRead()),&eventloop,SLOT(quit()));
    QTimer::singleShot(timeout,&eventloop,&QEventLoop::quit);
    eventloop.exec();

//    bool isSendStated = socket->waitForReadyRead(timeout);
    QByteArray array;

    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();

    if(data.isEmpty()){
        array.clear();
        array.append(static_cast<char>(0x00));
        QLOG_ERROR() << "Melsec timeout connect error!";
        return array;
    }
    ResponseFrame_MC_3E_ASCII resp(data);

    qDebug() << data;
    uint16_t ret = resp.errorCode();
    if(ret) return array;

    //data = resp.responseData();
    if(data.size() != 11 + 2 * N) return array;
    array.resize(2*N);

    int nFindSize = data.size() - 2*N;

    array = data.mid(nFindSize, 2*N);

    for(int i = 0; i < 2*N; i++)
    {
         array[i] = data[nFindSize + i];
    }
    return array;
}

QByteArray Melsec_Comm_3E_ASCII::writeBytes(QTcpSocket *socket, uint32_t timeout, uint16_t code, uint8_t addr, uint16_t N, uint16_t values[])
{
    CommandFrame_0401_ASCII frame;
    QByteArray cmd = frame.writeBytes(code, addr, N, values);
    qDebug() << "send cmd" <<cmd;
    socket->write(cmd);
    //socket->flush();


//    bool isSendStated = socket->waitForReadyRead(timeout);
    QByteArray array;
//    if(!isSendStated){
//        array.clear();
//        array.append(static_cast<char>(0x00));
//        qDebug() << "timeout connect error!";
//        return array;
//    }

    //接收状态，防止读取的时候出现该状态
    QByteArray data = socket->readAll();
    ResponseFrame_MC_3E_ASCII resp(data);

    qDebug() << data;
    uint16_t ret = resp.errorCode();
    if(ret) return array;
    array.clear();
    array.append(static_cast<char>(0x01));
    return array;
}

ResponseFrame_MC_3E_ASCII::ResponseFrame_MC_3E_ASCII(QByteArray frame)
{
    m_data = frame;
}

bool ResponseFrame_MC_3E_ASCII::parser(QByteArray frame)
{
    m_data = frame;
    return false;
}

uint16_t ResponseFrame_MC_3E_ASCII::errorCode()
{ 
    return m_data.mid(9,2).toUShort();//读取故障报警代码
}

QByteArray ResponseFrame_MC_3E_ASCII::responseData()
{
    return m_data.mid(22);
}

QByteArray ResponseFrame_MC_3E_ASCII::errorData()
{
    return m_data.mid(22);
}

QByteArray ErrorData_MC_3E_ASCII::netNumber()
{
    return m_data.mid(0, 2);
}

QByteArray ErrorData_MC_3E_ASCII::PLCNumber()
{
    return m_data.mid(2, 2);
}

QByteArray ErrorData_MC_3E_ASCII::IONumber()
{
    return m_data.mid(4, 4);
}

QByteArray ErrorData_MC_3E_ASCII::command()
{
    return m_data.mid(10, 4);
}

QByteArray ErrorData_MC_3E_ASCII::subCommand()
{
    return m_data.mid(14, 4);
}

QByteArray CommandFrame_1401_ASCII::build(char code[2], uint32_t addr, QBitArray array)
{
    QByteArray text;
    text.append("0010"); // CPU 监视定时器
    text.append("1401"); // 命令
    text.append("0001"); // 子命令
    text.append(code);
    text.append(address(addr));
    uint16_t N = array.size();
    text.append(uint16ToHex(N));
    for(uint16_t i = 0; i < N; i++)
    {
        if(array.at(i))
        {
            text.push_back('1');
        }
        else
        {
            text.push_back('0');
        }
    }

    QByteArray length = textLength(text);

    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append(length);
    m_data.append(text);
    return m_data;
}

QByteArray CommandFrame_1401_ASCII::build(char code[], uint32_t addr, QVector<uint16_t> array)
{
    QByteArray text;
    text.append("0010"); // CPU 监视定时器
    text.append("1401");
    text.append("0000");
    text.append(code);
    text.append(address(addr));
    uint16_t N = array.size();
    text.append(uint16ToHex(N)); //软元件的点数
    for(uint16_t i = 0; i < N; i++)
    {
        uint16_t x = array.at(i);
        text.push_back(uint16ToHex(x));
    }
    QByteArray length = textLength(text);
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append(length);
    m_data.append(text);
    return m_data;
}

QByteArray CommandFrame_1401_ASCII::bitToArray(QBitArray array)
{
    QByteArray data;
    int N = array.size();
    for(int i = 0; i < N; i++)
    {
        if(array.at(i))
        {
            data.push_back('1');
        }
        else
        {
            data.push_back('0');
        }
    }
    return data;
}


QByteArray CommandFrame_MC_3E_ASCII::textLength(QByteArray text)
{
    uint16_t N = text.length()/2;
    return uint16ToHex(N);
}

QByteArray CommandFrame_MC_3E_ASCII::address(uint32_t addr)
{
    return uint24ToHex(addr);
}

QByteArray CommandFrame_MC_3E_ASCII::uint16ToHex(uint16_t input)
{
    QString data = QString("%1").sprintf("%04X",(input & 0xff) <<8 | input >> 8);
    return QByteArray(data.toLatin1());
}

QByteArray CommandFrame_MC_3E_ASCII::uint24ToHex(uint32_t x)
{
    QString strNumD = QString("%1").arg(x, 6, 16, QLatin1Char('0'));
    strNumD = strNumD.toUpper();
    return QByteArray(strNumD.toLatin1());
}

QByteArray CommandFrame_MC_3E_ASCII::uint32ToHex(uint32_t x)
{
    QString strNumD = QString("%1").arg(x, 8, 16, QLatin1Char('0'));
    strNumD = strNumD.toUpper();
    return QByteArray(strNumD.toLatin1());
}

QByteArray CommandFrame_MC_3E_ASCII::uint48ToHex(uint64_t x)
{
    QString strNumD = QString("%1").arg(x, 12, 16, QLatin1Char('0'));
    strNumD = strNumD.toUpper();
    return QByteArray(strNumD.toLatin1());
}

QByteArray CommandFrame_MC_3E_ASCII::uint64ToHex(uint64_t x)
{
    QString strNumD = QString("%1").arg(x, 16, 16, QLatin1Char('0'));
    strNumD = strNumD.toUpper();
    return QByteArray(strNumD.toLatin1());
}

QByteArray CommandFrame_1001_ASCII::build(int mode, int clear_mode)
{
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append("0014"); // 长度 20
    m_data.append("0010"); // CPU 监视定时器
    m_data.append("1001"); // 命令
    m_data.append("0000"); // 子命令
    if(mode == 1)
    {
        m_data.append("0001"); //不强制执行
    }
    else
    {
        m_data.append("0003"); //强制执行
    }
    switch (clear_mode)
    {
    case 0:
        m_data.append("00"); //清空模式,不清空软元件存储器
        break;
    case 1:
        m_data.append("01"); //清空模式,清空锁存范围外的软元件存储器
        break;
    case 2:
        m_data.append("02"); //清空模式,清空包括锁存范围在内的全部软元件存储器
        break;
    default:
        m_data.append("00"); //清空模式
        break;
    }
    m_data.append("00");
    return m_data;
}

QByteArray CommandFrame_1002_ASCII::build(int mode)
{
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append("000B"); // 长度 12
    m_data.append("0010"); // CPU 监视定时器
    m_data.append("1002"); // 命令
    m_data.append("0000"); // 子命令
    if(mode == 1)
    {
        m_data.append("0001"); //不强制执行
    }
    else
    {
        m_data.append("0003"); //强制执行
    }
    return m_data;
}

QByteArray CommandFrame_1003_ASCII::build(int mode)
{
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append("000B"); // 长度 12
    m_data.append("0010"); // CPU 监视定时器
    m_data.append("1003"); // 命令
    m_data.append("0000"); // 子命令
    if(mode == 1)
    {
        m_data.append("0001"); //不强制执行
    }
    else
    {
        m_data.append("0003"); //强制执行
    }
    return m_data;
}

QByteArray CommandFrame_1006_ASCII::build(int mode)
{
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append("000B"); // 长度 12
    m_data.append("0010"); // CPU 监视定时器
    m_data.append("1006"); // 命令
    m_data.append("0000"); // 子命令
    if(mode == 1)
    {
        m_data.append("0001"); //不强制执行
    }
    else
    {
        m_data.append("0003"); //强制执行
    }
    return m_data;
}

QByteArray CommandFrame_1005_ASCII::build(int mode)
{
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append("000B"); // 长度 12
    m_data.append("0010"); // CPU 监视定时器
    m_data.append("1005"); // 命令
    m_data.append("0000"); // 子命令
    if(mode == 1)
    {
        m_data.append("0001"); //不强制执行
    }
    else
    {
        m_data.append("0003"); //强制执行
    }
    return m_data;
}

QByteArray CommandFrame_0101_ASCII::build()
{
    m_data.clear();
    m_data.append("500000FFFF0300");
    m_data.append("0800"); // 长度 8
    m_data.append("1000"); // CPU 监视定时器
    m_data.append("0101"); // 命令
    m_data.append("0000"); // 子命令
    return m_data;
}

QByteArray CommandFrame_0619_ASCII::build(QByteArray t)
{
    QByteArray text;
    text.append("0010"); // CPU 监视定时器
    text.append("0619");
    text.append("0000");
    uint16_t N = t.size();
    text.append(uint16ToHex(N)); //软元件的点数
    text.append(t);

    QByteArray length = textLength(text);
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append(length);
    m_data.append(text);
    return m_data;
}

QByteArray CommandFrame_0401_ASCII::readBits(char code[], uint32_t addr, uint16_t N)
{
    QByteArray text;
    text.append("0010"); // CPU 监视定时器
    text.append("0401"); // 命令
    text.append("0001"); // 子命令
    text.append(code);//起始软元件
    text.append(address(addr));
    text.append(uint16ToHex(N)); //软元件的点数

    QByteArray length = textLength(text);
    m_data.clear();
    m_data.append("500000FF03FF00");
    m_data.append(length);
    m_data.append(text);
    return m_data;
}

QByteArray CommandFrame_0401_ASCII::readBytes(uint16_t code, uint8_t addr, uint16_t N)
{
    QString text;
    text.append("1000"); // CPU监视定时器
    text.append("0104"); // 命令
    text.append("0000"); // 子命令
    text.append(uint16ToHex(code)+"00");
    text.append(QString("%1").sprintf("%02X",addr&0xff));
    text.append(uint16ToHex(N));//软元件的点数

    QString length = QString(uint16ToHex(text.length()/2));
    m_data.clear();
    m_data.append("500000FFFF0300");
    m_data.append(length);
    m_data.append(text);

    QByteArray tmpAarray;
    QString tmp;
    bool ok;

    for(int i=0;i<m_data.length();i++){
        if(i%2==0 && i < m_data.length()-1){
            tmp.insert(0,m_data.at(i));
            tmp.insert(1,m_data.at(i+1));
            tmpAarray.append(static_cast<char>(tmp.toInt(&ok, 16)));
            tmp.clear();
        }
    }
    return tmpAarray;
}

QByteArray CommandFrame_0401_ASCII::writeBytes(uint16_t code, uint8_t addr, uint16_t N, uint16_t values[])
{
    QString text;
    text.append("1000"); // CPU监视定时器
    text.append("0114"); // 命令
    text.append("0000"); // 子命令
    text.append(uint16ToHex(code)+"00");
    text.append(QString("%1").sprintf("%02X",addr&0xff));
    text.append(uint16ToHex(N));//软元件的点数

    for(int i = 0;i < N; i++){
        text.append(uint16ToHex(values[i]));
    }

    QString length = QString(uint16ToHex(text.length()/2));
    m_data.clear();
    m_data.append("500000FFFF0300");
    m_data.append(length);
    m_data.append(text);

    QByteArray tmpAarray;
    QString tmp;
    bool ok;

    for(int i=0;i<m_data.length();i++){
        if(i%2==0 && i < m_data.length()-1){
            tmp.insert(0,m_data.at(i));
            tmp.insert(1,m_data.at(i+1));
            tmpAarray.append(static_cast<char>(tmp.toInt(&ok, 16)));
            tmp.clear();
        }
    }
    return tmpAarray;
}
