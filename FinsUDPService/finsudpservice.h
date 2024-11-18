#ifndef FINSUDPSERVICE_H
#define FINSUDPSERVICE_H

#include "finsudpservice_global.h"
#include <QObject>
#include <QUdpSocket>
#include <QMutex>
class FINSUDPSERVICESHARED_EXPORT FinsUDPService:public QObject
{
    Q_OBJECT
public:
    FinsUDPService();
    void send_message_to_plc(const uchar function_code,
                             const short data_address,
                             const short data_length/*length is word length*/,
                             const QByteArray data_array);
    void send_message_to_plc(const uchar function_code,
                             const uchar section_code,//B1:WÇø 82:DÇø
                               const short data_address,
                               const short data_length/*length is word length*/,
                               const QByteArray data_array);

    void init_ip_port(const QString remoteip,const int remotport);
private:
    QUdpSocket *m_pUdpSocket;
    QByteArray make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
    QByteArray make_plc_message(const uchar function_code, const uchar section_code,const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
    QMutex m_mutex;
    QString m_pRemoteIP;
    int m_pRemotePort;
    bool m_pInitSign;
    quint8 process_ip_string(const QString ipstr);
    quint8 get_last_ip(const int id);
    QString bytes_to_str(QByteArray data);
signals:
    void signal_proc_plc_msg(const QByteArray data_array);
    void signal_proc_real_data(QByteArray data);
    void signal_finish_washing(const int washtimes);
public slots:
    void slot_receive_message();
    void slot_proc_plc_msg(const QByteArray data_array);
};

#endif // FINSUDPSERVICE_H
