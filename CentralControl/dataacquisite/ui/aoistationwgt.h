#ifndef AOISTATIONWGT_H
#define AOISTATIONWGT_H

#include <QObject>
#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include <QListWidget>
class AOIStationWgt : public QWidget
{
    Q_OBJECT

public:
    static AOIStationWgt* get_instance()
    {
        if(m_pAOIStationWgt == nullptr)
            m_pAOIStationWgt = new AOIStationWgt();
        return m_pAOIStationWgt;
    }
private:
    explicit AOIStationWgt(QWidget *parent = nullptr);

private:
    static AOIStationWgt* m_pAOIStationWgt;

    BallWidget* m_pEquipStatus;
    QLineEdit* m_pLineEditOKQuantity;
    QLineEdit* m_pLineEditNGQuantity;
    QListWidget* m_pListWgt;

signals:

public slots:
    void slot_warnings(QList<QString> lstInfo);
    void slot_ok_ng(QList<QString> lstInfo,int machineID);
    void slot_equipment_state(QByteArray data);
};

#endif // AOISTATIONWGT_H
