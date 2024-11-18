#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QDataStream>
class TcpClient : public QObject
{
    Q_OBJECT
public:
//    friend class GDataFactory;
    explicit TcpClient();
    bool connect_to_tcp_server(QString sIp,int sPort);

    void send_to_tcp_server(QByteArray data);
    void disconnect_from_server();
    void set_identity_id(int id){IdentityID = id;}
private:
    int IdentityID;
private:
//    explicit TcpClient();

//    static TcpClient* _instance;
//    static TcpClient* get_instance()
//    {
//        if(_instance == nullptr)
//            _instance = new TcpClient();
//        return _instance;
//    }

//    QTcpSocket* m_pTcpSocketClient;
    QTcpSocket* m_pTcpSocketClient;
signals:
    void signal_rev_result(int id);
public slots:
    void slot_rev_from_tcp_server();

};

#endif // TCPCLIENT_H
