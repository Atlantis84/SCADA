#include "aoibusinessm201obj.h"
#include <QDomDocument>
#include <QDateTime>
#include "QsLog.h"
#include <QTimerEvent>
#include "dataacquisitionconfig.h"
#include "gdataacquisitionfactory.h"
#include <QTextCodec>
AOIBusinessM201Obj* AOIBusinessM201Obj::m_pInstance = nullptr;

AOIBusinessM201Obj::AOIBusinessM201Obj(QObject *parent) : QObject(parent)
{
    m_pTcpCommonUse = new TcpCommonUse();
    m_pCurrentRecipeName = "";
    m_pStandardDefectRate = DataAcquisitionConfig::get_instance()->get_config_para("DEFECT_RATE").toDouble();
    m_pRealDefectRate = 0.0;

    m_pDefectHour = QTime::currentTime().hour();

    m_pMqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pMqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pMqttClient->setPort(pt);
    connect(m_pMqttClient, &QMqttClient::stateChanged, this, &AOIBusinessM201Obj::updateLogStateChange);
    connect(m_pMqttClient, &QMqttClient::disconnected, this, &AOIBusinessM201Obj::brokerDisconnected);

    m_pMqttClient->setUsername("admin");
    m_pMqttClient->setPassword("Admin12345");
    if(m_pMqttClient->state() == QMqttClient::Disconnected)
        m_pMqttClient->connectToHost();
}

static int timerCount = 0;
void AOIBusinessM201Obj::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(event->timerId() == m_pTimerID)
    {
        timerCount++;
        if((timerCount % 3) == 0)
        {
            m_pTcpCommonUse->send_data_to_server(generate_common_xml("EAP_LinkTest_Request"));
        }
    }

    if (event->timerId() == m_pClockChecker) {
//        QTime time = QTime::currentTime();
//        if((time.minute() == 5) && (time.second() == 0))
//        QLOG_ERROR()<<"get defect rate";
            GDataAcquisitionFactory::get_instance()->exec_http_get_method_to_get_aoi_defect();
    }

    if(event->timerId() == m_pAcquisiteTimerID)
    {
        auto subscription = m_pMqttClient->subscribe(QString("HD_M201_AOI_TOPIC"));
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of aoi-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus = "3";
        QJsonObject rejectJsonObject;
        QString strRejectCode = "";
        systemstatus = m_pDefectRateDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();

//        QLOG_ERROR()<<"SystemState is:"<<systemstatus;
        if(systemstatus == "")
            systemstatus = "3";
        data.insert("SystemState",systemstatus);

        if(GDataAcquisitionFactory::get_instance()->isEqual(m_pRealDefectRate,1000000.0))
        {
//            data.insert("SystemState","2");
            data.insert("ORG","4330");
            data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
            data.insert("DeviceName","AOI");
            data.insert("SystemNo","HS0443302021000000023");
            data.insert("CONTENT",content);
            data.insert("FAULT",fault);

            strRejectCode = "HD_M201_AOI_1";
            rejectJsonObject.insert("RATE",QString::number(0));
            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
            rejectJsonObject.insert("STATION","AOI");
            rejectJsonObject.insert("RATE_MESSAGE",u8"");
            rejectJsonObject.insert("STATE","0");
            rejectrate.append(rejectJsonObject);

            data.insert("REJECTRATE",rejectrate);
        }
        else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pRealDefectRate,m_pStandardDefectRate))
        {
//            data.insert("SystemState","2");
            data.insert("ORG","4330");
            data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
            data.insert("DeviceName","AOI");
            data.insert("SystemNo","HS0443302021000000023");
            data.insert("CONTENT",content);
            data.insert("FAULT",fault);

            strRejectCode = "HD_M201_AOI_1";
            rejectJsonObject.insert("RATE",QString::number(m_pRealDefectRate));
            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
            rejectJsonObject.insert("STATION","AOI");
            QString info = QString(u8"疵点率超限报警，标准值:170，实时值:%1").arg(m_pRealDefectRate);
            info.append(u8"，报警时间为:");
            QString tmpDefectHour = QString::number(m_pDefectHour+1);
            QString tmpDefectHourPlus = QString::number(m_pDefectHour+2);
            info.append(tmpDefectHour);
            info.append("-");
            info.append(tmpDefectHourPlus);
            info.append(u8"时");
            rejectJsonObject.insert("RATE_MESSAGE",info);
            rejectJsonObject.insert("STATE","1");
            rejectrate.append(rejectJsonObject);

            data.insert("REJECTRATE",rejectrate);
        }
        else
        {
//            data.insert("SystemState","2");
            data.insert("ORG","4330");
            data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
            data.insert("DeviceName","AOI");
            data.insert("SystemNo","HS0443302021000000023");
            data.insert("CONTENT",content);
            data.insert("FAULT",fault);

            strRejectCode = "HD_M201_AOI_1";
            rejectJsonObject.insert("RATE",QString::number(m_pRealDefectRate));
            rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
            rejectJsonObject.insert("STATION","AOI");
            rejectJsonObject.insert("RATE_MESSAGE",u8"");
            rejectJsonObject.insert("STATE","0");
            rejectrate.append(rejectJsonObject);

            data.insert("REJECTRATE",rejectrate);
        }

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QString tmpData = codec->toUnicode(jsonDoc.toJson());
        QLOG_INFO()<<"aoi submit msg to andon:"<<tmpData;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pMqttClient->publish(QString("HD_M201_AOI_TOPIC"),byteArray);
    }
}

