#ifndef TCONFINSUDPPROCESSTESTSTATION_H
#define TCONFINSUDPPROCESSTESTSTATION_H

#include <QObject>
#include "finsudpprocessbase.h"
#include "dataacquisitiondefines.h"
#include <QMap>
#include "mqservice.h"
class TconFinsUDPProcessTestStation : public FinsUDPProcessBase
{
    Q_OBJECT
public:
    static TconFinsUDPProcessTestStation* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new TconFinsUDPProcessTestStation();
        return m_pObj;
    }
    void insert_data_to_db();
    void load_data_from_db();
    void process_warnings_data(QByteArray data);

    void start_mq_service();
    void stop_mq_service();
private:
    TconFinsUDPProcessTestStation();

    MQService* m_pMqSerViceSrcA;
    MQService* m_pMqSerViceSrcB;

    static TconFinsUDPProcessTestStation* m_pObj;

    int m_pTimerID;

    void timerEvent(QTimerEvent *event);
    QMap<int,QString> m_pTconTestStationDataTableA;
    QMap<int,QString> m_pTconTestStationDataTableB;

    QMap<int,QString> m_pMapWarningsInfoOfDB;
    void process_ok_ng_data(QByteArray data);
    void process_equipment_state(QByteArray data);
signals:
    void signal_warnings(QList<QString> lstInfo);
    void signal_ok_ng(QList<QString> lstInfo,int machineID);
    void signal_equipment_state(QByteArray data);

public slots:
    virtual void process_plc_data(QByteArray data);
};

#endif // TCONFINSUDPPROCESSTESTSTATION_H
