#ifndef CCDKEYPARTWIDGET_H
#define CCDKEYPARTWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include "ballwidget.h"
#include <QTableWidget>
#include "commonheaders.h"
class CCDKeyPartWidget : public QWidget
{
    Q_OBJECT

public:
    static CCDKeyPartWidget* get_instance()
    {
        if(m_pCCDKeyPartWidget == nullptr)
            m_pCCDKeyPartWidget = new CCDKeyPartWidget();
        return m_pCCDKeyPartWidget;
    }

private:
    explicit CCDKeyPartWidget(QWidget *parent = nullptr);

    static CCDKeyPartWidget* m_pCCDKeyPartWidget;

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

    PressStationSerialCom* m_pPressStationSerialCom;

    int m_pTimerID;

protected:
    void showEvent(QShowEvent* event);
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);

    void reset_station_status();

    DataCommSerial* pComm;
    void read_serial_number();
    void stop_serial_port();

    PadCheckFullData m_pCheckData;
    PadResultFullData1 m_pResultData;

    void push_key_bar_to_list(QString barcode);
    QByteArray format_check_json_info();
    QList<QString> m_pListCode;
    QList<QString> m_pListDesc;
    void update_ui_according_to_mes_info();
    void set_table_data();
    void notify_fpc_to_press();
    void reset_check_result_data_after_over();
    QByteArray format_result_json_info();
signals:

public slots:
    void slot_restart();
    void slot_update_start_info();
    void slot_station_config();
    void slot_rev_mano_meter_result(uint16_t r);
    void slot_rev_mes_info(const QByteArray& data);
    void slot_press_result(int resultcode);
    void slot_rev_serial_number(const QByteArray& data);
    void slot_rev_set_info(const int characterid,const QString EquipCode,const QString EquipName,const QString StationCode);
};

#endif // CCDKEYPARTWIDGET_H
