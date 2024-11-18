#ifndef PRESSSTATIONSERIALCOM_H
#define PRESSSTATIONSERIALCOM_H

#include <QObject>
#include <QTimer>
#include <QSerialPort>
//#include "modbus.h"
#include "modbusclientdll.h"

enum SCAN_RES_TYPE{
    ResScanOK,
    ResScanNG
};

enum PRESS_RESULT{
    ResPressStart,
    ResPressEnd,
    ResTimeOut
};

#define PRESS_TIMES_10S     (1)
#define PRESS_TIMES_30S     (2)

enum PRESS_SWITCH_RESULT{
    PRESS_SWITCH_RESULT_OK,
    PRESS_SWITCH_RESULT_NG
};

class PressStationSerialCom : public QObject
{
Q_OBJECT
public:
    PressStationSerialCom();

    //RS232通信连接
    void connectToMCU(QString com_port, QString baud);
    //RS232端开连接
    void disconnectToMCU();
    //发送开始扫码指令
    void startScanRes(SCAN_RES_TYPE Res);


    //初始化压力表
    void initManoMeter(QString portName,QString bardRate);
    //读取压力表数据
    void readPressMeter();


    //读取设备状态

    //切换应用程序
    void switchPressTimes(int code);

signals:
    //下压OK结果
    void notify_press_result(int code);
    void notify_press_meter(uint16_t value);
    void notify_switch_press_code_result(int result);

private slots:
    void slot_rev_serial_number();
    void slot_press_time_out();
    void slot_press_comm_timeout(const QString& msg);

private:
    //校验切换程序指令OK
    bool check_switch_press_code_ok(const QByteArray m_response_data);
    //校验切换程序指令NG
    bool check_switch_press_code_ng(const QByteArray m_response_data);
    //校验扫码回传OK指令
    bool Check_OK_Scan(const QByteArray m_response_data);
    //校验扫码回传NG指令
    bool check_ng_scan(const QByteArray m_response_data);
    //校验开始下压指令
    bool check_start_press(const QByteArray m_response_data);
    //校验下压结果
    bool check_result_press(const QByteArray m_response_data);
private:
    QTimer* m_pPressTimeout = nullptr;
    int n_press_time_out;
    QSerialPort* m_serial = nullptr;
    
    const uchar CMD_START[2] = {0xAA,0xBB};//帧头
    const uchar CMD_END[2] = {0xCC,0xDD};//帧尾
    bool m_is_sync = false;

//    struct timeval my_time; //modbus超时时间
//    modbus_t *my_bus=nullptr;   //定义第一个modbus类
    uint16_t modbus_hold_reg[100];      //缓存读取到的数据
//    uint16_t modbus_input_reg[100];
    int modbus_num;  //modbus从机的个数

    ModbusClientDll *m_pModbusClient1;
};

#endif // PRESSSTATIONSERIALCOM_H
