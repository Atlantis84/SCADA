#include "wavesolderingm201wgt.h"
#include "QsLog.h"
#include <QTextCodec>
#include "gdataacquisitionfactory.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTimerEvent>

WaveSolderingM201Wgt* WaveSolderingM201Wgt::m_pInstance = nullptr;

WaveSolderingM201Wgt::WaveSolderingM201Wgt()
{
    m_pMotor1FrequencyFloor = DataAcquisitionConfig::get_instance()->get_config_para("MOTOR_1_FRE_FLOOR").toDouble();
    m_pMotor1FrequencyUpper = DataAcquisitionConfig::get_instance()->get_config_para("MOTOR_1_FRE_UPPER").toDouble();
    m_pMotor2FrequencyFloor = DataAcquisitionConfig::get_instance()->get_config_para("MOTOR_2_FRE_FLOOR").toDouble();
    m_pMotor2FrequencyUpper = DataAcquisitionConfig::get_instance()->get_config_para("MOTOR_2_FRE_UPPER").toDouble();
    m_pPlolderSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("POLDER_SPEED_FLOOR").toDouble();
    m_pPlolderSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("POLDER_SPEED_UPPER").toDouble();
    m_pSolderTempFloor = DataAcquisitionConfig::get_instance()->get_config_para("SOLER_TEMP_FLOOR").toDouble();
    m_pSolderTempUpper = DataAcquisitionConfig::get_instance()->get_config_para("SOLER_TEMP_UPPER").toDouble();
    m_pBottomPre1Floor = DataAcquisitionConfig::get_instance()->get_config_para("PRE_HEAT_1_FLOOR").toDouble();
    m_pBottomPre1Upper = DataAcquisitionConfig::get_instance()->get_config_para("PRE_HEAT_1_UPPER").toDouble();
    m_pBottomPre2Floor = DataAcquisitionConfig::get_instance()->get_config_para("PRE_HEAT_2_FLOOR").toDouble();
    m_pBottomPre2Upper = DataAcquisitionConfig::get_instance()->get_config_para("PRE_HEAT_2_UPPER").toDouble();
    m_pBottomPre3Floor = DataAcquisitionConfig::get_instance()->get_config_para("PRE_HEAT_3_FLOOR").toDouble();
    m_pBottomPre3Upper = DataAcquisitionConfig::get_instance()->get_config_para("PRE_HEAT_3_UPPER").toDouble();

    m_pMotor1FrequencyActualValue = 9.0;
    m_pMotor2FrequencyActualValue = 9.0;

    load_data_from_db();
    QPushButton* btnTest = new QPushButton(u8"测试");
    connect(btnTest,SIGNAL(clicked()),this,SLOT(slot_btn_test()));

    m_pMqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pMqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pMqttClient->setPort(pt);
    connect(m_pMqttClient, &QMqttClient::stateChanged, this, &WaveSolderingM201Wgt::updateLogStateChange);
    connect(m_pMqttClient, &QMqttClient::disconnected, this, &WaveSolderingM201Wgt::brokerDisconnected);

    m_pMqttClient->setUsername("admin");
    m_pMqttClient->setPassword("Admin12345");

    if(m_pMqttClient->state() == QMqttClient::Disconnected)
        m_pMqttClient->connectToHost();

    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        m_pTimerID = startTimer(5000);
    }

    this->layout()->addWidget(btnTest);
}

void WaveSolderingM201Wgt::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constM201WaveSolderingWarningsTable);
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
            m_pMapWarningsInfoOfDB.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}

void WaveSolderingM201Wgt::process_info_from_tcp_server(const QByteArray &data)
{
    if(data == "you can input a command now")
    {
        emit signal_to_start_acquisition_after_confirm(DEV_TYPE_M201::M201WaveSoldering);
        return;
    }

    QString tmpStr = data;
    if(tmpStr.contains("stationName"))
    {
        emit signal_notify_change_production_result(data);
        QLOG_ERROR()<<tmpStr;
        return;
    }

    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QString tmpData = codec->toUnicode(data);
//    QLOG_ERROR()<<tmpData;

    if(find_warnings(tmpData))
//    if(1)
    {
        process_warnings_data(tmpData);
    }
    else {
        QJsonArray fault;
        QJsonObject faultJsonObject;
        fault.append(faultJsonObject);

        if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pWaveSolderingDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }

    if(tmpData.contains(u8"三色灯状态"))
    {
        process_equipment_state(tmpData);
    }

    if(tmpData.contains(u8"运输速度,"))
    {
        process_set_value(tmpData);
    }

    if(tmpData.contains(u8"运输速度实际值,"))
    {
        process_practical_value(tmpData);
    }

    if(!(tmpData.contains(";")) && !(tmpData.contains("No alarm")))
        process_ok_ng_data(tmpData);
}

