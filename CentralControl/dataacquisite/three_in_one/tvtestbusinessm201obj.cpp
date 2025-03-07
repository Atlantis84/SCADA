#include "tvtestbusinessm201obj.h"
#include <QTimerEvent>
#include "dataacquisitionconfig.h"
#include "QsLog.h"
#include <QDateTime>
#include "gdataacquisitionfactory.h"
#include <QTextCodec>
TvTestBusinessm201Obj* TvTestBusinessm201Obj::m_pInstance = nullptr;
TvTestBusinessm201Obj::TvTestBusinessm201Obj(QObject *parent) : QObject(parent)
{
    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        m_pTimerID1 = startTimer(5000);
        m_pTimerID2 = startTimer(5000);
        m_pTimerID3 = startTimer(5000);
        m_pTimerID4 = startTimer(5000);
        m_pTimerID5 = startTimer(5000);
        m_pTimerID6 = startTimer(5000);
        m_pTimerID7 = startTimer(5000);
        m_pTimerID8 = startTimer(5000);
        m_pTimerID9 = startTimer(5000);
        m_pTimerID10 = startTimer(5000);
        m_pTimerID11 = startTimer(5000);
        m_pTimerID12 = startTimer(5000);
    }

    m_pPassRate = DataAcquisitionConfig::get_instance()->get_config_para("PASS_RATE").toDouble();

    m_pMqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pMqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pMqttClient->setPort(pt);
    connect(m_pMqttClient, &QMqttClient::stateChanged, this, &TvTestBusinessm201Obj::updateLogStateChange);
    connect(m_pMqttClient, &QMqttClient::disconnected, this, &TvTestBusinessm201Obj::brokerDisconnected);

    m_pMqttClient->setUsername("admin");
    m_pMqttClient->setPassword("Admin12345");
    if(m_pMqttClient->state() == QMqttClient::Disconnected)
        m_pMqttClient->connectToHost();

    QStringList lstStations;
    lstStations<<"M201ZDC01"<<"M201ZDC02"<<"M201ZDC03"<<"M201ZDC04"<<"M201ZDC05"<<"M201ZDC06"
                 <<"M201ZDC07"<<"M201ZDC08"<<"M201ZDC09"<<"M201ZDC10"<<"M201ZDC11"<<"M201ZDC12";

    for (int i=0;i<lstStations.size();i++)
        m_pMapStationOutput.insert(lstStations[i],0);
}

