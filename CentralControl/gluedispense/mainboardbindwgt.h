#ifndef MAINBOARDBINDWGT_H
#define MAINBOARDBINDWGT_H

#include <QObject>
#include <QWidget>
#include "ballwidget.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>
#include "commonheaders.h"
#include "paddatastructure.h"
class MainBoardBindWgt : public QWidget
{
    Q_OBJECT
public:
    static MainBoardBindWgt* get_instance()
    {
        if(m_pMainBoardBindWgt == nullptr)
            m_pMainBoardBindWgt = new MainBoardBindWgt();
        return m_pMainBoardBindWgt;
    }
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent* event);
    void timerEvent(QTimerEvent *event);

    QString m_pCurrentMiddleFrameNumber;
    int check_repeated_ok_result_of_glue(const QString middleframenumber);
private:
    explicit MainBoardBindWgt(QWidget *parent = nullptr);

    double m_pGlueUpperValue;
    double m_pGlueLowerValue;
    double m_pGlueStandardValue;

    double m_pGlueBeforeValue;
    double m_pGlueAfterValue;
    double m_pGlueCurrentValue;

    QList<QString> m_pListCode;
    QList<QString> m_pListDesc;

    static MainBoardBindWgt* m_pMainBoardBindWgt;

    QTextEdit* m_pLogTextEdit;

    QLineEdit* m_pLineEditStationCode;
    QLineEdit* m_pLineEditEquipCode;
    QLineEdit* m_pLineEditEquipName;
    QLineEdit* m_pLineEditGlueStandard;

    QLineEdit* m_pLineEditGlueInfo;
    QCheckBox* ngCheckBox;

    BallWidget* m_pGlueCheckState;
    BallWidget* m_pPasteGlueCheckState;
    BallWidget* m_pKeyPartCheckState;
    BallWidget* m_pPressFitCheckState;
    BallWidget* m_pFinalCheckState;

    QTableWidget* tableWidget;
    QWidget* createTableWidget();
    DataCommSerial* pComm;
    void read_serial_number();
    void stop_serial_port();

    int m_pTimerID;

    bool m_pAlreadyPressOver;
    //just for test,delete later
//    DataCommSerial* pCommGlue;
//    void read_serial_number_glue();
//    void stop_serial_port_glue();
    //just for test,delete later

    PadCheckFullData m_pCheckData;
    PadResultFullData m_pResultData;

    void push_glue_info_to_list();

    QByteArray format_check_json_info();
    QByteArray format_result_json_info();

    void update_ui_according_to_mes_info();
    void set_table_data();
    void reset_station_status();

    void check_glue_info_of_db(const QString middleframenumber,bool& midfexistsign,bool& oksign);
    void push_key_bar_to_list(QString barcode);
    void insert_glue_result_to_db(QList<QString> info,const QString result,QList<QString> lstglueinfo);
    void notify_fpc_to_press();
    void reset_check_result_data_after_over();
    void truncate_data_in_db();
    QString get_current_time();

    PressStationSerialCom* m_pPressStationSerialCom;
signals:

public slots:
    void slot_station_config();
    void slot_state_changed(int state);
    void slot_rev_mes_info(const QByteArray& data);
    void slot_rev_serial_number(const QByteArray& data);
    //just for test,delete later
//    void slot_rev_serial_number_glue(const QByteArray& data);
    //just for test,delete later
    void slot_rev_set_info(const int characterid,const QString EquipCode, const QString EquipName, const QString StationCode);
    void slot_press_result(int resultcode);
    void slot_weight_result(int nWorkId, QString weight);
    void slot_restart();

    void slot_rev_mano_meter_result(uint16_t p);
};

#endif // MAINBOARDBINDWGT_H
