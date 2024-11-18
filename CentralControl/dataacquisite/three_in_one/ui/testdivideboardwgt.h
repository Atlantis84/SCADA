#ifndef TESTDIVIDEBOARDWGT_H
#define TESTDIVIDEBOARDWGT_H

#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
#include <QListWidget>
#include "dataacquisitiondefines.h"
#include "qmqttclient.h"
class TestDivideBoardWgt : public QWidget
{
    Q_OBJECT

public:
    static TestDivideBoardWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new TestDivideBoardWgt();
        return m_pInstance;
    }

    void set_divide_params(QVector<double> vpara);
protected:
    void timerEvent(QTimerEvent* event);
    int m_pTimerID;

    double m_pNullKnifeSpeedUpper;
    double m_pNullKnifeSpeedFloor;
    double m_pDownKnifeSpeedUpper;
    double m_pDownKnifeSpeedFloor;
    double m_pMainRollSpeedUpper;
    double m_pMainRollSpeedFloor;
    double m_pHighSpeedUpper;
    double m_pHighSpeedFloor;
    double m_pLowSpeedUpper;
    double m_pLowSpeedFloor;
    double m_pProduceSpeedUpper;
    double m_pProduceSpeedFloor;

private:
    explicit TestDivideBoardWgt(QWidget *parent = nullptr);

    void check_para_state(const QList<QString> lstinfo);

    static TestDivideBoardWgt* m_pInstance;

    QLineEdit* m_pA_ZAxis;
    QLineEdit* m_pB_ZAxis;
    QLineEdit* m_pA_Yxis;
    QLineEdit* m_pB_Yxis;
    QLineEdit* m_pA_Xxis;
    QLineEdit* m_pB_Xxis;
    QLineEdit* m_pA_XSpeed;
    QLineEdit* m_pB_XSpeed;
    QLineEdit* m_pA_YSpeed;
    QLineEdit* m_pB_YSpeed;
    QLineEdit* m_pA_ZSpeed;
    QLineEdit* m_pB_ZSpeed;
    QLineEdit* m_pA_JGSpeedD;
    QLineEdit* m_pB_JGSpeed;
    QLineEdit* m_pA_HighSpeed;
    QLineEdit* m_pB_HighSpeed;
    QLineEdit* m_pA_LowSpeed;
    QLineEdit* m_pB_LowSpeed;
    QLineEdit* m_pA_BUJU1;
    QLineEdit* m_pB_BUJU1;
    QLineEdit* m_pA_BUJU2;
    QLineEdit* m_pB_BUJU2;
    QList<QLineEdit*> lstLineEdit;

    BallWidget* m_pEquipStatus;
    QLineEdit* m_pLineEditOKQuantity;
    QLineEdit* m_pLineEditNGQuantity;
    QListWidget* m_pListWgt;

    QMap<DATA_TYPE_M201,QVariant> m_pDivideMachineDataTable;
    QMap<int,QString> m_pMapWarningsInfoOfDB;

    void load_data_from_db();
    QMqttClient* m_pMqttClient;

signals:

public slots:
    void slot_test();

    void slot_equipment_error_devide(const int error);
    void slot_equipment_output_devide(const int output);
    void slot_equipment_state_devide(const int status);
    void slot_equipment_paras_devide(const QList<QString> lstinfo);

    void updateLogStateChange();
    void brokerDisconnected();
};

#endif // TESTDIVIDEBOARDWGT_H
