#ifndef WAVESOLDERINGM202WGT_H
#define WAVESOLDERINGM202WGT_H

#include <QObject>
#include "wavesolderingwgt.h"
class WaveSolderingM202Wgt : public WaveSolderingWgt
{
    Q_OBJECT
public:
    static WaveSolderingM202Wgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new WaveSolderingM202Wgt();
        return m_pInstance;
    }

private:
    WaveSolderingM202Wgt();
    static WaveSolderingM202Wgt* m_pInstance;

    void process_warnings_data(const QString data);
    void process_ok_ng_data(const QString data);
    void process_equipment_state(const QString data);
    void load_data_from_db();
    void process_set_value(const QString data);
    void process_practical_value(const QString data);
    bool find_warnings(const QString data);

signals:
    void signal_to_start_acquisition_after_confirm(const DEV_TYPE_M201 dType);
public slots:
    void process_info_from_tcp_server(const QByteArray& data);
    void slot_connect_state(int connectState);
};

#endif // WAVESOLDERINGM202WGT_H
