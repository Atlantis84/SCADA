#include "scannerservice.h"
#include "iniservice.h"

#include <QDateTime>
#include <QThread>
#include <QThreadPool>
#include <QSerialPort>
#include <QSerialPortInfo>

ScannerService::ScannerService()
{
    //禁用QRunnable自动删除
    this->setAutoDelete(false);
}
void ScannerService::clearQueue()
{
    clearQueueLA();
    clearQueueLB();
}
void ScannerService::clearQueueLA()
{
    lastLACode="";
    lastLACodeTime=QDateTime::currentDateTime().addMSecs(IniService::Scanner_MinIntervalTimeMS*-1);
    //LA
    QString laCodesDesc="";
    lastLACodesMutex.lock();
    lastLACodes.clear();
    lastLACodesMutex.unlock();
    //显示
    emit scannerQueueChanged(laCodesDesc,true);
}
void ScannerService::clearQueueLB()
{
    lastLBCode="";
    lastLBCodeTime=QDateTime::currentDateTime().addMSecs(IniService::Scanner_MinIntervalTimeMS*-1);
    //LB
    QString lbCodesDesc="";
    lastLBCodesMutex.lock();
    lastLBCodes.clear();
    lastLBCodesMutex.unlock();
    //显示
    emit scannerQueueChanged(lbCodesDesc,false);
}
void ScannerService::showScannerQueueDesc()
{
    //LA
    QString laCodesDesc="";
    lastLACodesMutex.lock();
    if(lastLACodes.isEmpty()==false)
    {
        foreach(QString code,lastLACodes)
        {
            laCodesDesc.append(code);
            laCodesDesc.append("\n");
        }
    }
    lastLACodesMutex.unlock();
    //LB
    QString lbCodesDesc="";
    lastLBCodesMutex.lock();
    if(lastLBCodes.isEmpty()==false)
    {
        foreach(QString code,lastLBCodes)
        {
            lbCodesDesc.append(code);
            lbCodesDesc.append("\n");
        }
    }
    lastLBCodesMutex.unlock();
    //显示
    emit scannerQueueChanged(laCodesDesc,true);
    emit scannerQueueChanged(lbCodesDesc,false);
}
void ScannerService::run()
{

    //打开串口
    bool laComOpened=false;
    QSerialPort* laCom=0;
    if(IniService::Scanner_LA_Enable)
    {
        laCom=new QSerialPort();
        if(ScannerService::openSerialPort(laCom,//
                                          IniService::Scanner_LA_ComPortName,//
                                          IniService::Scanner_LA_ComBaudRate,//
                                          IniService::Scanner_LA_ComDataBit,//
                                          IniService::Scanner_LA_ComParityBit,//
                                          IniService::Scanner_LA_ComStopBit))
        {
            laComOpened=true;
        }
    }
    bool lbComOpened=false;
    QSerialPort* lbCom=0;
    if(IniService::Scanner_LB_Enable)
    {
        lbCom=new QSerialPort();
        if(ScannerService::openSerialPort(lbCom,//
                                          IniService::Scanner_LB_ComPortName,//
                                          IniService::Scanner_LB_ComBaudRate,//
                                          IniService::Scanner_LB_ComDataBit,//
                                          IniService::Scanner_LB_ComParityBit,//
                                          IniService::Scanner_LB_ComStopBit))
        {
            lbComOpened=true;
        }
    }
    //清空
    clearQueue();
    //循环读取
    stoped=false;
    while (started) {
        //线程等待
        taskSleep(100);
        //读取A线条码
        if(laComOpened&&laCom->waitForReadyRead(300))
        {
            //
            QByteArray byteArray=laCom->readAll();
            QString code=QString::fromUtf8(byteArray);
            code=code.replace("\n","").replace("\r","");
            //
            taskSleep(200);
            //防断帧
            if(laComOpened&&laCom->waitForReadyRead(300))
            {
                byteArray=laCom->readAll();
                code.append(QString::fromUtf8(byteArray));
                code=code.replace("\n","").replace("\r","");
            }
            //解析
            if(code.isEmpty()==false//
                    &&lastLACode!=code)
            {
                lastLACode=code;
                //
                if(lastLACodeTime>QDateTime::currentDateTime())
                {
                    continue;
                }
                lastLACodeTime=QDateTime::currentDateTime().addMSecs(IniService::Scanner_MinIntervalTimeMS);
                //入栈
                lastLACodesMutex.lock();
                lastLACodes.enqueue(code);
                if(lastLACodes.size()>IniService::Scanner_CodeQueue_MaxSize)
                {
                    lastLACodes.dequeue();
                }
                lastLACodesMutex.unlock();
                //新条码回调
                emit scannerGetNewCode(code,true);
                //显示
                showScannerQueueDesc();
                qDebug("LA扫描到条码:%s",qUtf8Printable(code));
            }
        }
        //读取B线条码
        if(lbComOpened&&lbCom->waitForReadyRead(300))
        {
            //
            QByteArray byteArray=lbCom->readAll();
            QString code=QString::fromUtf8(byteArray);
            code=code.replace("\n","").replace("\r","");
            //
            taskSleep(200);
            //防断帧
            if(lbComOpened&&lbCom->waitForReadyRead(300))
            {
                byteArray=lbCom->readAll();
                code.append(QString::fromUtf8(byteArray));
                code=code.replace("\n","").replace("\r","");
            }
            //解析
            if(code.isEmpty()==false//
                    &&lastLBCode!=code)
            {
                lastLBCode=code;
                //
                if(lastLBCodeTime>QDateTime::currentDateTime())
                {
                    continue;
                }
                lastLBCodeTime=QDateTime::currentDateTime().addMSecs(IniService::Scanner_MinIntervalTimeMS);
                //入栈
                lastLBCodesMutex.lock();
                lastLBCodes.enqueue(code);
                if(lastLBCodes.size()>IniService::Scanner_CodeQueue_MaxSize)
                {
                    lastLBCodes.dequeue();
                }
                lastLBCodesMutex.unlock();
                //新条码回调
                emit scannerGetNewCode(code,false);
                //显示
                showScannerQueueDesc();
                qDebug("LB扫描到条码:%s",qUtf8Printable(code));
            }
        }
        //写串口
        //laCom->write("123456789");
        //laCom->flush();
        //laCom->waitForBytesWritten(3000);
    }
    stoped=true;
    //关闭串口
    if(laComOpened)
    {
        closeSerialPort(laCom);
    }
    if(lbComOpened)
    {
        closeSerialPort(lbCom);
    }
}
void ScannerService::taskSleep(int sleepTimeMs)
{
    if(sleepTimeMs<1)
    {
        QThread::msleep(1);
    }
    qint64 outTimeStamp = QDateTime::currentMSecsSinceEpoch()+sleepTimeMs;
    while (outTimeStamp>QDateTime::currentMSecsSinceEpoch()&&started) {
        QThread::msleep(2);
    }
}
void ScannerService::startService()
{
    if(this->stoped==true)
    {
        QThreadPool::globalInstance()->start(this);
    }
    this->started=true;
    this->stoped=false;
}
void ScannerService::stopService()
{
    started=false;
}
bool ScannerService::isStarted()
{
    return started;
}
bool ScannerService::isStoped()
{
    return stoped;
}
bool ScannerService::closeSerialPort(QSerialPort* com)
{
    if(com->isOpen())
    {
        com->clear();
        com->close();
        delete com;
        return true;
    }
    return false;
}
bool ScannerService::openSerialPort(QSerialPort* com,QString portName,int baudRate,int dataBits,QString parityBit,int stopBits)
{
    if(com->isOpen())
    {
        com->clear();
        com->close();
        return false;
    }
    //串口名
    com->setPortName(portName);
    if(com->open(QIODevice::ReadWrite)==false)
    {
        return false;
    }
    //波特率
    com->setBaudRate(baudRate);
    //数据位
    if(dataBits==8)
    {
        com->setDataBits(QSerialPort::Data8);
    }
    else if(dataBits==7)
    {
        com->setDataBits(QSerialPort::Data7);
    }
    else if(dataBits==6)
    {
        com->setDataBits(QSerialPort::Data6);
    }
    else if(dataBits==5)
    {
        com->setDataBits(QSerialPort::Data5);
    }
    //校验位
    if(QString::compare(parityBit,"None")==0)
    {
        com->setParity(QSerialPort::NoParity);
    }
    else if(QString::compare(parityBit,"Even")==0)
    {
        com->setParity(QSerialPort::EvenParity);
    }
    else if(QString::compare(parityBit,"Odd")==0)
    {
        com->setParity(QSerialPort::OddParity);
    }
    else if(QString::compare(parityBit,"Mark")==0)
    {
        com->setParity(QSerialPort::MarkParity);
    }
    else if(QString::compare(parityBit,"Space")==0)
    {
        com->setParity(QSerialPort::SpaceParity);
    }
    //停止位
    if(stopBits==1)
    {
         com->setStopBits(QSerialPort::OneStop);
    }
    else  if(stopBits==1)
    {
         com->setStopBits(QSerialPort::TwoStop);
    }
    //流控
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->clear();
    com->clearError();
    return true;
}
QString ScannerService::getLastLACode()
{
    QString code="";
    //加锁
    lastLACodesMutex.lock();
    if(lastLACodes.size()>0)
    {
        code=lastLACodes.dequeue();
    }
    //解锁
    lastLACodesMutex.unlock();
    //显示
    showScannerQueueDesc();
    qDebug("LA条码出栈:%s",qUtf8Printable(code));
    return code;
}
QString ScannerService::getLastLBCode()
{
    QString code="";
    //加锁
    lastLBCodesMutex.lock();
    if(lastLBCodes.size()>0)
    {
        code=lastLBCodes.dequeue();
    }
    //解锁
    lastLBCodesMutex.unlock();
    //显示
    showScannerQueueDesc();
    qDebug("LB条码出栈:%s",qUtf8Printable(code));
    return code;
}
