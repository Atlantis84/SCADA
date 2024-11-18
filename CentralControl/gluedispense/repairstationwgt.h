#ifndef REPAIRSTATIONWGT_H
#define REPAIRSTATIONWGT_H

#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QSerialPortInfo>
#include "paddatastructure.h"

class RepairStationWgt : public QWidget
{
    Q_OBJECT
public:
    explicit RepairStationWgt(QWidget *parent = nullptr);

    static RepairStationWgt* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new RepairStationWgt();
        return m_pObj;
    }

    void format_result_json_info();

signals:
    void serialPortInserted(QString port);
    void serialPortRemoved(QString);
public slots:
    void slot_station_config();
    void slot_read_serial_number();
private:
    static RepairStationWgt* m_pObj;

    QLineEdit* m_pLineEditStationCode;
    QLineEdit* m_pLineEditEquipCode;
    QLineEdit* m_pLineEditEquipName;

    BallWidget* m_pScanBoard;//扫码信息
    BallWidget* m_pClearSign;//清除老化标志位
    BallWidget* m_pMesResult;//MES上传

    QTextEdit* m_pLogTextEdit;
    QLineEdit* m_pScanSNEdit;

    QList<QSerialPortInfo> m_previousPorts;

    int m_nTimeId = -1;

    PadResultFullData m_pResultData;

    typedef  struct {
       QString m_serial_number;
       QString m_result;
    }ClearAgingResult;
    ClearAgingResult m_clearAgingResult;


    void log_add_text_edit(QString text);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // REPAIRSTATIONWGT_H
