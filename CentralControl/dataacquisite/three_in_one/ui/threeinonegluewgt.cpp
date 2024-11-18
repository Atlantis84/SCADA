#include "threeinonegluewgt.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "QsLog.h"
#include "gdatafactory.h"
#include <QPushButton>
#include <QSqlQuery>
#include "gdataacquisitionfactory.h"
#include <QDomDocument>
#include "aoibusinessm201obj.h"
#include <QTextCodec>
ThreeInOneGlueWgt* ThreeInOneGlueWgt::m_pInstance = nullptr;
ThreeInOneGlueWgt::ThreeInOneGlueWgt(QWidget *parent) : QWidget(parent)
{
    m_pEquipStatus = new BallWidget();
    m_pLineEditOKQuantity = new QLineEdit();
    m_pLineEditOKQuantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pLineEditOKQuantity->setAlignment(Qt::AlignCenter);
    m_pLineEditOKQuantity->setReadOnly(true);

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");

    QVBoxLayout* vAll = new QVBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addStretch();
    hBox1->addWidget(new QLabel(u8" 设备状态:"));
    hBox1->addWidget(m_pEquipStatus);
    hBox1->addStretch();
    grpBox1->setLayout(hBox1);

    QGroupBox* grpBox2 = new QGroupBox(u8"设备产量");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(new QLabel(u8"当前产量:"));
    hBox2->addWidget(m_pLineEditOKQuantity);
    grpBox2->setLayout(hBox2);

    QGroupBox* grpBox3 = new QGroupBox(u8"报警信息");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox3 = new QHBoxLayout();
    hBox3->addWidget(m_pListWgt);
    grpBox3->setLayout(hBox3);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);
    vAll->addWidget(grpBox3);

    QPushButton* btnTest = new QPushButton(u8"切换测试方案");
    btnTest->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200px;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                                  QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnTest,SIGNAL(clicked()),this,SLOT(slot_btn_test()));
    vAll->addWidget(btnTest);

    this->setLayout(vAll);

    load_data_from_db();

    m_pMqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pMqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pMqttClient->setPort(pt);
    connect(m_pMqttClient, &QMqttClient::stateChanged, this, &ThreeInOneGlueWgt::updateLogStateChange);
    connect(m_pMqttClient, &QMqttClient::disconnected, this, &ThreeInOneGlueWgt::brokerDisconnected);

    m_pMqttClient->setUsername("admin");
    m_pMqttClient->setPassword("Admin12345");
    if(m_pMqttClient->state() == QMqttClient::Disconnected)
        m_pMqttClient->connectToHost();

    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        m_pTimerID = -1;
        m_pTimerID = startTimer(5000);
    }
}

void ThreeInOneGlueWgt::slot_equipment_state_glue(const int status)
{//0 red 1 green 2 yellow
    if(status == 1)
    {
        this->m_pEquipStatus->set_color(0);
        if(m_pGlueMachineDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pGlueMachineDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        else
            m_pGlueMachineDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
    }
    else if(status == 2)
    {
        this->m_pEquipStatus->set_color(1);
        if(m_pGlueMachineDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pGlueMachineDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
        else
            m_pGlueMachineDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
    }
    else
    {
        this->m_pEquipStatus->set_color(2);
        if(m_pGlueMachineDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pGlueMachineDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
        else
            m_pGlueMachineDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
    }
}

//static int glue_warning_id = 1;
void ThreeInOneGlueWgt::slot_equipment_error_glue(const int error)
{
//    if(error == 0)
//        return;

//    if(glue_warning_id == 59)
//        glue_warning_id = 1;
//    glue_warning_id++;
//    int tmperror = glue_warning_id;
//    int tmperror = error + 1;
    int tmperror = error;
    QString warninginfo = "";
    QMap<int,QString>::const_iterator it;
    bool findsign = false;
    for (it=m_pMapWarningsInfoOfDB.constBegin();it!=m_pMapWarningsInfoOfDB.constEnd();++it)
    {
        if(it.key() == tmperror)
        {
            this->m_pListWgt->addItem(it.value());
            warninginfo = it.value();
            findsign = true;
        }
    }

    if(findsign)
    {
        QStringList lstInfo = warninginfo.split(',');

        QJsonArray fault;
        QJsonObject faultJsonObject;
        QString strFaultCode = "HD_M201_GLUE_DISPENSE_";
        strFaultCode = strFaultCode.append(QString::number(tmperror));
        faultJsonObject.insert("FAULT_CODE",strFaultCode);
        lstInfo[0].prepend(u8"点胶机");
        faultJsonObject.insert("FAULT_NAME",lstInfo[0]);
        QLOG_WARN()<<u8"点胶机停机原因为:"<<lstInfo[0];
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);

        if(m_pGlueMachineDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pGlueMachineDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pGlueMachineDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
    else
    {
        QJsonArray fault;

        if(m_pGlueMachineDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pGlueMachineDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pGlueMachineDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
}

static int glue_quantity = -1;
void ThreeInOneGlueWgt::slot_equipment_output_glue(const int output)
{
    QTime time = QTime::currentTime();
    if(time.minute() == 0)
    {
        if(glue_quantity == -1)
            glue_quantity = output;
        else
        {
            if((output-glue_quantity)<10)
                ;
            else {
                QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                QLOG_ERROR()<<u8"点胶机"<<strhour<<QString::number(output-glue_quantity);
                glue_quantity = output;
            }
        }
    }

//    QLOG_WARN()<<u8"点胶机产量为:"<<output;
    this->m_pLineEditOKQuantity->setText(QString::number(output));
}

void ThreeInOneGlueWgt::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constM201GlueMachineWarningsTable);
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

void ThreeInOneGlueWgt::timerEvent(QTimerEvent *event)
{
    if(GDataAcquisitionFactory::get_instance()->isTimeBetween730And830() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1830And2030())
        return;

    if(event->timerId() == m_pTimerID)
    {
        if(m_pGlueMachineDataTable.size() != 2)
        {
//            QLOG_ERROR()<<"the data-map size of glue is:"<<m_pGlueMachineDataTable.size();
            QLOG_WARN()<<"the size of Data in MQ from business is WRONG";
            return;
        }

        QString strTopic = "HD_M201_GLUE_DISPENSE_TOPIC";
        auto subscription = m_pMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of glue-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pGlueMachineDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        fault = m_pGlueMachineDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","GlueDispenseM201");
        data.insert("SystemNo","HS0543302024FZSB01031");
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *tc = QTextCodec::codecForName("utf-8");
        QString str = tc->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"glue submit msg to andon:"<<str;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pMqttClient->publish(strTopic,byteArray);
    }
}

void ThreeInOneGlueWgt::slot_btn_test()
{
    AOIBusinessM201Obj::get_instance()->change_production_of_aoi("13644");
    return;

    QString ip = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_GLUE_IP");
    quint16 port = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_GLUE_PORT").toInt());

//分板机换产测试
//    if(aaaa == false)
//    {
//        QString boardType = "13633";
//        QString machineType = "350238";
//        aaaa = true;
//        GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_divide_machine("192.168.0.200",8081,boardType,machineType);
//    }
//    else
//    {
//        aaaa = false;
//        QString boardType = "13634";
//        QString machineType = "350237";
//        GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_divide_machine("192.168.0.200",8081,boardType,machineType);
//    }

//    QString boardType = "13633hfg";
//    QString machineType = "350238";
//    aaaa = true;
    //    GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_divide_machine("10.50.130.113",8081,boardType,machineType);
}

void ThreeInOneGlueWgt::updateLogStateChange()
{

}

void ThreeInOneGlueWgt::brokerDisconnected()
{

}
