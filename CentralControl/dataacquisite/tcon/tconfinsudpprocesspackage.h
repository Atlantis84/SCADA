#ifndef TCONFINSUDPPROCESSPACKAGE_H
#define TCONFINSUDPPROCESSPACKAGE_H

#include <QObject>
#include "finsudpprocessbase.h"
#include "dataacquisitiondefines.h"
#include <QMap>
#include <mqservice.h>

class TconFinsUDPProcessPackage : public FinsUDPProcessBase
{
    Q_OBJECT
public:
    static TconFinsUDPProcessPackage* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new TconFinsUDPProcessPackage();
        return m_pObj;
    }

    void insert_data_to_db();
    void load_data_from_db();
    void process_warnings_data(QByteArray data);
    void process_equipment_state(QByteArray data);
    void stop_mq_service();
    void start_mq_service();
private:
    TconFinsUDPProcessPackage();

    static TconFinsUDPProcessPackage* m_pObj;
    int m_pTimerID;

    void timerEvent(QTimerEvent *event);
    QMap<int,QString> m_pTconPackageDataTable;
    QMap<int,QString> m_pMapWarningsInfoOfDB;

    MQService* m_pMqSerViceSrc;
signals:
    void signal_warnings(QList<QString> lstInfo);
    void signal_equipment_state(QByteArray data);

public slots:
    virtual void process_plc_data(QByteArray data);
};

#endif // TCONFINSUDPPROCESSPACKAGE_H