QByteArray AOIBusinessM201Obj::generate_common_xml(QString xmlsign)
{
    QDomDocument doc;

    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    //添加根节点
    QDomElement root=doc.createElement("Message");
    doc.appendChild(root);

    //添加第一个子节点及其子元素
    QDomElement header_node = doc.createElement("Header");

    QDomElement message_name_node = doc.createElement("MESSAGENAME");
    QDomText message_name_node_text = doc.createTextNode(xmlsign);
    message_name_node.appendChild(message_name_node_text);
    header_node.appendChild(message_name_node);

    QDomElement transaction_id_node = doc.createElement("TRANSACTIONID");
    QDomText transaction_id_text = doc.createTextNode(get_time_stamp());
    transaction_id_node.appendChild(transaction_id_text);
    header_node.appendChild(transaction_id_node);

    QDomElement reply_subject_name_node = doc.createElement("REPLYSUBJECTNAME");
    QDomText reply_subject_name_text = doc.createTextNode("127.0.0.1:0000");
    reply_subject_name_node.appendChild(reply_subject_name_text);
    header_node.appendChild(reply_subject_name_node);

    root.appendChild(header_node);

    QDomElement body_node = doc.createElement("Body");

    QDomElement equipment_id_node = doc.createElement("EquipmentID");
    QDomText equipment_id_text = doc.createTextNode("THT01");
    equipment_id_node.appendChild(equipment_id_text);
    body_node.appendChild(equipment_id_node);

    root.appendChild(body_node);

    QDomElement return_node = doc.createElement("Return");

    QDomElement return_code_node = doc.createElement("ReturnCode");
    QDomText return_code_text = doc.createTextNode("");
    return_code_node.appendChild(return_code_text);
    QDomElement return_message_node = doc.createElement("ReturnMessage");
    QDomText return_message_text = doc.createTextNode("");
    return_message_node.appendChild(return_message_text);
    return_node.appendChild(return_code_node);
    return_node.appendChild(return_message_node);

    root.appendChild(return_node);

    return doc.toByteArray();
}

QString AOIBusinessM201Obj::get_time_stamp()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString format = "yyyyMMddHHmmssffff";
    QString timestamp = currentDateTime.toString(format);
    timestamp.append("00000");
    return timestamp;
}

void AOIBusinessM201Obj::process_info_from_tcp_server(const QByteArray &data)
{
    QLOG_INFO()<<"the info from AOI is:"<<data;
    parse_xml_file(data);
}

void AOIBusinessM201Obj::updateLogStateChange()
{

}

void AOIBusinessM201Obj::brokerDisconnected()
{

}

void AOIBusinessM201Obj::start_to_data_acquisite()
{
    m_pTimerID = startTimer(1000);
    m_pAcquisiteTimerID = startTimer(5000);
    m_pClockChecker = startTimer(5000);
}

