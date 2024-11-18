#ifndef AOIBUSINESSM201OBJ_H
#define AOIBUSINESSM201OBJ_H

#include <QObject>
#include <tcpcommonuse.h>
#include "dataacquisitiondefines.h"
#include "qmqttclient.h"
class AOIBusinessM201Obj : public QObject
{
    Q_OBJECT
public:
    static AOIBusinessM201Obj* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new AOIBusinessM201Obj();
        return m_pInstance;
    }

    TcpCommonUse* get_tcp_common_use()
    {
        if(m_pTcpCommonUse == nullptr)
            m_pTcpCommonUse = new TcpCommonUse();
        return m_pTcpCommonUse;
    }

    void start_to_data_acquisite();

    void change_production_of_aoi(QString recipename);
    void start_or_stop_equipment();

    void set_real_defect_rate(double rate,int hour)
    {
        m_pRealDefectRate = rate;
        m_pDefectHour = hour;
    }
    void set_standard_defect_rate(double rate)
    {
        m_pStandardDefectRate = rate;
    }
private:
    explicit AOIBusinessM201Obj(QObject *parent = nullptr);

    static AOIBusinessM201Obj* m_pInstance;

    TcpCommonUse* m_pTcpCommonUse;

    void timerEvent(QTimerEvent* event);
    QByteArray generate_common_xml(QString xmlsign);
    QString get_time_stamp();
    int m_pTimerID;
    int m_pAcquisiteTimerID;
    int m_pClockChecker;
    int m_pDefectHour;

    QMqttClient* m_pMqttClient;
    QString m_pCurrentRecipeName;
    double m_pRealDefectRate;
    double m_pStandardDefectRate;

    QMap<DATA_TYPE_M201,QVariant> m_pDefectRateDataTable;

    void parse_xml_file(QString data);

    void process_equipment_status(QString data);
    void process_alarm_info(QString data);
    void process_pcb_report(QString data);
    void process_change_production_response(QString data);
    void process_edc_report(QString data);
    void process_stop_command(QString data);

signals:
    void signal_notify_change_production_result(const int r);

public slots:
    void process_info_from_tcp_server(const QByteArray& data);

    void updateLogStateChange();
    void brokerDisconnected();
};

#endif // AOIBUSINESSM201OBJ_H
