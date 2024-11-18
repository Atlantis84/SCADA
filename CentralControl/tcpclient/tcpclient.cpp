#include "tcpclient.h"
#include <QDebug>
#include "QsLog.h"
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QTimerEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "gdatafactory.h"
#include <QTextCodec>
#include <QThread>
#include <QCoreApplication>
TcpClient::TcpClient()
{
    m_pTcpSocketClient = nullptr;
    IdentityID = -1;
}

bool TcpClient::connect_to_tcp_server(QString sIp, int sPort)
{
    if(m_pTcpSocketClient == nullptr)
        m_pTcpSocketClient = new QTcpSocket(this);
    if(m_pTcpSocketClient->state() != QAbstractSocket::UnconnectedState)
    {
        QLOG_ERROR()<<"the connect state is not UnconnectedState, cannot connect to server!";
        return false;
    }

    m_pTcpSocketClient->connectToHost(sIp,sPort);
    if(m_pTcpSocketClient->waitForConnected(1000))
    {
        connect(m_pTcpSocketClient,&QTcpSocket::readyRead,this,&TcpClient::slot_rev_from_tcp_server);
        QLOG_WARN()<<u8"connect to tcp server SUCCESS!";
        return true;
    }
    else {
        QLOG_ERROR()<<u8"connect to tcp server FAILED!";
        return false;
    }
}

void TcpClient::disconnect_from_server()
{
    if(m_pTcpSocketClient != nullptr)
        m_pTcpSocketClient->disconnectFromHost();
    else
        return;
    QThread::msleep(300);
    QLOG_WARN()<<m_pTcpSocketClient->state();
    if(m_pTcpSocketClient->state() == QAbstractSocket::UnconnectedState)
    {
        disconnect(m_pTcpSocketClient,&QTcpSocket::readyRead,this,&TcpClient::slot_rev_from_tcp_server);
        QLOG_INFO()<<"the client disconnect from server SUCCESS!";
    }
    else
        QLOG_ERROR()<<"the client disconnect from server FAILED!";
}

void TcpClient::send_to_tcp_server(QByteArray data)
{
//    QJsonObject  root;
//    root.insert("ProductSn",QString(data));

//    QJsonDocument doc_data(root);
//    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<u8"the sending JSON info is:"<<QString(data);

    m_pTcpSocketClient->write(data,data.length());
    m_pTcpSocketClient->flush();
}

void TcpClient::slot_rev_from_tcp_server()
{
    QJsonParseError parseJsonErr;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_pTcpSocketClient->readAll(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
        QLOG_WARN()<<"parse json failed!";
    else
    {
        QJsonObject jsonObject = jsonDocument.object();
        if(this->IdentityID == 1)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"Antenna_Test station1 change production SUCCESS!";
                    emit signal_rev_result(1);
                }
                else
                    QLOG_WARN()<<"Antenna_Test station1 change production FAILED!";
            }

            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station1 change production SUCCESS!";
                    emit signal_rev_result(1);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station1 change production FAILED!";
            }
        }

        if(this->IdentityID == 2)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"Antenna_Test station2 change production SUCCESS!";
                    emit signal_rev_result(2);
                }
                else
                    QLOG_WARN()<<"Antenna_Test station2 change production FAILED!";
            }

            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station2 change production SUCCESS!";
                    emit signal_rev_result(2);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station2 change production FAILED!";
            }
        }

        if(this->IdentityID == 3)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"Antenna_Test station3 change production SUCCESS!";
                    emit signal_rev_result(3);
                }
                else
                    QLOG_WARN()<<"Antenna_Test station3 change production FAILED!";
            }

            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station3 change production SUCCESS!";
                    emit signal_rev_result(3);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station2 change production FAILED!";
            }
        }

        if(this->IdentityID == 4)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"Antenna_Test station4 change production SUCCESS!";
                    emit signal_rev_result(4);
                }
                else
                    QLOG_WARN()<<"Antenna_Test station4 change production FAILED!";
            }

            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station4 change production SUCCESS!";
                    emit signal_rev_result(4);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station4 change production FAILED!";
            }
        }

        if(this->IdentityID == 5)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station5 change production SUCCESS!";
                    emit signal_rev_result(5);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station5 change production FAILED!";
            }
        }

        if(this->IdentityID == 6)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station6 change production SUCCESS!";
                    emit signal_rev_result(6);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station6 change production FAILED!";
            }
        }

        if(this->IdentityID == 7)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station7 change production SUCCESS!";
                    emit signal_rev_result(7);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station7 change production FAILED!";
            }
        }

        if(this->IdentityID == 8)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_PCBA")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<"PCBA_Test station8 change production SUCCESS!";
                    emit signal_rev_result(8);
                }
                else
                    QLOG_WARN()<<"PCBA_Test station8 change production FAILED!";
            }
        }
    }
}
