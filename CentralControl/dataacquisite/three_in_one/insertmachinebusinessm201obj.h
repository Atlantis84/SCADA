#ifndef INSERTMACHINEBUSINESSM201OBJ_H
#define INSERTMACHINEBUSINESSM201OBJ_H
#include <QMap>
#include <QObject>
#include "qmqttclient.h"
#include "dataacquisitiondefines.h"
class InsertMachineBusinessM201Obj : public QObject
{
    Q_OBJECT

public:
    static InsertMachineBusinessM201Obj* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new InsertMachineBusinessM201Obj();
        return m_pInstance;
    }

    void set_throw_rate_paras(QVector<double> vec)
    {
        if(vec.size() == 4)
        {
            m_pInsertMachine1StandardThrowRate = vec[0];
            m_pInsertMachine2StandardThrowRate = vec[1];
            m_pInsertMachine3StandardThrowRate = vec[2];
            m_pInsertMachine4StandardThrowRate = vec[3];
        }
    }

private:
    explicit InsertMachineBusinessM201Obj(QObject *parent = nullptr);

    static InsertMachineBusinessM201Obj* m_pInstance;

    int m_pStatusOfInsertMachine1;
    int m_pStatusOfInsertMachine2;
    int m_pStatusOfInsertMachine3;
    int m_pStatusOfInsertMachine4;

    double m_pInsertMachine1StandardThrowRate;
    double m_pInsertMachine2StandardThrowRate;
    double m_pInsertMachine3StandardThrowRate;
    double m_pInsertMachine4StandardThrowRate;

    QMqttClient* m_pInsertMachine1MqttClient;
    QMqttClient* m_pInsertMachine2MqttClient;
    QMqttClient* m_pInsertMachine3MqttClient;
    QMqttClient* m_pInsertMachine4MqttClient;

    QMap<DATA_TYPE_M201,QVariant> m_pInsertMachine1DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pInsertMachine2DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pInsertMachine3DataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pInsertMachine4DataTable;

protected:
    void timerEvent(QTimerEvent* event);
    int m_pTimerIDOfInsertMachine1;
    int m_pTimerIDOfInsertMachine2;
    int m_pTimerIDOfInsertMachine3;
    int m_pTimerIDOfInsertMachine4;

signals:

public slots:

    void slot_info_from_insert_machine1(QVector<unsigned short> data);
    void slot_info_from_insert_machine2(QVector<unsigned short> data);
    void slot_info_from_insert_machine3(QVector<unsigned short> data);
    void slot_info_from_insert_machine4(QVector<unsigned short> data);
    void slot_current_error(const int stationid, const QString errorcode);

    void updateLogStateChangeInsertMachine1();
    void brokerDisconnectedInsertMachine1();

    void updateLogStateChangeInsertMachine2();
    void brokerDisconnectedInsertMachine2();

    void updateLogStateChangeInsertMachine3();
    void brokerDisconnectedInsertMachine3();

    void updateLogStateChangeInsertMachine4();
    void brokerDisconnectedInsertMachine4();
};

#endif // INSERTMACHINEBUSINESSM201OBJ_H
