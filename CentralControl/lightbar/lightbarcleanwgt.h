#ifndef LIGHTBARCLEANWGT_H
#define LIGHTBARCLEANWGT_H

#include <QWidget>
#include <QLineEdit>
#include "resultwidget.h"
class QPushButton;
class LightBarCleanWgt : public QWidget
{
    Q_OBJECT
public:
    explicit LightBarCleanWgt(QWidget *parent = nullptr);
    void set_product_sn(const int sign,const QString sn);
    void trigger_timer_to_query();

    void set_ok_ng_state(bool state);
    void set_procedure_state(bool state);
private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);
    void timerEvent(QTimerEvent *event);
    int m_pTimerID;
    int m_pTimerIDMonitor;
    int m_pWashTimes;

    QLineEdit* lineEdit_SN;
    QLineEdit* lineEdit_SN2;
    QLineEdit* lineEdit_PcbCode;
    QLineEdit* lineEdit_ModelCode;
    QLineEdit* lineEdit_ResultCode;
    QLineEdit* lineEdit_Result;

    ResultWidget* resultWgt;
    ResultWidget* resultProcedureWgt;

    void insert_wash_record_to_db();
    void insert_wash_record_to_db_manual();
    void send_cmd_to_plc_accroding_to_mes_info();

    QPushButton* btnManualPass;

    QString m_pCurrentSN;
    bool m_pWashingSign;

signals:

public slots:
    void slot_btn_click();
    void slot_finish_washing(const int washtimes);
    void slot_btn_check_washing_records();
    void slot_btn_new_product_number();

    void slot_rev_sn_info(int codeSign,const QByteArray& data);
    void slot_rev_mes_result(const QByteArray& data);
};

#endif // LIGHTBARCLEANWGT_H
