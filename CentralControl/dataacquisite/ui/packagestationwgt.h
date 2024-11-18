#ifndef PACKAGESTATIONWGT_H
#define PACKAGESTATIONWGT_H

#include <QObject>
#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include <QListWidget>
class PackageStationWgt : public QWidget
{
    Q_OBJECT
public:

    static PackageStationWgt* get_instance()
    {
        if(m_pPackageStationWgt == nullptr)
            m_pPackageStationWgt = new PackageStationWgt();
        return m_pPackageStationWgt;
    }

private:
    explicit PackageStationWgt(QWidget *parent = nullptr);

private:
    static PackageStationWgt* m_pPackageStationWgt;

    BallWidget* m_pEquipStatus;
    QLineEdit* m_pLineEditSuctionNozzle;
    QListWidget* m_pListWgt;

signals:

public slots:
    void slot_warnings(QList<QString> lstInfo);
    void slot_equipment_state(QByteArray data);
};

#endif // PACKAGESTATIONWGT_H
