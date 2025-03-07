#include "gdataacquisitionfactory.h"
#include "gdatafactory.h"
#include <QEventLoop>
#include <QSqlQuery>
#include <QtGlobal>
#include <QtMath>
GDataAcquisitionFactory* GDataAcquisitionFactory::m_pInstance = nullptr;
DMDBAccessInterface* GDataAcquisitionFactory::m_pDMDBAccessThreeInOne = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService01 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService02 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService03 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService04 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService05 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService06 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService07 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService08 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService09 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService10 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService11 = nullptr;
FinsUDPService* GDataAcquisitionFactory::m_pFinsUdpService12 = nullptr;

GDataAcquisitionFactory::GDataAcquisitionFactory(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QStringList>("QStringList");
    qRegisterMetaType<QVector<unsigned short>>("QVector<unsigned short>");
    m_pAccessManagerParams = new QNetworkAccessManager();
    m_pAccessManagerDefect = new QNetworkAccessManager();
    m_pHttpBasedOnCurl = new HttpBasedOnCurl();

    m_pDataCommSerial = nullptr;
    connect_common_signals_slots();

    m_pTimer = new QTimer();
    connect(m_pTimer, &QTimer::timeout, this, &GDataAcquisitionFactory::slot_monitor_tcp_connect_state_M201);
//    m_pTimerM202 = new QTimer();
//    connect(m_pTimerM202,&QTimer::timeout,this,&GDataAcquisitionFactory::slot_monitor_tcp_connect_state_M202);

    connect(this,SIGNAL(signal_send_serial_number_to_ui(const QByteArray&)),GDataFactory::get_one_button_change_m201_wgt(),SLOT(slot_rev_serial_number(const QByteArray&)));

    m_pTimerAOIM201 = new QTimer();
    connect(m_pTimerAOIM201,&QTimer::timeout,this,&GDataAcquisitionFactory::slot_monitor_tcp_connect_state_aoi_M201);
    connect(AOIBusinessM201Obj::get_instance()->get_tcp_common_use(),SIGNAL(signal_proc_server_info(const QByteArray&)),
            AOIBusinessM201Obj::get_instance(),SLOT(process_info_from_tcp_server(const QByteArray&)));

    connect(AOIBusinessM201Obj::get_instance(),SIGNAL(signal_notify_change_production_result(const int)),OneButtonChangeM201Wgt::get_instance(),SLOT(slot_rev_result_of_change_production_for_aoi(const int)));

    connect(WaveSolderingM201Thread::get_instance()->get_tcp_common_use(),SIGNAL(signal_proc_server_info(const QByteArray&)),GDataFactory::get_wave_soldering_wgt_M201(),SLOT(process_info_from_tcp_server(const QByteArray&)));
    connect(WaveSolderingM202Thread::get_instance()->get_tcp_common_use(),SIGNAL(signal_proc_server_info(const QByteArray&)),GDataFactory::get_wave_soldering_wgt_M202(),SLOT(process_info_from_tcp_server(const QByteArray&)));

    connect(PlcBusinessM201Thread::get_instance()->get_up_board_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),PlcBusinessM201Obj::get_instance(),SLOT(slot_up_board_info(QVector<unsigned short>)));
    connect(PlcBusinessM201Thread::get_instance()->get_up_utensil_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),PlcBusinessM201Obj::get_instance(),SLOT(slot_up_utensil_info(QVector<unsigned short>)));
    connect(PlcBusinessM201Thread::get_instance()->get_pick_up_cover_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),PlcBusinessM201Obj::get_instance(),SLOT(slot_pick_up_cover_info(QVector<unsigned short>)));
    connect(PlcBusinessM201Thread::get_instance()->get_down_utensil_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),PlcBusinessM201Obj::get_instance(),SLOT(slot_down_utensil_info(QVector<unsigned short>)));

    connect(InsertMachineM201Thread::get_instance()->get_insert_machine1_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),
            InsertMachineBusinessM201Obj::get_instance(),SLOT(slot_info_from_insert_machine1(QVector<unsigned short>)));
    connect(InsertMachineM201Thread::get_instance()->get_insert_machine2_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),
            InsertMachineBusinessM201Obj::get_instance(),SLOT(slot_info_from_insert_machine2(QVector<unsigned short>)));
    connect(InsertMachineM201Thread::get_instance()->get_insert_machine3_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),
            InsertMachineBusinessM201Obj::get_instance(),SLOT(slot_info_from_insert_machine3(QVector<unsigned short>)));
    connect(InsertMachineM201Thread::get_instance()->get_insert_machine4_modbus_obj(),SIGNAL(signal_to_business(QVector<unsigned short>)),
            InsertMachineBusinessM201Obj::get_instance(),SLOT(slot_info_from_insert_machine4(QVector<unsigned short>)));

    connect(this,SIGNAL(signal_send_tv_info_to_business_process(const QJsonObject)),TvTestBusinessm201Obj::get_instance(),SLOT(rev_tv_test_info(const QJsonObject)));

    m_pTcpCommonUseOfFirstScanner = new TcpCommonUse();
    connect(m_pTcpCommonUseOfFirstScanner,SIGNAL(signal_proc_server_info(const QByteArray&)),this,SLOT(process_info_from_tcp_server_first_scanner(const QByteArray&)));

    m_pTcpCommonUseOfSecondScanner = new TcpCommonUse();
    connect(m_pTcpCommonUseOfSecondScanner,SIGNAL(signal_proc_server_info(const QByteArray&)),this,SLOT(process_info_from_tcp_server_second_scanner(const QByteArray&)));


    m_pAccessManagerAOIDefect = new QNetworkAccessManager();
    connect(m_pAccessManagerAOIDefect,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_rev_http_get_aoi_defect(QNetworkReply*)));
    m_pAccessManagerHeadParams = new QNetworkAccessManager();
    connect(m_pAccessManagerHeadParams,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_rev_http_post_head_params(QNetworkReply*)));
    m_pAccessManagerEndParams = new QNetworkAccessManager();
    connect(m_pAccessManagerEndParams,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_rev_http_post_end_params(QNetworkReply*)));

    m_pGetUrl = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_DEFECT_URL");
    m_pPostUrlHead = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_PARAMS_URL");
    m_pPostUrlEnd = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_PARAMS_URL");

    //init udp business for glue-machine&devide-machine
    quint16 port = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UDP_SERVER_PORT").toInt());
    get_M201_udp_business_obj()->init_udp_server(port);
    //init udp business for glue-machine&devide-machine

    connect(m_pHttpBasedOnCurl,SIGNAL(signal_rev_get_reply(const std::string&)),this,SLOT(slot_rev_http_get_aoi_defect(const std::string&)));
    connect(m_pHttpBasedOnCurl,SIGNAL(signal_rev_head_post_reply(const std::string&)),this,SLOT(slot_rev_http_post_head_params(const std::string&)));
    connect(m_pHttpBasedOnCurl,SIGNAL(signal_rev_end_post_reply(const std::string&)),this,SLOT(slot_rev_http_post_end_params(const std::string&)));
}

