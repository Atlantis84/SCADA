#include "tcpcommonuse.h"
#include "QsLog.h"
#include <QEventLoop>
#include <QTimerEvent>
using namespace QsLogging;
TcpCommonUse::TcpCommonUse():m_pTimerID(-9999),m_pTcpServer(new QTcpServer),
    m_pTcpClient(new QTcpSocket),m_pServerIp("127.0.0.0"),m_pServerPort(8080)
{
    connect(m_pTcpClient,&QTcpSocket::stateChanged,this,&TcpCommonUse::slot_state_changed);
}

TcpCommonUse::~TcpCommonUse()
{
}

void TcpCommonUse::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        if(m_pTcpClient->state() == QAbstractSocket::UnconnectedState)
        {
            disconnect(m_pTcpClient,&QTcpSocket::readyRead,this,&TcpCommonUse::slot_rev_from_tcp_server);
            if(connect_to_tcp_server(m_pServerIp,m_pServerPort))
            {
                if(m_pTimerID != -1)
                    killTimer(m_pTimerID);
                QLOG_INFO()<<"re-connect to server SUCCESS!";
            }
            else
            {
                QLOG_WARN()<<"re-connect to server FAILED!";
            }
        }
    }
}

//server
bool TcpCommonUse::start_tcp_server(int port)
{
    if(m_pTcpServer->listen(QHostAddress::Any, port))
    {
        QLOG_INFO()<<"start tcp server SUCCESS!";
        connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(slot_new_connection()));
        return true;
    }
    else {
        QLOG_ERROR()<<"start tcp server FAILED!";
        return false;
    }
}

qint64 TcpCommonUse::send_data_to_client(QByteArray data)
{
    QLOG_INFO()<<"send data to client,the data is:"<<data;
    qint64 r = m_pServerSocket->write(data,data.length());
    m_pServerSocket->flush();
    return r;
}

void TcpCommonUse::slot_new_connection()
{
    QLOG_INFO()<<"income new tcp connection!";
    m_pServerSocket = m_pTcpServer->nextPendingConnection();
    connect(m_pServerSocket,SIGNAL(readyRead()),this,SLOT(slot_rev_msg_from_client()));
    connect(m_pServerSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(slot_disconnection_from_server(QAbstractSocket::SocketError)),Qt::DirectConnection);
}

void TcpCommonUse::slot_disconnection_from_server(QAbstractSocket::SocketError error)
{
//    int error = m_pServerSocket->error();
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
    {
        QString hostAddr = m_pServerSocket->QAbstractSocket::peerAddress().toString();
        QLOG_WARN()<<u8"the client "<<hostAddr<<u8" disconnect from the server!";
    }
        break;
    default:
        break;
    }
}

void TcpCommonUse::slot_rev_msg_from_client()
{
    QByteArray m_ReadBytes = m_pServerSocket->readAll();
    QLOG_INFO()<<"rev msg from client,the data is:"<<m_ReadBytes;
    emit signal_proc_client_info(m_ReadBytes);
    m_pServerSocket->flush();
}

//client
bool TcpCommonUse::connect_to_tcp_server(QString sIp, int sPort)
{
    m_pServerIp = sIp;
    m_pServerPort = sPort;

    if(m_pTcpClient == nullptr)
        return false;

    if(m_pTcpClient->state() != QAbstractSocket::UnconnectedState)
    {
        QLOG_ERROR()<<"the connect state is not UnconnectedState, cannot connect to server!";
        return false;
    }

    m_pTcpClient->connectToHost(sIp,sPort);
    if(m_pTcpClient->waitForConnected(100))
    {
        connect(m_pTcpClient,&QTcpSocket::readyRead,this,&TcpCommonUse::slot_rev_from_tcp_server);
        QLOG_WARN()<<u8"connect to tcp server SUCCESS!";
        return true;
    }
    else {
        QLOG_ERROR()<<u8"connect to tcp server FAILED!";
        return false;
    }
}

qint64 TcpCommonUse::send_data_to_server(QByteArray data)
{
    QLOG_INFO()<<"send data to server,the data is:"<<data;
    qint64 r = m_pTcpClient->write(data,data.length());
    m_pTcpClient->flush();
    return r;
}

static bool disconnectByManual = false;
void TcpCommonUse::disconnect_from_server()
{
    if(m_pTimerID != -1)
    {
        killTimer(m_pTimerID);
        m_pTimerID = -1;
    }
    disconnectByManual = true;
    m_pTcpClient->disconnectFromHost();
    QEventLoop eventloop;
    QTimer::singleShot(300,&eventloop,&QEventLoop::quit);
    QLOG_WARN()<<m_pTcpClient->state();
    if(m_pTcpClient->state() == QAbstractSocket::UnconnectedState)
    {
        disconnect(m_pTcpClient,&QTcpSocket::readyRead,this,&TcpCommonUse::slot_rev_from_tcp_server);
        QLOG_INFO()<<"the client disconnect from server SUCCESS!";
    }
    else
        QLOG_ERROR()<<"the client disconnect from server FAILED!";
}

QAbstractSocket::SocketState TcpCommonUse::get_client_state()
{
    return m_pTcpClient->state();
}

void TcpCommonUse::slot_state_changed(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
    {
        QLOG_WARN()<<"the state of client is changed，start timer to reconnect server！";
        //start timer to connect server
        if(disconnectByManual == true)
            disconnectByManual = false;
        else
        {
//            disconnect(m_pTcpClient,&QTcpSocket::readyRead,this,&TcpCommonUse::slot_rev_from_tcp_server);
//            m_pTimerID = startTimer(5000);
        }
    }
}

void TcpCommonUse::slot_rev_from_tcp_server()
{
    QByteArray data = m_pTcpClient->readAll();
    QLOG_INFO()<<"rev msg from tcp server:";
    emit signal_proc_server_info(data);
}