void TvTestBusinessm201Obj::rev_tv_test_info(const QJsonObject obj)
{
//    QLOG_ERROR()<<u8"收到TV消息";
//    QLOG_ERROR()<<QJsonDocument(obj).toJson();

    QStringList lstStations;
    lstStations<<"M201ZDC01"<<"M201ZDC02"<<"M201ZDC03"<<"M201ZDC04"<<"M201ZDC05"<<"M201ZDC06"
                 <<"M201ZDC07"<<"M201ZDC08"<<"M201ZDC09"<<"M201ZDC10"<<"M201ZDC11"<<"M201ZDC12";

    QList<QMap<DATA_TYPE_M201,QVariant>> lstMap;
    lstMap<<m_pTestStation1DataTable<<m_pTestStation2DataTable<<m_pTestStation3DataTable<<m_pTestStation4DataTable<<m_pTestStation5DataTable<<m_pTestStation6DataTable
            <<m_pTestStation7DataTable<<m_pTestStation8DataTable<<m_pTestStation9DataTable<<m_pTestStation10DataTable<<m_pTestStation11DataTable<<m_pTestStation12DataTable;
    QJsonValue lineCodeValue = obj.value(QStringLiteral("lineCode"));
    QJsonValue stationNameValue = obj.value(QStringLiteral("stationName"));
    QString toolNumValue = obj.value(QStringLiteral("toolNo")).toString();
    QString passRateValue = obj.value(QStringLiteral("passRate")).toString();
    QJsonValue reTypeValue = obj.value(QStringLiteral("reType"));
    QString outputQuantity = obj.value(QStringLiteral("outputQuantity")).toString();
    m_pMapStationOutput[stationNameValue.toString()] = outputQuantity.toInt();

    if(lineCodeValue.toString() != "M201")
        return;

    double tmpRate = passRateValue.toDouble();
    QString tmpStationName = stationNameValue.toString();
//    QLOG_ERROR()<<tmpStationName<<u8"实际直通率为:"<<tmpRate;
//    if(tmpRate > m_pPassRate)
    if(GDataAcquisitionFactory::get_instance()->isLess(tmpRate,m_pPassRate))
    {
        QJsonArray reject;
        QJsonObject rejectJsonObject;
        QString strRejectCode = "";
        for (int i=0;i<lstStations.size();i++)
        {
            if(stationNameValue.toString() == lstStations[i])
            {
//                QLOG_ERROR()<<lstStations[i]<<u8"产量为:"<<outputQuantity;
                strRejectCode = "HD_M201_TEST_STATION_"+QString::number(i+1)+"_";
                strRejectCode.append(toolNumValue);

                rejectJsonObject.insert("RATE",QString::number(tmpRate));
                rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
                rejectJsonObject.insert("STATION",stationNameValue.toString());
                QString tmpstrstation = QString::number(i+1);
                tmpstrstation.append(u8"工装");
                tmpstrstation.append(toolNumValue);
                tmpstrstation.prepend(u8"测试机台");
                tmpstrstation.append(u8"直通率报警,");
                QString realvalue = QString(u8"实时值:%1").arg(tmpRate);
                realvalue.append("%,");
                tmpstrstation.append(realvalue);
                QString standardvalue = QString(u8"标准值:%1").arg(m_pPassRate);
                standardvalue.append("%");
                tmpstrstation.append(standardvalue);
                rejectJsonObject.insert("RATE_MESSAGE",tmpstrstation);
                rejectJsonObject.insert("STATE","1");
                reject.append(rejectJsonObject);

                switch(i)
                {
                case 0:
                    if(m_pTestStation1DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation1DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation1DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 1:
                    if(m_pTestStation2DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation2DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation2DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 2:
                    if(m_pTestStation3DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation3DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation3DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 3:
                    if(m_pTestStation4DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation4DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation4DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 4:
                    if(m_pTestStation5DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation5DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation5DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 5:
                    if(m_pTestStation6DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation6DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation6DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 6:
                    if(m_pTestStation7DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation7DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation7DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 7:
                    if(m_pTestStation8DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation8DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation8DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 8:
                    if(m_pTestStation9DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation9DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation9DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 9:
                    if(m_pTestStation10DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation10DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation10DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 10:
                    if(m_pTestStation11DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation11DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation11DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 11:
                    if(m_pTestStation12DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation12DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation12DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                }
            }
        }
    }
    else
    {
        QJsonArray reject;
        QJsonObject rejectJsonObject;
        QString strRejectCode = "";
        for (int i=0;i<lstStations.size();i++)
        {
            if(stationNameValue.toString() == lstStations[i])
            {
                QString outp = u8"测试机台";
                outp.append(i+1);
                outp.append(toolNumValue);
                outp.append("工装产量:");
                outp.append(outputQuantity);
                QLOG_WARN()<<outp;

                outp = "";
                outp = u8"测试机台";
                outp.append(i+1);
                outp.append(toolNumValue);
                outp.append("工装通过率:");
                outp.append(QString::number(tmpRate));
                QLOG_WARN()<<outp;

                strRejectCode = "HD_M201_TEST_STATION_"+QString::number(i+1)+"_";
                strRejectCode.append(toolNumValue);

                rejectJsonObject.insert("RATE",QString::number(tmpRate));
                rejectJsonObject.insert("REJECTRATECODE",strRejectCode);
                rejectJsonObject.insert("STATION",stationNameValue.toString());
                rejectJsonObject.insert("RATE_MESSAGE",u8"直通率无报警");
                rejectJsonObject.insert("STATE","0");
                reject.append(rejectJsonObject);

                switch(i)
                {
                case 0:
                    if(m_pTestStation1DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation1DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation1DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 1:
                    if(m_pTestStation2DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation2DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation2DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 2:
                    if(m_pTestStation3DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation3DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation3DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 3:
                    if(m_pTestStation4DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation4DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation4DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 4:
                    if(m_pTestStation5DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation5DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation5DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 5:
                    if(m_pTestStation6DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation6DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation6DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 6:
                    if(m_pTestStation7DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation7DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation7DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 7:
                    if(m_pTestStation8DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation8DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation8DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 8:
                    if(m_pTestStation9DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation9DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation9DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 9:
                    if(m_pTestStation10DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation10DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation10DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 10:
                    if(m_pTestStation11DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation11DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation11DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                case 11:
                    if(m_pTestStation12DataTable.contains(DATA_TYPE_M201::Reject_Content_M201))
                        m_pTestStation12DataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;
                    else
                        m_pTestStation12DataTable.insert(DATA_TYPE_M201::Reject_Content_M201,reject);
                    break;
                }
            }
        }
    }
}

void TvTestBusinessm201Obj::updateLogStateChange()
{

}

void TvTestBusinessm201Obj::brokerDisconnected()
{

}

void TvTestBusinessm201Obj::format_send_info(const QString strTopic, const QString deviceName, const QString sysNo, const int stationID)
{
    QList<QMap<DATA_TYPE_M201,QVariant>> lstMap;
    lstMap<<m_pTestStation1DataTable<<m_pTestStation2DataTable<<m_pTestStation3DataTable<<m_pTestStation4DataTable<<m_pTestStation5DataTable<<m_pTestStation6DataTable
            <<m_pTestStation7DataTable<<m_pTestStation8DataTable<<m_pTestStation9DataTable<<m_pTestStation10DataTable<<m_pTestStation11DataTable<<m_pTestStation12DataTable;


//    if(lstMap[stationID].size() != 1)
//    {
//        QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
//        return;
//    }

    //when the output quantity of station is over 50,send the message
    if(m_pMapStationOutput[deviceName] < 50)
        return;

    auto subscription = m_pMqttClient->subscribe(strTopic);
    if (!subscription)
        QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
    else
        QLOG_INFO()<<"the MQTT subscribe of test-station1-m201 is success";

    QJsonObject data;
    QJsonArray content,fault,rejectrate;
    QString systemstatus;
    rejectrate = lstMap[stationID][DATA_TYPE_M201::Reject_Content_M201].toJsonArray();

    QString stationstate = lstMap[stationID][DATA_TYPE_M201::Equipment_State_M201].toString();
    data.insert("SystemState",stationstate);
    data.insert("ORG","4330");
    data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
    data.insert("DeviceName",deviceName);
    data.insert("SystemNo",sysNo);
    data.insert("CONTENT",content);
    data.insert("FAULT",fault);
    data.insert("REJECTRATE",rejectrate);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(data);

    QTextCodec *tc = QTextCodec::codecForName("utf-8");
    QString str = tc->toUnicode(jsonDoc.toJson());
//    QLOG_ERROR()<<"tv-test:"<<str;

    QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
    m_pMqttClient->publish(strTopic,byteArray);
}

void TvTestBusinessm201Obj::set_tv_pass_rate_para(double passrate)
{
    m_pPassRate = passrate;
}

void TvTestBusinessm201Obj::set_tv_station_info(int stationid, int errorcode, int stationstate)
{
    QLOG_ERROR()<<"the station-id received is:"<<stationid<<", station-status is:"<<stationstate;
    switch (stationid) {
    case 1:
        if(m_pTestStation1DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation1DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation1DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 2:
        if(m_pTestStation2DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation2DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation2DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 3:
        if(m_pTestStation3DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation3DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation3DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 4:
        if(m_pTestStation4DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation4DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation4DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 5:
        if(m_pTestStation5DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation5DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation5DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 6:
        if(m_pTestStation6DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation6DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation6DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 7:
        if(m_pTestStation7DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation7DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation7DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 8:
        if(m_pTestStation8DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation8DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation8DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 9:
        if(m_pTestStation9DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation9DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation9DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 10:
        if(m_pTestStation10DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation10DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation10DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 11:
        if(m_pTestStation11DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation11DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation11DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    case 12:
        if(m_pTestStation12DataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pTestStation12DataTable[DATA_TYPE_M201::Equipment_State_M201] = stationstate;
        else
            m_pTestStation12DataTable.insert(DATA_TYPE_M201::Equipment_State_M201,stationstate);
        break;
    }
}

void TvTestBusinessm201Obj::timerEvent(QTimerEvent *event)
{
    if(GDataAcquisitionFactory::get_instance()->isTimeBetween730And830() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1830And2030())
        return;

    if(event->timerId() == m_pTimerID1)
        format_send_info("HD_M201_TEST_STATION_1_TOPIC","M201ZDC01","HS0543302024FZSB01060",0);
    else if(event->timerId() == m_pTimerID2)
        format_send_info("HD_M201_TEST_STATION_2_TOPIC","M201ZDC02","HS0543302024FZSB01061",1);
    else if(event->timerId() == m_pTimerID3)
        format_send_info("HD_M201_TEST_STATION_3_TOPIC","M201ZDC03","HS0543302024FZSB01062",2);
    else if(event->timerId() == m_pTimerID4)
        format_send_info("HD_M201_TEST_STATION_4_TOPIC","M201ZDC04","HS0543302024FZSB01063",3);
    else if(event->timerId() == m_pTimerID5)
        format_send_info("HD_M201_TEST_STATION_5_TOPIC","M201ZDC05","HS0543302024FZSB01064",4);
    else if(event->timerId() == m_pTimerID6)
        format_send_info("HD_M201_TEST_STATION_6_TOPIC","M201ZDC06","HS0543302024FZSB01065",5);
    else if(event->timerId() == m_pTimerID7)
        format_send_info("HD_M201_TEST_STATION_7_TOPIC","M201ZDC07","HS0543302024FZSB01066",6);
    else if(event->timerId() == m_pTimerID8)
        format_send_info("HD_M201_TEST_STATION_8_TOPIC","M201ZDC08","HS0543302024FZSB01067",7);
    else if(event->timerId() == m_pTimerID9)
        format_send_info("HD_M201_TEST_STATION_9_TOPIC","M201ZDC09","HS0543302024FZSB01068",8);
    else if(event->timerId() == m_pTimerID10)
        format_send_info("HD_M201_TEST_STATION_10_TOPIC","M201ZDC10","HS0543302024FZSB01069",9);
    else if(event->timerId() == m_pTimerID11)
        format_send_info("HD_M201_TEST_STATION_11_TOPIC","M201ZDC11","HS0543302024FZSB01070",10);
    else if(event->timerId() == m_pTimerID12)
        format_send_info("HD_M201_TEST_STATION_12_TOPIC","M201ZDC12","HS0543302024FZSB01071",11);
}
