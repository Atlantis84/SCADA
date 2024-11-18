#ifndef QMELSECUDPCLIENT_H
#define QMELSECUDPCLIENT_H

#include <QObject>
#include "qmelsecudpclient_global.h"
#include <QUdpSocket>
#include <QMutex>

#define RESULT_TRANS_DEC        (0)
#define RESULT_TRANS_BITS       (1)

class QMELSECUDPCLIENTSHARED_EXPORT QMelsecUdpClient:public QObject
{
    Q_OBJECT
public:
    QMelsecUdpClient();

    void init_ip_port(const QString remoteip,const int remotport, const quint16 local_port);

    void send_message_to_plc(uint16_t code, QString addr, uint16_t length);

    QString QByteArrayToValue(QByteArray array, int flags, int WordLength);

public slots:
    void slot_receive_message();
    void slot_proc_plc_msg(const QByteArray data_array);

signals:
    void signal_proc_plc_msg(const QByteArray data_array);
    void signal_proc_real_data(QByteArray data);
    void signal_finish_washing(const int washtimes);

private:
    QString m_pRemoteIP;
    int m_pRemotePort;
    bool m_pInitSign;
    QUdpSocket *m_pUdpSocket;
    QMutex m_mutex;
    int sendDataLength = -1;
    QMap<QString,uint8_t>map_melsec_values;

    QString bytes_to_str(QByteArray data);
};

#endif // QMELSECUDPCLIENT_H
