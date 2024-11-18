#include "wavesolderingm203wgt.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
WaveSolderingM203Wgt* WaveSolderingM203Wgt::m_pInstance = nullptr;
WaveSolderingM203Wgt::WaveSolderingM203Wgt()
{

}

void WaveSolderingM203Wgt::process_warnings_data(const QString data)
{

}

void WaveSolderingM203Wgt::process_ok_ng_data(const QString data)
{

}

void WaveSolderingM203Wgt::process_equipment_state(const QString data)
{

}

void WaveSolderingM203Wgt::load_data_from_db()
{

}

void WaveSolderingM203Wgt::process_set_value(const QString data)
{

}

void WaveSolderingM203Wgt::process_practical_value(const QString data)
{

}

bool WaveSolderingM203Wgt::find_warnings(const QString data)
{
    bool result = false;
    return result;
}

void WaveSolderingM203Wgt::process_info_from_tcp_server(const QByteArray &data)
{

}

void WaveSolderingM203Wgt::slot_connect_state(int connectState)
{

}
