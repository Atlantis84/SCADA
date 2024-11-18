#include "glueweighcommservice.h"
#include "gdatafactory.h"
#include <QEventLoop>
#include <QApplication>


static bool isTurnReadRun = false;

glueWeighCommService::glueWeighCommService(QObject *parent)
{
    QByteArray StrNameByte = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_GLUE").toLatin1();
    char *port_name = StrNameByte.data();
    int modbus_baud = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_GLUE_BARD_RATE").toInt();

    m_pModbusClient = new ModbusClientDll;
    m_pModbusClient->init_ip_port(MODBUS_TYPE::MODBUS_COM,port_name,modbus_baud);
    modbus_num = 1;

    ntimeOut = GDataFactory::get_factory()->get_config_para("RED_FLAG_GLUE_TIMEOUT").toInt();
    m_gl_delay_ms=GDataFactory::get_factory()->get_config_para("RED_FLAG_GLUE_DELAY_MS").toInt();
    m_pTimerOut = new QTimer();
    moveToThread(this);
    m_pTimerOut->setObjectName("timeOut");
    connect(m_pTimerOut, &QTimer::timeout, this, &glueWeighCommService::timerOutExp);

    m_pTimerDelay=new QTimer();
    m_pTimerDelay->moveToThread(this);
    m_pTimerDelay->setObjectName("timeDelay");
    connect(m_pTimerDelay, &QTimer::timeout,this,&glueWeighCommService::timerOutExp);
    isWorkingState = false;

    m_serial = new QSerialPort();
    m_serial->moveToThread(this);

    if(!this->isRunning()){
        this->start();
    }
}

void glueWeighCommService::startReadModbusSignal(bool readEnable)
{
    if(readEnable){
        isRunningFlags = true;
        isWorkingState = false;
    }else{
        isRunningFlags = false;
        disconnectToWeight();
    }
}

void glueWeighCommService::timerOutExp()
{
    QString senderName = QObject::sender()->objectName();
    QLOG_INFO() << senderName;
    if(senderName=="timeOut"){
        m_pTimerOut->stop();
        isWorkingState = false;
        isTurnReadRun = false;
        QLOG_ERROR() << "glueWeighCommService::timerOutExp";
        //发送超时错误信号
//        emit notify_glue_weight_error_text(u8"点胶称重工位超时，请重新称重！");
//        quit();
//        wait();
//        deleteLater();
    }else if(senderName=="timeDelay"){
        QLOG_INFO() << "start read weight data!";
        m_pTimerDelay->stop();
        connectToWeight();//开始称重
        isTurnReadRun = true;
    }
}

void glueWeighCommService::connectToWeight()
{
    if(m_serial->isOpen()){
       m_serial->close();
    }

    QString portName = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_WEIGH");
    QString portRate = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_WEIGH_BARD_RATE");

    qint32 val_baud = QSerialPort::Baud9600;
    if(m_serial != nullptr){
        if(portRate == "115200"){
            val_baud = QSerialPort::Baud115200;
        }else if(portRate == "9600"){
            val_baud = QSerialPort::Baud9600;
        }else if(portRate == "4800"){
            val_baud = QSerialPort::Baud4800;
        }

        m_serial->setPortName(portName);
        if(!m_serial->open(QIODevice::ReadWrite))
        {
            QLOG_INFO()<<portName<<u8"打开控制器失败!";
        }else{
            m_serial->setBaudRate(val_baud,QSerialPort::AllDirections);
            m_serial->setDataBits(QSerialPort::Data8);
            m_serial->setFlowControl(QSerialPort::NoFlowControl);
            m_serial->setParity(QSerialPort::NoParity);
            m_serial->setStopBits(QSerialPort::OneStop);
            QLOG_INFO()<<portName<<u8"打开控制器成功!";
            connect(m_serial,&QSerialPort::readyRead, this, &glueWeighCommService::slot_rev_serial_number);
        }
    }
}

void glueWeighCommService::disconnectToWeight()
{
    disconnect(m_serial,&QSerialPort::readyRead, this, &glueWeighCommService::slot_rev_serial_number);
    m_serial->clear();
    m_serial->close();
    //m_serial->deleteLater();
}

void glueWeighCommService::run()
{
    int modbus_slave_addr = 1;
    int ret = -1;
    while(1){
        if(isRunningFlags){
            //modbus_set_slave(my_bus,modbus_slave_addr);//设置需要连接的从机地址
            //ret = modbus_read_registers(my_bus,0x4,1,modbus_hold_reg);//读取保持寄存器的第0位开始的前1位
            ret = m_pModbusClient->sendMessageToPLC(MODBUS_READ_SIGN,0x4,1,modbus_hold_reg);
            if(ret == MODBUS_STATUS::RES_OK){
                modbus_slave_addr++;
                if(modbus_slave_addr > modbus_num)
                {
                    modbus_slave_addr = 1;
                }
                uint16_t res = modbus_hold_reg[0];
                if(!isWorkingState && (res & 0x1) == 0x1){//读取到开始测试信号
                    //读取点胶前产品重量
                    isWorkingState = true;
                    QLOG_INFO() << u8"点胶前开始称重！";
                    nWeigWorkStationId = GLUE_BEFORE_WEIGHT;
                    m_pTimerDelay->start(m_gl_delay_ms);
                    //启动超时判断
                    //m_pTimerOut->start(ntimeOut);
                }

                if(isWorkingState && (res&0x2) == 0x2){
                    m_pTimerOut->stop();
                    //读取点胶后产品重量
                    isWorkingState = false;
                    QLOG_INFO() << u8"点胶后开始称重！";
                    nWeigWorkStationId = GLUE_AFTER_WEIGHT;
                    m_pTimerDelay->start(m_gl_delay_ms);
                }
            }
        }

        QEventLoop eventloop;
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        QApplication::processEvents();
    }
}

void glueWeighCommService::slot_rev_serial_number()
{
    QString strRes;
    static QByteArray m_response_data;
    if(isTurnReadRun){
        QByteArray data = m_serial->readAll();
        QLOG_DEBUG() << "slot_rev_serial_number" << data;

        if(!data.isEmpty()){
            m_response_data.append(data);
            if(m_response_data.length() < 16){
                QLOG_INFO() << "receive data length error";
                return;
            }else if(m_response_data.length() > 16){
                m_response_data.clear();
            }else if(m_response_data.length() == 16){
                if(!m_response_data.contains("g") ||!m_response_data.contains(0x67)){
                    m_response_data.clear();
                    QLOG_WARN() << "receive weight data format is error, please check it!";
                    return;
                }else{
                    for(int i=0; i<m_response_data.size(); i++){
                        char data = static_cast<char>(m_response_data[i]);
                        if((data >= 0x30 && data <= 0x39) || data == 0x2e || data == 0x2d){
                            strRes.append(data);
                        }
                    }
                }

                QLOG_INFO() << nWeigWorkStationId << "receive glue weight is" << strRes;
                emit notify_result_weight(nWeigWorkStationId, strRes);

                disconnectToWeight();
                isTurnReadRun = false;
                m_response_data.clear();
            }
        }else{
            //m_response_data.clear();
            QLOG_INFO() << "slot receive serial number is empty";
            return;
        }
    }

    //this->exit();
}




