#ifndef M201UDPBUSINESSOBJ_H
#define M201UDPBUSINESSOBJ_H

#include <QObject>
#include <QUdpSocket>
#include <QMutex>
class M201UdpBusinessObj : public QObject
{
    Q_OBJECT

public:
    static M201UdpBusinessObj* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new M201UdpBusinessObj();
        return m_pInstance;
    }

    void init_udp_server(const quint16 port);
    void change_production_of_divide_machine(const QString ip,const quint16 port,const QString boardType,const QString machineType);
    void change_production_of_glue_machine(const QString ip,const quint16 port,const QString boardType,const QString machineType);
    void change_production_of_vision(const QString ip,const quint16 port,const int taskid);
private:
    explicit M201UdpBusinessObj(QObject *parent = nullptr);

    QUdpSocket *m_pUdpSocket;
    QUdpSocket *m_pUdpSocketToClient;
    QMutex m_mutex;

protected:
    static M201UdpBusinessObj* m_pInstance;

signals:
    void signal_proc_client_info(const QByteArray& data_array);

    void signal_equipment_error_glue(const int errorID);
    void signal_equipment_output_glue(const int output);
    void signal_equipment_state_glue(const int status);

    void signal_equipment_error_devide(const int errorID);
    void signal_equipment_output_devide(const int output);
    void signal_equipment_state_devide(const int status);
    void signal_equipment_paras_devide(const QList<QString> lstinfo);

    void signal_notify_result_of_change_production_for_divide_glue(const int id);//1:divide-machine; 2:glue-machiine

public slots:
    void slot_receive_udp_message_from_client();
    void slot_proc_client_info(const QByteArray& data);
};

#endif // M201UDPBUSINESSOBJ_H
