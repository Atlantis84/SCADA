#ifndef TVTESTBUSINESSM201OBJ_H
#define TVTESTBUSINESSM201OBJ_H

#include <QObject>
#include <QJsonObject>
#include "qmqttclient.h"
#include "dataacquisitiondefines.h"
class TvTestBusinessm201Obj : public QObject
{
    Q_OBJECT
public:
    static TvTestBusinessm201Obj* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new TvTestBusinessm201Obj();
        return m_pInstance;
    }

    void set_tv_pass_rate_para(double passrate);

protected:
    void timerEvent(QTimerEvent* event);
    int m_pTimerID1,m_pTimerID2,m_pTimerID3,m_pTimerID4,m_pTimerID5,m_pTimerID6,
    m_pTimerID7,m_pTimerID8,m_pTimerID9,m_pTimerID10,m_pTimerID11,m_pTimerID12;

    QMap<DATA_TYPE_M201,QVariant> m_pTestStation1DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation2DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation3DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation4DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation5DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation6DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation7DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation8DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation9DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation10DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation11DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pTestStation12DataTable;

    QMap<QString,int> m_pMapStationOutput;

private:
    explicit TvTestBusinessm201Obj(QObject *parent = nullptr);

    static TvTestBusinessm201Obj* m_pInstance;
    QMqttClient* m_pMqttClient;

    double m_pPassRate;

    void format_send_info(const QString strTopic,const QString deviceName, const QString sysNo,const int stationID);
signals:

public slots:
    void rev_tv_test_info(const QJsonObject obj);
    void updateLogStateChange();
    void brokerDisconnected();
};

#endif // TVTESTBUSINESSM201OBJ_H