void AOIBusinessM201Obj::change_production_of_aoi(QString recipename)
{
    m_pCurrentRecipeName = recipename;
    QDomDocument doc;

    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    //添加根节点
    QDomElement root=doc.createElement("Message");
    doc.appendChild(root);

    //添加第一个子节点及其子元素
    QDomElement header_node = doc.createElement("Header");

    QDomElement message_name_node = doc.createElement("MESSAGENAME");
    QDomText message_name_node_text = doc.createTextNode("RECIPE_PARAMETER_DOWNLOAD_COMMAND");
    message_name_node.appendChild(message_name_node_text);
    header_node.appendChild(message_name_node);

    QDomElement transaction_id_node = doc.createElement("TRANSACTIONID");
    QDomText transaction_id_text = doc.createTextNode("");
    transaction_id_node.appendChild(transaction_id_text);
    header_node.appendChild(transaction_id_node);

    QDomElement reply_subject_name_node = doc.createElement("REPLYSUBJECTNAME");
    QDomText reply_subject_name_text = doc.createTextNode("");
    reply_subject_name_node.appendChild(reply_subject_name_text);
    header_node.appendChild(reply_subject_name_node);

    root.appendChild(header_node);

    QDomElement body_node = doc.createElement("Body");

    QDomElement equipment_id_node = doc.createElement("EquipmentID");
    QDomText equipment_id_text = doc.createTextNode("THT01");
    equipment_id_node.appendChild(equipment_id_text);
    body_node.appendChild(equipment_id_node);

    QDomElement recipe_id_node = doc.createElement("RecipeID");
    QDomText recipe_id_text = doc.createTextNode(recipename);
    recipe_id_node.appendChild(recipe_id_text);
    body_node.appendChild(recipe_id_node);

    QDomElement recipe_flag_node = doc.createElement("RecipeFlag");
    QDomText recipe_flag_text = doc.createTextNode("true");
    recipe_flag_node.appendChild(recipe_flag_text);
    body_node.appendChild(recipe_flag_node);

    root.appendChild(body_node);

    QDomElement return_node = doc.createElement("Return");

    QDomElement return_code_node = doc.createElement("ReturnCode");
    QDomText return_code_text = doc.createTextNode("");
    return_code_node.appendChild(return_code_text);
    QDomElement return_message_node = doc.createElement("ReturnMessage");
    QDomText return_message_text = doc.createTextNode("");
    return_message_node.appendChild(return_message_text);
    return_node.appendChild(return_code_node);
    return_node.appendChild(return_message_node);

    root.appendChild(return_node);
    QLOG_ERROR()<<doc.toString();

    m_pTcpCommonUse->send_data_to_server(doc.toByteArray());
}

void AOIBusinessM201Obj::change_production_of_aoi()
{
    QJsonObject  root;
    root.insert("lineName","C202");
    root.insert("stationName","INSERT");
    root.insert("testFileName","13014-1线");
    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    m_pTcpCommonUse->send_data_to_server(request_data);
}

void AOIBusinessM201Obj::start_or_stop_equipment()
{
    QDomDocument doc;

    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    //添加根节点
    QDomElement root=doc.createElement("Message");
    doc.appendChild(root);

    //添加第一个子节点及其子元素
    QDomElement header_node = doc.createElement("Header");

    QDomElement message_name_node = doc.createElement("MESSAGENAME");
    QDomText message_name_node_text = doc.createTextNode("PROCESS_STOP_COMMAND");
    message_name_node.appendChild(message_name_node_text);
    header_node.appendChild(message_name_node);

    QDomElement transaction_id_node = doc.createElement("TRANSACTIONID");
    QDomText transaction_id_text = doc.createTextNode("");
    transaction_id_node.appendChild(transaction_id_text);
    header_node.appendChild(transaction_id_node);

    QDomElement reply_subject_name_node = doc.createElement("REPLYSUBJECTNAME");
    QDomText reply_subject_name_text = doc.createTextNode("");
    reply_subject_name_node.appendChild(reply_subject_name_text);
    header_node.appendChild(reply_subject_name_node);

    root.appendChild(header_node);

    QDomElement body_node = doc.createElement("Body");

    QDomElement equipment_id_node = doc.createElement("EquipmentID");
    QDomText equipment_id_text = doc.createTextNode("THT01");
    equipment_id_node.appendChild(equipment_id_text);
    body_node.appendChild(equipment_id_node);

    QDomElement recipe_id_node = doc.createElement("ProcessStop");
    QDomText recipe_id_text = doc.createTextNode("Stop");
    recipe_id_node.appendChild(recipe_id_text);
    body_node.appendChild(recipe_id_node);;

    root.appendChild(body_node);

    QDomElement return_node = doc.createElement("Return");

    QDomElement return_code_node = doc.createElement("ReturnCode");
    QDomText return_code_text = doc.createTextNode("");
    return_code_node.appendChild(return_code_text);
    QDomElement return_message_node = doc.createElement("ReturnMessage");
    QDomText return_message_text = doc.createTextNode("");
    return_message_node.appendChild(return_message_text);
    return_node.appendChild(return_code_node);
    return_node.appendChild(return_message_node);

    root.appendChild(return_node);

    m_pTcpCommonUse->send_data_to_server(doc.toByteArray());
}

