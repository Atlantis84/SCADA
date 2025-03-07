#ifndef WAVESOLDERINGM201WGT_H
#define WAVESOLDERINGM201WGT_H

#include <QObject>
#include "wavesolderingwgt.h"
#include "qmqttclient.h"
class WaveSolderingM201Wgt : public WaveSolderingWgt
{
    Q_OBJECT
public:
    static WaveSolderingM201Wgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new WaveSolderingM201Wgt();
        return m_pInstance;
    }

    void set_wave_soldering_params(QVector<double> vpara);

protected:
    void timerEvent(QTimerEvent* event);
    int m_pTimerID;
    void check_para_state(const QList<double> lstinfo);
    QMqttClient* m_pMqttClient;

private:
    WaveSolderingM201Wgt();
    static WaveSolderingM201Wgt* m_pInstance;

    void process_warnings_data(const QString data);
    void process_ok_ng_data(const QString data);
    void process_equipment_state(const QString data);
    void load_data_from_db();
    void process_set_value(const QString data);
    void process_practical_value(const QString data);
    bool find_warnings(const QString data);

signals:
    void signal_to_start_acquisition_after_confirm(const DEV_TYPE_M201 dType);
    void signal_notify_change_production_result(const QByteArray& data);
public slots:
    void process_info_from_tcp_server(const QByteArray& data);
    void slot_connect_state(int connectState);
    void slot_btn_test();
    void updateLogStateChange();
    void brokerDisconnected();
};

#endif // WAVESOLDERINGM201WGT_H
