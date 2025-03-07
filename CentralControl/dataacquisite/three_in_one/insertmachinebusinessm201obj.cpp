#include "insertmachinebusinessm201obj.h"
#include <QVector>
#include "QsLog.h"
#include <QTimerEvent>
#include "dataacquisitionconfig.h"
#include <QDateTime>
#include "gdatafactory.h"
#include "gdataacquisitionfactory.h"
InsertMachineBusinessM201Obj* InsertMachineBusinessM201Obj::m_pInstance = nullptr;
InsertMachineBusinessM201Obj::InsertMachineBusinessM201Obj(QObject *parent) : QObject(parent)
{
    QJsonArray fault;
    m_pInsertMachine1DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    m_pInsertMachine2DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    m_pInsertMachine3DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    m_pInsertMachine4DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);

    m_pStatusOfInsertMachine1 = 0;
    m_pStatusOfInsertMachine2 = 0;
    m_pStatusOfInsertMachine3 = 0;
    m_pStatusOfInsertMachine4 = 0;

    m_pInsertMachine1StandardThrowRate = DataAcquisitionConfig::get_instance()->get_config_para("INSERT_MACHINE_1_THROW_RATE_STANDARD").toDouble();
    m_pInsertMachine2StandardThrowRate = DataAcquisitionConfig::get_instance()->get_config_para("INSERT_MACHINE_2_THROW_RATE_STANDARD").toDouble();
    m_pInsertMachine3StandardThrowRate = DataAcquisitionConfig::get_instance()->get_config_para("INSERT_MACHINE_3_THROW_RATE_STANDARD").toDouble();
    m_pInsertMachine4StandardThrowRate = DataAcquisitionConfig::get_instance()->get_config_para("INSERT_MACHINE_4_THROW_RATE_STANDARD").toDouble();


    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        m_pTimerIDOfInsertMachine1 = -1;
        m_pTimerIDOfInsertMachine1 = startTimer(5000);
        m_pTimerIDOfInsertMachine2 = -1;
        m_pTimerIDOfInsertMachine2 = startTimer(5000);
        m_pTimerIDOfInsertMachine3 = -1;
        m_pTimerIDOfInsertMachine3 = startTimer(5000);
        m_pTimerIDOfInsertMachine4 = -1;
        m_pTimerIDOfInsertMachine4 = startTimer(5000);

        //insert-machine-error
        connect(InsertMachineM201ErrorThread::get_instance(),SIGNAL(signal_current_error(const int, const QString)),this,SLOT(slot_current_error(const int, const QString)));
        InsertMachineM201ErrorThread::get_instance()->start();
        //insert-machine-error
    }

    m_pInsertMachine1MqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pInsertMachine1MqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pInsertMachine1MqttClient->setPort(pt);
    connect(m_pInsertMachine1MqttClient, &QMqttClient::stateChanged, this, &InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine1);
    connect(m_pInsertMachine1MqttClient, &QMqttClient::disconnected, this, &InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine1);

    m_pInsertMachine1MqttClient->setUsername("admin");
    m_pInsertMachine1MqttClient->setPassword("Admin12345");
    if(m_pInsertMachine1MqttClient->state() == QMqttClient::Disconnected)
        m_pInsertMachine1MqttClient->connectToHost();

    m_pInsertMachine2MqttClient = new QMqttClient();
    strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pInsertMachine2MqttClient->setHostname(strHostname);
    pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pInsertMachine2MqttClient->setPort(pt);
    connect(m_pInsertMachine2MqttClient, &QMqttClient::stateChanged, this, &InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine2);
    connect(m_pInsertMachine2MqttClient, &QMqttClient::disconnected, this, &InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine2);

    m_pInsertMachine2MqttClient->setUsername("admin");
    m_pInsertMachine2MqttClient->setPassword("Admin12345");
    if(m_pInsertMachine2MqttClient->state() == QMqttClient::Disconnected)
        m_pInsertMachine2MqttClient->connectToHost();

    m_pInsertMachine3MqttClient = new QMqttClient();
    strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pInsertMachine3MqttClient->setHostname(strHostname);
    pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pInsertMachine3MqttClient->setPort(pt);
    connect(m_pInsertMachine3MqttClient, &QMqttClient::stateChanged, this, &InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine3);
    connect(m_pInsertMachine3MqttClient, &QMqttClient::disconnected, this, &InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine3);

    m_pInsertMachine3MqttClient->setUsername("admin");
    m_pInsertMachine3MqttClient->setPassword("Admin12345");
    if(m_pInsertMachine3MqttClient->state() == QMqttClient::Disconnected)
        m_pInsertMachine3MqttClient->connectToHost();

    m_pInsertMachine4MqttClient = new QMqttClient();
    strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pInsertMachine4MqttClient->setHostname(strHostname);
    pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pInsertMachine4MqttClient->setPort(pt);
    connect(m_pInsertMachine4MqttClient, &QMqttClient::stateChanged, this, &InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine4);
    connect(m_pInsertMachine4MqttClient, &QMqttClient::disconnected, this, &InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine4);

    m_pInsertMachine4MqttClient->setUsername("admin");
    m_pInsertMachine4MqttClient->setPassword("Admin12345");
    if(m_pInsertMachine4MqttClient->state() == QMqttClient::Disconnected)
        m_pInsertMachine4MqttClient->connectToHost();
}

