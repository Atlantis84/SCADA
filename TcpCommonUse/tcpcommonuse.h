#ifndef TCPCOMMONUSE_H
#define TCPCOMMONUSE_H

#include "tcpcommonuse_global.h"
#include <QObject>
#include <QTimer>
#include <QThread>

class TCPCOMMONUSESHARED_EXPORT TcpCommonUse: public QObject
{
    Q_OBJECT

public:
    TcpCommonUse();
    ~TcpCommonUse();

    //server
    bool start_tcp_server(int port);
    qint64 send_data_to_client(QByteArray data);

    //client
    bool connect_to_tcp_server(QString sIp,int sPort);
    qint64 send_data_to_server(QByteArray data);
    void disconnect_from_server();
    QAbstractSocket::SocketState get_client_state();

private:
    void timerEvent(QTimerEvent *event);
    int m_pTimerID;

    //server
    QTcpServer* m_pTcpServer;

    QTcpSocket* m_pServerSocket;
    //server

    //client
    QTcpSocket* m_pTcpClient;
    QString m_pServerIp;
    int m_pServerPort;
    //client

signals:
    //server
    void signal_proc_client_info(const QByteArray &data);
    //server

    //client
    void signal_proc_server_info(const QByteArray &data);
    //client

private slots:
    //server
    void slot_new_connection();
    void slot_disconnection_from_server(QAbstractSocket::SocketError error);
    void slot_rev_msg_from_client();
    //server

    //client
    void slot_rev_from_tcp_server();
    void slot_state_changed(QAbstractSocket::SocketState state);
    //client
};

#endif // TCPCOMMONUSE_H
