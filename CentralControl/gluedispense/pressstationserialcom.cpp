#include "pressstationserialcom.h"
#include <gdatafactory.h>
#include <QsLog.h>
#include <QEventLoop>

PressStationSerialCom::PressStationSerialCom()
{
    m_pPressTimeout = new QTimer(this);
    connect(m_pPressTimeout, &QTimer::timeout, this, &PressStationSerialCom::slot_press_time_out);
    n_press_time_out = GDataFactory::get_factory()->get_config_para("RED_FLAG_PRESS_TIMEOUT").toInt();
    m_serial = new QSerialPort(this);

    m_pModbusClient1 = new ModbusClientDll;
}

void PressStationSerialCom::initManoMeter(QString portName,QString bardRate)
{
    QLOG_WARN()<<"mano meter com-port is:"<<portName;
    QLOG_WARN()<<"mano meter com-port-baud-rate is:"<<bardRate;
    m_pModbusClient1->init_ip_port(MODBUS_TYPE::MODBUS_COM,portName,bardRate.toInt());
    modbus_num = 1;
}

void PressStationSerialCom::connectToMCU(QString com_port, QString baud)
{
    if(m_serial->isOpen()){
       m_serial->close();
    }

    qint32 val_baud = QSerialPort::Baud9600;
    if(m_serial != nullptr){
        if(baud == "115200"){
            val_baud = QSerialPort::Baud115200;
        }else if(baud == "9600"){
            val_baud = QSerialPort::Baud9600;
        }else if(baud == "4800"){
            val_baud = QSerialPort::Baud4800;
        }

        m_serial->setPortName(com_port);
        if(!m_serial->open(QIODevice::ReadWrite))
        {
            QLOG_INFO()<<com_port<<u8"打开控制器失败!";
        }else{
            m_serial->setBaudRate(val_baud,QSerialPort::AllDirections);
            m_serial->setDataBits(QSerialPort::Data8);
            m_serial->setFlowControl(QSerialPort::NoFlowControl);
            m_serial->setParity(QSerialPort::NoParity);
            m_serial->setStopBits(QSerialPort::OneStop);
            QLOG_INFO()<<com_port<<u8"打开控制器成功!";
            connect(m_serial,&QSerialPort::readyRead, this, &PressStationSerialCom::slot_rev_serial_number);
        }
    }
}

void PressStationSerialCom::disconnectToMCU()
{
    if(m_serial == nullptr)
        return;
    m_serial->close();
//    delete m_serial;
//    m_serial = nullptr;
}

void PressStationSerialCom::startScanRes(SCAN_RES_TYPE Res)
{
    if(Res == ResScanOK){
        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0xAA));
        aa.append(static_cast<char>(0xBB));
        aa.append(static_cast<char>(0x0A));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0x99));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0x4A));
        aa.append(static_cast<char>(0xED));
        aa.append(static_cast<char>(0xCC));
        aa.append(static_cast<char>(0xDD));
        datas.append(aa);
        QByteArray responseData;

        m_is_sync = true;

        if(!m_serial->isOpen()){
            QLOG_INFO() << "press serial port open fail";
        }else{
            if(m_serial!=nullptr&&m_serial->isOpen()){
                m_serial->write(aa);
                m_serial->flush();
                //m_serial->waitForBytesWritten(1000);
            }

            while (m_serial->waitForReadyRead(1000)){
                responseData += m_serial->readAll();

                if(responseData.right(2).compare(QByteArray::fromRawData((const char*)&CMD_END[0],2))==0
                        && !responseData.isEmpty()  ){
                    break;
                }
            }

            if(Check_OK_Scan(responseData)){

                QLOG_INFO() << "scanner check command is ok!";
            }
            m_is_sync = false;
        }
    }else if(ResScanNG){
        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0xAA));
        aa.append(static_cast<char>(0xBB));
        aa.append(static_cast<char>(0x0A));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0x99));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0x4B));
        aa.append(static_cast<char>(0xEE));
        aa.append(static_cast<char>(0xCC));
        aa.append(static_cast<char>(0xDD));
        datas.append(aa);

        QByteArray responseData;
        m_is_sync = true;

        if(!m_serial->isOpen()){
            QLOG_INFO() << "press serial port open fail";
        }else{
            if(m_serial!=nullptr&&m_serial->isOpen()){
                m_serial->write(aa);
                m_serial->flush();
            }

            while (m_serial->waitForReadyRead(1000)){
                responseData += m_serial->readAll();

                if(responseData.right(2).compare(QByteArray::fromRawData((const char*)&CMD_END[0],2))==0
                        && !responseData.isEmpty()  ){
                    break;
                }
            }

            if(check_ng_scan(responseData)){

                QLOG_INFO() << "scanner ng check command is ok!";
            }
            m_is_sync = false;
        }
    }
}

