#include "plcbusinessm201obj.h"
#include "QsLog.h"
#include "gdataacquisitionfactory.h"
#include <QSqlQuery>
#include <QTimerEvent>
#include <QDateTime>
#include <QTextCodec>
PlcBusinessM201Obj* PlcBusinessM201Obj::m_pInstance = nullptr;
PlcBusinessM201Obj::PlcBusinessM201Obj(QObject *parent) : QObject(parent)
{
    m_pUpBoardStatus = 0;
    m_pUpUtensilStatus = 0;
    m_pPickUpCoverStatus = 0;
    m_pDownUtensilStatus = 0;
    load_data_from_db();

    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        m_pUpBoardTimerID = -1;
        m_pUpBoardTimerID  = startTimer(5000);
        m_pUpUtensilTimerID = -1;
        m_pUpUtensilTimerID  = startTimer(5000);
        m_pPickUpCoverTimerID = -1;
        m_pPickUpCoverTimerID  = startTimer(5000);
        m_pDownUtensilTimerID = -1;
        m_pDownUtensilTimerID  = startTimer(5000);
    }

    m_pUpBoardMqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pUpBoardMqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pUpBoardMqttClient->setPort(pt);
    connect(m_pUpBoardMqttClient, &QMqttClient::stateChanged, this, &PlcBusinessM201Obj::updateLogStateChangeUpBoard);
    connect(m_pUpBoardMqttClient, &QMqttClient::disconnected, this, &PlcBusinessM201Obj::brokerDisconnectedUpBoard);

    m_pUpBoardMqttClient->setUsername(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_USR_NAME"));
    m_pUpBoardMqttClient->setPassword(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PASSWORD"));
    if(m_pUpBoardMqttClient->state() == QMqttClient::Disconnected)
        m_pUpBoardMqttClient->connectToHost();

    m_pUpUtensilMqttClient = new QMqttClient();
    strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pUpUtensilMqttClient->setHostname(strHostname);
    pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pUpUtensilMqttClient->setPort(pt);
    connect(m_pUpUtensilMqttClient, &QMqttClient::stateChanged, this, &PlcBusinessM201Obj::updateLogStateChangeUpUtensil);
    connect(m_pUpUtensilMqttClient, &QMqttClient::disconnected, this, &PlcBusinessM201Obj::brokerDisconnectedUpUtensil);

    m_pUpUtensilMqttClient->setUsername(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_USR_NAME"));
    m_pUpUtensilMqttClient->setPassword(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PASSWORD"));
    if(m_pUpUtensilMqttClient->state() == QMqttClient::Disconnected)
        m_pUpUtensilMqttClient->connectToHost();

    m_pPickUpCoverMqttClient = new QMqttClient();
    strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pPickUpCoverMqttClient->setHostname(strHostname);
    pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pPickUpCoverMqttClient->setPort(pt);
    connect(m_pPickUpCoverMqttClient, &QMqttClient::stateChanged, this, &PlcBusinessM201Obj::updateLogStateChangePickUpCover);
    connect(m_pPickUpCoverMqttClient, &QMqttClient::disconnected, this, &PlcBusinessM201Obj::brokerDisconnectedPickUpCover);

    m_pPickUpCoverMqttClient->setUsername(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_USR_NAME"));
    m_pPickUpCoverMqttClient->setPassword(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PASSWORD"));
    if(m_pPickUpCoverMqttClient->state() == QMqttClient::Disconnected)
        m_pPickUpCoverMqttClient->connectToHost();

    m_pDownUtensilMqttClient = new QMqttClient();
    strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pDownUtensilMqttClient->setHostname(strHostname);
    pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pDownUtensilMqttClient->setPort(pt);
    connect(m_pDownUtensilMqttClient, &QMqttClient::stateChanged, this, &PlcBusinessM201Obj::updateLogStateChangeDownUtensil);
    connect(m_pDownUtensilMqttClient, &QMqttClient::disconnected, this, &PlcBusinessM201Obj::brokerDisconnectedDownUtensil);

    m_pDownUtensilMqttClient->setUsername(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_USR_NAME"));
    m_pDownUtensilMqttClient->setPassword(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PASSWORD"));
    if(m_pDownUtensilMqttClient->state() == QMqttClient::Disconnected)
        m_pDownUtensilMqttClient->connectToHost();
}

void PlcBusinessM201Obj::load_data_from_db()
{
    //up-board
    QString strSql = QString("select * from public.%1").arg(constM201UpBoardWarningsTable);
    QSqlQuery queryResult;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get warning info, query database success!";
        while(queryResult.next())
        {
            QString strInfo="";
            strInfo.append(queryResult.value(1).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(2).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(3).toString());
            m_pMapWarningsInfoOfUpBoard.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
    //up-board

    //up-utensil
    strSql = QString("select * from public.%1").arg(constM201UpUtensilWarningsTable);
    queryResult.clear();
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get warning info, query database success!";
        while(queryResult.next())
        {
            QString strInfo="";
            strInfo.append(queryResult.value(1).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(2).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(3).toString());
            m_pMapWarningsInfoOfUtensil.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
    //up-utensil

    //pick-up-cover
    strSql = QString("select * from public.%1").arg(constM201PickUpCoverWarningsTable);
    queryResult.clear();
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get warning info, query database success!";
        while(queryResult.next())
        {
            QString strInfo="";
            strInfo.append(queryResult.value(1).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(2).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(3).toString());
            m_pMapWarningsInfoOfPickUpCover.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
    //pick-up-cover

    //down-utensil
    strSql = QString("select * from public.%1").arg(constM201DownUtensilWarningsTable);
    queryResult.clear();
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get warning info, query database success!";
        while(queryResult.next())
        {
            QString strInfo="";
            strInfo.append(queryResult.value(1).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(2).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(3).toString());
            m_pMapWarningsInfoOfDownUtensil.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
    //down-utensil
}

bool PlcBusinessM201Obj::find_warnings_of_up_board(const int warningid)
{
    bool result = false;

    QMap<int,QString>::const_iterator it;
    for (it=m_pMapWarningsInfoOfUpBoard.constBegin();it!=m_pMapWarningsInfoOfUpBoard.constEnd();++it) {
        if(it.key() == warningid)
            result = true;
        else
            continue;
    }

    return result;
}

bool PlcBusinessM201Obj::find_warnings_of_up_utensil(const int warningid)
{
    bool result = false;

    QMap<int,QString>::const_iterator it;
    for (it=m_pMapWarningsInfoOfUtensil.constBegin();it!=m_pMapWarningsInfoOfUtensil.constEnd();++it) {
        if(it.key() == warningid)
            result = true;
        else
            continue;
    }

    return result;
}

bool PlcBusinessM201Obj::find_warnings_of_pick_up_cover(const int warningid)
{
    bool result = false;

    QMap<int,QString>::const_iterator it;
    for (it=m_pMapWarningsInfoOfPickUpCover.constBegin();it!=m_pMapWarningsInfoOfPickUpCover.constEnd();++it) {
        if(it.key() == warningid)
            result = true;
        else
            continue;
    }

    return result;
}

bool PlcBusinessM201Obj::find_warnings_of_down_utensil(const int warningid)
{
    bool result = false;

    QMap<int,QString>::const_iterator it;
    for (it=m_pMapWarningsInfoOfDownUtensil.constBegin();it!=m_pMapWarningsInfoOfDownUtensil.constEnd();++it) {
        if(it.key() == warningid)
            result = true;
        else
            continue;
    }

    return result;
}

void PlcBusinessM201Obj::timerEvent(QTimerEvent *event)
{
    if(GDataAcquisitionFactory::get_instance()->isTimeBetween730And830() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1830And2030())
        return;

    if(event->timerId() == m_pUpBoardTimerID)
    {
//        mutex_up_board.lock();
        if(m_pUpBoardDataTable.size() != 2)
        {
            QLOG_WARN()<<"the size of Data in MQ from business-upboard is WRONG";
            return;
        }

//        QString strTopic = "HD_M201_UP_BOARD_TOPIC";
        QString strTopic = DataAcquisitionConfig::get_instance()->get_config_para("UP_BOARD_TOPIC");
        auto subscription = m_pUpBoardMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of upboard-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pUpBoardDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        fault = m_pUpBoardDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
//        data.insert("DeviceName","UpBoardM201");
//        data.insert("SystemNo","HS0543302024FZSB01028");
        data.insert("DeviceName",DataAcquisitionConfig::get_instance()->get_config_para("UP_BOARD_DEVICE_NAME"));
        data.insert("SystemNo",DataAcquisitionConfig::get_instance()->get_config_para("UP_BOARD_ASSET_NUMBER"));
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);

        QTextCodec *tc = QTextCodec::codecForName("utf-8");
        QString str = tc->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"up-board submit msg to andon:"<<str;

        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pUpBoardMqttClient->publish(strTopic,byteArray);
//        mutex_up_board.unlock();
    }
    else if(event->timerId() == m_pUpUtensilTimerID)
    {
        if(m_pUpUtensilDataTable.size() != 2)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

//        QString strTopic = "HD_M201_UP_UTENSIL_TOPIC";
        QString strTopic = DataAcquisitionConfig::get_instance()->get_config_para("UP_UTENSIL_TOPIC");
        auto subscription = m_pUpUtensilMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of uputensil-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pUpUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        fault = m_pUpUtensilDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
//        data.insert("DeviceName","UpUtensilM201");
//        data.insert("SystemNo","HS0543302024FZSB01017");
        data.insert("DeviceName",DataAcquisitionConfig::get_instance()->get_config_para("UP_UTENSIL_DEVICE_NAME"));
        data.insert("SystemNo",DataAcquisitionConfig::get_instance()->get_config_para("UP_UTENSIL_ASSET_NUMBER"));
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *tc = QTextCodec::codecForName("utf-8");
        QString str = tc->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"up-utensil submit msg to andon:"<<str;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pUpUtensilMqttClient->publish(strTopic,byteArray);
    }
    else if(event->timerId() == m_pPickUpCoverTimerID)
    {
        if(m_pPickUpCoverDataTable.size() != 2)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

//        QString strTopic = "HD_M201_PICK_UP_COVER_TOPIC";
        QString strTopic = DataAcquisitionConfig::get_instance()->get_config_para("PICK_UP_COVER_TOPIC");
        auto subscription = m_pPickUpCoverMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of pickupcover-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pPickUpCoverDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        fault = m_pPickUpCoverDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
//        data.insert("DeviceName","PickUpCoverM201");
//        data.insert("SystemNo","HS0543302024FZSB01040");
        data.insert("DeviceName",DataAcquisitionConfig::get_instance()->get_config_para("PICK_UP_COVER_DEVICE_NAME"));
        data.insert("SystemNo",DataAcquisitionConfig::get_instance()->get_config_para("PICK_UP_COVER_ASSET_NUMBER"));
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *tc = QTextCodec::codecForName("utf-8");
        QString str = tc->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"pick-up-cover submit msg to andon:"<<str;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pPickUpCoverMqttClient->publish(strTopic,byteArray);
    }
    else if(event->timerId() == m_pDownUtensilTimerID)
    {
        if(m_pDownUtensilDataTable.size() != 2)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

//        QString strTopic = "HD_M201_DOWN_UTENSIL_TOPIC";
        QString strTopic = DataAcquisitionConfig::get_instance()->get_config_para("DOWN_UTENSIL_TOPIC");
        auto subscription = m_pDownUtensilMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of downutensil-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pDownUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        fault = m_pDownUtensilDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
//        data.insert("DeviceName","DownUtensilM201");
//        data.insert("SystemNo","HS0543302024FZSB01026");
        data.insert("DeviceName",DataAcquisitionConfig::get_instance()->get_config_para("DOWN_UTENSIL_DEVICE_NAME"));
        data.insert("SystemNo",DataAcquisitionConfig::get_instance()->get_config_para("DOWN_UTENSIL_ASSET_NUMBER"));
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *tc = QTextCodec::codecForName("utf-8");
        QString str = tc->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"down-utensil submit msg to andon:"<<str;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pDownUtensilMqttClient->publish(strTopic,byteArray);
    }
}

//static int up_utensil_warning_id = 0;
static int up_utensil_quantity = -1;
void PlcBusinessM201Obj::slot_up_utensil_info(QVector<unsigned short> data)
{
    if(data.length() != 17)
        return;

#pragma region "machine-status"
        if(data[0] == 1)
        {
            if(m_pUpUtensilStatus != 1)
            {
                m_pUpUtensilStatus = 1;
                QLOG_WARN()<<"the status of up-utensil change to Stopping";
            }
            if(m_pUpUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pUpUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
            else
                m_pUpUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }
        else if(data[0] == 2)
        {
            if(m_pUpUtensilStatus != 2)
            {
                m_pUpUtensilStatus = 2;
                QLOG_WARN()<<"the status of up-utensil change to Running";
            }
            if(m_pUpUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pUpUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
            else
                m_pUpUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
        }
        else
        {
            if(m_pUpUtensilStatus != 3)
            {
                m_pUpUtensilStatus = 3;
                QLOG_WARN()<<"the status of up-utensil change to Pausing";
            }
            if(m_pUpUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                m_pUpUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
            else
                m_pUpUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
        }
#pragma endregion

#pragma region "warning"
        int warningid = data[2];

        //just for test,delete later
//        if(up_utensil_warning_id == 88)
//            up_utensil_warning_id = 0;
//        up_utensil_warning_id++;
//        warningid = up_utensil_warning_id;
        //just for test,delete later
        if(find_warnings_of_up_utensil(warningid))
        {
            QStringList lst = m_pMapWarningsInfoOfUtensil[warningid].split(',');
            //当有故障时，设备状态设置为停机
            if(m_pUpUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            {
                m_pUpUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
            }
            else
            {
                m_pUpUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
            }

            QJsonArray fault;
            QJsonObject faultJsonObject;
            QString strFaultCode = "HD_M201_UP_UTENSIL_";
            strFaultCode = strFaultCode.append(QString::number(warningid));
            faultJsonObject.insert("FAULT_CODE",strFaultCode);
            lst[0].prepend(u8"上夹具");
            faultJsonObject.insert("FAULT_NAME",lst[0]);
            QLOG_WARN()<<u8"上夹具停机原因为:"<<lst[0];
            faultJsonObject.insert("STATE","1");
            fault.append(faultJsonObject);

            if(m_pUpUtensilDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
                m_pUpUtensilDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
            else
                m_pUpUtensilDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
        }
        else
        {
            QJsonArray fault;

            if(m_pUpUtensilDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
                m_pUpUtensilDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
            else
                m_pUpUtensilDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
        }
#pragma endregion

#pragma region "quantity"
        int producequantity = data[10]+data[11]*0xFFFF;
        QTime time = QTime::currentTime();
        if(time.minute() == 0)
        {
            if(up_utensil_quantity == -1)
                up_utensil_quantity = producequantity;
            else
            {
                if((producequantity-up_utensil_quantity)<10)
                    ;
                else {
                    QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                    QLOG_ERROR()<<u8"上夹具机"<<strhour<<QString::number(producequantity-up_utensil_quantity);
                    up_utensil_quantity = producequantity;
                }

            }
        }

        QLOG_WARN()<<u8"上夹具累积产量为:"<<producequantity;
#pragma endregion
}

//static int up_board_warning_id = 0;
static int up_board_quantity = -1;
void PlcBusinessM201Obj::slot_up_board_info(QVector<unsigned short> data)
{
//    mutex_up_board.lock();
    if(data.length() != 17)
        return;

#pragma region "machine-status"
    if(data[0] == 1)
    {
        if(m_pUpBoardStatus != 1)
        {
            m_pUpBoardStatus = 1;
            QLOG_WARN()<<"the status of up-board change to Stopping";
        }
        if(m_pUpBoardDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pUpBoardDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        else
            m_pUpBoardDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
    }
    else if(data[0] == 2)
    {
        if(m_pUpBoardStatus != 2)
        {
            m_pUpBoardStatus = 2;
            QLOG_WARN()<<"the status of up-board change to Running";
        }
        if(m_pUpBoardDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pUpBoardDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
        else
            m_pUpBoardDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
    }
    else
    {
        if(m_pUpBoardStatus != 3)
        {
            m_pUpBoardStatus = 3;
            QLOG_WARN()<<"the status of up-board change to Pausing";
        }
        if(m_pUpBoardDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pUpBoardDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
        else
            m_pUpBoardDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
    }
#pragma endregion

#pragma region "warning"
    int warningid = data[2];
//    if(up_board_warning_id == 31)
//        up_board_warning_id = 0;
//    up_board_warning_id++;
//    warningid = up_board_warning_id;
    if(find_warnings_of_up_board(warningid))
    {
        QStringList lst = m_pMapWarningsInfoOfUpBoard[warningid].split(',');
//        QLOG_ERROR()<<"the warningid of up-board is:"<<warningid;
//        QLOG_ERROR()<<lst[0];
        //当有故障时，设备状态设置为停机
        if(m_pUpBoardDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
        {
            m_pUpBoardDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        }
        else
        {
            m_pUpBoardDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }

        QJsonArray fault;
        QJsonObject faultJsonObject;
        QString strFaultCode = "HD_M201_UP_BOARD_";
        strFaultCode = strFaultCode.append(QString::number(warningid));
        faultJsonObject.insert("FAULT_CODE",strFaultCode);
        lst[0].prepend(u8"上板机");
        faultJsonObject.insert("FAULT_NAME",lst[0]);
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);

        if(m_pUpBoardDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pUpBoardDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pUpBoardDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
    else
    {
        QJsonArray fault;

        if(m_pUpBoardDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pUpBoardDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pUpBoardDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
#pragma endregion

#pragma region "quantity"
    int producequantity = data[10]+data[11]*0xFFFF;
    QTime time = QTime::currentTime();    
    if(time.minute() == 0)
    {
        if(up_board_quantity == -1)
            up_board_quantity = producequantity;
        else
        {
            if((producequantity-up_board_quantity)<10)
                ;
            else
            {
                QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                QLOG_ERROR()<<u8"上板机"<<strhour<<QString::number(producequantity-up_board_quantity);
                up_board_quantity = producequantity;
            }
        }
    }
    QLOG_WARN()<<u8"上板机产量为:"<<producequantity;
#pragma endregion

//    mutex_up_board.unlock();
}

//static int pick_up_cover_warning_id = 0;
static int pick_up_cover_quantity = -1;
void PlcBusinessM201Obj::slot_pick_up_cover_info(QVector<unsigned short> data)
{
    if(data.length() != 17)
        return;

#pragma region "machine-status"
    if(data[0] == 1)
    {
        if(m_pPickUpCoverStatus != 1)
        {
            m_pPickUpCoverStatus = 1;
            QLOG_WARN()<<"the status of pick-up-cover change to Stopping";
        }
        if(m_pPickUpCoverDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pPickUpCoverDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        else
            m_pPickUpCoverDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
    }
    else if(data[0] == 2)
    {
        if(m_pPickUpCoverStatus != 2)
        {
            m_pPickUpCoverStatus = 2;
            QLOG_WARN()<<"the status of pick-up-cover change to Running";
        }
        if(m_pPickUpCoverDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pPickUpCoverDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
        else
            m_pPickUpCoverDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
    }
    else
    {
        if(m_pPickUpCoverStatus != 3)
        {
            m_pPickUpCoverStatus = 3;
            QLOG_WARN()<<"the status of pick-up-cover change to Pausing";
        }
        if(m_pPickUpCoverDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pPickUpCoverDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
        else
            m_pPickUpCoverDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
    }
#pragma endregion

#pragma region "warning"
    int warningid = data[2];
    //just for test,delete later
//    if(pick_up_cover_warning_id == 64)
//        pick_up_cover_warning_id = 0;
//    pick_up_cover_warning_id++;
//    warningid = pick_up_cover_warning_id;
    //just for test,delete later
    if(find_warnings_of_pick_up_cover(warningid))
    {
        QStringList lst = m_pMapWarningsInfoOfPickUpCover[warningid].split(',');
        //当有故障时，设备状态设置为停机
        if(m_pPickUpCoverDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
        {
            m_pPickUpCoverDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        }
        else
        {
            m_pPickUpCoverDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }

        QJsonArray fault;
        QJsonObject faultJsonObject;
        QString strFaultCode = "HD_M201_PICK_UP_COVER_";
        strFaultCode = strFaultCode.append(QString::number(warningid));
        faultJsonObject.insert("FAULT_CODE",strFaultCode);
        lst[0].prepend(u8"取上盖机");
        faultJsonObject.insert("FAULT_NAME",lst[0]);
        QLOG_WARN()<<u8"取上盖机停机原因为:"<<lst[0];
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);

        if(m_pPickUpCoverDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pPickUpCoverDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pPickUpCoverDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
    else
    {
        QJsonArray fault;

        if(m_pPickUpCoverDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pPickUpCoverDataTable[DATA_TYPE_M201::Fault_Content_M201] = "";
        else
            m_pPickUpCoverDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,"");
    }
#pragma endregion

#pragma region "quantity"
    int producequantity = data[10]+data[11]*0xFFFF;

    QTime time = QTime::currentTime();
    if(time.minute() == 0)
    {
        if(pick_up_cover_quantity == -1)
            pick_up_cover_quantity = producequantity;
        else
        {
            if((producequantity-pick_up_cover_quantity)<10)
                ;
            else {
                QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                QLOG_ERROR()<<u8"取上盖机"<<strhour<<QString::number(producequantity-pick_up_cover_quantity);
                pick_up_cover_quantity = producequantity;
            }

        }
    }

    QLOG_WARN()<<u8"取上盖机累积产量为:"<<producequantity;
#pragma endregion
}

//static int down_utensil_warning_id = 0;
static int down_utensil_quantity = -1;
void PlcBusinessM201Obj::slot_down_utensil_info(QVector<unsigned short> data)
{
    if(data.length() != 17)
        return;

    // "machine-status"
    if(data[0] == 1)
    {
        if(m_pDownUtensilStatus != 1)
        {
            m_pDownUtensilStatus = 1;
            QLOG_WARN()<<"the status of down-utensil change to Stopping";
        }
        if(m_pDownUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDownUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        else
            m_pDownUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
    }
    else if(data[0] == 2)
    {
        if(m_pDownUtensilStatus != 2)
        {
            m_pDownUtensilStatus = 2;
            QLOG_WARN()<<"the status of down-utensil change to Running";
        }
        if(m_pDownUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDownUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
        else
            m_pDownUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
    }
    else
    {
        if(m_pDownUtensilStatus != 3)
        {
            m_pDownUtensilStatus = 3;
            QLOG_WARN()<<"the status of down-utensil change to Pausing";
        }
        if(m_pDownUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDownUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
        else
            m_pDownUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
    }

    // "warning"
    int warningid = data[2];
    //just for test,delete later
//    if(down_utensil_warning_id == 83)
//        down_utensil_warning_id = 0;
//    down_utensil_warning_id++;
//    warningid = down_utensil_warning_id;
    //just for test,delete later
    if(find_warnings_of_down_utensil(warningid))
    {
        QStringList lst = m_pMapWarningsInfoOfDownUtensil[warningid].split(',');
        //当有故障时，设备状态设置为停机
        if(m_pDownUtensilDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
        {
            m_pDownUtensilDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        }
        else
        {
            m_pDownUtensilDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
        }

        QJsonArray fault;
        QJsonObject faultJsonObject;
        QString strFaultCode = "HD_M201_DOWN_UTENSIL_";
        strFaultCode = strFaultCode.append(QString::number(warningid));
        faultJsonObject.insert("FAULT_CODE",strFaultCode);
        lst[0].prepend(u8"下夹具机");
        faultJsonObject.insert("FAULT_NAME",lst[0]);
        QLOG_WARN()<<u8"下夹具停机原因为:"<<lst[0];
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);

        if(m_pDownUtensilDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pDownUtensilDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pDownUtensilDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
    else
    {
        QJsonArray fault;

        if(m_pDownUtensilDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pDownUtensilDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pDownUtensilDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }

    int producequantity = data[10]+data[11]*0xFFFF;

    QTime time = QTime::currentTime();
    if(time.minute() == 0)
    {
        if(down_utensil_quantity == -1)
            down_utensil_quantity = producequantity;
        else
        {
            if((producequantity-down_utensil_quantity)<10)
                ;
            else {
                QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                QLOG_ERROR()<<u8"下夹具机"<<strhour<<QString::number(producequantity-down_utensil_quantity);
                down_utensil_quantity = producequantity;
            }

        }
    }

    QLOG_WARN()<<u8"下夹具累积产量为:"<<producequantity;
}

void PlcBusinessM201Obj::updateLogStateChangeUpBoard()
{
    QLOG_WARN()<<"the MQTT state of upboard-m201 is changed";
}

void PlcBusinessM201Obj::brokerDisconnectedUpBoard()
{
    QLOG_WARN()<<"the MQTT of upboard-m201 disconnect now";
}

void PlcBusinessM201Obj::updateLogStateChangeUpUtensil()
{
    QLOG_WARN()<<"the MQTT state of uputensil-m201 is changed";
}

void PlcBusinessM201Obj::brokerDisconnectedUpUtensil()
{
    QLOG_WARN()<<"the MQTT of uputensil-m201 disconnect now";
}

void PlcBusinessM201Obj::updateLogStateChangePickUpCover()
{
    QLOG_WARN()<<"the MQTT state of pickupcover-m201 is changed";
}

void PlcBusinessM201Obj::brokerDisconnectedPickUpCover()
{
    QLOG_WARN()<<"the MQTT of pickupcover-m201 disconnect now";
}

void PlcBusinessM201Obj::updateLogStateChangeDownUtensil()
{
    QLOG_WARN()<<"the MQTT state of downutensil-m201 is changed";
}

void PlcBusinessM201Obj::brokerDisconnectedDownUtensil()
{
    QLOG_WARN()<<"the MQTT of downutensil-m201 disconnect now";
}
