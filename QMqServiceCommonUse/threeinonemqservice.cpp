#include "threeinonemqservice.h"
#include "threeinoneiniconfig.h"
#include "QsLog.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QTextStream>
#include <QJsonParseError>
#include <QMutexLocker>
#include <QDateTime>
ThreeInOneMqService* ThreeInOneMqService::m_pInstance = nullptr;

ThreeInOneMqService::ThreeInOneMqService()
{
    ThreeInOneIniConfig::get_instance()->load_ini_config();
    QLOG_WARN()<<ThreeInOneIniConfig::get_instance()->get_ini_info("TestStation_MachineNameK");
}

void ThreeInOneMqService::push(int devNo, QMap<int, QVariant> data)
{
    this->devNoCollect = devNo;
    this->mapCollectData = data;

    mapDevBaseInfo.clear();

    if(devNoCollect == DEV_TYPE::M201UpBoard){
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("UpBoard_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("UpBoard_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("UpBoard_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201DivideBoard)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("Sub_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("Sub_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("Sub_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201UpUtensil)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("UpUtensil_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("UpUtensil_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("UpUtensil_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201GlueMachine)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("GlueDispenser_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("GlueDispenser_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("GlueDispenser_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201InsertMachineA)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceNameA"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceCodeA"));
    }
    else if(devNoCollect == DEV_TYPE::M201InsertMachineB)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceNameB"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceCodeB"));
    }
    else if(devNoCollect == DEV_TYPE::M201InsertMachineC)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceNameC"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceCodeC"));
    }
    else if(devNoCollect == DEV_TYPE::M201InsertMachineD)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceNameD"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("InsertionMachine_DeviceCodeD"));
    }
    else if(devNoCollect == DEV_TYPE::M201WaveSoldering)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("WaveSoldering_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("WaveSoldering_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("WaveSoldering_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201PickUpCover)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("PickUpCover_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("PickUpCover_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("PickUpCover_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201AOI)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("AOI_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("AOI_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("AOI_DeviceCode"));
    }
    else if(devNoCollect == DEV_TYPE::M201DownUtensil)
    {
        mapDevBaseInfo.insert("DeviceType",ThreeInOneIniConfig::get_instance()->get_ini_info("DownUtensil_DeviceType"));
        mapDevBaseInfo.insert("DeviceName",ThreeInOneIniConfig::get_instance()->get_ini_info("DownUtensil_DeviceName"));
        mapDevBaseInfo.insert("DeviceCode",ThreeInOneIniConfig::get_instance()->get_ini_info("DownUtensil_DeviceCode"));
    }

    donePushAction = false;
}

void ThreeInOneMqService::connect()
{
    connected=false;
    disconnected=false;
    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if(!amqp_socket_open(socket,qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_Host),ThreeInOneIniConfig::Push_HNMQ_Port))
    {
        //登录
        if(amqp_login(conn,//
                      qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_VirtualHost),//
                      0,//
                      131072,//
                      0,//
                      AMQP_SASL_METHOD_PLAIN,//
                      qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_UName),//
                      qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_UPwd)).reply_type==AMQP_RESPONSE_NORMAL)
        {
            //信道
            amqp_channel_open(conn, channel);
            if(amqp_get_rpc_reply(conn).reply_type==AMQP_RESPONSE_NORMAL)
            {
                //交换机
                amqp_exchange_declare(conn,//
                                      channel,//
                                      amqp_cstring_bytes(qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_ExchangeName)),//
                                      amqp_cstring_bytes(qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_ExchangeType)),//
                                      0,//
                                      ThreeInOneIniConfig::Push_HNMQ_EnablePersistance?1:0,//持久化
                                      ThreeInOneIniConfig::Push_HNMQ_EnableAutoDelete?1:0,//自动删除
                                      0,//内部
                                      amqp_empty_table);
                if(amqp_get_rpc_reply(conn).reply_type==AMQP_RESPONSE_NORMAL)
                {
                    connected=true;
                }
                else
                {
                    QLOG_WARN()<<"MQ channel open fail";
                    qWarning("%s",qUtf8Printable("MQ信道打开失败@"+QString::number(channel)));
                }
            }
            else
            {
                QLOG_WARN()<<"MQ channel open fail";
                qWarning("%s",qUtf8Printable("MQ信道打开失败@"+QString::number(channel)));
            }
        }
        else
        {
            QLOG_WARN()<<"MQ user login fail";
            qWarning("%s",qUtf8Printable("MQ用户登录失败@"+ThreeInOneIniConfig::Push_HNMQ_UName+":"+ThreeInOneIniConfig::Push_HNMQ_UPwd));
        }
    }
    else
    {
        QLOG_WARN()<<"MQ network connect error";
        qWarning("%s",qUtf8Printable("MQ网络连接失败@"+ThreeInOneIniConfig::Push_HNMQ_Host+":"+QString::number(ThreeInOneIniConfig::Push_HNMQ_Port)));
    }
}

void ThreeInOneMqService::pushMessage(QString msg)
{
    try {
        //连接状态判断
        if(isConnected()==false)
        {
            disconnect();
            connect();
        }

        //推送
        if(!amqp_basic_publish(conn,//
                              channel,//
                              amqp_cstring_bytes(qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_ExchangeName)),//
                              amqp_cstring_bytes(qUtf8Printable(ThreeInOneIniConfig::Push_HNMQ_RouteKey)),//
                              0,//
                              0,//
                              &props,//
                              amqp_cstring_bytes(qUtf8Printable(msg))))
        {
            QLOG_INFO()<<u8"MQ push success";
        }
        else
        {
            QLOG_WARN()<<u8"MQ push fail";
            connected=false;
        }
    } catch (...) {
        QLOG_ERROR()<<"MQ catch ERROR!";
    }
}

void ThreeInOneMqService::pushObjectByJson()
{
    try
    {
        QMutexLocker lk(&mqPushMutex);
        if(mapCollectData.size() != 4)
        {
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }
        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = mapCollectData[DATA_TYPE_M201::Equipment_State_M201].toString();
        content = mapCollectData[DATA_TYPE_M201::Param_Content_M201].toJsonArray();
        fault = mapCollectData[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();
        rejectrate = mapCollectData[DATA_TYPE_M201::Reject_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName",mapDevBaseInfo["DeviceName"]);
        data.insert("SystemNo",mapDevBaseInfo["DeviceCode"]);
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        QString jsonMsg=QString::fromUtf8(byteArray);

        pushMessage(jsonMsg);
    } catch (...)
    {
        QLOG_ERROR()<<u8"MQ异常捕捉!";
    }
}