void PressStationSerialCom::readPressMeter()
{
    int modbus_slave_addr = 1;
    int ret = -1;

    ret = m_pModbusClient1->sendMessageToPLC(MODBUS_READ_SIGN,0x56,1,modbus_hold_reg);
    //ret = modbus_read_registers(my_bus,0x56,1,modbus_hold_reg);//读取仪表显示的值
    QLOG_WARN()<<"pressure send command"<<ret;

    if(ret == RES_OK){
        QLOG_WARN()<<"rev pressure value:"<<modbus_hold_reg[0];
        modbus_slave_addr++;
        if(modbus_slave_addr > modbus_num)
        {
            modbus_slave_addr = 1;
        }
        uint16_t res = modbus_hold_reg[0];

        //发送气压数据
        emit notify_press_meter(res);
    }
}

void PressStationSerialCom::switchPressTimes(int code)
{
    if(code == PRESS_TIMES_10S){
        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0xAA));
        aa.append(static_cast<char>(0xBB));
        aa.append(static_cast<char>(0x0A));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0xCD));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0x01));
        aa.append(static_cast<char>(0xD8));
        aa.append(static_cast<char>(0xCC));
        aa.append(static_cast<char>(0xDD));
        datas.append(aa);
        QByteArray responseData;

        m_is_sync = true;

        if(!m_serial->isOpen()){
            QLOG_INFO() << "press serial port open fail";
        }else{
            if(m_serial!=nullptr&&m_serial->isOpen()){
                m_serial->write(aa);
                m_serial->flush();
                //m_serial->waitForBytesWritten(1000);
            }

            while (m_serial->waitForReadyRead(1000)){
                responseData += m_serial->readAll();

                if(responseData.right(2).compare(QByteArray::fromRawData((const char*)&CMD_END[0],2))==0
                        && !responseData.isEmpty()  ){
                    break;
                }
            }

            if(check_switch_press_code_ok(responseData)){
                emit notify_switch_press_code_result(PRESS_SWITCH_RESULT::PRESS_SWITCH_RESULT_OK);
                QLOG_INFO() << "press tools switch command is ok!";
            }else{
                emit notify_switch_press_code_result(PRESS_SWITCH_RESULT::PRESS_SWITCH_RESULT_NG);
                QLOG_INFO() << "press tools switch command is ng!";
            }
            m_is_sync = false;
        }
    }else if(code == PRESS_TIMES_30S){
        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0xAA));
        aa.append(static_cast<char>(0xBB));
        aa.append(static_cast<char>(0x0A));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0xCD));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0x02));
        aa.append(static_cast<char>(0xD9));
        aa.append(static_cast<char>(0xCC));
        aa.append(static_cast<char>(0xDD));
        datas.append(aa);
        QByteArray responseData;

        m_is_sync = true;

        if(!m_serial->isOpen()){
            QLOG_INFO() << "press serial port open fail";
        }else{
            if(m_serial!=nullptr&&m_serial->isOpen()){
                m_serial->write(aa);
                m_serial->flush();
                //m_serial->waitForBytesWritten(1000);
            }

            while (m_serial->waitForReadyRead(1000)){
                responseData += m_serial->readAll();

                if(responseData.right(2).compare(QByteArray::fromRawData((const char*)&CMD_END[0],2))==0
                        && !responseData.isEmpty()  ){
                    break;
                }
            }

            if(Check_OK_Scan(responseData)){

                QLOG_INFO() << "scanner check command is ok!";
            }
            m_is_sync = false;
        }
    }
}