void AOIBusinessM201Obj::parse_xml_file(QString data)
{
    QDomDocument doc;
    QString MessageName = "";
//    QLOG_ERROR()<<"rev msg from AOI";
    if(doc.setContent(data))
        QLOG_INFO()<<"the content from AOI is NORMAL";
    else
    {
        QLOG_WARN()<<"the content from AOI exist ERROR";
        return;
    }

    QDomElement root=doc.documentElement();

    QDomNode header_node = root.firstChildElement("Header");

    if(!header_node.isNull()&&header_node.isElement())
    {
       QDomNode message_name_node = header_node.firstChildElement("MESSAGENAME");
       if(!message_name_node.isNull()&&message_name_node.isElement())
       {
            MessageName = message_name_node.toElement().text();
//            QLOG_ERROR()<<"MessageName is:"<<MessageName;
            if(MessageName == "EAP_LinkTest_Request_R")
                ;
            else if(MessageName == "EQP_STATUS_REPORT")
                process_equipment_status(data);
            else if(MessageName == "ALARM_REPORT")
                process_alarm_info(data);
            else if(MessageName == "RECIPE_PARAMETER_DOWNLOAD_COMMAND_R")
                process_change_production_response(data);
            else if(MessageName == "EDC_REPORT")
                process_edc_report(data);
            else if(MessageName == "PROCESS_STOP_COMMAND_R")
                process_stop_command(data);
       }
       else
           QLOG_WARN()<<"message name node exist ERROR";
    }
    else
        QLOG_WARN()<<"the Header node from AOI exist ERROR";
}

void AOIBusinessM201Obj::process_equipment_status(QString data)
{
//    QLOG_ERROR()<<u8"收到AOI状态信息";
    QDomDocument doc;
    doc.setContent(data);
    QDomElement root=doc.documentElement();

    QDomNode body_node = root.firstChildElement("Body");
    if(!body_node.isNull()&&body_node.isElement())
    {
        QDomNode equipment_id_node = body_node.firstChildElement("EquipmentID");
        QDomNode equipment_status_node = body_node.firstChildElement("EquipmentStatus");
        if(!equipment_id_node.isNull()&&equipment_id_node.isElement())
        {
            QString eID = equipment_id_node.toElement().text();
            QLOG_INFO()<<"the equipment id is:"<<eID;
            if(!equipment_status_node.isNull()&&equipment_status_node.isElement())
            {
                //reply the server first
                m_pTcpCommonUse->send_data_to_server(generate_common_xml("EQP_STATUS_REPORT_R"));
                QString eStatus = equipment_status_node.toElement().text();
                QLOG_ERROR()<<"the equipment status of AOI is:"<<eStatus;
                if(eStatus == "1")
                    eStatus = "2";
                else if(eStatus == "3")
                    eStatus = "3";
                else
                    eStatus = "1";

                if(m_pDefectRateDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
                    m_pDefectRateDataTable[DATA_TYPE_M201::Equipment_State_M201] = eStatus;
                else
                    m_pDefectRateDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,eStatus);
            }
        }
    }
}

void AOIBusinessM201Obj::process_alarm_info(QString data)
{
    QDomDocument doc;
    doc.setContent(data);
    QDomElement root=doc.documentElement();

    QDomNode body_node = root.firstChildElement("Body");
    if(!body_node.isNull()&&body_node.isElement())
    {
        m_pTcpCommonUse->send_data_to_server(generate_common_xml("ALARM_REPORT_R"));
        QDomNode equipment_id_node = body_node.firstChildElement("EquipmentID");
        QDomNode alarm_status_node = body_node.firstChildElement("AlarmStatus");
        QDomNode alarm_level_node = body_node.firstChildElement("AlarmLevel");
        QDomNode alarm_code_node = body_node.firstChildElement("AlarmCode");
        QDomNode alarm_text_node = body_node.firstChildElement("AlarmText");

        if(!equipment_id_node.isNull()&&equipment_id_node.isElement())
        {
            QString eID = equipment_id_node.toElement().text();
            QLOG_INFO()<<"the equipment id is:"<<eID;
        }

        if(!alarm_status_node.isNull()&&alarm_status_node.isElement())
        {
            QString aStatus = alarm_status_node.toElement().text();
            QLOG_INFO()<<"the equipment alarm status is:"<<aStatus;
        }
    }
}

