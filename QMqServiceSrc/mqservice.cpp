#include "mqservice.h"
#include "iniservice.h"
#include "logservice.h"
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <QThread>
#include <QDebug>
#include <QRunnable>
#include <QThreadPool>
#include <QDateTime>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QTextStream>
#include <QJsonParseError>
#include <QsLog.h>
#include <QMutexLocker>
using namespace QsLogging;

MQService::MQService()
{
    //加载配置
    IniService::loadFromFile();
    //注册日志函数
    qInstallMessageHandler(LogService::traceLogHandler);

    //修改默认线程池大小
    QThreadPool::globalInstance()->setMaxThreadCount(100);

    channel = 1;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2;
    //禁用QRunnable自动删除
    this->setAutoDelete(false);
}

MQService::~MQService()
{
    stopService();
}

//void MQService::setLoginUserModel(UserModel* userModel)
//{
//    loginUserModel=userModel;
//}
void MQService::push(int DevNo, QMap<int, QString>data)
{
    this->devNoCollect = DevNo;
    this->mapCollectData = data;

    mapDevBaseInfo.clear();

    if(devNoCollect == DEV_TYPE::TconUpperBoardDevA){
        dataItemProfix = IniService::UpperBoardCodeProfixDataLA;
        //报警
        mapDevBaseInfo.insert("DeviceType",IniService::Upper_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Upper_DeviceNameA);
        mapDevBaseInfo.insert("DeviceCode",IniService::Upper_DeviceCodeA);
        mapDevBaseInfo.insert("LoginUserCode",IniService::UpperLoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::UpperLoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::UpperBoardAlarmProfixDataLA);
    }else if(devNoCollect == DEV_TYPE::TconUpperBoardDevB){
        dataItemProfix = IniService::UpperBoardCodeProfixDataLB;

        mapDevBaseInfo.insert("DeviceType",IniService::Upper_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Upper_DeviceNameB);
        mapDevBaseInfo.insert("DeviceCode",IniService::Upper_DeviceCodeB);
        mapDevBaseInfo.insert("LoginUserCode",IniService::UpperLoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::UpperLoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::UpperBoardAlarmProfixDataLB);
    }else if (devNoCollect == DEV_TYPE::TconSubBoardDevA) {
        dataItemProfix = IniService::SubBoardCodeProfixDataLA;

        mapDevBaseInfo.insert("DeviceType",IniService::Sub_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Sub_DeviceNameA);
        mapDevBaseInfo.insert("DeviceCode",IniService::Sub_DeviceCodeA);
        mapDevBaseInfo.insert("LoginUserCode",IniService::SubLoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::SubLoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::SubBoardAlarmProfixDataLA);
    }else if (devNoCollect == DEV_TYPE::TconSubBoardDevB) {
        dataItemProfix = IniService::SubBoardCodeProfixDataLB;

        mapDevBaseInfo.insert("DeviceType",IniService::Sub_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Sub_DeviceNameB);
        mapDevBaseInfo.insert("DeviceCode",IniService::Sub_DeviceCodeB);
        mapDevBaseInfo.insert("LoginUserCode",IniService::SubLoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::SubLoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::SubBoardAlarmProfixDataLB);
    }else if (devNoCollect == DEV_TYPE::TconTestDevA) {
        dataItemProfix = IniService::TestBoardCodeProfixDataLA;

        mapDevBaseInfo.insert("DeviceType",IniService::Test_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Test_DeviceNameA);
        mapDevBaseInfo.insert("DeviceCode",IniService::Test_DeviceCodeA);
        mapDevBaseInfo.insert("MachineCode",IniService::Test_MachineCodeA);
        mapDevBaseInfo.insert("MachineName",IniService::Test_MachineNameA);
        mapDevBaseInfo.insert("LoginUserCode",IniService::LoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::LoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::TestBoardAlarmProfixDataLA);
    }else if (devNoCollect == DEV_TYPE::TconTestDevB) {
        dataItemProfix = IniService::TestBoardCodeProfixDataLB;

        mapDevBaseInfo.insert("DeviceType",IniService::Test_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Test_DeviceNameB);
        mapDevBaseInfo.insert("DeviceCode",IniService::Test_DeviceCodeB);
        mapDevBaseInfo.insert("MachineCode",IniService::Test_MachineCodeB);
        mapDevBaseInfo.insert("MachineName",IniService::Test_MachineNameB);
        mapDevBaseInfo.insert("LoginUserCode",IniService::TestLoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::TestLoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::TestBoardAlarmProfixDataLB);
    }else if (devNoCollect == DEV_TYPE::TconAOIdev) {
        dataItemProfix = IniService::AOIBoardCodeProfixData;
        mapDevBaseInfo.insert("DeviceType",IniService::AOI_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::AOI_DeviceName);
        mapDevBaseInfo.insert("DeviceCode",IniService::AOI_DeviceCode);
        mapDevBaseInfo.insert("LoginUserCode",IniService::LoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::LoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::AOIBoardAlarmProfixData);
    }else if (devNoCollect == DEV_TYPE::TconPackeageDev) {
        dataItemProfix = IniService::PacketBoardCodeProfixData;

        mapDevBaseInfo.insert("DeviceType",IniService::Packet_DeviceType);
        mapDevBaseInfo.insert("DeviceName",IniService::Packet_DeviceName);
        mapDevBaseInfo.insert("DeviceCode",IniService::Packet_DeviceCode);
        mapDevBaseInfo.insert("LoginUserCode",IniService::LoginUserCode);
        mapDevBaseInfo.insert("LoginUserName",IniService::LoginUserName);
        mapDevBaseInfo.insert("AlarmProfix",IniService::PacketBoardAlarmProfixData);
    }

    donePushAction = false;
}
void MQService::run()
{
    stoped=false;
    while (started) {
        //线程等待
        taskSleep(500);
        //配置更新
        if(IniService::Push_SettingUpdated)
        {
            disconnect();
            connect();
            IniService::Push_SettingUpdated=false;
        }
        //推送LA
        if(donePushAction==false)
        {            //推送
            donePushAction=true;
            this->pushObjectByJson();

        }
    }
    stoped=true;
}
void MQService::taskSleep(int sleepTimeMs)
{
    if(sleepTimeMs<1)
    {
        QThread::msleep(1);
    }
    qint64 outTimeStamp = QDateTime::currentMSecsSinceEpoch()+sleepTimeMs;
    while (outTimeStamp>QDateTime::currentMSecsSinceEpoch()&&started) {
        QThread::msleep(2);
    }
}
void MQService::startService()
{
    if(started==false)
    {
        this->started=true;
        QThreadPool::globalInstance()->start(this);
    }
}
void MQService::stopService()
{
    started=false;
}
bool MQService::isStarted()
{
    return started;
}
bool MQService::isStoped()
{
    return stoped;
}