void WaveSolderingM201Wgt::slot_connect_state(int connectState)
{
    m_pTcpConnectStatus->set_color(connectState);
}

void WaveSolderingM201Wgt::slot_btn_test()
{
    m_pWaveSolderingDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";

    QJsonArray content;
    QJsonObject contentJsonObject;
    contentJsonObject.insert("SET_VALUE_UPPER","");
    contentJsonObject.insert("SET_VALUE_FLOOR","");
    contentJsonObject.insert("ACTUAL_VALUE","");
    contentJsonObject.insert("MEASURED_TYPE","");
    contentJsonObject.insert("MEASURED_TYPE_NAME","");
    contentJsonObject.insert("COMPARE_RESULT","");
    content.append(contentJsonObject);
    m_pWaveSolderingDataTable[DATA_TYPE_M201::Param_Content_M201] = content;

    QJsonArray fault;
    QJsonObject faultJsonObject;
    faultJsonObject.insert("FAULT_CODE","");
    faultJsonObject.insert("FAULT_NAME","");
    faultJsonObject.insert("STATE","1");
    fault.append(faultJsonObject);
    m_pWaveSolderingDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;

    QJsonArray reject;
    QJsonObject rejectJsonObject;
    rejectJsonObject.insert("RATE","");
    rejectJsonObject.insert("STATION","");
    rejectJsonObject.insert("RATE_MESSAGE","");
    rejectJsonObject.insert("STATE","");
    reject.append(rejectJsonObject);
    m_pWaveSolderingDataTable[DATA_TYPE_M201::Reject_Content_M201] = reject;


//    m_pWaveSolderingDataTable[]
//    QString str = "SetParams:ConvSpdSV,+300;";
    //    WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->send_data_to_server(str.toUtf8());
}

void WaveSolderingM201Wgt::updateLogStateChange()
{
    QLOG_WARN()<<"the MQTT state of wavesoldering-m201 is changed";
}

void WaveSolderingM201Wgt::brokerDisconnected()
{
    QLOG_WARN()<<"the MQTT of wavesoldering-m201 disconnect now";
}

void WaveSolderingM201Wgt::process_set_value(const QString data)
{
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    if(lst1.size() >= 11)
        ;
    else
        return;

    m_pPlolderSpd->set_OKs(lst1[0].split(',')[1]);
    m_pB1PreHeat->set_OKs(lst1[1].split(',')[1]);
    m_pB2PreHeat->set_OKs(lst1[2].split(',')[1]);
    m_pB3PreHeat->set_OKs(lst1[3].split(',')[1]);
    m_pSolderPot->set_OKs(lst1[4].split(',')[1]);
    m_pWave1SV->setText(lst1[5].split(',')[1]);
    m_pMotor1FrequencyActualValue = lst1[5].split(',')[1].toDouble();
    m_pWave2SV->setText(lst1[6].split(',')[1]);
    m_pMotor2FrequencyActualValue = lst1[6].split(',')[1].toDouble();

    QString strSetInfo = "";
    for(int i=0;i<7;i++)
    {
        if(i<6)
        {
            strSetInfo.append(lst1[i]);
            strSetInfo.append(";");
        }
        else
            strSetInfo.append(lst1[i]);
    }
}

