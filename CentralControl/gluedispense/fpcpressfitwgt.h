#ifndef FPCPRESSFITWGT_H
#define FPCPRESSFITWGT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>
#include "ballwidget.h"
#include "commonheaders.h"
#include "paddatastructure.h"
#include "pressstationserialcom.h"
class FPCPressFitWgt : public QWidget
{
    Q_OBJECT
public:
    static FPCPressFitWgt* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new FPCPressFitWgt();
        return m_pObj;
    }
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent* event);
    void timerEvent(QTimerEvent *event);
private:
    explicit FPCPressFitWgt(QWidget *parent = nullptr);

    static FPCPressFitWgt* m_pObj;

    PadCheckFullData m_pCheckData;
    PadResultFullData m_pResultData;
    int m_pGlueUpperValue;
    int m_pGlueLowerValue;
    int m_pGlueStandardValue;
    QList<QString> m_pListDesc;
    QList<QString> m_pListCode;
    int m_press_process = -1;
    QString m_printBARCODE;

    PressStationSerialCom* m_pPressStationService = nullptr;
    int m_pTimerID;

    QTextEdit* m_pLogTextEdit;

    QLineEdit* m_pLineEditStationCode;
    QLineEdit* m_pLineEditEquipCode;
    QLineEdit* m_pLineEditEquipName;

    BallWidget* m_pScanMainBoard;
    BallWidget* m_pScanKeyPart;
    BallWidget* m_pKeyPartCheckState;
    BallWidget* m_pPressFitCheckState;
    BallWidget* m_pFinalCheckState;

    QTableWidget* tableWidget;
    QWidget* createTableWidget();

    DataCommSerial* pComm;
    void read_serial_number();

    void stop_serial_port();

    QByteArray format_check_json_info();
    QByteArray format_result_json_info();

    void update_ui_according_to_mes_info();
    void set_table_data();
    void log_add_text_edit(QString text);
signals:

public slots:
    void slot_station_config();
    void slot_update_start_info();
    void slot_rev_serial_number(const QByteArray& data);
    void slot_rev_set_info(const int characterid,const QString EquipCode, const QString EquipName, const QString StationCode);
    void slot_rev_mes_info(const QByteArray& data);
    void slot_recv_preas_result_info(int code);
    void slot_rev_mano_meter_result(uint16_t r);
};

#endif // FPCPRESSFITWGT_H