void InsertMachineBusinessM201Obj::timerEvent(QTimerEvent *event)
{
    if(GDataAcquisitionFactory::get_instance()->isTimeBetween730And830() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1830And2030())
        return;

    if(event->timerId() == m_pTimerIDOfInsertMachine1)
    {
        if(m_pInsertMachine1DataTable.size() != 3)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

        QString strTopic = "HD_M201_INSERT_MACHINE_1_TOPIC";
        auto subscription = m_pInsertMachine1MqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of insert-machine1-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pInsertMachine1DataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        rejectrate = m_pInsertMachine1DataTable[DATA_TYPE_M201::Reject_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
//        QLOG_ERROR()<<u8"异型插1设备状态为:"<<systemstatus;
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","InsertMachine1M201");
        data.insert("SystemNo","HS0543302022GNDJ10001");
        data.insert("CONTENT",content);
        if(systemstatus == "1")
            data.insert("FAULT",m_pInsertMachine1DataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray());
        else
            data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QLOG_WARN()<<"insert-machine1 submit msg to andon:"<<jsonDoc.toJson();
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pInsertMachine1MqttClient->publish(strTopic,byteArray);
    }
    else if(event->timerId() == m_pTimerIDOfInsertMachine2)
    {
        if(m_pInsertMachine2DataTable.size() != 3)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

        QString strTopic = "HD_M201_INSERT_MACHINE_2_TOPIC";
        auto subscription = m_pInsertMachine2MqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of insert-machine2-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pInsertMachine2DataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        rejectrate = m_pInsertMachine2DataTable[DATA_TYPE_M201::Reject_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","InsertMachine2M201");
        data.insert("SystemNo","HS0543302022GNDJ10002");
        data.insert("CONTENT",content);
        if(systemstatus == "1")
            data.insert("FAULT",m_pInsertMachine2DataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray());
        else
            data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QLOG_WARN()<<"insert-machine2 submit msg to andon:"<<jsonDoc.toJson();
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pInsertMachine2MqttClient->publish(strTopic,byteArray);
    }
    else if(event->timerId() == m_pTimerIDOfInsertMachine3)
    {
        if(m_pInsertMachine3DataTable.size() != 3)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

        QString strTopic = "HD_M201_INSERT_MACHINE_3_TOPIC";
        auto subscription = m_pInsertMachine3MqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of insert-machine3-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pInsertMachine3DataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        rejectrate = m_pInsertMachine3DataTable[DATA_TYPE_M201::Reject_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","InsertMachine3M201");
        data.insert("SystemNo","HS0543302022GNDJ10003");
        data.insert("CONTENT",content);
        if(systemstatus == "1")
            data.insert("FAULT",m_pInsertMachine3DataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray());
        else
            data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QLOG_WARN()<<"insert-machine3 submit msg to andon:"<<jsonDoc.toJson();
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pInsertMachine3MqttClient->publish(strTopic,byteArray);
    }
    else if(event->timerId() == m_pTimerIDOfInsertMachine4)
    {
        if(m_pInsertMachine4DataTable.size() != 3)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

        QString strTopic = "HD_M201_INSERT_MACHINE_4_TOPIC";
        auto subscription = m_pInsertMachine4MqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of insert-machine4-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pInsertMachine4DataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        rejectrate = m_pInsertMachine4DataTable[DATA_TYPE_M201::Reject_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","InsertMachine4M201");
        data.insert("SystemNo","HS0543302022GNDJ10004");
        data.insert("CONTENT",content);
        if(systemstatus == "1")
            data.insert("FAULT",m_pInsertMachine4DataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray());
        else
            data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QLOG_WARN()<<"insert-machine4 submit msg to andon:"<<jsonDoc.toJson();
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        QString str = jsonDoc.toJson();

//        QLOG_ERROR()<<str;
        m_pInsertMachine4MqttClient->publish(strTopic,byteArray);
    }
}

static int insert_machine_1_quantity = -1;
static int test_count = 0;
void InsertMachineBusinessM201Obj::slot_info_from_insert_machine1(QVector<unsigned short> data)
{
    if(data.length() == 20)
    {
        QString status = "";
        for (int i=0;i<data.length();i++) {
            if(data[i] == 0)
                continue;
            QByteArray aa;
            aa.append(static_cast<char>(data[i]>>8));
            char c = static_cast<char>(aa[0]);
            status.append(c);
            aa.clear();
            if((data[i] & 0xFF) != 0){
                aa.append(static_cast<char>(data[i] & 0xFF));
                c = static_cast<char>(aa[0]);
                status.append(c);
            }
        }

//        QLOG_ERROR()<<"the status of insert-machine1 is:"<<status;

        if(status.contains("Running"))
        {
            QLOG_INFO()<<"the current state of insert-machine1 is Running";
            if(m_pStatusOfInsertMachine1 != 2)
            {
                m_pStatusOfInsertMachine1 = 2;
                QLOG_WARN()<<"the status of insert machine1 change to Running!";
                //MQ post
                //MQ post
            }

            if(m_pInsertMachine1DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine1DataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
            else
                m_pInsertMachine1DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
        }
        else if(status.contains("Error"))
        {
            QLOG_INFO()<<"the current state of insert-machine1 is Stopping";
            if(m_pStatusOfInsertMachine1 != 1)
            {
                m_pStatusOfInsertMachine1 = 1;
                QLOG_WARN()<<"the status of insert machine1 change to Stopping!";
                //MQ post
                //MQ post
            }
            if(m_pInsertMachine1DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine1DataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
            else
                m_pInsertMachine1DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }
        else
        {
            QLOG_INFO()<<"the current state of insert-machine1 is Pausing";
            if(m_pStatusOfInsertMachine1 != 3)
            {
                m_pStatusOfInsertMachine1 = 3;
                QLOG_WARN()<<"the status of insert machine1 change to Pausing!";
                //MQ post
                //MQ post
            }
            if(m_pInsertMachine1DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine1DataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
            else
                m_pInsertMachine1DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
        }
    }
    else {
        if(data.length() == 8)
        {
            int producequantity = data[0]*0xffff + data[1];
//            QLOG_ERROR()<<u8"异型插1产量为:"<<producequantity;

            QTime time = QTime::currentTime();
            if(time.minute() == 0)
            {
                if(insert_machine_1_quantity == -1)
                    insert_machine_1_quantity = producequantity;
                else
                {
                    if((producequantity-insert_machine_1_quantity)<30)
                        ;
                    else {
                        QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                        QLOG_ERROR()<<u8"异型插A"<<strhour<<QString::number(producequantity-insert_machine_1_quantity);
                        insert_machine_1_quantity = producequantity;
                    }

                }
            }

            int searchfailed = data[4]*0xffff + data[5];
            int pickfailed = data[6]*0xffff + data[7];

            double throwrate = static_cast<double>(searchfailed + pickfailed)/static_cast<double>(producequantity);

            //0.034
            int tmpThrowrate = throwrate*1000;
            throwrate = static_cast<double>(tmpThrowrate)/1000.0;
//            QLOG_ERROR()<<u8"异型插1抛料率实际值:"<<throwrate;
//            QLOG_ERROR()<<u8"异型插1抛料率标准值:"<<m_pInsertMachine1StandardThrowRate;

            QJsonArray reject;
//            QJsonObject rejectJsonObject;
//            QString strRejectCode = "HD_M201_INSERT_MACHINE_1_1";
//            rejectJsonObject.insert("RATE",QString::number(throwrate));
//            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
//            rejectJsonObject.insert("STATION","InsertMachine1");
//            QString ratemsg = QString(u8"异型插1抛料率报警,标准值为:%1,实际值为:%2").arg(m_pInsertMachine1StandardThrowRate).
//                    arg(throwrate);
//            rejectJsonObject.insert("RATE_MESSAGE",ratemsg);
            if(GDataFactory::get_factory()->isLess(throwrate,m_pInsertMachine1StandardThrowRate))
//            if(1)
            {
//                rejectJsonObject.insert("STATE","0");
//                reject.append(rejectJsonObject);
                ;
            }
            else
            {
                QJsonObject rejectJsonObject;
                QString strRejectCode = "HD_M201_INSERT_MACHINE_1_1";
                rejectJsonObject.insert("RATE",QString::number(throwrate));
                rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
                rejectJsonObject.insert("STATION","InsertMachine1");
                QString ratemsg = QString(u8"异型插1抛料率报警,标准值为:%1,实际值为:%2").arg(m_pInsertMachine1StandardThrowRate).
                        arg(throwrate);
                rejectJsonObject.insert("RATE_MESSAGE",ratemsg);

                if(producequantity > 100)
                    rejectJsonObject.insert("STATE","1");
                else
                    rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }

            if(m_pInsertMachine1DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                m_pInsertMachine1DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
            else
                m_pInsertMachine1DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);

        }
    }
}

static int insert_machine_2_quantity = -1;
void InsertMachineBusinessM201Obj::slot_info_from_insert_machine2(QVector<unsigned short> data)
{
    if(data.length() == 20)
    {
        QString status = "";
        for (int i=0;i<data.length();i++) {
            if(data[i] == 0)
                continue;
            QByteArray aa;
            aa.append(static_cast<char>(data[i]>>8));
            char c = static_cast<char>(aa[0]);
            status.append(c);
            aa.clear();
            if((data[i] & 0xFF) != 0){
                aa.append(static_cast<char>(data[i] & 0xFF));
                c = static_cast<char>(aa[0]);
                status.append(c);
            }
        }

//        QLOG_ERROR()<<"the status of insert-machine2 is:"<<status;
        if(status.contains("Running"))
        {
            QLOG_INFO()<<"the current state of insert-machine2 is Running";
            if(m_pInsertMachine2DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine2DataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
            else
                m_pInsertMachine2DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
        }
        else if(status.contains("Error"))
        {
            QLOG_INFO()<<"the current state of insert-machine2 is Stopping";
            if(m_pInsertMachine2DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine2DataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
            else
                m_pInsertMachine2DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }
        else
        {
            QLOG_INFO()<<"the current state of insert-machine2 is Pausing";
            if(m_pInsertMachine2DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine2DataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
            else
                m_pInsertMachine2DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
        }
    }
    else {
        if(data.length() == 8)
        {
            int producequantity = data[0]*0xffff + data[1];
//            QLOG_ERROR()<<u8"异型插2产量为:"<<producequantity;

            QTime time = QTime::currentTime();
            if(time.minute() == 0)
            {
                if(insert_machine_2_quantity == -1)
                    insert_machine_2_quantity = producequantity;
                else
                {
                    if((producequantity-insert_machine_2_quantity)<30)
                        ;
                    else {
                        QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                        QLOG_ERROR()<<u8"异型插B"<<strhour<<QString::number(producequantity-insert_machine_2_quantity);
                        insert_machine_2_quantity = producequantity;
                    }

                }
            }

            int searchfailed = data[4]*0xffff + data[5];
            int pickfailed = data[6]*0xffff + data[7];

            double throwrate = static_cast<double>(searchfailed + pickfailed)/static_cast<double>(producequantity);

//            QLOG_ERROR()<<u8"异型插2抛料率实际值:"<<throwrate;
//            QLOG_ERROR()<<u8"异型插2抛料率标准值:"<<m_pInsertMachine2StandardThrowRate;
            int tmpThrowrate = throwrate*1000;
            throwrate = static_cast<double>(tmpThrowrate)/1000.0;

            QJsonArray reject;
            QJsonObject rejectJsonObject;
            QString strRejectCode = "HD_M201_INSERT_MACHINE_2_1";
            rejectJsonObject.insert("RATE",QString::number(throwrate));
            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
            rejectJsonObject.insert("STATION","InsertMachine2");
            QString ratemsg = QString(u8"异型插2抛料率报警,标准值为:%1,实际值为:%2").arg(m_pInsertMachine2StandardThrowRate).
                    arg(throwrate);
            rejectJsonObject.insert("RATE_MESSAGE",ratemsg);
            if(GDataFactory::get_factory()->isLess(throwrate,m_pInsertMachine2StandardThrowRate))
//            if((throwrate<m_pInsertMachine2StandardThrowRate))
            {
                rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }
            else
            {
                QLOG_WARN()<<u8"异型插2抛料率报警";
                if(producequantity > 100)
                    rejectJsonObject.insert("STATE","1");
                else
                    rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }

            if(m_pInsertMachine2DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                m_pInsertMachine2DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
            else
                m_pInsertMachine2DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
        }
    }
}

static int insert_machine_3_quantity = -1;
void InsertMachineBusinessM201Obj::slot_info_from_insert_machine3(QVector<unsigned short> data)
{
    if(data.length() == 20)
    {
        QString status = "";
        for (int i=0;i<data.length();i++) {
            if(data[i] == 0)
                continue;
            QByteArray aa;
            aa.append(static_cast<char>(data[i]>>8));
            char c = static_cast<char>(aa[0]);
            status.append(c);
            aa.clear();
            if((data[i] & 0xFF) != 0){
                aa.append(static_cast<char>(data[i] & 0xFF));
                c = static_cast<char>(aa[0]);
                status.append(c);
            }
        }

//        QLOG_ERROR()<<"the status of insert-machine3 is:"<<status;
        if(status.contains("Running"))
        {
            QLOG_INFO()<<"the current state of insert-machine3 is Running";
            if(m_pInsertMachine3DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine3DataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
            else
                m_pInsertMachine3DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
        }
        else if(status.contains("Error"))
        {
            QLOG_INFO()<<"the current state of insert-machine3 is Stopping";
            if(m_pInsertMachine3DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine3DataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
            else
                m_pInsertMachine3DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }
        else
        {
            QLOG_INFO()<<"the current state of insert-machine3 is Pausing";
            if(m_pInsertMachine3DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine3DataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
            else
                m_pInsertMachine3DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
        }
    }
    else {
        if(data.length() == 8)
        {
            int producequantity = data[0]*0xffff + data[1];
//            QLOG_ERROR()<<u8"异型插3产量为:"<<producequantity;

            QTime time = QTime::currentTime();
            if(time.minute() == 0)
            {
                if(insert_machine_3_quantity == -1)
                    insert_machine_3_quantity = producequantity;
                else
                {
                    if((producequantity-insert_machine_3_quantity)<30)
                        ;
                    else {
                        QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                        QLOG_ERROR()<<u8"异型插C"<<strhour<<QString::number(producequantity-insert_machine_3_quantity);
                        insert_machine_3_quantity = producequantity;
                    }

                }
            }

            int searchfailed = data[4]*0xffff + data[5];
            int pickfailed = data[6]*0xffff + data[7];

            double throwrate = static_cast<double>(searchfailed + pickfailed)/static_cast<double>(producequantity);

//            QLOG_ERROR()<<u8"异型插3抛料率实际值:"<<throwrate;
//            QLOG_ERROR()<<u8"异型插3抛料率标准值:"<<m_pInsertMachine3StandardThrowRate;
            int tmpThrowrate = throwrate*1000;
            throwrate = static_cast<double>(tmpThrowrate)/1000.0;

            QJsonArray reject;
            QJsonObject rejectJsonObject;
            QString strRejectCode = "HD_M201_INSERT_MACHINE_3_1";
            rejectJsonObject.insert("RATE",QString::number(throwrate));
            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
            rejectJsonObject.insert("STATION","InsertMachine3");
            QString ratemsg = QString(u8"异型插3抛料率报警,标准值为:%1,实际值为:%2").arg(m_pInsertMachine3StandardThrowRate).
                    arg(throwrate);
            rejectJsonObject.insert("RATE_MESSAGE",ratemsg);
            if(GDataFactory::get_factory()->isLess(throwrate,m_pInsertMachine3StandardThrowRate))
//            if((throwrate<m_pInsertMachine3StandardThrowRate))
            {
                rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }
            else
            {
                if(producequantity > 100)
                    rejectJsonObject.insert("STATE","1");
                else
                    rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }

            if(m_pInsertMachine3DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                m_pInsertMachine3DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
            else
                m_pInsertMachine3DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
        }
    }
}

static int insert_machine_4_quantity = -1;
void InsertMachineBusinessM201Obj::slot_info_from_insert_machine4(QVector<unsigned short> data)
{
    if(data.length() == 20)
    {
        QString status = "";
        for (int i=0;i<data.length();i++) {
            if(data[i] == 0)
                continue;
            QByteArray aa;
            aa.append(static_cast<char>(data[i]>>8));
            char c = static_cast<char>(aa[0]);
            status.append(c);
            aa.clear();
            if((data[i] & 0xFF) != 0){
                aa.append(static_cast<char>(data[i] & 0xFF));
                c = static_cast<char>(aa[0]);
                status.append(c);
            }
        }

//        QLOG_ERROR()<<"the status of insert-machine4 is:"<<status;
        if(status.contains("Running"))
        {
            QLOG_INFO()<<"the current state of insert-machine4 is Running";
            if(m_pInsertMachine4DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine4DataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
            else
                m_pInsertMachine4DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
        }
        else if(status.contains("Error"))
        {
            QLOG_INFO()<<"the current state of insert-machine4 is Stopping";
            if(m_pInsertMachine4DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine4DataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
            else
                m_pInsertMachine4DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }
        else
        {
            QLOG_INFO()<<"the current state of insert-machine4 is Pausing";
            if(m_pInsertMachine4DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pInsertMachine4DataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
            else
                m_pInsertMachine4DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
        }
    }
    else {
        if(data.length() == 8)
        {
            int producequantity = data[0]*0xffff + data[1];
//            QLOG_ERROR()<<u8"异型插4产量为:"<<producequantity;

            QTime time = QTime::currentTime();
            if(time.minute() == 0)
            {
                if(insert_machine_4_quantity == -1)
                    insert_machine_4_quantity = producequantity;
                else
                {
                    if((producequantity-insert_machine_4_quantity)<30)
                        ;
                    else {
                        QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                        QLOG_ERROR()<<u8"异型插D"<<strhour<<QString::number(producequantity-insert_machine_4_quantity);
                        insert_machine_4_quantity = producequantity;
                    }

                }
            }

            int searchfailed = data[4]*0xffff + data[5];
            int pickfailed = data[6]*0xffff + data[7];

            double throwrate = static_cast<double>(searchfailed + pickfailed)/static_cast<double>(producequantity);

//            QLOG_ERROR()<<u8"异型插4抛料率实际值:"<<throwrate;
//            QLOG_ERROR()<<u8"异型插4抛料率标准值:"<<m_pInsertMachine4StandardThrowRate;
            int tmpThrowrate = static_cast<int>(throwrate*1000.0);
            throwrate = static_cast<double>(tmpThrowrate)/1000.0;

            QJsonArray reject;
            QJsonObject rejectJsonObject;
            QString strRejectCode = "HD_M201_INSERT_MACHINE_4_1";
            rejectJsonObject.insert("RATE",QString::number(throwrate));
            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
            rejectJsonObject.insert("STATION","InsertMachine4");
            QString ratemsg = QString(u8"异型插3抛料率报警,标准值为:%1,实际值为:%2").arg(m_pInsertMachine4StandardThrowRate).
                    arg(throwrate);
            rejectJsonObject.insert("RATE_MESSAGE",ratemsg);
            if(GDataFactory::get_factory()->isLess(throwrate,m_pInsertMachine4StandardThrowRate))
//            if(throwrate<m_pInsertMachine4StandardThrowRate)
            {
                rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }
            else
            {
                if(producequantity > 100)
                    rejectJsonObject.insert("STATE","1");
                else
                    rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);
            }

            if(m_pInsertMachine4DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                m_pInsertMachine4DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
            else
                m_pInsertMachine4DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
        }
    }
}

QJsonArray format_error_json_array(const QString errorid,const QString errorcode)
{
    QJsonArray fault;
    QString tmpname = errorid;
    if(tmpname == "HD_M201_INSERT_MACHINE_1_")
        tmpname = u8"异型插1:";
    else if(tmpname == "HD_M201_INSERT_MACHINE_2_")
        tmpname = u8"异型插2:";
    else if(tmpname == "HD_M201_INSERT_MACHINE_3_")
        tmpname = u8"异型插3:";
    else if(tmpname == "HD_M201_INSERT_MACHINE_4_")
        tmpname = u8"异型插4:";
    else
        ;

    if(errorcode == "[Code:1002]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("2");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"安全胶盖被打开");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }
    else if(errorcode == "[Code:1011]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("3");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"头-轴马达出错");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }
    else if(errorcode == "[Code:1015]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("4");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"移动XY失败");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }
    else if(errorcode == "[Code:12120]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("5");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"抛料时发生失败");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }
    else if(errorcode == "[Code:5001]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("6");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"等待供料器到位超时");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }
    else if(errorcode == "[Code:2000]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("7");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"生产贴片数据中记录超出机器到达的范围");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }
    else if(errorcode == "[Code:1012]")
    {
        QString tmperrorid = errorid;
        tmperrorid.append("8");
        QJsonObject faultJsonObject;
        faultJsonObject.insert("FAULT_CODE",tmperrorid);
        tmpname.append(u8"轴移动超时");
        faultJsonObject.insert("FAULT_NAME",tmpname);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);
    }

    return fault;
}

void InsertMachineBusinessM201Obj::slot_current_error(const int stationid, const QString errorcode)
{
    QString strid = "";
    QJsonArray fault;
    switch (stationid)
    {
    case 1:
        strid = "HD_M201_INSERT_MACHINE_1_";
        fault = format_error_json_array(strid,errorcode);
        if(m_pInsertMachine1DataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pInsertMachine1DataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pInsertMachine1DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
        break;
    case 2:
        strid = "HD_M201_INSERT_MACHINE_2_";
        fault = format_error_json_array(strid,errorcode);
        if(m_pInsertMachine2DataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pInsertMachine2DataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pInsertMachine2DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
        break;
    case 3:
        strid = "HD_M201_INSERT_MACHINE_3_";
        fault = format_error_json_array(strid,errorcode);
        if(m_pInsertMachine3DataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pInsertMachine3DataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pInsertMachine3DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
        break;
    case 4:
        strid = "HD_M201_INSERT_MACHINE_4_";
        fault = format_error_json_array(strid,errorcode);
        if(m_pInsertMachine4DataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pInsertMachine4DataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pInsertMachine4DataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
        break;
    default:
        break;
    }
}

void InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine1()
{
    QLOG_WARN()<<"the MQTT state of insert-machine1-m201 is changed";
}

void InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine1()
{
    QLOG_WARN()<<"the MQTT of insert-machine1-m201 disconnect now";
}

void InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine2()
{
    QLOG_WARN()<<"the MQTT state of insert-machine2-m201 is changed";
}

void InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine2()
{
    QLOG_WARN()<<"the MQTT of insert-machine2-m201 disconnect now";
}

void InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine3()
{
    QLOG_WARN()<<"the MQTT state of insert-machine3-m201 is changed";
}

void InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine3()
{
    QLOG_WARN()<<"the MQTT of insert-machine3-m201 disconnect now";
}

void InsertMachineBusinessM201Obj::updateLogStateChangeInsertMachine4()
{
    QLOG_WARN()<<"the MQTT state of insert-machine4-m201 is changed";
}

void InsertMachineBusinessM201Obj::brokerDisconnectedInsertMachine4()
{
    QLOG_WARN()<<"the MQTT of insert-machine4-m201 disconnect now";
}