void MQService::MqStartService(bool flag)
{
    if(flag){
        startService();
    }else{
        stopService();
    }
}

void MQService::connect()
{
    //Socket端口打开
    connected=false;
    disconnected=false;
    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if(!amqp_socket_open(socket,qUtf8Printable(IniService::Push_HNMQ_Host),IniService::Push_HNMQ_Port))
    {
        //登录
        if(amqp_login(conn,//
                      qUtf8Printable(IniService::Push_HNMQ_VirtualHost),//
                      0,//
                      131072,//
                      0,//
                      AMQP_SASL_METHOD_PLAIN,//
                      qUtf8Printable(IniService::Push_HNMQ_UName),//
                      qUtf8Printable(IniService::Push_HNMQ_UPwd)).reply_type==AMQP_RESPONSE_NORMAL)
        {
            //信道
            amqp_channel_open(conn, channel);
            if(amqp_get_rpc_reply(conn).reply_type==AMQP_RESPONSE_NORMAL)
            {
                //交换机
                amqp_exchange_declare(conn,//
                                      channel,//
                                      amqp_cstring_bytes(qUtf8Printable(IniService::Push_HNMQ_ExchangeName)),//
                                      amqp_cstring_bytes(qUtf8Printable(IniService::Push_HNMQ_ExchangeType)),//
                                      0,//
                                      IniService::Push_HNMQ_EnablePersistance?1:0,//持久化
                                      IniService::Push_HNMQ_EnableAutoDelete?1:0,//自动删除
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
            qWarning("%s",qUtf8Printable("MQ用户登录失败@"+IniService::Push_HNMQ_UName+":"+IniService::Push_HNMQ_UPwd));
        }
    }
    else
    {
        QLOG_WARN()<<"MQ network connect error";
        qWarning("%s",qUtf8Printable("MQ网络连接失败@"+IniService::Push_HNMQ_Host+":"+QString::number(IniService::Push_HNMQ_Port)));
    }
}
void MQService::disconnect()
{
    //断开连接
    if(disconnected==false)
    {
        amqp_channel_close(conn, channel, AMQP_REPLY_SUCCESS);
        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(conn);
        disconnected=true;
    }
}
bool MQService::isConnected()
{
    return connected;
}
void MQService::pushMessage(QString msg)
{
    try {
        //连接状态判断
        if(isConnected()==false)
        {
            disconnect();
            connect();
        }
        //日志输出
        if(IniService::Push_HNMQ_EnableTraceLog)
        {
            qDebug("%s",qUtf8Printable("MQ开始推送消息：\n"+msg));
        }
        //推送
        if(!amqp_basic_publish(conn,//
                              channel,//
                              amqp_cstring_bytes(qUtf8Printable(IniService::Push_HNMQ_ExchangeName)),//
                              amqp_cstring_bytes(qUtf8Printable(IniService::Push_HNMQ_RouteKey)),//
                              0,//
                              0,//
                              &props,//
                              amqp_cstring_bytes(qUtf8Printable(msg))))
        {
            if(IniService::Push_HNMQ_EnableTraceLog)
            {
                QLOG_INFO()<<u8"MQ push success";
            }
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
QString MQService::getDataItemName(QString profix,QString name,bool enable)
{
    if(enable==false)
    {
        return name;
    }
    //启用
    QString dataItemName("");
    dataItemName.append(profix);
    dataItemName.append(name);
    return dataItemName;
}
void  MQService::pushObjectByJson()
{    try
     {
        //加锁
            //boardModel->Mutex.lock();
            QMutexLocker lk(&mqPushMutex);
            //
//            QLOG_WARN()<<"the current device type is:"<<devNoCollect;
            QJsonArray items;
            /**
            * 基础数据
            */
            QJsonObject lineCode;
            lineCode.insert("FieldName",getDataItemName(dataItemProfix,"lineCode"));
            lineCode.insert("Value",IniService::App_LineCode);
            items.append(lineCode);
            //
            QJsonObject lineName;
            lineName.insert("FieldName",getDataItemName(dataItemProfix,"lineName"));
            lineName.insert("Value",IniService::App_LineName);
            items.append(lineName);
            //
            QJsonObject deviceCode;
            deviceCode.insert("FieldName",getDataItemName(dataItemProfix,"deviceCode"));
            deviceCode.insert("Value",mapDevBaseInfo.value("DeviceCode"));
            items.append(deviceCode);
            //
            QJsonObject deviceType;
            deviceType.insert("FieldName",getDataItemName(dataItemProfix,"deviceType"));
            deviceType.insert("Value",mapDevBaseInfo.value("DeviceType"));
            items.append(deviceType);
            //
            QJsonObject deviceName;
            deviceName.insert("FieldName",getDataItemName(dataItemProfix,"deviceName"));
            deviceName.insert("Value",mapDevBaseInfo.value("DeviceName"));
            items.append(deviceName);

            if(devNoCollect == DEV_TYPE::TconTestDevA || devNoCollect == DEV_TYPE::TconTestDevB){
                //producttype
                QJsonObject ProductType;
                ProductType.insert("FieldName",getDataItemName(dataItemProfix,"productType"));
                ProductType.insert("Value",mapCollectData[DATA_TYPE::Product_Type]);
                items.append(ProductType);

                //
                QJsonObject machineCode;
                machineCode.insert("FieldName",getDataItemName(dataItemProfix,"machineCode"));
                machineCode.insert("Value",mapDevBaseInfo.value("MachineCode"));
                items.append(machineCode);
                //
                QJsonObject machineName;
                machineName.insert("FieldName",getDataItemName(dataItemProfix,"machineName"));
                machineName.insert("Value",mapDevBaseInfo.value("MachineName"));
                items.append(machineName);

                QStringList OkNgList;
                if(mapCollectData.value(DATA_TYPE::Ok_Ng_Quantity).contains(",")){
                    OkNgList = mapCollectData.value(DATA_TYPE::Ok_Ng_Quantity).split(",");
                }
//                QLOG_WARN()<<"mapCollectData Ok_Ng_Quantity value is "<<mapCollectData.value(DATA_TYPE::Ok_Ng_Quantity);

                if(OkNgList.length() == 3)
                {
                    QJsonObject objTotal;
                    QJsonObject objOks;
                    QJsonObject objNgs;
                    objTotal.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum"));
                    objTotal.insert("Value",OkNgList[0]);

                    objOks.insert("FieldName",getDataItemName(dataItemProfix,"totalNum"));
                    objOks.insert("Value",OkNgList[2]);

                    objNgs.insert("FieldName",getDataItemName(dataItemProfix,"defectNum"));
                    objNgs.insert("Value",OkNgList[1]);

                    items.append(objTotal);
                    items.append(objOks);
                    items.append(objNgs);
                }

                //工装动作次数
                QStringList MoveList;
                if(mapCollectData.value(DATA_TYPE::Key_Part_Action_Times).contains(",")){
                    MoveList = mapCollectData.value(DATA_TYPE::Key_Part_Action_Times).split(",");
                }
//                QLOG_WARN()<<"mapCollectData Key_Part_Action_Times value is "<<mapCollectData.value(DATA_TYPE::Key_Part_Action_Times);
                if(MoveList.length() == 5)
                {
                    if(devNoCollect == DEV_TYPE::TconTestDevA)
                    {
                        QJsonObject movements01;
                        movements01.insert("FieldName",getDataItemName(dataItemProfix,"movements01"));
                        movements01.insert("Value",MoveList[0]);
                        items.append(movements01);

                        QJsonObject movements02;
                        movements02.insert("FieldName",getDataItemName(dataItemProfix,"movements02"));
                        movements02.insert("Value",MoveList[1]);
                        items.append(movements02);

                        QJsonObject movements03;
                        movements03.insert("FieldName",getDataItemName(dataItemProfix,"movements03"));
                        movements03.insert("Value",MoveList[2]);
                        items.append(movements03);

                        QJsonObject movements04;
                        movements04.insert("FieldName",getDataItemName(dataItemProfix,"movements04"));
                        movements04.insert("Value",MoveList[3]);
                        items.append(movements04);

                        QJsonObject movements05;
                        movements05.insert("FieldName",getDataItemName(dataItemProfix,"movements05"));
                        movements05.insert("Value",MoveList[4]);
                        items.append(movements05);
                    }
                    else {
                        QJsonObject movements01;
                        movements01.insert("FieldName",getDataItemName(dataItemProfix,"movements01"));
                        movements01.insert("Value",MoveList[0]);
                        items.append(movements01);

                        QJsonObject movements02;
                        movements02.insert("FieldName",getDataItemName(dataItemProfix,"movements02"));
                        movements02.insert("Value",MoveList[1]);
                        items.append(movements02);

                        QJsonObject movements03;
                        movements03.insert("FieldName",getDataItemName(dataItemProfix,"movements03"));
                        movements03.insert("Value",MoveList[2]);
                        items.append(movements03);

                        QJsonObject movements04;
                        movements04.insert("FieldName",getDataItemName(dataItemProfix,"movements04"));
                        movements04.insert("Value",MoveList[3]);
                        items.append(movements04);

                        QJsonObject movements05;
                        movements05.insert("FieldName",getDataItemName(dataItemProfix,"movements05"));
                        movements05.insert("Value",MoveList[4]);
                        items.append(movements05);
                    }
                }

                //工装OK数、NG数、通过率
                QStringList ratioList;
                if(mapCollectData.value(DATA_TYPE::Work_Device_Ok_Ng_Ratio).contains(",")){
                    ratioList = mapCollectData.value(DATA_TYPE::Work_Device_Ok_Ng_Ratio).split(",");
                }

                if(ratioList.length() == 15)
                {
                    //测试机台工装1
                    QJsonObject oks01;
                    oks01.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum01"));
                    oks01.insert("Value",ratioList[0]);
                    items.append(oks01);

                    QJsonObject ngs01;
                    ngs01.insert("FieldName",getDataItemName(dataItemProfix,"defectNum01"));
                    ngs01.insert("Value",ratioList[1]);
                    items.append(ngs01);

                    QJsonObject ratio01;
                    ratio01.insert("FieldName",getDataItemName(dataItemProfix,"yieldRate01"));
                    ratio01.insert("Value",ratioList[2]);
                    items.append(ratio01);

                    //测试机台工装2
                    QJsonObject oks02;
                    oks02.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum02"));
                    oks02.insert("Value",ratioList[3]);
                    items.append(oks02);

                    QJsonObject ngs02;
                    ngs02.insert("FieldName",getDataItemName(dataItemProfix,"defectNum02"));
                    ngs02.insert("Value",ratioList[4]);
                    items.append(ngs02);

                    QJsonObject ratio02;
                    ratio02.insert("FieldName",getDataItemName(dataItemProfix,"yieldRate02"));
                    ratio02.insert("Value",ratioList[5]);
                    items.append(ratio02);

                    //测试机台工装3
                    QJsonObject oks03;
                    oks03.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum03"));
                    oks03.insert("Value",ratioList[6]);
                    items.append(oks03);

                    QJsonObject ngs03;
                    ngs03.insert("FieldName",getDataItemName(dataItemProfix,"defectNum03"));
                    ngs03.insert("Value",ratioList[7]);
                    items.append(ngs03);

                    QJsonObject ratio03;
                    ratio03.insert("FieldName",getDataItemName(dataItemProfix,"yieldRate03"));
                    ratio03.insert("Value",ratioList[8]);
                    items.append(ratio03);

                    //测试机台工装4
                    QJsonObject oks04;
                    oks04.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum04"));
                    oks04.insert("Value",ratioList[9]);
                    items.append(oks04);

                    QJsonObject ngs04;
                    ngs04.insert("FieldName",getDataItemName(dataItemProfix,"defectNum04"));
                    ngs04.insert("Value",ratioList[10]);
                    items.append(ngs04);

                    QJsonObject ratio04;
                    ratio04.insert("FieldName",getDataItemName(dataItemProfix,"yieldRate04"));
                    ratio04.insert("Value",ratioList[11]);
                    items.append(ratio04);

                    //测试机台工装5
                    QJsonObject oks05;
                    oks05.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum05"));
                    oks05.insert("Value",ratioList[12]);
                    items.append(oks05);

                    QJsonObject ngs05;
                    ngs05.insert("FieldName",getDataItemName(dataItemProfix,"defectNum05"));
                    ngs05.insert("Value",ratioList[13]);
                    items.append(ngs05);

                    QJsonObject ratio05;
                    ratio05.insert("FieldName",getDataItemName(dataItemProfix,"yieldRate05"));
                    ratio05.insert("Value",ratioList[14]);
                    items.append(ratio05);
                }
            }
            //工单号---需要从工厂类拿到或者转产进行更新，此先写死进行测试
            QJsonObject Monumber;
            Monumber.insert("FieldName",getDataItemName(dataItemProfix,"monumber"));
            Monumber.insert("Value",mapCollectData[DATA_TYPE::Product_ProjectID]);
            items.append(Monumber);

            //AOI设备存在productType、累计良品数、累计生产数、累计不良数，单独进行处理，从工厂类或转产时进行更新，此处写死进行测试
            if(devNoCollect == DEV_TYPE::TconAOIdev)
            {
                QJsonObject ProductType;
                ProductType.insert("FieldName",getDataItemName(dataItemProfix,"productType"));
                ProductType.insert("Value",mapCollectData[DATA_TYPE::Product_Type]);
                items.append(ProductType);

                QStringList OkNgList;
                if(mapCollectData.value(DATA_TYPE::Ok_Ng_Quantity).contains(",")){
                    OkNgList = mapCollectData.value(DATA_TYPE::Ok_Ng_Quantity).split(",");
                }
                QLOG_WARN()<<"TconAOIdev"<<"mapCollectData[DATA_TYPE::Ok_Ng_Quantity] = "<<mapCollectData[DATA_TYPE::Ok_Ng_Quantity];

                if(OkNgList.length() == 3)
                {
                    QJsonObject objTotal;
                    QJsonObject objOks;
                    QJsonObject objNgs;
                    objTotal.insert("FieldName",getDataItemName(dataItemProfix,"yieldNum"));
                    objTotal.insert("Value",OkNgList[1]);

                    objOks.insert("FieldName",getDataItemName(dataItemProfix,"totalNum"));
                    objOks.insert("Value",OkNgList[0]);

                    objNgs.insert("FieldName",getDataItemName(dataItemProfix,"defectNum"));
                    objNgs.insert("Value",OkNgList[2]);

                    items.append(objTotal);
                    items.append(objOks);
                    items.append(objNgs);
                }
            }

            if(devNoCollect == DEV_TYPE::TconSubBoardDevA || devNoCollect == DEV_TYPE::TconSubBoardDevB){
                QJsonObject cutterlife;
                cutterlife.insert("FieldName",getDataItemName(dataItemProfix,"cutterlife"));
                cutterlife.insert("Value",mapCollectData[DATA_TYPE::Key_Part_Action_Times]);
                items.append(cutterlife);
            }

            if(devNoCollect == DEV_TYPE::TconUpperBoardDevA || devNoCollect == DEV_TYPE::TconUpperBoardDevB){
                QJsonObject output;
                output.insert("FieldName",getDataItemName(dataItemProfix,"totalNum"));
                output.insert("Value",mapCollectData[DATA_TYPE::Ok_Ng_Quantity]);
                items.append(output);
                QLOG_WARN()<<"TconUpperBoardDevA"<<"mapCollectData[DATA_TYPE::Ok_Ng_Quantity] = "<<mapCollectData[DATA_TYPE::Ok_Ng_Quantity];

                QJsonObject currentwidth;
                currentwidth.insert("FieldName",getDataItemName(dataItemProfix,"currentwidth"));
                currentwidth.insert("Value",mapCollectData[DATA_TYPE::Key_Part_Action_Times]);
                items.append(currentwidth);
            }

            //
            QJsonObject loginUserCode;
            loginUserCode.insert("FieldName",getDataItemName(dataItemProfix,"loginUserCode"));
            loginUserCode.insert("Value", mapDevBaseInfo.value("LoginUserCode"));
            items.append(loginUserCode);
            //
            QJsonObject loginUserName;
            loginUserName.insert("FieldName",getDataItemName(dataItemProfix,"loginUserName"));
            loginUserName.insert("Value",mapDevBaseInfo.value("LoginUserName"));
            items.append(loginUserName);

            /**
            * 设备状态
            */
            QStringList EquipmentStateList = mapCollectData.value(DATA_TYPE::Equipment_State).split(",");
//            QLOG_WARN()<<"EquipmentStateList = "<<mapCollectData[DATA_TYPE::Equipment_State];
            //
            QJsonObject statusValue;
            statusValue.insert("FieldName",getDataItemName(dataItemProfix,"statusValue"));
            statusValue.insert("Value",EquipmentStateList[EquipmentStateListValueIndex]);
            items.append(statusValue);
            //
            QJsonObject statusName;
            if(EquipmentStateList[EquipmentStateListValueIndex] == "1")
            {
                statusName.insert("FieldName",getDataItemName(dataItemProfix,"statusName"));
                statusName.insert("Value",u8"停机");
                items.append(statusName);
            }
            else if(EquipmentStateList[EquipmentStateListValueIndex] == "2")
            {
                statusName.insert("FieldName",getDataItemName(dataItemProfix,"statusName"));
                statusName.insert("Value",u8"正常");
                items.append(statusName);
            }
            else if(EquipmentStateList[EquipmentStateListValueIndex] == "3")
            {
                statusName.insert("FieldName",getDataItemName(dataItemProfix,"statusName"));
                statusName.insert("Value",u8"空闲");
                items.append(statusName);
            }
            else
            {
                statusName.insert("FieldName",getDataItemName(dataItemProfix,"statusName"));
                statusName.insert("Value",u8"未知");
                items.append(statusName);
            }
            //
            QJsonObject statusTimeMs;
            statusTimeMs.insert("FieldName",getDataItemName(dataItemProfix,"statusTimeMs"));
            statusTimeMs.insert("Value",EquipmentStateList[EquipmentStateListTimeMsIndex]);
            items.append(statusTimeMs);
            //
            QJsonObject statusTimeDisc;
            statusTimeDisc.insert("FieldName",getDataItemName(dataItemProfix,"statusTimeDisc"));
            statusTimeDisc.insert("Value",EquipmentStateList[EquipmentStateListTimeDiscIndex]);
            items.append(statusTimeDisc);

            /**
            * 设备报警
            */
            QJsonObject HDZDJGalarmlist;
            QJsonObject faultCode;
            QJsonObject faultName;
            QJsonObject faultDisc;
            QStringList WarningList;
            if(mapCollectData.value(DATA_TYPE::Equipment_Warnings).contains(",")){
                WarningList = mapCollectData.value(DATA_TYPE::Equipment_Warnings).split(",");
            }
//            QLOG_WARN()<<"Equipment_Warnings = "<<mapCollectData[DATA_TYPE::Equipment_Warnings];

            if(!WarningList.isEmpty() && !WarningList[WARNNING_CODE_INDEXOF].isEmpty())
            {
                HDZDJGalarmlist.insert("FieldName",getDataItemName(dataItemProfix,"GWWarning"));
                HDZDJGalarmlist.insert("Value",mapDevBaseInfo.value("AlarmProfix")+WarningList[WARNNING_CODE_INDEXOF]);
                items.append(HDZDJGalarmlist);
                //
                faultCode.insert("FieldName",getDataItemName(dataItemProfix,"faultCode"));
                faultCode.insert("Value",mapDevBaseInfo.value("AlarmProfix")+WarningList[WARNNING_CODE_INDEXOF]);
                items.append(faultCode);
                //
                faultName.insert("FieldName",getDataItemName(dataItemProfix,"faultName"));
                faultName.insert("Value",WarningList[WARNNING_NAME_INDEXOF]);
                items.append(faultName);
                //
                faultDisc.insert("FieldName",getDataItemName(dataItemProfix,"faultDisc"));
                faultDisc.insert("Value",WarningList[WARNNING_DISC_INDEXOF]);
                items.append(faultDisc);
            }
            else
            {
                HDZDJGalarmlist.insert("FieldName",getDataItemName(dataItemProfix,"GWWarning"));
                HDZDJGalarmlist.insert("Value","");
                items.append(HDZDJGalarmlist);
                //
                faultCode.insert("FieldName",getDataItemName(dataItemProfix,"faultCode"));
                faultCode.insert("Value","");
                items.append(faultCode);
                //
                faultName.insert("FieldName",getDataItemName(dataItemProfix,"faultName"));
                faultName.insert("Value","");
                items.append(faultName);
                //
                faultDisc.insert("FieldName",getDataItemName(dataItemProfix,"faultDisc"));
                faultDisc.insert("Value","");
                items.append(faultDisc);
            }

            //
            QJsonObject data;
            data.insert("SystemState",EquipmentStateList[EquipmentStateListValueIndex]);
            data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
            data.insert("DeviceName",mapDevBaseInfo["DeviceName"]);
            data.insert("SystemNo",mapDevBaseInfo["DeviceCode"]);
            data.insert("Values",items);
            /**
            * 推送
            */
            QJsonDocument jsonDoc;
            jsonDoc.setObject(data);
            QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
            QString jsonMsg=QString::fromUtf8(byteArray);
//            QLOG_WARN()<<"the final json to EAM is:"<<jsonMsg;
            //
            //解锁
                //boardModel->Mutex.unlock();
                //
            pushMessage(jsonMsg);
     } catch (...)
     {
        QLOG_ERROR()<<u8"MQ异常捕捉!";
     }
}
