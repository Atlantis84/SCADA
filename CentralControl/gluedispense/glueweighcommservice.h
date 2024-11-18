#ifndef GLUEWEIGHCOMMSERVICE_H
#define GLUEWEIGHCOMMSERVICE_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include <datacommserial.h>
#include <QThread>
#include "modbusclientdll.h"

#define GLUE_BEFORE_WEIGHT   (1)
#define GLUE_AFTER_WEIGHT    (2)

class glueWeighCommService : public QThread
{
    Q_OBJECT
public:
    explicit glueWeighCommService(QObject *parent = nullptr);

    //modbus read start and finish signal
    void startReadModbusSignal(bool readEnable = true);

signals:
    void notify_result_weight(int nWorkId, QString weight);
    void notify_glue_weight_error_text(QString text);
    void notify_comm_timeout_msg(const QString msg_text);//称重串口报错信息

public slots:
    void timerOutExp();
    void slot_rev_serial_number();

private:
    void connectToWeight();
    void disconnectToWeight();

private:
    ModbusClientDll *m_pModbusClient;
    int modbus_num;  //modbus从机的个数
    uint16_t modbus_hold_reg[100];      //缓存读取到的数据

    QTimer *m_pTimerOut = nullptr;
    QTimer *m_pTimerDelay = nullptr;
    int ntimeOut;//超时时间
    int m_gl_delay_ms;
    bool isWorkingState = false;
    bool isRunningFlags = false;

    QSerialPort* m_serial = nullptr;

    //称参数配置
    DataCommSerial *pCommPress;
    //bool m_is_gl_comm = false;
    int nWeigWorkStationId;

    // QThread interface
protected:
    void run();
};

#endif // GLUEWEIGHCOMMSERVICE_H