void AOIBusinessM201Obj::process_pcb_report(QString data)
{
    QDomDocument doc;
    doc.setContent(data);
    QDomElement root=doc.documentElement();

    QDomNode body_node = root.firstChildElement("Body");
    if(!body_node.isNull()&&body_node.isElement())
    {
        m_pTcpCommonUse->send_data_to_server(generate_common_xml("JOB_RECEIVE_REPORT_R"));
        QDomNode equipment_id_node = body_node.firstChildElement("EquipmentID");
    }
}

static int re_send_sign = 0;
void AOIBusinessM201Obj::process_change_production_response(QString data)
{
    QLOG_ERROR()<<"rev AOI Reply Info";
    QDomDocument doc;
    doc.setContent(data);
    QDomElement root=doc.documentElement();

    QDomNode body_node = root.firstChildElement("Body");
    if(!body_node.isNull()&&body_node.isElement())
    {
        QDomNode equipment_id_node = body_node.firstChildElement("EquipmentID");
//        if(!equipment_id_node.isNull()&&equipment_id_node.isElement())
//            QLOG_INFO()<<"the equipment id is:"<<equipment_id_node.toElement().text();
    }

    QDomNode return_node = root.firstChildElement("Return");
    if(!return_node.isNull()&&return_node.isElement())
    {
        QDomNode return_code_node = return_node.firstChildElement("ReturnCode");
        if(!return_code_node.isNull()&& return_code_node.isElement())
        {
            QString retCode = return_code_node.toElement().text();
            if(retCode == "01")
            {
                QLOG_INFO()<<"the AOI change production SUCCESS";
                emit signal_notify_change_production_result(1);
            }
            else
            {
                emit signal_notify_change_production_result(0);
                if(re_send_sign >= 1)
                    ;
                else
                {
                    re_send_sign++;
                    QLOG_WARN()<<"retry to change production";
                    change_production_of_aoi(m_pCurrentRecipeName);
                }
            }
        }
    }
}

void AOIBusinessM201Obj::process_edc_report(QString data)
{
    QDomDocument doc;
    doc.setContent(data);
    QDomElement root=doc.documentElement();

    QDomNode body_node = root.firstChildElement("Body");
    if(!body_node.isNull()&&body_node.isElement())
    {
        //if exist edc_report,the machine status is running
        QString eStatus = "2";
        if(m_pDefectRateDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDefectRateDataTable[DATA_TYPE_M201::Equipment_State_M201] = eStatus;
        else
            m_pDefectRateDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,eStatus);
        //if exist edc_report,the machine status is running

        m_pTcpCommonUse->send_data_to_server(generate_common_xml("EDC_REPORT_R"));
        QDomNode equipment_id_node = body_node.firstChildElement("EquipmentID");
    }
}

static int start_stop_sign = 0;
void AOIBusinessM201Obj::process_stop_command(QString data)
{
    QDomDocument doc;
    doc.setContent(data);
    QDomElement root=doc.documentElement();

    QDomNode body_node = root.firstChildElement("Body");
    if(!body_node.isNull()&&body_node.isElement())
    {
        QDomNode equipment_id_node = body_node.firstChildElement("EquipmentID");
//        if(!equipment_id_node.isNull()&&equipment_id_node.isElement())
//            QLOG_INFO()<<"the equipment id is:"<<equipment_id_node.toElement().text();
    }

    QDomNode return_node = root.firstChildElement("Return");
    if(!return_node.isNull()&&return_node.isElement())
    {
        QDomNode return_code_node = return_node.firstChildElement("ReturnCode");
        if(!return_code_node.isNull()&& return_code_node.isElement())
        {
            QString retCode = return_code_node.toElement().text();
            if(retCode == "01")
                QLOG_INFO()<<"change production SUCCESS";
            else
            {
                if(start_stop_sign >= 1)
                    ;
                else
                {
                    start_stop_sign++;
                    QLOG_WARN()<<"retry to change production";
                    start_or_stop_equipment();
                }
            }
        }
    }
}
