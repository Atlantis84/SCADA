#ifndef WORKSTATIONINFOWGT_H
#define WORKSTATIONINFOWGT_H

#include <QObject>
#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
class WorkStationInfoWgt : public QWidget
{
    Q_OBJECT
public:
    explicit WorkStationInfoWgt(QWidget *parent = nullptr,QString name="");
    void set_OKs(const QString OKs){m_pLineEditOKs->setText(OKs);}
    void set_NGs(const QString NGs){m_pLineEditNGs->setText(NGs);}
    void set_status(const QString status){m_pLineEditStatus->setText(status);}
private:
    QLineEdit* m_pLineEditStatus;
    QLineEdit* m_pLineEditOKs;
    QLineEdit* m_pLineEditNGs;
signals:

public slots:
};

#endif // WORKSTATIONINFOWGT_H
