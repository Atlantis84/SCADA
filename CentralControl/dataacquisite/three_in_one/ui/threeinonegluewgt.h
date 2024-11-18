#ifndef THREEINONEGLUEWGT_H
#define THREEINONEGLUEWGT_H

#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include <QListWidget>
#include "dataacquisitiondefines.h"
#include "qmqttclient.h"
class ThreeInOneGlueWgt : public QWidget
{
    Q_OBJECT

public:
    static ThreeInOneGlueWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new ThreeInOneGlueWgt();
        return m_pInstance;
    }

private:
    explicit ThreeInOneGlueWgt(QWidget *parent = nullptr);
    void load_data_from_db();

protected:
    void timerEvent(QTimerEvent* event);
    int m_pTimerID;

private:

    static ThreeInOneGlueWgt* m_pInstance;

    BallWidget* m_pEquipStatus;
    QLineEdit* m_pLineEditOKQuantity;
    QLineEdit* m_pLineEditNGQuantity;
    QListWidget* m_pListWgt;

    QMap<DATA_TYPE_M201,QVariant> m_pGlueMachineDataTable;
    QMap<int,QString> m_pMapWarningsInfoOfDB;

    QMqttClient* m_pMqttClient;

signals:

public slots:
    void slot_equipment_state_glue(const int status);
    void slot_equipment_error_glue(const int error);
    void slot_equipment_output_glue(const int output);

    void slot_btn_test();

    void updateLogStateChange();
    void brokerDisconnected();
};

#endif // THREEINONEGLUEWGT_H
