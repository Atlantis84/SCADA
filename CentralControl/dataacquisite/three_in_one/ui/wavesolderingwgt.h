#ifndef WAVESOLDERINGWGT_H
#define WAVESOLDERINGWGT_H

#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include <QListWidget>
#include "workstationinfowgt.h"
#include "tcpcommonuse.h"
#include "dataacquisitiondefines.h"
class WaveSolderingWgt : public QWidget
{
    Q_OBJECT

public:
    explicit WaveSolderingWgt(QWidget *parent = nullptr);

protected:
//    static WaveSolderingWgt* m_pInstance;
    BallWidget* m_pEquipStatus;
    BallWidget* m_pTcpConnectStatus;
    QLineEdit* m_pLineEditOKQuantity;
    QListWidget* m_pListWgt;

    QLineEdit* m_pWave1SV;
    QLineEdit* m_pWave2SV;

    WorkStationInfoWgt* m_pPlolderSpd;
    WorkStationInfoWgt* m_pB1PreHeat;
    WorkStationInfoWgt* m_pB2PreHeat;
    WorkStationInfoWgt* m_pB3PreHeat;
    WorkStationInfoWgt* m_pFBPreHeat;
    WorkStationInfoWgt* m_pT1PreHeat;
    WorkStationInfoWgt* m_pT2PreHeat;
    WorkStationInfoWgt* m_pT3PreHeat;
    WorkStationInfoWgt* m_pFTPreHeat;
    WorkStationInfoWgt* m_pSolderPot;

    virtual void process_warnings_data(const QString data) = 0;
    virtual void process_ok_ng_data(const QString data) = 0;
    virtual void process_equipment_state(const QString data) = 0;
    virtual void load_data_from_db() = 0;
    virtual void process_set_value(const QString data) = 0;
    virtual void process_practical_value(const QString data) = 0;
    virtual bool find_warnings(const QString data) = 0;

    QMap<DATA_TYPE_M201,QVariant> m_pWaveSolderingDataTable;

    QMap<int,QString> m_pMapWarningsInfoOfDB;

    double m_pMotor1FrequencyFloor;
    double m_pMotor1FrequencyUpper;
    double m_pMotor2FrequencyFloor;
    double m_pMotor2FrequencyUpper;
    double m_pPlolderSpeedFloor;
    double m_pPlolderSpeedUpper;
    double m_pSolderTempFloor;
    double m_pSolderTempUpper;
    double m_pBottomPre1Floor;
    double m_pBottomPre1Upper;
    double m_pBottomPre2Floor;
    double m_pBottomPre2Upper;
    double m_pBottomPre3Floor;
    double m_pBottomPre3Upper;

    double m_pMotor1FrequencyActualValue;
    double m_pMotor2FrequencyActualValue;
signals:

public slots:
    virtual void process_info_from_tcp_server(const QByteArray& data) = 0;
    virtual void slot_connect_state(int connectState) = 0;
};

#endif // WAVESOLDERINGWGT_H
