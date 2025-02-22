﻿#include "datacommserial.h"
#include <QDebug>
#include <QDateTime>
#include "QsLog.h"
DataCommSerial::DataCommSerial(QObject *parent) : QThread(parent)
{
    m_pResponseValidate = nullptr;
    m_ttl_interval_ms = 1000;
    m_wait_timeout = 5000;
}

void DataCommSerial::init(const tagSerialPortInfo& info)
{
    m_serialport_info = info;
}

void DataCommSerial::send_time_to_live(int interval_time_ms, int timeout_ms)
{
    m_serial_work_mode = serial_send_time_to_live;
    m_ttl_interval_ms = interval_time_ms;
    m_wait_timeout = timeout_ms;
}

void DataCommSerial::sendRequest_only(const QList<QByteArray>& datas, int timeout_ms)
{
    m_serial_work_mode = serial_send_request_only;
    m_request_datas = datas;
    m_wait_timeout = timeout_ms;
}

void DataCommSerial::sendRequest_and_recvResponse(const QList<QByteArray>& datas, int timeout_ms)
{
    m_serial_work_mode = serial_send_request_recv_response;
    m_request_datas = datas;
    m_wait_timeout = timeout_ms;
}

void DataCommSerial::sendRequest_and_checkResponse(const QList<QByteArray>& datas, int timeout_ms, IResponseValidate* pValidate)
{
    m_serial_work_mode = serial_send_request_check_response;
    m_request_datas = datas;
    m_wait_timeout = timeout_ms;
    m_pResponseValidate = pValidate;
}

void DataCommSerial::recvResponse_only()
{
    m_serial_work_mode = serial_recv_response_only;
}

void DataCommSerial::run()
{
    tagSerialPortInfo& cfg = m_serialport_info;
    serialport.setPortName(cfg.portName);
    if(!serialport.setBaudRate(cfg.baudRate, cfg.directions)) return;
    if(!serialport.setDataBits(cfg.dataBits)) return;
    if(!serialport.setFlowControl(cfg.flowControl)) return;
    if(!serialport.setParity(cfg.parity)) return;
    if(!serialport.setStopBits(cfg.stopBits)) return;
    if(!serialport.open(QIODevice::ReadWrite))
    {
        emit sigCatchException("serialport open failture" + serialport.errorString());
//        qDebug() << "serialport open failture" << serialport.errorString();
        QLOG_ERROR()<<"serialport open failture"<<serialport.errorString();
        return;
    }


    if(serial_send_time_to_live==m_serial_work_mode)
    {
        while (true)
        {
            QString ttl = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
            serialport.write(ttl.toLocal8Bit());
            if(!serialport.waitForBytesWritten(m_wait_timeout))
            {
                emit sigCatchException("waitForBytesWritten timeout");
                qDebug() << "waitForBytesWritten timeout" << ttl;
            }

            msleep(static_cast<unsigned long>(m_ttl_interval_ms));
        }
    }
    else if(serial_send_request_only==m_serial_work_mode)
    {
        foreach(QByteArray request_data, m_request_datas)
        {
            serialport.write(request_data);
            if(!serialport.waitForBytesWritten(m_wait_timeout))
            {
                emit sigCatchException("waitForBytesWritten timeout");
                qDebug() << "waitForBytesWritten timeout";
                return;
            }
        }
    }
    else if(serial_send_request_recv_response==m_serial_work_mode)
    {
        foreach(QByteArray request_data, m_request_datas)
        {
            serialport.write(request_data);
            QLOG_INFO() << "send bytes to mcu";
            if(serialport.waitForBytesWritten(m_wait_timeout))
            {
                QLOG_INFO() << "write bytes to mcu";
                if(serialport.waitForReadyRead(m_wait_timeout))
                {
                    QByteArray response_data = serialport.readAll();
                    while(serialport.waitForReadyRead(100))
                    {
                        response_data += serialport.readAll();
                    }

                    QLOG_INFO() << response_data;
                    emit sigRecvResponse(response_data);
                }
                else
                {
                    emit sigCatchException("serial_send_request_recv_response waitForReadyRead timeout");
                    qDebug() << "serial_send_request_recv_response waitForReadyRead timeout";
                    return;
                }
            }
            else
            {
                emit sigCatchException("waitForBytesWritten timeout");
                qDebug() << "waitForBytesWritten timeout";
                return;
            }
        }
    }
    else if(serial_send_request_check_response==m_serial_work_mode)
    {
        foreach(QByteArray request_data, m_request_datas)
        {
            qint64 writeR=0;
            writeR = serialport.write(request_data);
//            qDebug()<<QString("write result is:%1").arg(writeR);
            if(serialport.waitForBytesWritten(m_wait_timeout))
            {
                if(serialport.waitForReadyRead(m_wait_timeout))
                {
                    QByteArray response_data = serialport.readAll();
                    while(serialport.waitForReadyRead(50))
                    {
                        response_data += serialport.readAll();
                    }

                    if(m_pResponseValidate)
                    {
                        bool bValid = m_pResponseValidate->validate(response_data);
//                        qDebug() << bValid << response_data.toHex();
//                        qDebug() << bValid << response_data;
                        emit sigCheckResponse(bValid, response_data);
                    }
                    else
                    {
                        emit sigCatchException("m_pResponseValidate invalid");
                        qDebug() << "m_pResponseValidate invalid" << m_pResponseValidate;
                        return;
                    }
                }
                else
                {
                    emit sigCatchException("waitForReadyRead timeout");
                    qDebug() << "waitForReadyRead timeout";
                    return;
                }
            }
            else
            {
                emit sigCatchException("waitForBytesWritten timeout");
                qDebug() << "waitForBytesWritten timeout";
                return;
            }
        }
    }
    else if(serial_recv_response_only==m_serial_work_mode)
    {
        while (true)
        {
            QByteArray response_data = serialport.readAll();

            while (serialport.waitForReadyRead(50))
            {
                response_data += serialport.readAll();
            }

            if(!response_data.isEmpty())
            {
                emit sigRecvResponse(response_data);
//                qDebug() << response_data;
            }
        }
    }
}