void WaveSolderingM201Wgt::check_para_state(const QList<double> lstinfo)
{
    QJsonArray content;
    for (int i=0;i<lstinfo.size();i++)
    {
        //波峰马达1频率
        if(i == 0)
        {
            double a = lstinfo[i];
            QLOG_WARN()<<u8"波峰马达1频率为:"<<a;
//            if((a > m_pMotor1FrequencyFloor) && (a < m_pMotor1FrequencyUpper))
            if((static_cast<int>(a) <= 2)||(static_cast<int>(a) >= 25))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMotor1FrequencyUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMotor1FrequencyFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_83");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"波峰扰流波马达频率");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pMotor1FrequencyFloor,a) ||
                    (GDataAcquisitionFactory::get_instance()->isLess(m_pMotor1FrequencyUpper,a)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMotor1FrequencyUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMotor1FrequencyFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_83");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"波峰扰流波马达频率");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMotor1FrequencyUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMotor1FrequencyFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_83");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"波峰扰流波马达频率");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //波峰马达2
        if(i == 1)
        {
            double a = lstinfo[i];
            QLOG_WARN()<<u8"波峰马达2频率为:"<<a;
//            if((a > m_pMotor2FrequencyFloor) && (a < m_pMotor2FrequencyUpper))
            if((static_cast<int>(a) <= 2)||(static_cast<int>(a) >= 25))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMotor2FrequencyUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMotor2FrequencyFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_84");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"波峰平流波马达频率:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(a,m_pMotor2FrequencyFloor) &&
                    GDataAcquisitionFactory::get_instance()->isLess(a,m_pMotor2FrequencyUpper))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMotor2FrequencyUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMotor2FrequencyFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_84");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"波峰平流波马达频率:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMotor2FrequencyUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMotor2FrequencyFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_84");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"波峰平流波马达频率:");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
        }

        //链速
        int tmpPolderSpeed = 0;
        if(i == 2)
        {
            double a = lstinfo[i];
            tmpPolderSpeed = static_cast<int>(a);
            QLOG_WARN()<<u8"链速为:"<<a;
//            if((a > m_pPlolderSpeedFloor) && (a < m_pPlolderSpeedUpper))
            if(static_cast<int>(a) == 0)
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pPlolderSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pPlolderSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_78");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"链速");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pPlolderSpeedFloor,a) ||
                    (GDataAcquisitionFactory::get_instance()->isLess(m_pPlolderSpeedUpper,a)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pPlolderSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pPlolderSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_78");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"链速:");
                if(tmpPolderSpeed == 0)
                    contentJsonObject.insert("COMPARE_RESULT","OK");
                else
                    contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pPlolderSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pPlolderSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_78");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"链速");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //预热1
        if(i == 3)
        {
            double a = lstinfo[i];
            QLOG_WARN()<<u8"预热1温度为:"<<a;
//            if((a > m_pBottomPre1Floor) && (a < m_pBottomPre1Upper))
            if(static_cast<int>(a) < 10)
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre1Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre1Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_80");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热1温度:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pBottomPre1Floor,a) ||
                    (GDataAcquisitionFactory::get_instance()->isLess(m_pBottomPre1Upper,a)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre1Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre1Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_80");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热1温度:");
                if(tmpPolderSpeed == 0)
                    contentJsonObject.insert("COMPARE_RESULT","OK");
                else
                    contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre1Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre1Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_80");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热1温度:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //预热2
        if(i == 4)
        {
            double a = lstinfo[i];
            QLOG_WARN()<<u8"预热2温度为:"<<a;
//            if((a > m_pBottomPre2Floor) && (a < m_pBottomPre2Upper))
            if(static_cast<int>(a) < 10)
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre2Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre2Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_81");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热2温度:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pBottomPre2Floor,a) ||
                    (GDataAcquisitionFactory::get_instance()->isLess(m_pBottomPre2Upper,a)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre2Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre2Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_81");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热2温度:");
                if(tmpPolderSpeed == 0)
                    contentJsonObject.insert("COMPARE_RESULT","OK");
                else
                    contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre2Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre2Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_81");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热2温度:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //预热3
        if(i == 5)
        {
            double a = lstinfo[i];
            QLOG_WARN()<<u8"预热3温度为:"<<a;
//            if((a > m_pBottomPre3Floor) && (a < m_pBottomPre3Upper))
            if(static_cast<int>(a) < 10)
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre3Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre3Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_82");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热3温度:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pBottomPre3Floor,a) ||
                    (GDataAcquisitionFactory::get_instance()->isLess(m_pBottomPre3Upper,a)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre3Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre3Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_82");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热3温度:");
                if(tmpPolderSpeed == 0)
                    contentJsonObject.insert("COMPARE_RESULT","OK");
                else
                    contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pBottomPre3Upper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pBottomPre3Floor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_82");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"预热3温度:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //炉温
        if(i == 6)
        {
            double a = lstinfo[i];
            QLOG_WARN()<<u8"炉温为:"<<a;
//            if((a > m_pSolderTempFloor) && (a < m_pSolderTempUpper))
            if(static_cast<int>(a) < 20)
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pSolderTempUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pSolderTempFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_79");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"炉温:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else if(GDataAcquisitionFactory::get_instance()->isGreater(m_pSolderTempFloor,a) ||
                    (GDataAcquisitionFactory::get_instance()->isLess(m_pPlolderSpeedUpper,a)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pSolderTempUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pSolderTempFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_79");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"炉温:");
                if(tmpPolderSpeed == 0)
                    contentJsonObject.insert("COMPARE_RESULT","OK");
                else
                    contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pSolderTempUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pSolderTempFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_WAVE_SOLDERING_79");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"炉温:");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }
    }

    if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Param_Content_M201))
        m_pWaveSolderingDataTable[DATA_TYPE_M201::Param_Content_M201] = content;
    else
        m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Param_Content_M201,content);
}

void WaveSolderingM201Wgt::process_practical_value(const QString data)
{
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    if(lst1.size() >= 5)
        ;
    else
        return;

    QList<double> lstinfo;
    lstinfo.push_back(m_pMotor1FrequencyActualValue);
    lstinfo.push_back(m_pMotor2FrequencyActualValue);
    //
    m_pPlolderSpd->set_NGs(lst1[0].split(',')[1]);
    lstinfo.push_back(lst1[0].split(',')[1].toDouble());
    m_pB1PreHeat->set_NGs(lst1[1].split(',')[1]);
    lstinfo.push_back(lst1[1].split(',')[1].toDouble());
    m_pB2PreHeat->set_NGs(lst1[2].split(',')[1]);
    lstinfo.push_back(lst1[2].split(',')[1].toDouble());
    m_pB3PreHeat->set_NGs(lst1[3].split(',')[1]);
    lstinfo.push_back(lst1[3].split(',')[1].toDouble());
    m_pSolderPot->set_NGs(lst1[4].split(',')[1]);
    lstinfo.push_back(lst1[4].split(',')[1].toDouble());
    check_para_state(lstinfo);

    QString strPracticalInfo = "";
    for(int i=0;i<5;i++)
    {
        if(i<4)
        {
            strPracticalInfo.append(lst1[i]);
            strPracticalInfo.append(";");
        }
        else
            strPracticalInfo.append(lst1[i]);
    }

//    if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Param_Practical_Value))
//        m_pWaveSolderingDataTable[DATA_TYPE::Param_Practical_Value] = strPracticalInfo;
//    else
//        m_pWaveSolderingDataTable.insert(DATA_TYPE::Param_Practical_Value,strPracticalInfo);
}

bool WaveSolderingM201Wgt::find_warnings(const QString data)
{
    bool result = false;
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    for (int i=0;i<lst1.size();i++) {
        QMap<int,QString>::const_iterator it;
        for (it=m_pMapWarningsInfoOfDB.constBegin();it!=m_pMapWarningsInfoOfDB.constEnd();++it) {
            if(it.value().trimmed().contains(lst1[i].trimmed()))
                result = true;
            else
                continue;
        }
    }

    return result;
}

//static int wave_soldering_warning_id = 0;
void WaveSolderingM201Wgt::process_warnings_data(const QString data)
{
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    int warningID = -1;
    QString warningInfo = "";
    for (int i=0;i<lst1.size();i++) {
        QMap<int,QString>::const_iterator it;
        for (it=m_pMapWarningsInfoOfDB.constBegin();it!=m_pMapWarningsInfoOfDB.constEnd();++it)
        {
            if(it.value().trimmed().contains(lst1[i].trimmed()))
            {
                warningID = it.key();
                warningInfo = it.value();
            }
            else
                continue;
        }
    }

    //just for test,delete later
//    if(wave_soldering_warning_id == 77)
//        wave_soldering_warning_id = 0;
//    wave_soldering_warning_id++;
//    warningID = wave_soldering_warning_id;
    //just for test,delete later

    if(warningID != -1)
    {
        m_pListWgt->addItem(warningInfo);

        QStringList lstInfo = warningInfo.split(',');

        QJsonArray fault;
        QJsonObject faultJsonObject;
        QString strFaultCode = "HD_M201_WAVE_SOLDERING_";
        strFaultCode = strFaultCode.append(QString::number(warningID));
        faultJsonObject.insert("FAULT_CODE",strFaultCode);
        lstInfo[0].prepend(u8"波峰焊");
        faultJsonObject.insert("FAULT_NAME",lstInfo[0]);
        QLOG_WARN()<<u8"波峰焊停机原因:"<<lstInfo[0];
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);

        if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pWaveSolderingDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
    else {
        QJsonArray fault;

        if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pWaveSolderingDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
}

static int wave_soldering_quantity = -1;
void WaveSolderingM201Wgt::process_ok_ng_data(const QString data)
{
//    QLOG_ERROR()<<u8"波峰焊产量为:"<<data;

    QString tmpStr = data;
    int producequantity = tmpStr.toInt();

    QTime time = QTime::currentTime();
    if(time.minute() == 0)
    {
        if(wave_soldering_quantity == -1)
            wave_soldering_quantity = producequantity;
        else
        {
            if((producequantity-wave_soldering_quantity)<10)
                ;
            else {
                QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                QLOG_ERROR()<<u8"波峰焊"<<strhour<<QString::number(producequantity-wave_soldering_quantity);
                wave_soldering_quantity = producequantity;
            }

        }
    }
    m_pLineEditOKQuantity->setText(data);

//    if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Ok_Ng_Quantity))
//        m_pWaveSolderingDataTable[DATA_TYPE::Ok_Ng_Quantity] = data;
//    else
//        m_pWaveSolderingDataTable.insert(DATA_TYPE::Ok_Ng_Quantity,data);
}

void WaveSolderingM201Wgt::process_equipment_state(const QString data)
{
    QStringList lst1 = data.split(";");

    if(lst1.size()>0)
        ;
    else
        return;

    QStringList lst2 = lst1[0].split(',');
    if(lst2.size()>=2)
        ;
    else
        return;

    QString strState = "";
    if(lst2[1] == "1")
    {
        m_pEquipStatus->set_color(1);
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("2");
        if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pWaveSolderingDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
        else
            m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
    }
    else if(lst2[1] == "0")
    {
        m_pEquipStatus->set_color(2);

        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("3");
        if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pWaveSolderingDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
        else
            m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
    }
    else
    {
        m_pEquipStatus->set_color(0);

        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("1");
        if(m_pWaveSolderingDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pWaveSolderingDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        else
            m_pWaveSolderingDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
    }
}

void WaveSolderingM201Wgt::set_wave_soldering_params(QVector<double> vpara)
{
    QVector<double> v;
    if(vpara.size() == 14)
    {
        m_pMotor1FrequencyFloor = vpara[0];
        m_pMotor1FrequencyUpper = vpara[1];

        m_pMotor2FrequencyFloor = vpara[2];
        m_pMotor2FrequencyUpper = vpara[3];

        m_pPlolderSpeedFloor = vpara[4];
        m_pPlolderSpeedUpper = vpara[5];

        m_pSolderTempFloor = vpara[6];
        m_pSolderTempUpper = vpara[7];

        m_pBottomPre1Floor = vpara[8];
        m_pBottomPre1Upper = vpara[9];

        m_pBottomPre2Floor = vpara[10];
        m_pBottomPre2Upper = vpara[11];

        m_pBottomPre3Floor = vpara[12];
        m_pBottomPre3Upper = vpara[13];
    }
}

void WaveSolderingM201Wgt::timerEvent(QTimerEvent *event)
{
    if(GDataAcquisitionFactory::get_instance()->isTimeBetween730And830() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1830And2030() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1130And1230())
        return;
    if(event->timerId() == m_pTimerID)
    {
        //publish to ANDON
        if(m_pWaveSolderingDataTable.size() < 3)
        {
            QLOG_WARN()<<m_pWaveSolderingDataTable.size();
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

        QString strTopic = "HD_M201_WAVE_SOLDERING_TOPIC";
        auto subscription = m_pMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of wavesoldering-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pWaveSolderingDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        content = m_pWaveSolderingDataTable[DATA_TYPE_M201::Param_Content_M201].toJsonArray();
        fault = m_pWaveSolderingDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","WaveSolderingM201");
        data.insert("SystemNo","HS0543302022GNDJ00105");
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *tc = QTextCodec::codecForName("utf-8");
        QString str = tc->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"wave-soldering submit msg to andon:"<<str;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pMqttClient->publish(strTopic,byteArray);
    }
}