void PressStationSerialCom::slot_rev_serial_number()
{
    if(!m_is_sync){
        static QByteArray m_response_data;
        QByteArray data = m_serial->readAll();
        QLOG_DEBUG() << "slot_rev_serial_number" << data;

        if(!data.isEmpty()){
            m_response_data.append(data);
            QByteArray hexResData = m_response_data.toHex();
            QLOG_INFO()<<u8"接收串口临时数据为："<<hexResData;
            if(hexResData.contains("ccdd")){
                QLOG_INFO()<<u8"接收串口数据为："<<hexResData<< "1111";
            }else{
                return;
            }
        }else{
            QLOG_INFO() << "slot receive serial number is empty";
            return;
        }

        if(check_start_press(m_response_data)){
            QList<QByteArray> datas;
            QByteArray aa;
            aa.append(static_cast<char>(0xAA));
            aa.append(static_cast<char>(0xBB));
            aa.append(static_cast<char>(0x0A));
            aa.append(static_cast<char>(0x01));
            aa.append(static_cast<char>(0xAA));
            aa.append(static_cast<char>(0x03));
            aa.append(static_cast<char>(0x46));
            aa.append(static_cast<char>(0xFE));
            aa.append(static_cast<char>(0xCC));
            aa.append(static_cast<char>(0xDD));
            datas.append(aa);

            m_serial->write(aa);
            m_serial->flush();

            m_pPressTimeout->start(n_press_time_out);

            //启动开始下压超时定时器
            QLOG_INFO() << u8"开始压合";
            emit notify_press_result(PRESS_RESULT::ResPressStart);
        }else if(check_result_press(m_response_data)){
            m_pPressTimeout->stop();
            QLOG_INFO() << u8"压合OK"<<m_response_data.toHex();
            QList<QByteArray> datas;
            QByteArray aa;
            aa.append(static_cast<char>(0xAA));
            aa.append(static_cast<char>(0xBB));
            aa.append(static_cast<char>(0x0A));
            aa.append(static_cast<char>(0x01));
            aa.append(static_cast<char>(0xAA));
            aa.append(static_cast<char>(0x03));
            aa.append(static_cast<char>(0x4D));
            aa.append(static_cast<char>(0x05));
            aa.append(static_cast<char>(0xCC));
            aa.append(static_cast<char>(0xDD));
            datas.append(aa);

            QEventLoop eventloop;
            QTimer::singleShot(100,&eventloop,&QEventLoop::quit);
            eventloop.exec();

            m_serial->write(aa);
            m_serial->flush();

            emit notify_press_result(PRESS_RESULT::ResPressEnd);//发送下压OK结果
        }
        m_response_data.clear();
    }
}

void PressStationSerialCom::slot_press_time_out()
{
    //压合超时，自动报NG
    emit notify_press_result(PRESS_RESULT::ResTimeOut);
}

void PressStationSerialCom::slot_press_comm_timeout(const QString &msg)
{
    emit notify_press_result(PRESS_RESULT::ResTimeOut);
}

//检查串口接收到的报文是否正确
bool PressStationSerialCom::check_switch_press_code_ok(const QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0A) return false;
    //判断报文头是否正确
    if(0xAA != static_cast<uchar>(m_response_data[0])) return false;
    if(0xBB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x0A != static_cast<uchar>(m_response_data[2])) return false;
    //判断功能码是否正确
    if(0x00 != static_cast<uchar>(m_response_data[3])) return false;
    //判断地址域是否正确
    if(0xCD != static_cast<uchar>(m_response_data[4])) return false;
    //判断控制域是否正确
    if(0x00 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x0A != static_cast<uchar>(m_response_data[6])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<7;i++)
    {
        nCheckCode += static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[7])) return false;
    if(0xCC != static_cast<uchar>(m_response_data[8]))   return false;
    if(0xDD != static_cast<uchar>(m_response_data[9]))   return false;
    return true;
}

//检查串口接收到的报文是否正确
bool PressStationSerialCom::check_switch_press_code_ng(const QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0A) return false;
    //判断报文头是否正确
    if(0xAA != static_cast<uchar>(m_response_data[0])) return false;
    if(0xBB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x0A != static_cast<uchar>(m_response_data[2])) return false;
    //判断功能码是否正确
    if(0x00 != static_cast<uchar>(m_response_data[3])) return false;
    //判断地址域是否正确
    if(0xCD != static_cast<uchar>(m_response_data[4])) return false;
    //判断控制域是否正确
    if(0x00 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x0B != static_cast<uchar>(m_response_data[6])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<7;i++)
    {
        nCheckCode += static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[7])) return false;
    if(0xCC != static_cast<uchar>(m_response_data[8]))   return false;
    if(0xDD != static_cast<uchar>(m_response_data[9]))   return false;
    return true;
}

