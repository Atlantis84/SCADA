#ifndef TESTSTATIONWGT_H
#define TESTSTATIONWGT_H

#include <QObject>
#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include "workstationinfowgt.h"
#include <QListWidget>
class TestStationWgt : public QWidget
{
    Q_OBJECT
public:
    static TestStationWgt* get_instance()
    {
        if(m_pTestStationWgt == nullptr)
            m_pTestStationWgt = new TestStationWgt();
        return m_pTestStationWgt;
    }
private:
    explicit TestStationWgt(QWidget *parent = nullptr);

private:
    static TestStationWgt* m_pTestStationWgt;

    BallWidget* m_pEquip1Status;
    BallWidget* m_pEquip2Status;
    QLineEdit* m_pLineEditStation1Quantity;
    QLineEdit* m_pLineEditStation2Quantity;
    QList<WorkStationInfoWgt*> m_pWorkStationInfoLst;
    QListWidget* m_pListWgt;

signals:

public slots:
    void slot_warnings(QList<QString> lstInfo);
    void slot_ok_ng(QList<QString> lstInfo,int machineID);//machineID:1-test station1 2-test station2
    void slot_equipment_state(QByteArray data);
};

#endif // TESTSTATIONWGT_H
