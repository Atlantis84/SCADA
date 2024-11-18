#ifndef ONEBUTTONCHANGEWGTM201_H
#define ONEBUTTONCHANGEWGTM201_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include "tboxresultwgt.h"
#include "modbusclientdll.h"
#include "tcpcommonuse.h"
#include <QComboBox>
class OneButtonChangeM201Wgt : public QWidget
{
    Q_OBJECT

public:
    static OneButtonChangeM201Wgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new OneButtonChangeM201Wgt();
        return m_pInstance;
    }

private:
    explicit OneButtonChangeM201Wgt(QWidget *parent = nullptr);

    static OneButtonChangeM201Wgt* m_pInstance;

    void timerEvent(QTimerEvent* event);
    int m_pTimerID;
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);
    bool copy_file_from_src_to_dest(const QString fileNameWithPath, const QString dstPath);
    bool deleteSpecificFile(const QString &filePath);
    bool clearFile(const QString &filePath);
    bool writeToFile(const QString &fileName, const QString &text);

    void test_change_production();
    void insert_new_product_to_db();
    void load_product_info_from_db();

    QLineEdit* lineEdit_SN;
    QTextEdit* textEditMesInfo;
    TBoxResultWgt* tboxResultWgt;

    QString m_pCurrentStyle;
    QString m_pCurrentMaterialNumber;

    QCheckBox* cbStation1;
    QCheckBox* cbStation2;
    QCheckBox* cbStation3;
    QCheckBox* cbStation4;
    QCheckBox* cbStation5;
    QCheckBox* cbStation6;
    QCheckBox* cbStation7;
    QCheckBox* cbStation8;
    QCheckBox* cbStation9;
    QCheckBox* cbStation10;

    QList<int> lstStations;
    QList<QCheckBox*> lstCheckBox;

    ModbusClientDll* m_pUpBoardModbusObj;
    ModbusClientDll* m_pUpUtensilModbusObj;
    TcpCommonUse*    m_pTcpCommonUseWaveSoldering;
    ModbusClientDll* m_pPickUpCoverModbusObj;
    ModbusClientDll* m_pDownUtensilModbusObj;

    QComboBox* m_pCmbStationStyle;
    QComboBox* m_pCmbMaterialNumber;
    QLineEdit* m_pLineEditBoardStyle;
    QLineEdit* m_pLineEditMaterialNumber;

    QMap<QString,QStringList> m_pMapProductInfo;

    bool m_pAlreadyStartTimer;

signals:
    void signal_notify_to_change_production(const QString boardStyle,const QString materialNumber);
public slots:
    void slot_rev_logs(const QString str);
    void slot_btn_change_production();
    void slot_from_mapper(int id);
    void slot_rev_tcp_server_wave_soldering_info(const QByteArray& data);
    void slot_rev_serial_number(const QByteArray& data);
    void slot_rev_info_from_mes(const QByteArray& data);
    void slot_change_production(const QString boardStyle,const QString materialNumber);

    void slot_rev_result_of_change_production_for_divide_glue(const int id);
    void slot_rev_result_of_change_production_for_aoi(const int r);
    void slot_comb_index_changed(int index);
    void slot_comb_material_number_changed(int index);
    void slot_btn_insert_product_info();
    void slot_btn_read_file();
    void slot_btn_export_file();
};

#endif // ONEBUTTONCHANGEWGTM201_H