//---------------------------------------------------------------------------------------------
//检查串口接收到的报文是否正确  AA BB OA 00 99 00 4B EE CC DD（扫码OK结果）
bool PressStationSerialCom::Check_OK_Scan(const QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0A) return false;
    //判断报文头是否正确
    if(0xAA != static_cast<uchar>(m_response_data[0])) return false;
    if(0xBB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x0A != static_cast<uchar>(m_response_data[2])) return false;
    //判断功能码是否正确
    if(0x00 != static_cast<uchar>(m_response_data[3])) return false;
    //判断地址域是否正确
    if(0x99 != static_cast<uchar>(m_response_data[4])) return false;
    //判断控制域是否正确
    if(0x00 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x4B != static_cast<uchar>(m_response_data[6])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<7;i++)
    {
        nCheckCode += static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[7])) return false;
    if(0xCC != static_cast<uchar>(m_response_data[8]))   return false;
    if(0xDD != static_cast<uchar>(m_response_data[9]))   return false;
    return true;
}
//---------------------------------------------------------------------------------------------
//检查串口接收到的报文是否正确  AA BB OA 00 99 00 4F F2 CC DD（扫码NG结果）
bool PressStationSerialCom::check_ng_scan(const QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0A) return false;
    //判断报文头是否正确
    if(0xAA != static_cast<uchar>(m_response_data[0])) return false;
    if(0xBB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x0A != static_cast<uchar>(m_response_data[2])) return false;
    //判断功能码是否正确
    if(0x00 != static_cast<uchar>(m_response_data[3])) return false;
    //判断地址域是否正确
    if(0x99 != static_cast<uchar>(m_response_data[4])) return false;
    //判断控制域是否正确
    if(0x00 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x4F != static_cast<uchar>(m_response_data[6])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<7;i++)
    {
        nCheckCode += static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[7])) return false;
    if(0xCC != static_cast<uchar>(m_response_data[8]))   return false;
    if(0xDD != static_cast<uchar>(m_response_data[9]))   return false;
    return true;
}
//---------------------------------------------------------------------------------------------
//检查串口接收到的报文是否正确  AA BB OA 01 AA 03 45 FD CC DD（接收开始下压指令）
bool PressStationSerialCom::check_start_press(const QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0A) return false;
    //判断报文头是否正确
    if(0xAA != static_cast<uchar>(m_response_data[0])) return false;
    if(0xBB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x0A != static_cast<uchar>(m_response_data[2])) return false;
    //判断功能码是否正确
    if(0x01 != static_cast<uchar>(m_response_data[3])) return false;
    //判断地址域是否正确
    if(0xAA != static_cast<uchar>(m_response_data[4])) return false;
    //判断控制域是否正确
    if(0x03 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x45 != static_cast<uchar>(m_response_data[6])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<7;i++)
    {
        nCheckCode += static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[7])) return false;
    if(0xCC != static_cast<uchar>(m_response_data[8]))   return false;
    if(0xDD != static_cast<uchar>(m_response_data[9]))   return false;
    return true;
}
//---------------------------------------------------------------------------------------------
//检查串口接收到的报文是否正确  AA BB OA 01 AA 03 4C 04 CC DD（下压检测OK）
bool PressStationSerialCom::check_result_press(const QByteArray m_response_data)
{
    //优先判断响应报文的长度是否正确，否则后面代码存在数组超限风险
    int nSize = m_response_data.size();
    if(nSize != 0x0A) return false;
    //判断报文头是否正确
    if(0xAA != static_cast<uchar>(m_response_data[0])) return false;
    if(0xBB != static_cast<uchar>(m_response_data[1])) return false;
    //判断长度域是否正确
    if(0x0A != static_cast<uchar>(m_response_data[2])) return false;
    //判断功能码是否正确
    if(0x01 != static_cast<uchar>(m_response_data[3])) return false;
    //判断地址域是否正确
    if(0xAA != static_cast<uchar>(m_response_data[4])) return false;
    //判断控制域是否正确
    if(0x03 != static_cast<uchar>(m_response_data[5])) return false;
    if(0x4C != static_cast<uchar>(m_response_data[6])) return false;
    //判断校验码是否正确
    quint8 nCheckCode = 0;
    for(int i=2;i<7;i++)
    {
        nCheckCode += static_cast<uchar>(m_response_data[i]);
    }
    if(nCheckCode != static_cast<uchar>(m_response_data[7])) return false;
    if(0xCC != static_cast<uchar>(m_response_data[8]))   return false;
    if(0xDD != static_cast<uchar>(m_response_data[9]))   return false;
    return true;
}