void GDataAcquisitionFactory::timerEvent(QTimerEvent *event)
{
//    if(event->timerId() == m_pTimerCollectErrors)
//    {
//        QTime time = QTime::currentTime();
//        if((time.hour() == 0)&&(time.minute() == 10)&&(time.second() == 0))
//            get_collect_machine_error_thread()->set_para_value(true);
//    }
}

static int reconnecttimes = 0;
void GDataAcquisitionFactory::slot_monitor_tcp_connect_state_M201()
{        
    if(WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->get_client_state() == QAbstractSocket::SocketState::ConnectedState)
        emit signal_connect_state_M201(1);
    else
    {
        emit signal_connect_state_M201(0);
        QLOG_WARN()<<"Wave-Soldering connect Failed!";
        WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->disconnect_from_server();

        //restart to connect server
        if(WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_1_IP"),
                                                      DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_1_PORT").toInt()))
        {
            QLOG_INFO()<<"connect to wave soldering of M201 Success!";
            QString loginString = "jtauto;27508459";
            WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->send_data_to_server(loginString.toLatin1());
        }
        else
            QLOG_WARN()<<"connect to wave soldering of M201 Failed!";
    }

    reconnecttimes++;
    if(reconnecttimes == 3)
        m_pTimer->stop();
}

void GDataAcquisitionFactory::slot_monitor_tcp_connect_state_M202()
{
    if(WaveSolderingM202Thread::get_instance()->get_tcp_common_use()->get_client_state() == QAbstractSocket::SocketState::ConnectedState)
        emit signal_connect_state_M202(1);
    else
    {
        emit signal_connect_state_M202(0);
        QLOG_WARN()<<"Wave-Soldering connect Failed!";
        WaveSolderingM202Thread::get_instance()->get_tcp_common_use()->disconnect_from_server();

        //restart to connect server
        if(WaveSolderingM202Thread::get_instance()->get_tcp_common_use()->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_2_IP"),
                                                      DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_2_PORT").toInt()))
        {
            QLOG_INFO()<<"connect to wave soldering of M202 Success!";
            QString loginString = "jtauto;27508459";
            WaveSolderingM202Thread::get_instance()->get_tcp_common_use()->send_data_to_server(loginString.toLatin1());
        }
        else
            QLOG_WARN()<<"connect to wave soldering of M202 Failed!";
    }
}

void GDataAcquisitionFactory::slot_monitor_tcp_connect_state_aoi_M201()
{
    if(AOIBusinessM201Obj::get_instance()->get_tcp_common_use()->get_client_state() == QAbstractSocket::SocketState::ConnectedState)
        ;
    else
    {
        QLOG_WARN()<<"AOI connect status is ABNORMAL";
        AOIBusinessM201Obj::get_instance()->get_tcp_common_use()->disconnect_from_server();
        if(AOIBusinessM201Obj::get_instance()->get_tcp_common_use()->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_IP"),
                                                                                           DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_PORT").toInt()))
        {
            QLOG_INFO()<<"connect to AOI SUCCESS";
        }
        else
        {
            QLOG_WARN()<<"connect to AOI FAILED";
        }
    }
}

void GDataAcquisitionFactory::slot_monitor_tcp_connect_state_first_scanner()
{

}

void GDataAcquisitionFactory::slot_monitor_tcp_connect_state_second_scanner()
{

}

void GDataAcquisitionFactory::slot_acquisite_data_on_time()
{

}

void GDataAcquisitionFactory::slot_rev_serial_number_m201(const QByteArray &data)
{
    emit signal_send_serial_number_to_ui(data);
}

static bool startSign = false;
static bool startSignM202 = false;
void GDataAcquisitionFactory::slot_to_start_acquisition_after_confirm(const DEV_TYPE_M201 dType)
{
    switch (dType) {
    case DEV_TYPE_M201::M201WaveSoldering:
    {
        if(!startSign)
        {
            m_pTimer->start(2000);
            startSign = !startSign;
        }
        WaveSolderingM201Thread::get_instance()->start();
    }
        break;
    case DEV_TYPE_M201::M202WaveSoldering:
    {
        if(!startSignM202)
        {
            m_pTimerM202->start(2000);
            startSignM202 = !startSignM202;
        }
        WaveSolderingM202Thread::get_instance()->start();
    }
        break;
    default:
        break;
    }
}

void GDataAcquisitionFactory::process_info_from_tcp_server_first_scanner(const QByteArray &data)
{
//    QLOG_ERROR()<<"head-sn:"<<data;
    QString str = data;
    QStringList lst = str.split('/');
    exec_http_post_method_to_get_head_params(lst[lst.length()-1]);
//    exec_http_post_method_to_get_head_params("ZTP248GJ29T");
}

void GDataAcquisitionFactory::process_info_from_tcp_server_second_scanner(const QByteArray &data)
{
//    QLOG_ERROR()<<"end-sn:"<<data;
    QString str = data;
    QStringList lst = str.split('/');
//    exec_http_post_method_to_get_end_params("ZTP248GJ29T");
    exec_http_post_method_to_get_end_params(lst[lst.length()-1]);
}

void GDataAcquisitionFactory::slot_rev_http_get_aoi_defect(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("data"))
        {
            QJsonValue dataValue = jsonObject.value(QStringLiteral("data"));
            if(dataValue.isObject())
            {
                QJsonObject dataObj = dataValue.toObject();
                if(dataObj.contains("content"))
                {
                    QJsonValue arrayValue = dataObj.value(QStringLiteral("program"));
                    if(arrayValue.isArray())
                    {
                        QJsonArray array = arrayValue.toArray();
                        process_aoi_defect_standard_paras(array);
                    }
                }
            }
        }
    }
    else
        QLOG_WARN()<<"the reply of head-params from mes is ERROR";
}

void GDataAcquisitionFactory::slot_rev_http_post_head_params(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("data"))
        {
            QJsonValue dataValue = jsonObject.value(QStringLiteral("data"));
            if(dataValue.isObject())
            {
                QJsonObject dataObj = dataValue.toObject();
                if(dataObj.contains("program"))
                {
                    QJsonValue arrayValue = dataObj.value(QStringLiteral("program"));
                    if(arrayValue.isArray())
                    {
                        QJsonArray array = arrayValue.toArray();
                        for(int i=0;i<array.size();i++)
                        {
                            QJsonValue iconArray = array.at(i);
                            QJsonObject icon = iconArray.toObject();

                            if(icon.contains("devicE_NAME"))
                            {
                                if((icon.value("devicE_NAME").toString() == "M201-YXC-04")/*||
                                        (icon.value("devicE_NAME").toString() == "M201-YXC-03")||
                                        (icon.value("devicE_NAME").toString() == "M201-YXC-02")||
                                        (icon.value("devicE_NAME").toString() == "M201-YXC-01")*/)
                                {
                                    if(icon.contains("prograM_DETAIL"))
                                    {
                                        QJsonValue programValue = icon.value("prograM_DETAIL");
                                        if(programValue.isArray())
                                        {
                                            QJsonArray pArray = programValue.toArray();
                                            process_insert_machine_standard_paras(pArray);
                                        }
                                    }
                                }

                                if(icon.value("devicE_NAME").toString() == "M201-FBJ-01")
                                {
                                    if(icon.contains("prograM_DETAIL"))
                                    {
                                        QJsonValue programValue = icon.value("prograM_DETAIL");
                                        if(programValue.isArray())
                                        {
                                            QJsonArray pArray = programValue.toArray();
                                            process_divide_board_standard_paras(pArray);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
        QLOG_WARN()<<"the reply of head-params from mes is ERROR";
}

void GDataAcquisitionFactory::slot_rev_http_post_end_params(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("data"))
        {
            QJsonValue dataValue = jsonObject.value(QStringLiteral("data"));
            if(dataValue.isObject())
            {
                QJsonObject dataObj = dataValue.toObject();
                if(dataObj.contains("program"))
                {
                    QJsonValue arrayValue = dataObj.value(QStringLiteral("program"));
                    if(arrayValue.isArray())
                    {
                        QJsonArray array = arrayValue.toArray();
                        for(int i=0;i<array.size();i++)
                        {
                            QJsonValue iconArray = array.at(i);
                            QJsonObject icon = iconArray.toObject();

                            if(icon.contains("devicE_NAME"))
                            {
                                if(icon.value("devicE_NAME").toString() == "M201-BFH-03")
                                {
                                    if(icon.contains("prograM_DETAIL"))
                                    {
                                        QJsonValue programValue = icon.value("prograM_DETAIL");
                                        if(programValue.isArray())
                                        {
                                            QJsonArray pArray = programValue.toArray();
                                            process_wave_soldering_standard_paras(pArray);
                                        }
                                    }
                                }

                                if(icon.value("devicE_NAME").toString() == "M201-ZDC-01")
                                {
                                    if(icon.contains("prograM_DETAIL"))
                                    {
                                        QJsonValue programValue = icon.value("prograM_DETAIL");
                                        if(programValue.isArray())
                                        {
                                            QJsonArray pArray = programValue.toArray();
                                            process_test_station_pass_rate_paras(pArray);
                                        }
                                    }
                                }

                                if(icon.value("devicE_NAME").toString() == "M201-LHAOI-04")
                                {
                                    if(icon.contains("prograM_DETAIL"))
                                    {
                                        QJsonValue programValue = icon.value("prograM_DETAIL");
                                        if(programValue.isArray())
                                        {
                                            QJsonArray pArray = programValue.toArray();
                                            process_aoi_defect_standard_paras(pArray);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
        QLOG_WARN()<<"the reply of head-params from mes is ERROR";
}

void GDataAcquisitionFactory::slot_rev_http_get_aoi_defect(const string &data)
{
    QByteArray responseByte = QByteArray::fromStdString(data);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);
    QJsonObject jsonObject = jsonDocument.object();

    if(jsonObject.contains("data"))
    {
        QJsonValue dataValue = jsonObject.value(QStringLiteral("data"));
        if(dataValue.isObject())
        {
            QJsonObject dataObj = dataValue.toObject();
            if(dataObj.contains("content"))
            {
                QJsonValue arrayValue = dataObj.value(QStringLiteral("content"));
                if(arrayValue.isArray())
                {
                    QJsonArray array = arrayValue.toArray();
                    for(int i=0;i<array.size();i++)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        if(icon.contains("lineCode"))
                        {
                            if(icon.value("lineCode").toString() == "M201")
                            {
                                if(icon.contains("defectRate"))
                                {
                                    double df = icon.value("defectRate").toDouble();
//                                    QLOG_ERROR()<<"the real value of defect rate from mes is:"<<df;
                                    int hour = icon.value("hour").toInt();
//                                    QLOG_ERROR()<<"the time is:"<<hour;
                                    QString tmpStr = icon.value("defectRate").toString();
                                    AOIBusinessM201Obj::get_instance()->set_real_defect_rate(df,hour);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GDataAcquisitionFactory::slot_rev_http_post_head_params(const string &data)
{
//    QLOG_ERROR()<<"head-para:"<<QString::fromStdString(data);
    QByteArray responseByte = QByteArray::fromStdString(data);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);
    QJsonObject jsonObject = jsonDocument.object();

    if(jsonObject.contains("data"))
    {
        QJsonValue dataValue = jsonObject.value(QStringLiteral("data"));
        if(dataValue.isObject())
        {
            QJsonObject dataObj = dataValue.toObject();
            if(dataObj.contains("program"))
            {
                QJsonValue arrayValue = dataObj.value(QStringLiteral("program"));
                if(arrayValue.isArray())
                {
                    QJsonArray array = arrayValue.toArray();
                    for(int i=0;i<array.size();i++)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        if(icon.contains("devicE_NAME"))
                        {
                            if((icon.value("devicE_NAME").toString() == "M201-YXC-04")/*||
                                    (icon.value("devicE_NAME").toString() == "M201-YXC-03")||
                                    (icon.value("devicE_NAME").toString() == "M201-YXC-02")||
                                    (icon.value("devicE_NAME").toString() == "M201-YXC-01")*/)
                            {
                                if(icon.contains("prograM_DETAIL"))
                                {
                                    QJsonValue programValue = icon.value("prograM_DETAIL");
                                    if(programValue.isArray())
                                    {
                                        QJsonArray pArray = programValue.toArray();
                                        process_insert_machine_standard_paras(pArray);
                                    }
                                }
                            }

                            if(icon.value("devicE_NAME").toString() == "M201-FBJ-01")
                            {
                                if(icon.contains("prograM_DETAIL"))
                                {
                                    QJsonValue programValue = icon.value("prograM_DETAIL");
                                    if(programValue.isArray())
                                    {
                                        QJsonArray pArray = programValue.toArray();
                                        process_divide_board_standard_paras(pArray);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GDataAcquisitionFactory::slot_rev_http_post_end_params(const string &data)
{
//    QLOG_ERROR()<<"end-para:"<<QString::fromStdString(data);
    QByteArray responseByte = QByteArray::fromStdString(data);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);
    QJsonObject jsonObject = jsonDocument.object();

    if(jsonObject.contains("data"))
    {
        QJsonValue dataValue = jsonObject.value(QStringLiteral("data"));
        if(dataValue.isObject())
        {
            QJsonObject dataObj = dataValue.toObject();
            if(dataObj.contains("program"))
            {
                QJsonValue arrayValue = dataObj.value(QStringLiteral("program"));
                if(arrayValue.isArray())
                {
                    QJsonArray array = arrayValue.toArray();
                    for(int i=0;i<array.size();i++)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        if(icon.contains("devicE_NAME"))
                        {
                            if(icon.value("devicE_NAME").toString() == "M201-BFH-03")
                            {
                                if(icon.contains("prograM_DETAIL"))
                                {
                                    QJsonValue programValue = icon.value("prograM_DETAIL");
                                    if(programValue.isArray())
                                    {
                                        QJsonArray pArray = programValue.toArray();
                                        process_wave_soldering_standard_paras(pArray);
                                    }
                                }
                            }

                            if(icon.value("devicE_NAME").toString() == "M201-ZDC-01")
                            {
                                if(icon.contains("prograM_DETAIL"))
                                {
                                    QJsonValue programValue = icon.value("prograM_DETAIL");
                                    if(programValue.isArray())
                                    {
                                        QJsonArray pArray = programValue.toArray();
                                        process_test_station_pass_rate_paras(pArray);
                                    }
                                }
                            }

                            if(icon.value("devicE_NAME").toString() == "M201-LHAOI-04")
                            {
                                if(icon.contains("prograM_DETAIL"))
                                {
                                    QJsonValue programValue = icon.value("prograM_DETAIL");
                                    if(programValue.isArray())
                                    {
                                        QJsonArray pArray = programValue.toArray();
                                        process_aoi_defect_standard_paras(pArray);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GDataAcquisitionFactory::process_wave_soldering_standard_paras(QJsonArray array)
{
//    QLOG_ERROR()<<u8"根据Mes更新波峰焊标准值";
    double solderTempFloor,solderTempUpper,ploderSpeedFloor,ploderSpeedUpper,
            motor1FrequencyFloor,motor1FrequencyUpper,motor2FrequencyFloor,
            motor2FrequencyUpper,preHeat1Floor,preHeat1Upper,preHeat2Floor,preHeat2Upper,
            preHeat3Floor,preHeat3Upper;

    solderTempFloor= 0.0;solderTempUpper=0.0;ploderSpeedFloor=0.0;ploderSpeedUpper=0.0;
    motor1FrequencyFloor=0.0;motor1FrequencyUpper=0.0;motor2FrequencyFloor=0.0;
    motor2FrequencyUpper=0.0;preHeat1Floor=0.0;preHeat1Upper=0.0;preHeat2Floor=0.0;preHeat2Upper=0.0;
    preHeat3Floor=0.0;preHeat3Upper=0.0;
    for(int i=0;i<array.size();i++)
    {
        QJsonValue iconArray = array.at(i);
        QJsonObject icon = iconArray.toObject();

        if(icon.contains("iteM_NAME") && icon.contains("loW_VALUE") && icon.contains("uP_VALUE"))
        {
            if(icon.value("iteM_NAME").toString() == u8"焊锡温度")
            {
                solderTempFloor = icon.value("loW_VALUE").toDouble();
                solderTempUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"链条速度")
            {
                ploderSpeedFloor = icon.value("loW_VALUE").toDouble();
                ploderSpeedUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"预热一")
            {
                preHeat1Floor = icon.value("loW_VALUE").toDouble();
                preHeat1Upper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"预热二")
            {
                preHeat2Floor = icon.value("loW_VALUE").toDouble();
                preHeat2Upper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"预热三")
            {
                preHeat3Floor = icon.value("loW_VALUE").toDouble();
                preHeat3Upper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"扰流波频率")
            {
                motor1FrequencyFloor = icon.value("loW_VALUE").toDouble();
                motor1FrequencyUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"平流波频率")
            {
                motor2FrequencyFloor = icon.value("loW_VALUE").toDouble();
                motor2FrequencyUpper = icon.value("uP_VALUE").toDouble();
            }
        }
    }

    QVector<double> vec;
    vec.push_back(motor1FrequencyFloor);
    QLOG_WARN()<<u8"波峰马达1频率下限:"<<QString::number(motor1FrequencyFloor);
    vec.push_back(motor1FrequencyUpper);
    QLOG_WARN()<<u8"波峰马达1频率上限:"<<QString::number(motor1FrequencyUpper);
    vec.push_back(motor2FrequencyFloor);
    QLOG_WARN()<<u8"波峰马达2频率下限:"<<QString::number(motor2FrequencyFloor);
    vec.push_back(motor2FrequencyUpper);
    QLOG_WARN()<<u8"波峰马达2频率上限:"<<QString::number(motor2FrequencyUpper);
    vec.push_back(ploderSpeedFloor);
    QLOG_WARN()<<u8"链速下限:"<<QString::number(ploderSpeedFloor);
    vec.push_back(ploderSpeedUpper);
    QLOG_WARN()<<u8"链速上限:"<<QString::number(ploderSpeedUpper);
    vec.push_back(solderTempFloor);
    QLOG_WARN()<<u8"炉温下限:"<<QString::number(solderTempFloor);
    vec.push_back(solderTempUpper);
    QLOG_WARN()<<u8"炉温上限:"<<QString::number(solderTempUpper);
    vec.push_back(preHeat1Floor);
    QLOG_WARN()<<u8"预热1下限:"<<QString::number(preHeat1Floor);
    vec.push_back(preHeat1Upper);
    QLOG_WARN()<<u8"预热1上限:"<<QString::number(preHeat1Upper);
    vec.push_back(preHeat2Floor);
    QLOG_WARN()<<u8"预热2下限:"<<QString::number(preHeat2Floor);
    vec.push_back(preHeat2Upper);
    QLOG_WARN()<<u8"预热2上限:"<<QString::number(preHeat2Upper);
    vec.push_back(preHeat3Floor);
    QLOG_WARN()<<u8"预热3下限:"<<QString::number(preHeat3Floor);
    vec.push_back(preHeat3Upper);
    QLOG_WARN()<<u8"预热3上限:"<<QString::number(preHeat3Upper);

    WaveSolderingM201Wgt::get_instance()->set_wave_soldering_params(vec);
}

void GDataAcquisitionFactory::process_insert_machine_standard_paras(QJsonArray array)
{
//    QLOG_ERROR()<<u8"根据Mes更新异型插标准值";
    double throwrate = 0.0;
    for(int i=0;i<array.size();i++)
    {
        QJsonValue iconArray = array.at(i);
        QJsonObject icon = iconArray.toObject();

        if(icon.contains("iteM_NAME") && icon.contains("loW_VALUE") && icon.contains("uP_VALUE"))
        {
            if(icon.value("iteM_NAME").toString() == u8"抛料率")
                throwrate = icon.value("uP_VALUE").toDouble();
        }
    }

    QLOG_WARN()<<u8"异型插抛料率标准值:"<<throwrate;
    QVector<double> vec;
    for (int i=0;i<4;i++)
        vec.push_back(throwrate);

    InsertMachineBusinessM201Obj::get_instance()->set_throw_rate_paras(vec);
}

void GDataAcquisitionFactory::process_aoi_defect_standard_paras(QJsonArray array)
{
    double defectrate = 0.0;
    for(int i=0;i<array.size();i++)
    {
        QJsonValue iconArray = array.at(i);
        QJsonObject icon = iconArray.toObject();

        if(icon.contains("iteM_NAME") && icon.contains("loW_VALUE") && icon.contains("uP_VALUE"))
        {
            if(icon.value("iteM_NAME").toString() == u8"疵点率")
            {
                defectrate = icon.value("uP_VALUE").toDouble();
            }
        }
    }

    AOIBusinessM201Obj::get_instance()->set_standard_defect_rate(defectrate);
}

void GDataAcquisitionFactory::process_test_station_pass_rate_paras(QJsonArray array)
{
//    QLOG_ERROR()<<u8"根据Mes更新测试机台标准值";
    double passrate = 0.0;
    for(int i=0;i<array.size();i++)
    {
        QJsonValue iconArray = array.at(i);
        QJsonObject icon = iconArray.toObject();

        if(icon.contains("iteM_NAME") && icon.contains("loW_VALUE") && icon.contains("uP_VALUE"))
        {
            if(icon.value("iteM_NAME").toString() == u8"测试一次通过率")
                passrate = icon.value("standarD_VALUE").toDouble();
        }
    }

//    QLOG_ERROR()<<u8"测试一次通过率标准值:"<<passrate;
    TvTestBusinessm201Obj::get_instance()->set_tv_pass_rate_para(passrate);
}

void GDataAcquisitionFactory::process_divide_board_standard_paras(QJsonArray array)
{
//    QLOG_ERROR()<<u8"根据Mes更新分板机标准值";
    double mainRollSpeed,nullKnifeSpeedFloor,nullKnifeSpeedUpper,downKnifeSpeedFloor,downKnifeSpeedUpper,
            produceSpeedFloor,produceSpeedUpper,highSpeedFloor,highSpeedUpper,lowSpeedFloor,lowSpeedUpper;

    mainRollSpeed=0.0;nullKnifeSpeedFloor=0.0;nullKnifeSpeedUpper=0.0;downKnifeSpeedFloor=0.0;downKnifeSpeedUpper=0.0;
    produceSpeedFloor=0.0;produceSpeedUpper=0.0;highSpeedFloor=0.0;highSpeedUpper=0.0;lowSpeedFloor=0.0;lowSpeedUpper=0.0;

    for(int i=0;i<array.size();i++)
    {
        QJsonValue iconArray = array.at(i);
        QJsonObject icon = iconArray.toObject();

        if(icon.contains("iteM_NAME") && icon.contains("loW_VALUE") && icon.contains("uP_VALUE"))
        {
            if(icon.value("iteM_NAME").toString() == u8"主轴转速")
                mainRollSpeed = icon.value("standarD_VALUE").toDouble();

            if(icon.value("iteM_NAME").toString() == u8"空刀速度")
            {
                nullKnifeSpeedFloor = icon.value("loW_VALUE").toDouble();
                nullKnifeSpeedUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"下刀速度")
            {
                downKnifeSpeedFloor = icon.value("loW_VALUE").toDouble();
                downKnifeSpeedUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"加工速度")
            {
                produceSpeedFloor = icon.value("loW_VALUE").toDouble();
                produceSpeedUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"高速")
            {
                highSpeedFloor = icon.value("loW_VALUE").toDouble();
                highSpeedUpper = icon.value("uP_VALUE").toDouble();
            }

            if(icon.value("iteM_NAME").toString() == u8"低速")
            {
                lowSpeedFloor = icon.value("loW_VALUE").toDouble();
                lowSpeedUpper = icon.value("uP_VALUE").toDouble();
            }
        }
    }

    QVector<double> vec;

    vec.push_back(nullKnifeSpeedUpper);
//    QLOG_ERROR()<<u8"空刀速度上限标准值:"<<nullKnifeSpeedUpper;
    vec.push_back(nullKnifeSpeedFloor);
//    QLOG_ERROR()<<u8"空刀速度下限标准值:"<<nullKnifeSpeedFloor;
    vec.push_back(downKnifeSpeedUpper);
//    QLOG_ERROR()<<u8"下刀速度上限标准值:"<<downKnifeSpeedUpper;
    vec.push_back(downKnifeSpeedFloor);
//    QLOG_ERROR()<<u8"下刀速度下限标准值:"<<downKnifeSpeedFloor;
    vec.push_back(mainRollSpeed);
//    QLOG_ERROR()<<u8"主轴速度标准值:"<<mainRollSpeed;
    vec.push_back(mainRollSpeed);
    vec.push_back(highSpeedUpper);
    QLOG_WARN()<<u8"高速上限:"<<highSpeedUpper;
    vec.push_back(highSpeedFloor);
    QLOG_WARN()<<u8"高速下限:"<<highSpeedFloor;
    vec.push_back(lowSpeedUpper);
    QLOG_WARN()<<u8"低速上限:"<<lowSpeedUpper;
    vec.push_back(lowSpeedFloor);
    QLOG_WARN()<<u8"低速下限:"<<lowSpeedFloor;
    vec.push_back(produceSpeedUpper);
    QLOG_WARN()<<u8"生产速度上限:"<<produceSpeedUpper;
    vec.push_back(produceSpeedFloor);
    QLOG_WARN()<<u8"生产速度下限:"<<produceSpeedFloor;

    TestDivideBoardWgt::get_instance()->set_divide_params(vec);
}

void GDataAcquisitionFactory::start_data_acquisition_three_in_one()
{
    connect(OneButtonChangeM201Wgt::get_instance(),SIGNAL(signal_notify_wave_soldering_to_change_production(const bool,const QString,const QString)),WaveSolderingM201Thread::get_instance(),SLOT(slot_change_production(const bool,const QString,const QString)));
    connect(WaveSolderingM201Wgt::get_instance(),SIGNAL(signal_notify_change_production_result(const QByteArray&)),OneButtonChangeM201Wgt::get_instance(),SLOT(slot_rev_tcp_server_wave_soldering_info(const QByteArray&)));

    //wave-soldering M201
    if(WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_1_IP"),
                                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_1_PORT").toInt()))
    {
        QLOG_INFO()<<"connect to wave soldering of M201 Success!";
        QString loginString = "jtauto;27508459";
        WaveSolderingM201Thread::get_instance()->get_tcp_common_use()->send_data_to_server(loginString.toLatin1());
    }
    else
        QLOG_WARN()<<"connect to wave soldering of M201 Failed!";
    //wave-soldering M201

    //up-utensil
    PlcBusinessM201Thread::get_instance()->start();
    //up-utensil

    //insert-machine
    InsertMachineM201Thread::get_instance()->start();
    //insert-machine

    //AOI
    if(AOIBusinessM201Obj::get_instance()->get_tcp_common_use()->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_IP"),
                                                                                       DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_PORT").toInt()))
    {
        QLOG_ERROR()<<"connect to AOI SUCCESS";
        m_pTimerAOIM201->start(1000);
        AOIBusinessM201Obj::get_instance()->start_to_data_acquisite();
    }
    else
        QLOG_ERROR()<<"connect to AOI FAILED";
//    AOIBusinessM201Obj::get_instance()->start_to_data_acquisite();
    //AOI

    //first scanner
    if(m_pTcpCommonUseOfFirstScanner->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_FIRST_SCANNER_IP"),
                                                         DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_FIRST_SCANNER_PORT").toInt()))
        QLOG_INFO()<<"connect to first scanner success";
    else
        QLOG_WARN()<<"connect to first scanner failed";
    //first scanner

    //second scanner
    if(m_pTcpCommonUseOfSecondScanner->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_SECOND_SCANNER_IP"),
                                                         DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_SECOND_SCANNER_PORT").toInt()))
        QLOG_INFO()<<"connect to second scanner success";
    else
        QLOG_WARN()<<"connect to second scanner failed";
    //second scanner

    //collect errors
    m_pTimerCollectErrors = startTimer(1000);

    //20240102
    QList<QString> tmpLst;
    tmpLst<<"10.50.130.178"<<"10.50.130.179"<<"10.50.130.174"<<"10.50.130.175"<<"10.50.130.176"<<"10.50.130.177"
         <<"127.0.0.1"<<"127.0.0.1"<<"10.50.130.172"<<"10.50.130.173"<<"10.50.130.170"<<"10.50.130.171";
    //plc-test-station
    for(int i=1;i<13;i++)
    {
        get_fins_upd_obj_m201_test_station(i)->init_ip_port(tmpLst[i-1],9600);
        get_fins_upd_obj_m201_test_station(i)->moveToThread(TvTestBusinessM201Thread::get_instance());
        connect(get_fins_upd_obj_m201_test_station(i),SIGNAL(signal_proc_real_data(QByteArray)),this,SLOT(slot_rev_info_from_test_station_plc(QByteArray)));
    }

    TvTestBusinessM201Thread::get_instance()->start();
}

void GDataAcquisitionFactory::connect_common_signals_slots()
{
    connect(GDataFactory::get_wave_soldering_wgt_M201(),SIGNAL(signal_to_start_acquisition_after_confirm(const DEV_TYPE_M201)),this,SLOT(slot_to_start_acquisition_after_confirm(const DEV_TYPE_M201)));
    connect(this,SIGNAL(signal_connect_state_M201(int)),GDataFactory::get_wave_soldering_wgt_M201(),SLOT(slot_connect_state(int)));

    connect(GDataFactory::get_wave_soldering_wgt_M202(),SIGNAL(signal_to_start_acquisition_after_confirm(const DEV_TYPE_M201)),this,SLOT(slot_to_start_acquisition_after_confirm(const DEV_TYPE_M201)));
    connect(this,SIGNAL(signal_connect_state_M202(int)),GDataFactory::get_wave_soldering_wgt_M202(),SLOT(slot_connect_state(int)));
}

void GDataAcquisitionFactory::read_serial_number_m201()
{
    m_pDataCommSerial = new DataCommSerial();
    connect(m_pDataCommSerial,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number_m201(const QByteArray&)));
    if(m_pDataCommSerial)
    {
        QString portName = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_COM_PORT");
        QString portRate = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_BOARD_RATE");
        m_pDataCommSerial->init(tagSerialPortInfo(portName,portRate.toInt()));
        m_pDataCommSerial->recvResponse_only();
        m_pDataCommSerial->start();
    }
}

void GDataAcquisitionFactory::stop_serial_port_m201()
{
    if(m_pDataCommSerial == nullptr)
        return;
    m_pDataCommSerial->terminate();
    m_pDataCommSerial->quit();
    m_pDataCommSerial->closeSerialPort();
    if(m_pDataCommSerial)
    {
        delete m_pDataCommSerial;
        m_pDataCommSerial = nullptr;
    }
}

void GDataAcquisitionFactory::rev_tv_test_info(QJsonObject obj)
{
    emit signal_send_tv_info_to_business_process(obj);
}

void GDataAcquisitionFactory::exec_http_get_method_to_get_aoi_defect()
{
//    QNetworkReply* reply;
//    QEventLoop eventloop;
//    QNetworkRequest* request = new QNetworkRequest();
//    request->setUrl(QUrl(m_pGetUrl));
//    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
//    reply = m_pAccessManagerAOIDefect->get(*request);
//    if(reply->isFinished())
//    {
//        QLOG_INFO()<<"aoi defect http get request reply is NORMAL!";
//    }
//    else
//    {
//        QLOG_WARN()<<"aoi defect http get request reply is TIMEOUT!";;
//    }

    m_pHttpBasedOnCurl->get(m_pGetUrl.toStdString());
}

void GDataAcquisitionFactory::exec_http_post_method_to_get_head_params(const QString sn)
{
//    QLOG_ERROR()<<"head-sn:"<<sn;
//    return;
    QNetworkRequest request;
    request.setUrl(QUrl(m_pPostUrlHead));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root;
    root.insert("areaName",u8"M201线");
    root.insert("sn",sn);
    root.insert("workShopSection","THT");

    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
//    m_pAccessManagerHeadParams->post(request, request_data);
    m_pHttpBasedOnCurl->post(m_pPostUrlHead.toStdString(),std::string(request_data.data(),request_data.size()),1);
}

void GDataAcquisitionFactory::exec_http_post_method_to_get_end_params(const QString sn)
{
//    QLOG_ERROR()<<"end-sn:"<<sn;
//    return;
    QNetworkRequest request;
    request.setUrl(QUrl(m_pPostUrlEnd));
//    request.setUrl(QUrl("127.0.0.1:8080"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root;
    root.insert("areaName",u8"M201线");
    root.insert("sn",sn);
    root.insert("workShopSection","THT");

    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
//    m_pAccessManagerEndParams->post(request, request_data);
    m_pHttpBasedOnCurl->post(m_pPostUrlEnd.toStdString(),std::string(request_data.data(),request_data.size()),2);
}

bool GDataAcquisitionFactory::isGreater(double a, double b, double epsilon) {
    return (a - b) > epsilon;
}

bool GDataAcquisitionFactory::isLess(double a, double b, double epsilon) {
    return (b - a) > epsilon;
}

bool GDataAcquisitionFactory::isEqual(double a, double b, double epsilon)
{
    bool r = false;
    if(qAbs(a-b)<epsilon)
        r = true;
    else
        r = false;
    return r;
}

void GDataAcquisitionFactory::slot_insert_errors_to_db(QStringList lstInfo)
{
    if(lstInfo.size()<7)
        return;

    QString strSql = QString("select max(id) from public.%1").
            arg(constM201InsertMachineErrors);

    int id = 0;
    QSqlQuery queryResult;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }
    QDate currentDate = QDate::currentDate();
    QDate previousDate = currentDate.addDays(-1);
    strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7')").
            arg(constM201InsertMachineErrors).arg(id+1).
            arg("station1").
            arg(previousDate.toString("yyyy-MM-dd")).
            arg(lstInfo[0]).
            arg(lstInfo[2]).
            arg(lstInfo[4]);
    QString strError;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->ExecSqlWithConnectionName(strSql,strError))
    {
        QLOG_INFO()<<"Insert-Machine errors insert to db success";
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!"<<strError;
    }
}

void GDataAcquisitionFactory::slot_rev_info_from_test_station_plc(QByteArray data)
{
//    QLOG_ERROR()<<data;
    int stationid = data[3]*1;
//    QLOG_ERROR()<<"the station id is:"<<stationid;
    int state = data[1]*1;
//    QLOG_ERROR()<<"the station state is:"<<state;
    int errorcode = data[5]*1;
//    QLOG_ERROR()<<"the station error code is:"<<errorcode;
    if(data.size() == 6)
    {
        TvTestBusinessm201Obj::get_instance()->set_tv_station_info(stationid,errorcode,state);
    }
}

bool GDataAcquisitionFactory::isTimeBetween730And830()
{
    QTime currentTime = QTime::currentTime();
    QTime startTime(6, 30);
    QTime endTime(8, 30);

    if (currentTime >= startTime && currentTime <= endTime) {
        return true;
    } else {
        return false;
    }
}

bool GDataAcquisitionFactory::isTimeBetween1830And2030()
{
    QTime currentTime = QTime::currentTime();
    QTime startTime(18, 30);
    QTime endTime(20, 30);

    if (currentTime >= startTime && currentTime <= endTime) {
        return true;
    } else {
        return false;
    }
}

bool GDataAcquisitionFactory::isTimeBetween1130And1230()
{
    QTime currentTime = QTime::currentTime();
    QTime startTime(11, 30);
    QTime endTime(12, 30);

    if (currentTime >= startTime && currentTime <= endTime) {
        return true;
    } else {
        return false;
    }
}
