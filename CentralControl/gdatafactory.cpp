#include "gdatafactory.h"
#include "QsLog.h"
#include <QHostInfo>
#include <QApplication>
#include <QDir>
#include "imessagebox.h"
#include <QSqlQuery>
#include <QtMath>
#include <QEventLoop>
#include <QTimer>
GDataFactory* GDataFactory::m_pGDataFactory=nullptr;
MainWindow* GDataFactory::m_pMainWindow = nullptr;
WelcomeWidget* GDataFactory::m_pWelcomeWgt = nullptr;
DMDBAccessInterface* GDataFactory::m_pDMDBAccess = nullptr;
CentralWidget* GDataFactory::m_pCentralWgt=nullptr;
LeftWidget* GDataFactory::m_pLeftWgt =nullptr;
RightHeadWidget* GDataFactory::m_pRightHeadWgt=nullptr;
RightMainWindow* GDataFactory::m_pRightMainWindow = nullptr;
RightMainWidget* GDataFactory::m_pRightMainWgt=nullptr;
RightQueryWidget* GDataFactory::m_pRightQueryWgt=nullptr;
TestWidget* GDataFactory::m_pTestWgt=nullptr;
MesProcess* GDataFactory::m_pMesProcess = nullptr;
TestPlanWgt* GDataFactory::m_pTestPlanWgt = nullptr;
BoardStyleWidget* GDataFactory::m_pBoardStyleWgtA = nullptr;
BoardStyleWidget* GDataFactory::m_pBoardStyleWgtB = nullptr;
QsLogWidget* GDataFactory::m_pQsLogWgt = nullptr;
HistoryDisplayWgt* GDataFactory::m_pHistoryDisplayWgt = nullptr;
ElecEquipTestPlanWgt* GDataFactory::m_pElecEquipWgt = nullptr;
StationTabWgt* GDataFactory::m_pStationTabWgt = nullptr;
UserTabWgt* GDataFactory::m_pUserTabWgt = nullptr;
StationMonitorWgt* GDataFactory::m_pStationMonitorWgt = nullptr;
TvPlanWgt* GDataFactory::m_pTvPlanWgt = nullptr;
PlcWakeUpDlg* GDataFactory::m_pPlcWakeUpDlg = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerVision = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerAOI = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerPLC = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerPackage = nullptr;
LightBarCleanWgt* GDataFactory::m_pLightBarCleanWgt = nullptr;
LightBarWashingResultWgt* GDataFactory::m_pLightBarWashingResultWgt = nullptr;
LightBarNewProductWgt* GDataFactory::m_pLightBarNewProductWgt = nullptr;
PlateSplitter* GDataFactory::m_pPlateSplitter = nullptr;
TBoxWidget* GDataFactory::m_pTBoxWgt = nullptr;
CommonWaitingWgt* GDataFactory::m_pCommonWaitingWgt = nullptr;
FinsUDPService* GDataFactory::m_pFinsUDPService = nullptr;

//data acquisiton
FinsUDPService* GDataFactory::m_pFinsUDPServiceTestStation = nullptr;
FinsUDPService* GDataFactory::m_pFinsUDPServiceAOIStation = nullptr;
FinsUDPService* GDataFactory::m_pFinsUDPServicePackageStation = nullptr;
tconMelsecUpperBoard* GDataFactory::m_pMelsecTCPClientUpperBoardStationA = nullptr;
tconMelsecUpperBoard* GDataFactory::m_pMelsecTCPClientUpperBoardStationB = nullptr;
tconMelsecSubBoard* GDataFactory::m_pMelsecTCPClientSubBoardStationA = nullptr;
tconMelsecSubBoard* GDataFactory::m_pMelsecTCPClientSubBoardStationB = nullptr;
DMDBAccessInterface* GDataFactory::m_pDMDBAccessTcon = nullptr;
//data acquisiton

glueWeighCommService* GDataFactory::m_pGlueWeightCommService = nullptr;
PressStationSerialCom* GDataFactory::m_pPressCommServices = nullptr;

//tbox-screw-machine
FinsUDPService* GDataFactory::m_pFinsUDPServiceTBoxScrewMachine = nullptr;
//tbox-screw-machine
GDataFactory::GDataFactory()
{
    pCommLightBar = nullptr;
    pCommTBox = nullptr;
}

GDataFactory::~GDataFactory()
{
    m_pConfigFile.close();
    m_pConfigFile.flush();
}

bool GDataFactory::isGreater(double a, double b, double epsilon) {
    return (a - b) > epsilon;
}

bool GDataFactory::isLess(double a, double b, double epsilon) {
    return (b - a) > epsilon;
}

void GDataFactory::start_data_aquisiton()
{

    if(GDataFactory::get_factory()->get_config_para("TCON_DATA_ACQUISITION") == "1")
    {
        GDataFactory::get_fins_udp_service_tcon_test_station()->init_ip_port(GDataFactory::get_factory()->get_config_para("TCON_TEST_STATION_PLC_IP"),
                                                                             GDataFactory::get_factory()->get_config_para("TCON_TEST_STATION_PLC_PORT").toInt());
        GDataFactory::get_fins_udp_service_tcon_AOI_station()->init_ip_port(GDataFactory::get_factory()->get_config_para("TCON_AOI_STATION_PLC_IP"),
                                                                            GDataFactory::get_factory()->get_config_para("TCON_AOI_STATION_PLC_PORT").toInt());
        GDataFactory::get_fins_udp_service_tcon_package_station()->init_ip_port(GDataFactory::get_factory()->get_config_para("TCON_PACKAGE_STATION_PLC_IP"),
                                                                                GDataFactory::get_factory()->get_config_para("TCON_PACKAGE_STATION_PLC_PORT").toInt());

        get_fins_udp_service_tcon_test_station()->moveToThread(&ttr);
        get_fins_udp_service_tcon_AOI_station()->moveToThread(&ttrAOI);
        get_fins_udp_service_tcon_package_station()->moveToThread(&ttrPackage);

        ttr.start();
        TconFinsUDPProcessTestStation::get_instance()->start_mq_service();
        ttrAOI.start();
        TconFinsUDPProcessAOI::get_instance()->start_mq_service();
        ttrPackage.start();
        TconFinsUDPProcessPackage::get_instance()->start_mq_service();

        GDataFactory::get_melsec_tcp_tcon_subboard_stationA()->connectToPLC(get_config_para("TCON_DIVIDE_BOARD_LEFT_PLC_IP"),
                                                                            get_config_para("TCON_DIVIDE_BOARD_LEFT_PLC_PORT").toUShort(),
                                                                            1000);
        GDataFactory::get_melsec_tcp_tcon_subboard_stationB()->connectToPLC(get_config_para("TCON_DIVIDE_BOARD_RIGHT_PLC_IP"),
                                                                            get_config_para("TCON_DIVIDE_BOARD_RIGHT_PLC_PORT").toUShort(),
                                                                            1001);


        GDataFactory::get_melsec_tcp_tcon_subboard_stationA()->isStartSubBoard();
        GDataFactory::get_melsec_tcp_tcon_subboard_stationB()->isStartSubBoard();

        GDataFactory::get_melsec_tcp_tcon_upper_board_stationA()->connectToPLC(get_config_para("TCON_UP_BOARD_LEFT_PLC_IP"),
                                                                               get_config_para("TCON_UP_BOARD_LEFT_PLC_PORT").toUShort(),
                                                                               3301);
        GDataFactory::get_melsec_tcp_tcon_upper_board_stationA()->isStartUpperBoard();

        GDataFactory::get_melsec_tcp_tcon_upper_board_stationB()->connectToPLC(get_config_para("TCON_UP_BOARD_RIGHT_PLC_IP"),
                                                                               get_config_para("TCON_UP_BOARD_RIGHT_PLC_PORT").toUShort(),
                                                                               3300);
        GDataFactory::get_melsec_tcp_tcon_upper_board_stationB()->isStartUpperBoard();
    }
}

QByteArray GDataFactory::alter_order_of_byte_array(QByteArray data)
{
    QByteArray result;

    for(int i =0;i<data.length();i++)
    {
        if(i%2 == 0)
        {
            QByteArray tmpData;
            tmpData.append(data[i]);
            tmpData.append(data[i+1]);
            std::reverse(tmpData.begin(),tmpData.end());
            result.append(tmpData[0]);
            result.append(tmpData[1]);
        }
    }

    return result;
}

void GDataFactory::read_serial_number()
{
    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = ConfigInfo["COM_PORT"];
        QString portRate = ConfigInfo["COM_BARD_RATE"];
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));
        pComm->recvResponse_only();
        pComm->start();
    }
}

void GDataFactory::stop_serial_port()
{
    pComm->terminate();
    pComm->quit();
    pComm->closeSerialPort();
    if(pComm)
        delete pComm;
}

void GDataFactory::read_serial_number_light_bar()
{
    m_pFirstTimeSN = "";
    m_pSecondTimeSN = "";

    pCommLightBar = new DataCommSerial();
    connect(pCommLightBar,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number_light_bar(const QByteArray&)));
    if(pCommLightBar)
    {
        QString portName = ConfigInfo["COM_PORT_LIGHT_BAR"];
        QString portRate = ConfigInfo["COM_BARD_RATE_LIGHT_BAR"];
        pCommLightBar->init(tagSerialPortInfo(portName,portRate.toInt()));
        pCommLightBar->recvResponse_only();
        pCommLightBar->start();
    }
}

void GDataFactory::stop_serial_port_light_bar()
{
    if(pCommLightBar == nullptr)
        return;
    pCommLightBar->terminate();
    pCommLightBar->quit();
    pCommLightBar->closeSerialPort();
    if(pCommLightBar)
    {
        delete pCommLightBar;
        pCommLightBar = nullptr;
    }
}

void GDataFactory::read_serial_number_tbox()
{
    pCommTBox = new DataCommSerial();
    connect(pCommTBox,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number_tbox(const QByteArray&)));
    if(pCommTBox)
    {
        QString portName = ConfigInfo["COM_PORT_TBOX"];
        QString portRate = ConfigInfo["COM_BARD_RATE_TBOX"];
        pCommTBox->init(tagSerialPortInfo(portName,portRate.toInt()));
        pCommTBox->recvResponse_only();
        pCommTBox->start();
    }
}

void GDataFactory::stop_serial_port_tbox()
{
    if(pCommTBox == nullptr)
        return;
    pCommTBox->terminate();
    pCommTBox->quit();
    pCommTBox->closeSerialPort();
    if(pCommTBox)
    {
        delete pCommTBox;
        pCommTBox = nullptr;
    }
}

void GDataFactory::connect_common_signal_slot()
{
    connect(this,SIGNAL(signal_del_one_change_product_info(QString)),this,SLOT(slot_del_one_change_product_info(QString)));
    connect(get_left_wgt(),SIGNAL(signal_send_path(const QString)),get_right_query_wgt(),SLOT(slot_rev_path(const QString)));
    connect(get_right_query_wgt(),SIGNAL(signal_add_sub_window(const QString,const QString)),get_test_wgt(),
            SLOT(slot_rev_query_result(const QString,const QString)));
    connect(get_test_plan_wgt(),SIGNAL(signal_change_btn_icon()),get_test_wgt(),SLOT(slot_change_btn_icon()));
    connect(get_test_plan_wgt(),SIGNAL(signal_save_plan()),get_test_wgt(),SLOT(slot_save_plan()));
    connect(get_test_wgt(),SIGNAL(signal_kill_timeout_timer()),get_test_plan_wgt(),SLOT(slot_kill_timeout_timer()));
    connect(this,SIGNAL(signal_change_production_already_complete()),get_test_wgt(),SLOT(slot_change_production_already_complete()));
    connect(this,SIGNAL(signal_register_new_station_success()),get_test_plan_wgt(),SLOT(slot_refresh_treewgt()));
    connect(get_elec_equip_test_plan_wgt(),SIGNAL(signal_change_btn_icon()),get_test_wgt(),SLOT(slot_change_btn_icon()));

    connect(get_left_wgt(),SIGNAL(signal_create_tab_item(const int,const QString)),get_central_wgt(),SLOT(slot_create_tab_item(const int,const QString)));

    //tbox
    connect(this,SIGNAL(signal_product_sn_to_ui(const QString)),get_tbox_wgt(),SLOT(slot_rev_product_sn(const QString)));
    connect(get_mes_process(),SIGNAL(signal_to_ui_tbox(const QString,const QString,const QString)),get_tbox_wgt(),SLOT(slot_rev_product_info_from_mes(const QString,const QString,const QString)));
    //tbox

    //light-bar
    connect(this,SIGNAL(signal_product_sn_to_light_bar(int,const QByteArray&)),get_light_bar_wgt(),SLOT(slot_rev_sn_info(int,const QByteArray&)));
    connect(get_mes_process(),SIGNAL(signal_to_light_bar_info(const QByteArray&)),get_light_bar_wgt(),SLOT(slot_rev_mes_result(const QByteArray&)));
    connect(this,SIGNAL(signal_finish_washing(const int)),get_light_bar_wgt(),SLOT(slot_finish_washing(const int)));
    //light-bar

    //m201
    connect(get_mes_process(),SIGNAL(signal_to_ui_m201(const QByteArray&)),get_one_button_change_m201_wgt(),SLOT(slot_rev_info_from_mes(const QByteArray)));
    //m201

    connect(get_fins_udp_service(),SIGNAL(signal_proc_real_data(QByteArray)),this,SLOT(slot_proc_fins_udp_data(QByteArray)));
    m_pCurrentLineInfoValue = TCON_LINE;

    if(GDataFactory::get_factory()->get_config_para("TCON_DATA_ACQUISITION") == "1")
    {
        //tcon data-acquisition signals-slots
        connect(get_fins_udp_service_tcon_test_station(),SIGNAL(signal_proc_real_data(QByteArray)),TconFinsUDPProcessTestStation::get_instance(),SLOT(process_plc_data(QByteArray)));
        connect(get_fins_udp_service_tcon_AOI_station(),SIGNAL(signal_proc_real_data(QByteArray)),TconFinsUDPProcessAOI::get_instance(),SLOT(process_plc_data(QByteArray)));
        connect(get_fins_udp_service_tcon_package_station(),SIGNAL(signal_proc_real_data(QByteArray)),TconFinsUDPProcessPackage::get_instance(),SLOT(process_plc_data(QByteArray)));

        //tcon data-acquisition signals-slots
    }

    connect(get_fins_udp_service_tbox_screw_machine(),SIGNAL(signal_proc_real_data(QByteArray)),get_screw_machine_wgt(),SLOT(process_plc_data(QByteArray)));
}

void GDataFactory::load_json_config_1(char *filename)
{
    m_pConfigFile.setFileName(QString(filename));
    m_pConfigFileName=QString(filename);
    QString JsonValue;
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QLOG_INFO()<<"load json config file success!";
        JsonValue = m_pConfigFile.readAll();
        m_pConfigFile.close();

        QJsonParseError parseJsonErr;
        m_pJsonDocument = QJsonDocument::fromJson(JsonValue.toUtf8(),&parseJsonErr);
        if(!(parseJsonErr.error == QJsonParseError::NoError))
        {
            QLOG_INFO()<<"parse json file failed!";
            return;
        }
        else {
            QLOG_INFO()<<"parse json file success!";
        }

        m_pRootObj = m_pJsonDocument.object();
        if(m_pRootObj.contains("IPConfigs"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("IPConfigs"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    ConfigInfo.insert("AOI_IP",icon["AOI_IP"].toString());
                    ConfigInfo.insert("PLC_IP",icon["PLC_IP"].toString());
                    ConfigInfo.insert("REGISTER_IP",icon["REGISTER_IP"].toString());
                    ConfigInfo.insert("VISUAL_LEAD_IP",icon["VISUAL_LEAD_IP"].toString());
                    ConfigInfo.insert("MES_IP",icon["MES_IP"].toString());
                    ConfigInfo.insert("LIGHT_BAR_MES_IP",icon["LIGHT_BAR_MES_IP"].toString());
                    ConfigInfo.insert("CENTRAL_CONTROL_IP",icon["CENTRAL_CONTROL_IP"].toString());
                    ConfigInfo.insert("TCP_SERVER_IP",icon["TCP_SERVER_IP"].toString());
                }
            }
        }

        if(m_pRootObj.contains("DataAcquisition"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("DataAcquisition"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    ConfigInfo.insert("TCON_DATA_ACQUISITION",icon["TCON_DATA_ACQUISITION"].toString());
                    ConfigInfo.insert("TCON_AOI_STATION_PLC_IP",icon["TCON_AOI_STATION_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_PACKAGE_STATION_PLC_IP",icon["TCON_PACKAGE_STATION_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_TEST_STATION_PLC_IP",icon["TCON_TEST_STATION_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_AOI_STATION_PLC_PORT",icon["TCON_AOI_STATION_PLC_PORT"].toString());
                    ConfigInfo.insert("TCON_PACKAGE_STATION_PLC_PORT",icon["TCON_PACKAGE_STATION_PLC_PORT"].toString());
                    ConfigInfo.insert("TCON_TEST_STATION_PLC_PORT",icon["TCON_TEST_STATION_PLC_PORT"].toString());

                    ConfigInfo.insert("TCON_DB_ADDRESS",icon["TCON_DB_ADDRESS"].toString());
                    ConfigInfo.insert("TCON_DB_NAME",icon["TCON_DB_NAME"].toString());
                    ConfigInfo.insert("TCON_DB_USR_NAME",icon["TCON_DB_USR_NAME"].toString());
                    ConfigInfo.insert("TCON_DB_PASSWORD",icon["TCON_DB_PASSWORD"].toString());
                    ConfigInfo.insert("TCON_DB_PORT",icon["TCON_DB_PORT"].toString());

                    ConfigInfo.insert("TCON_UP_BOARD_LEFT_PLC_IP",icon["TCON_UP_BOARD_LEFT_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_UP_BOARD_LEFT_PLC_PORT",icon["TCON_UP_BOARD_LEFT_PLC_PORT"].toString());
                    ConfigInfo.insert("TCON_UP_BOARD_RIGHT_PLC_IP",icon["TCON_UP_BOARD_RIGHT_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_UP_BOARD_RIGHT_PLC_PORT",icon["TCON_UP_BOARD_RIGHT_PLC_PORT"].toString());
                    ConfigInfo.insert("TCON_DIVIDE_BOARD_LEFT_PLC_IP",icon["TCON_DIVIDE_BOARD_LEFT_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_DIVIDE_BOARD_LEFT_PLC_PORT",icon["TCON_DIVIDE_BOARD_LEFT_PLC_PORT"].toString());
                    ConfigInfo.insert("TCON_DIVIDE_BOARD_RIGHT_PLC_IP",icon["TCON_DIVIDE_BOARD_RIGHT_PLC_IP"].toString());
                    ConfigInfo.insert("TCON_DIVIDE_BOARD_RIGHT_PLC_PORT",icon["TCON_DIVIDE_BOARD_RIGHT_PLC_PORT"].toString());
                }
            }
        }

        if(m_pRootObj.contains("AlterProduction"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("AlterProduction"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    if(i==0)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        //load the pre board info
                        QString qStr[5];
                        qStr[0] = icon["PROJECT_ID_A"].toString();
                        qStr[1] = icon["HALF_MATERIAL_NUMBER_A"].toString();
                        qStr[2] = icon["MACHINE_STYLE_NAME_A"].toString();
                        qStr[3] = icon["BOARD_A_STYLE"].toString();
                        qStr[4] = icon["BOARD_SIZE_A"].toString();
                        for (int m=0;m<5;m++)
                            ListBoardInfoA.push_back(qStr[m]);

                        ConfigInfo.insert("BOARD_A_SN",icon["BOARD_A_SN"].toString());
                        ConfigInfo.insert("BOARD_A_STYLE",icon["BOARD_A_STYLE"].toString());
                        ConfigInfo.insert("MACHINE_STYLE_NAME_A",icon["MACHINE_STYLE_NAME_A"].toString());
                        ConfigInfo.insert("HALF_MATERIAL_NUMBER_A",icon["HALF_MATERIAL_NUMBER_A"].toString());
                        ConfigInfo.insert("PROJECT_ID_A",icon["PROJECT_ID_A"].toString());
                        ConfigInfo.insert("BOARD_SIZE_A",icon["BOARD_SIZE_A"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A1",icon["MACHINE_STATION_A1"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A2",icon["MACHINE_STATION_A2"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A3",icon["MACHINE_STATION_A3"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A4",icon["MACHINE_STATION_A4"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A5",icon["MACHINE_STATION_A5"].toString());
                    }
                    else
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        //load the pre board info
                        QString qStr[5];
                        qStr[0] = icon["PROJECT_ID_B"].toString();
                        qStr[1] = icon["HALF_MATERIAL_NUMBER_B"].toString();
                        qStr[2] = icon["MACHINE_STYLE_NAME_B"].toString();
                        qStr[3] = icon["BOARD_B_STYLE"].toString();
                        qStr[4] = icon["BOARD_SIZE_B"].toString();
                        for (int m=0;m<5;m++)
                            ListBoardInfoB.push_back(qStr[m]);

                        ConfigInfo.insert("BOARD_B_SN",icon["BOARD_B_SN"].toString());
                        ConfigInfo.insert("BOARD_B_STYLE",icon["BOARD_B_STYLE"].toString());
                        ConfigInfo.insert("MACHINE_STYLE_NAME_B",icon["MACHINE_STYLE_NAME_B"].toString());
                        ConfigInfo.insert("HALF_MATERIAL_NUMBER_B",icon["HALF_MATERIAL_NUMBER_B"].toString());
                        ConfigInfo.insert("PROJECT_ID_B",icon["PROJECT_ID_B"].toString());
                        ConfigInfo.insert("BOARD_SIZE_B",icon["BOARD_SIZE_B"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B1",icon["MACHINE_STATION_B1"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B2",icon["MACHINE_STATION_B2"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B3",icon["MACHINE_STATION_B3"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B4",icon["MACHINE_STATION_B4"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B5",icon["MACHINE_STATION_B5"].toString());
                    }
                }
            }
        }

        if(m_pRootObj.contains("PathConfigs"))
        {
            QJsonValue portValue = m_pRootObj.value(QStringLiteral("PathConfigs"));
            if(portValue.isObject())
            {
                QJsonObject obj = portValue.toObject();
                if(obj.contains("TBOX_SRC_FILE_PATH"))
                    ConfigInfo.insert("TBOX_SRC_FILE_PATH",obj.value("TBOX_SRC_FILE_PATH").toString());
                if(obj.contains("TBOX_DST_FILE_PATH"))
                    ConfigInfo.insert("TBOX_DST_FILE_PATH",obj.value("TBOX_DST_FILE_PATH").toString());
                if(obj.contains("TBOX_STATION1"))
                    ConfigInfo.insert("TBOX_STATION1",obj.value("TBOX_STATION1").toString());
                if(obj.contains("TBOX_STATION2"))
                    ConfigInfo.insert("TBOX_STATION2",obj.value("TBOX_STATION2").toString());
                if(obj.contains("TBOX_STATION3"))
                    ConfigInfo.insert("TBOX_STATION3",obj.value("TBOX_STATION3").toString());
                if(obj.contains("TBOX_STATION4"))
                    ConfigInfo.insert("TBOX_STATION4",obj.value("TBOX_STATION4").toString());
                if(obj.contains("TBOX_STATION5"))
                    ConfigInfo.insert("TBOX_STATION5",obj.value("TBOX_STATION5").toString());
                if(obj.contains("TBOX_STATION6"))
                    ConfigInfo.insert("TBOX_STATION6",obj.value("TBOX_STATION6").toString());
                if(obj.contains("TBOX_STATION7"))
                    ConfigInfo.insert("TBOX_STATION7",obj.value("TBOX_STATION7").toString());
                if(obj.contains("TBOX_STATION8"))
                    ConfigInfo.insert("TBOX_STATION8",obj.value("TBOX_STATION8").toString());
                if(obj.contains("TBOX_STATION_PORT"))
                    ConfigInfo.insert("TBOX_STATION_PORT",obj.value("TBOX_STATION_PORT").toString());
                if(obj.contains("TBOX_SCREW_MACHINE_WORK_STATION_PCB"))
                    ConfigInfo.insert("TBOX_SCREW_MACHINE_WORK_STATION_PCB",obj.value("TBOX_SCREW_MACHINE_WORK_STATION_PCB").toString());
                if(obj.contains("TBOX_SCREW_MACHINE_WORK_STATION_LOCK_SCREW"))
                    ConfigInfo.insert("TBOX_SCREW_MACHINE_WORK_STATION_LOCK_SCREW",obj.value("TBOX_SCREW_MACHINE_WORK_STATION_LOCK_SCREW").toString());
            }
        }

        if(m_pRootObj.contains("PortConfigs"))
        {
            QJsonValue portValue = m_pRootObj.value(QStringLiteral("PortConfigs"));
            if(portValue.isObject())
            {
                QJsonObject obj = portValue.toObject();
                if(obj.contains("AOI_PORT"))
                    ConfigInfo.insert("AOI_PORT",obj.value("AOI_PORT").toString());
                if(obj.contains("PLC_PORT"))
                    ConfigInfo.insert("PLC_PORT",obj.value("PLC_PORT").toString());
                if(obj.contains("REGISTER_PORT"))
                    ConfigInfo.insert("REGISTER_PORT",obj.value("REGISTER_PORT").toString());
                if(obj.contains("VISUAL_LEAD_PORT"))
                    ConfigInfo.insert("VISUAL_LEAD_PORT",obj.value("VISUAL_LEAD_PORT").toString());
                if(obj.contains("MES_PORT"))
                    ConfigInfo.insert("MES_PORT",obj.value("MES_PORT").toString());
                if(obj.contains("CENTRAL_CONTROL_PORT"))
                    ConfigInfo.insert("CENTRAL_CONTROL_PORT",obj.value("CENTRAL_CONTROL_PORT").toString());
                if(obj.contains("COM_PORT"))
                    ConfigInfo.insert("COM_PORT",obj.value("COM_PORT").toString());
                if(obj.contains("COM_BARD_RATE"))
                    ConfigInfo.insert("COM_BARD_RATE",obj.value("COM_BARD_RATE").toString());
                if(obj.contains("COM_PORT_LIGHT_BAR"))
                    ConfigInfo.insert("COM_PORT_LIGHT_BAR",obj.value("COM_PORT_LIGHT_BAR").toString());
                if(obj.contains("COM_BARD_RATE_LIGHT_BAR"))
                    ConfigInfo.insert("COM_BARD_RATE_LIGHT_BAR",obj.value("COM_BARD_RATE_LIGHT_BAR").toString());
                if(obj.contains("COM_BARD_RATE_TBOX"))
                    ConfigInfo.insert("COM_BARD_RATE_TBOX",obj.value("COM_BARD_RATE_TBOX").toString());
                if(obj.contains("COM_PORT_TBOX"))
                    ConfigInfo.insert("COM_PORT_TBOX",obj.value("COM_PORT_TBOX").toString());
                if(obj.contains("LIGHT_BAR_MES_PORT"))
                    ConfigInfo.insert("LIGHT_BAR_MES_PORT",obj.value("LIGHT_BAR_MES_PORT").toString());
                if(obj.contains("TCP_SERVER_PORT"))
                    ConfigInfo.insert("TCP_SERVER_PORT",obj.value("TCP_SERVER_PORT").toString());
            }
        }

        if(m_pRootObj.contains("AutoLogin"))
        {
            QJsonValue autologinValue = m_pRootObj.value(QStringLiteral("AutoLogin"));
            if(autologinValue.isObject())
            {
                QJsonObject obj = autologinValue.toObject();
                if(obj.contains("AUTO_LOGIN_SIGN"))
                    ConfigInfo.insert("AUTO_LOGIN_SIGN",obj.value("AUTO_LOGIN_SIGN").toString());
                if(obj.contains("USER_NAME"))
                    ConfigInfo.insert("USER_NAME",obj.value("USER_NAME").toString());
                if(obj.contains("PASSWORD"))
                    ConfigInfo.insert("PASSWORD",obj.value("PASSWORD").toString());
                if(obj.contains("PLC_WAKE_UP"))
                    ConfigInfo.insert("PLC_WAKE_UP",obj.value("PLC_WAKE_UP").toString());
                if(obj.contains("SCANNER_TIMER"))
                    ConfigInfo.insert("SCANNER_TIMER",obj.value("SCANNER_TIMER").toString());
                if(obj.contains("LOG_LEVEL"))
                    ConfigInfo.insert("LOG_LEVEL",obj.value("LOG_LEVEL").toString());
            }
        }

        if(m_pRootObj.contains("RedFlagConfigs"))
        {
            QJsonObject objValue = m_pRootObj.value(QStringLiteral("RedFlagConfigs")).toObject();
            if(objValue.contains("MiddleFrame"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("MiddleFrame"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("MIDDLE_FRAME_STATION_CODE"))
                        ConfigInfo.insert("MIDDLE_FRAME_STATION_CODE",obj.value("MIDDLE_FRAME_STATION_CODE").toString());
                    if(obj.contains("MIDDLE_FRAME_EQUIPMENT_CODE"))
                        ConfigInfo.insert("MIDDLE_FRAME_EQUIPMENT_CODE",obj.value("MIDDLE_FRAME_EQUIPMENT_CODE").toString());
                    if(obj.contains("MIDDLE_FRAME_EQUIPMENT_NAME"))
                        ConfigInfo.insert("MIDDLE_FRAME_EQUIPMENT_NAME",obj.value("MIDDLE_FRAME_EQUIPMENT_NAME").toString());
                    if(obj.contains("MIDDLE_FRAME_GLUE_NUMBER"))
                        ConfigInfo.insert("MIDDLE_FRAME_GLUE_NUMBER",obj.value("MIDDLE_FRAME_GLUE_NUMBER").toString());
                    if(obj.contains("MIDDLE_FRAME_GLUE_SCAN_TIME"))
                        ConfigInfo.insert("MIDDLE_FRAME_GLUE_SCAN_TIME",obj.value("MIDDLE_FRAME_GLUE_SCAN_TIME").toString());
                    if(obj.contains("MIDDLE_FRAME_GLUE_INTERVAL_TIME"))
                        ConfigInfo.insert("MIDDLE_FRAME_GLUE_INTERVAL_TIME",obj.value("MIDDLE_FRAME_GLUE_INTERVAL_TIME").toString());
                }
            }

            if(objValue.contains("CCDKeyPart"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("CCDKeyPart"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("CCD_KEY_PART_EQUIPMENT_CODE"))
                        ConfigInfo.insert("CCD_KEY_PART_EQUIPMENT_CODE",obj.value("CCD_KEY_PART_EQUIPMENT_CODE").toString());
                    if(obj.contains("CCD_KEY_PART_EQUIPMENT_NAME"))
                        ConfigInfo.insert("CCD_KEY_PART_EQUIPMENT_NAME",obj.value("CCD_KEY_PART_EQUIPMENT_NAME").toString());
                    if(obj.contains("CCD_KEY_PART_STATION_CODE"))
                        ConfigInfo.insert("CCD_KEY_PART_STATION_CODE",obj.value("CCD_KEY_PART_STATION_CODE").toString());
                }
            }

            if(objValue.contains("MainBoard"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("MainBoard"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("MAIN_BOARD_STATION_CODE"))
                        ConfigInfo.insert("MAIN_BOARD_STATION_CODE",obj.value("MAIN_BOARD_STATION_CODE").toString());
                    if(obj.contains("MAIN_BOARD_EQUIPMENT_CODE"))
                        ConfigInfo.insert("MAIN_BOARD_EQUIPMENT_CODE",obj.value("MAIN_BOARD_EQUIPMENT_CODE").toString());
                    if(obj.contains("MAIN_BOARD_EQUIPMENT_NAME"))
                        ConfigInfo.insert("MAIN_BOARD_EQUIPMENT_NAME",obj.value("MAIN_BOARD_EQUIPMENT_NAME").toString());
                    if(obj.contains("MAIN_BOARD_GLUE_NUMBER"))
                        ConfigInfo.insert("MAIN_BOARD_GLUE_NUMBER",obj.value("MAIN_BOARD_GLUE_NUMBER").toString());
                }
            }
            if(objValue.contains("FPCPressFit"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("FPCPressFit"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("PRESS_FIT_STATION_CODE"))
                        ConfigInfo.insert("PRESS_FIT_STATION_CODE",obj.value("PRESS_FIT_STATION_CODE").toString());
                    if(obj.contains("PRESS_FIT_EQUIPMENT_CODE"))
                        ConfigInfo.insert("PRESS_FIT_EQUIPMENT_CODE",obj.value("PRESS_FIT_EQUIPMENT_CODE").toString());
                    if(obj.contains("PRESS_FIT_EQUIPMENT_NAME"))
                        ConfigInfo.insert("PRESS_FIT_EQUIPMENT_NAME",obj.value("PRESS_FIT_EQUIPMENT_NAME").toString());
                }
            }
            if(objValue.contains("RedFlagPortPress"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("RedFlagPortPress"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("COM_PORT_RED_FLAG_PRESS"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_PRESS",obj.value("COM_PORT_RED_FLAG_PRESS").toString());
                    if(obj.contains("COM_PORT_RED_FLAG_PRESS_BARD_RATE"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_PRESS_BARD_RATE",obj.value("COM_PORT_RED_FLAG_PRESS_BARD_RATE").toString());
                    if(obj.contains("RED_FLAG_PRESS_TIMEOUT"))
                        ConfigInfo.insert("RED_FLAG_PRESS_TIMEOUT",obj.value("RED_FLAG_PRESS_TIMEOUT").toString());
                    if(obj.contains("COM_PORT_RED_FLAG_PRESS_METER"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_PRESS_METER",obj.value("COM_PORT_RED_FLAG_PRESS_METER").toString());
                    if(obj.contains("COM_PORT_RED_FLAG_PRESS_METER_BARD_RATE"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_PRESS_METER_BARD_RATE",obj.value("COM_PORT_RED_FLAG_PRESS_METER_BARD_RATE").toString());
                    if(obj.contains("COM_PORT_MANO_METER"))
                        ConfigInfo.insert("COM_PORT_MANO_METER",obj.value("COM_PORT_MANO_METER").toString());
                    if(obj.contains("COM_PORT_MANO_METER_BARD_RATE"))
                        ConfigInfo.insert("COM_PORT_MANO_METER_BARD_RATE",obj.value("COM_PORT_MANO_METER_BARD_RATE").toString());
                    if(obj.contains("PRESSURE_LOW_VALUE"))
                        ConfigInfo.insert("PRESSURE_LOW_VALUE",obj.value("PRESSURE_LOW_VALUE").toString());
                    if(obj.contains("PRESSURE_HIGH_VALUE"))
                        ConfigInfo.insert("PRESSURE_HIGH_VALUE",obj.value("PRESSURE_HIGH_VALUE").toString());
                }
            }
            if(objValue.contains("ReadFlagPrintSet"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("ReadFlagPrintSet"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("RED_FLAG_PRINT_MODEL_PATH"))
                        ConfigInfo.insert("RED_FLAG_PRINT_MODEL_PATH",obj.value("RED_FLAG_PRINT_MODEL_PATH").toString());
                    if(obj.contains("RED_FLAG_PRINT_PORT"))
                        ConfigInfo.insert("RED_FLAG_PRINT_PORT",obj.value("RED_FLAG_PRINT_PORT").toString());
                    if(obj.contains("RED_FLAG_PRINT_SN_INFO"))
                        ConfigInfo.insert("RED_FLAG_PRINT_SN_INFO",obj.value("RED_FLAG_PRINT_SN_INFO").toString());
                }
            }

            if(objValue.contains("RedFlagPortGlue"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("RedFlagPortGlue"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("COM_PORT_RED_FLAG_GLUE"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_GLUE",obj.value("COM_PORT_RED_FLAG_GLUE").toString());
                    if(obj.contains("COM_PORT_RED_FLAG_GLUE_BARD_RATE"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_GLUE_BARD_RATE",obj.value("COM_PORT_RED_FLAG_GLUE_BARD_RATE").toString());
                    if(obj.contains("COM_PORT_RED_FLAG_WEIGH"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_WEIGH",obj.value("COM_PORT_RED_FLAG_WEIGH").toString());
                    if(obj.contains("COM_PORT_RED_FLAG_WEIGH_BARD_RATE"))
                        ConfigInfo.insert("COM_PORT_RED_FLAG_WEIGH_BARD_RATE",obj.value("COM_PORT_RED_FLAG_WEIGH_BARD_RATE").toString());
                    if(obj.contains("RED_FLAG_GLUE_DELAY_MS"))
                        ConfigInfo.insert("RED_FLAG_GLUE_DELAY_MS",obj.value("RED_FLAG_GLUE_DELAY_MS").toString());
                    if(obj.contains("RED_FLAG_GLUE_TIMEOUT"))
                        ConfigInfo.insert("RED_FLAG_GLUE_TIMEOUT",obj.value("RED_FLAG_GLUE_TIMEOUT").toString());
                    if(obj.contains("RED_FLAG_SCANNER_BARD_RATE"))
                        ConfigInfo.insert("RED_FLAG_SCANNER_BARD_RATE",obj.value("RED_FLAG_SCANNER_BARD_RATE").toString());
                    if(obj.contains("RED_FLAG_SCANNER_PORT"))
                        ConfigInfo.insert("RED_FLAG_SCANNER_PORT",obj.value("RED_FLAG_SCANNER_PORT").toString());
                }
            }
            if(objValue.contains("TestGlueDispense")){
                QJsonValue subValue = objValue.value(QStringLiteral("TestGlueDispense"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("GLUE_DISPENSE_EQUIPMENT_CODE"))
                        ConfigInfo.insert("GLUE_DISPENSE_EQUIPMENT_CODE",obj.value("GLUE_DISPENSE_EQUIPMENT_CODE").toString());
                    if(obj.contains("GLUE_DISPENSE_EQUIPMENT_NAME"))
                        ConfigInfo.insert("GLUE_DISPENSE_EQUIPMENT_NAME",obj.value("GLUE_DISPENSE_EQUIPMENT_NAME").toString());
                    if(obj.contains("GLUE_DISPENSE_GLUE_NUMBER"))
                        ConfigInfo.insert("GLUE_DISPENSE_GLUE_NUMBER",obj.value("GLUE_DISPENSE_GLUE_NUMBER").toString());
                    if(obj.contains("GLUE_DISPENSE_STATION_CODE"))
                        ConfigInfo.insert("GLUE_DISPENSE_STATION_CODE",obj.value("GLUE_DISPENSE_STATION_CODE").toString());
                    if(obj.contains("GLUE_DISPENSE_WORKSTATION_SN"))
                        ConfigInfo.insert("GLUE_DISPENSE_WORKSTATION_SN",obj.value("GLUE_DISPENSE_WORKSTATION_SN").toString());
                    if(obj.contains("GLUE_DISPENSE_STANDARD_VALUE"))
                        ConfigInfo.insert("GLUE_DISPENSE_STANDARD_VALUE",obj.value("GLUE_DISPENSE_STANDARD_VALUE").toString());
                    if(obj.contains("GLUE_DISPENSE_UPPER_VALUE"))
                        ConfigInfo.insert("GLUE_DISPENSE_UPPER_VALUE",obj.value("GLUE_DISPENSE_UPPER_VALUE").toString());
                    if(obj.contains("GLUE_DISPENSE_LOWER_VALUE"))
                        ConfigInfo.insert("GLUE_DISPENSE_LOWER_VALUE",obj.value("GLUE_DISPENSE_LOWER_VALUE").toString());
                }
            }

            if(objValue.contains("RedFlagMesConfig"))
            {
                QJsonValue subValue = objValue.value(QStringLiteral("RedFlagMesConfig"));
                if(subValue.isObject())
                {
                    QJsonObject obj = subValue.toObject();
                    if(obj.contains("MES_CHECK_URL"))
                        ConfigInfo.insert("MES_CHECK_URL",obj.value("MES_CHECK_URL").toString());
                    if(obj.contains("MES_SAVE_URL"))
                        ConfigInfo.insert("MES_SAVE_URL",obj.value("MES_SAVE_URL").toString());
                }
            }
        }

        if(m_pRootObj.contains("ConfigInstructions"))
        {
            QJsonValue instructionValue = m_pRootObj.value(QStringLiteral("ConfigInstructions"));
            if(instructionValue.isString())
                qDebug()<<instructionValue.toString();
        }
    }
    else
    {
        QLOG_INFO()<<"load json config file failed!";
        return;
    }
    m_pConfigFile.flush();
}

QString GDataFactory::get_config_para(const QString keyname)
{
    return ConfigInfo[keyname];
}

void GDataFactory::set_config_para_1(const QString keyname, const QString keyvalue)
{
    ConfigInfo[keyname] = keyvalue;//update the config file,at the same time you have to update the info that has been loaded

    QJsonObject subObj;
    QJsonArray subArray;

    if(keyname == "AUTO_LOGIN_SIGN" || keyname == "USER_NAME" || keyname == "PASSWORD")
    {
        if(m_pRootObj.contains("AutoLogin") && m_pRootObj["AutoLogin"].isObject())
        {
            QJsonValueRef refValue = m_pRootObj.find("AutoLogin").value();
            QJsonObject obj = refValue.toObject();
            obj[keyname] = keyvalue;
            refValue = obj;
        }
    }

    if(keyname == "PRESS_FIT_EQUIPMENT_CODE" || keyname == "PRESS_FIT_EQUIPMENT_NAME" ||
            keyname == "PRESS_FIT_STATION_CODE")
    {
        if(m_pRootObj.contains("RedFlagConfigs") && m_pRootObj["RedFlagConfigs"].isObject())
        {
            QJsonObject inObj = m_pRootObj["RedFlagConfigs"].toObject();
            if(inObj.contains("FPCPressFit") && inObj["FPCPressFit"].isObject())
            {
                QJsonObject fpcObj = inObj["FPCPressFit"].toObject();
                fpcObj[keyname] = keyvalue;
                inObj["FPCPressFit"] = fpcObj;
            }
            m_pRootObj["RedFlagConfigs"] = inObj;
        }
    }

    if(keyname == "MAIN_BOARD_EQUIPMENT_CODE" || keyname == "MAIN_BOARD_EQUIPMENT_NAME" ||
            keyname == "MAIN_BOARD_GLUE_NUMBER" || keyname == "MAIN_BOARD_STATION_CODE")
    {
        if(m_pRootObj.contains("RedFlagConfigs") && m_pRootObj["RedFlagConfigs"].isObject())
        {
            QJsonObject inObj = m_pRootObj["RedFlagConfigs"].toObject();
            if(inObj.contains("MainBoard") && inObj["MainBoard"].isObject())
            {
                QJsonObject fpcObj = inObj["MainBoard"].toObject();
                fpcObj[keyname] = keyvalue;
                inObj["MainBoard"] = fpcObj;
            }
            m_pRootObj["RedFlagConfigs"] = inObj;
        }
    }

    if(keyname == "MIDDLE_FRAME_EQUIPMENT_CODE" || keyname == "MIDDLE_FRAME_EQUIPMENT_NAME" ||
            keyname == "MIDDLE_FRAME_GLUE_NUMBER" || keyname == "MIDDLE_FRAME_STATION_CODE" ||
            keyname == "MIDDLE_FRAME_GLUE_SCAN_TIME")
    {
        if(m_pRootObj.contains("RedFlagConfigs") && m_pRootObj["RedFlagConfigs"].isObject())
        {
            QJsonObject inObj = m_pRootObj["RedFlagConfigs"].toObject();
            if(inObj.contains("MiddleFrame") && inObj["MiddleFrame"].isObject())
            {
                QJsonObject fpcObj = inObj["MiddleFrame"].toObject();
                fpcObj[keyname] = keyvalue;
                inObj["MiddleFrame"] = fpcObj;
            }
            m_pRootObj["RedFlagConfigs"] = inObj;
        }
    }

    if(keyname == "GLUE_DISPENSE_EQUIPMENT_CODE" || keyname == "GLUE_DISPENSE_EQUIPMENT_NAME" ||
            keyname == "GLUE_DISPENSE_GLUE_NUMBER" || keyname == "GLUE_DISPENSE_LOWER_VALUE" ||
        keyname == "GLUE_DISPENSE_STANDARD_VALUE" || keyname == "GLUE_DISPENSE_STATION_CODE" ||
            keyname == "GLUE_DISPENSE_UPPER_VALUE")
    {
        if(m_pRootObj.contains("RedFlagConfigs") && m_pRootObj["RedFlagConfigs"].isObject())
        {
            QJsonObject inObj = m_pRootObj["RedFlagConfigs"].toObject();
            if(inObj.contains("TestGlueDispense") && inObj["TestGlueDispense"].isObject())
            {
                QJsonObject fpcObj = inObj["TestGlueDispense"].toObject();
                fpcObj[keyname] = keyvalue;
                inObj["TestGlueDispense"] = fpcObj;
            }
            m_pRootObj["RedFlagConfigs"] = inObj;
        }
    }

    if(keyname == "PRESSURE_LOW_VALUE" || keyname == "PRESSURE_HIGH_VALUE")
    {
        if(m_pRootObj.contains("RedFlagConfigs") && m_pRootObj["RedFlagConfigs"].isObject())
        {
            QJsonObject inObj = m_pRootObj["RedFlagConfigs"].toObject();
            if(inObj.contains("RedFlagPortPress") && inObj["RedFlagPortPress"].isObject())
            {
                QJsonObject fpcObj = inObj["RedFlagPortPress"].toObject();
                fpcObj[keyname] = keyvalue;
                inObj["RedFlagPortPress"] = fpcObj;
            }
            m_pRootObj["RedFlagConfigs"] = inObj;
        }
    }

    if((keyname == "BOARD_A_SN")
            || (keyname == "MACHINE_STATION_A1")|| (keyname == "MACHINE_STATION_A2")
            || (keyname == "MACHINE_STATION_A3")|| (keyname == "MACHINE_STATION_A4")
            || (keyname == "MACHINE_STATION_A5")|| (keyname == "BOARD_A_STYLE")
            || (keyname == "MACHINE_STYLE_NAME_A")|| (keyname == "HALF_MATERIAL_NUMBER_A")
            || (keyname == "BOARD_SIZE_A")|| (keyname == "PROJECT_ID_A"))
    {
        QJsonValueRef refValue = m_pRootObj.find("AlterProduction").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[0];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }

    if((keyname == "BOARD_B_SN")
            || (keyname == "MACHINE_STATION_B1")|| (keyname == "MACHINE_STATION_B2")
            || (keyname == "MACHINE_STATION_B3")|| (keyname == "MACHINE_STATION_B4")
            || (keyname == "MACHINE_STATION_B5")|| (keyname == "BOARD_B_STYLE")
            || (keyname == "MACHINE_STYLE_NAME_B")|| (keyname == "HALF_MATERIAL_NUMBER_B")
            || (keyname == "BOARD_SIZE_B")|| (keyname == "PROJECT_ID_B"))
    {
        QJsonValueRef refValue = m_pRootObj.find("AlterProduction").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[1];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }
    m_pJsonDocument.setObject(m_pRootObj);
}

void GDataFactory::save_config_file()
{
    m_pConfigFile.setFileName(m_pConfigFileName);
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_pConfigFile.resize(0);//this sentence is essential,or the file will be abnormal---20211123lilei
        m_pJsonDocument.setObject(m_pRootObj);
        m_pConfigFile.seek(0);
        m_pConfigFile.write(m_pJsonDocument.toJson());
        m_pConfigFile.close();
        m_pConfigFile.flush();
    }
}

void GDataFactory::del_change_production_machine_name(QString machinename)
{
    int i=0;
    bool hvFound = false;
    for (i=0;i<m_pMachineNamesOfChangeProduction.length();i++)
    {
        if(m_pMachineNamesOfChangeProduction[i] == machinename)
        {
            QLOG_ERROR()<<u8"发送信号";
            emit signal_del_one_change_product_info(machinename);
//            del_one_change_product_info(machinename);
            QLOG_INFO()<<machinename<<" change production SUCCESS!";
            hvFound = true;
        }
        else
            continue;
    }

    if(hvFound)
    {
//        m_pMachineNamesOfChangeProduction.removeAt(i);
        m_pMachineNamesOfChangeProduction.removeOne(machinename);
        if(m_pMachineNamesOfChangeProduction.length() == 0)
        {
//            truncate_change_info_table();
            emit signal_change_production_already_complete();
        }
    }
}

QMap<QString, QString> GDataFactory::get_PLC_or_AOI_change_info(QString keyvalue)
{
//    QMutexLocker lock(&m_mutex_PLC_AOI);
    QMap<QString,QString> tmpMap;
    if(mapOneButtonToVisionOrAOI.find(keyvalue) != mapOneButtonToVisionOrAOI.end())
        return mapOneButtonToVisionOrAOI[keyvalue];
    else {
        return tmpMap;
    }
}

void GDataFactory::add_change_production_info_for_PLC_or_AOI(QString machinename,QString boardtype,QString halfmaterialnumber)
{
//    QMutexLocker lock(&m_mutex_PLC_AOI);
    if(mapOneButtonToVisionOrAOI.find(machinename) != mapOneButtonToVisionOrAOI.end())
        ;
    else {
        QMap<QString,QString> tmpMap;
        QLOG_INFO()<<machinename<<" change production board_type:"<<boardtype<<",half_material_number:"<<halfmaterialnumber;
        tmpMap.insert(boardtype,halfmaterialnumber);
        mapOneButtonToVisionOrAOI.insert(machinename,tmpMap);
    }
}

QString GDataFactory::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]) ;
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}

quint8 GDataFactory::get_last_ip(const int id)
{
    if (id == 0)
    {
        QString localHostName = QHostInfo::localHostName();
        QString localIP = "";
//        qDebug()<<"localHostName:"<<localHostName;
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress addr,info.addresses())
            if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
//                qDebug()<<"\033[31mIPV4 Address:"<<addr.toString();
                localIP = addr.toString();
            }
        return process_ip_string(localIP);
    }
    else
    {
        return process_ip_string(get_config_para("PLC_IP"));
    }
}

void GDataFactory::init_http_service()
{
    QString configFileName = search_http_config_file();
    if(configFileName == nullptr)
        QLOG_WARN()<<"search http config file failed!";
    else
    {
        // Configure and start the TCP listener
        QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,this);
        listenerSettings->beginGroup("listener");
        new HttpListener(listenerSettings,new HttpRequestHandler(this),this);
        QLOG_INFO()<<"init http service success!";
    }
}

void GDataFactory::clear_alter_production()
{
    board_A_serial_number = "";
    board_B_serial_number = "";
    boardSign = 0;
    m_pMachineNamesOfChangeProduction.clear();
}

QList<QList<QString> > GDataFactory::get_all_machine_change_info()
{
//    QMutexLocker lock(&m_mutex);
    QList<QList<QString>> lstAll;
    QString strSql = QString("select * from public.%1").arg(constCurrentChangeInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
//        QLOG_INFO()<<"query success!";
        while(queryResult.next())
        {
            QList<QString> lstChangeInfo;
            lstChangeInfo.push_back(queryResult.value(1).toString());
            lstChangeInfo.push_back(queryResult.value(2).toString());
            lstChangeInfo.push_back(queryResult.value(3).toString());
            lstChangeInfo.push_back(queryResult.value(4).toString());
            lstChangeInfo.push_back(queryResult.value(5).toString());
            lstChangeInfo.push_back(queryResult.value(6).toString());
            lstChangeInfo.push_back(queryResult.value(7).toString());
            lstAll.push_back(lstChangeInfo);
        }
    }
    else {
        QLOG_WARN()<<"query failed!";
    }
    return lstAll;
}

QSqlQuery GDataFactory::get_query_result(QString strSql)
{
    QMutexLocker lock(&m_mutex);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
        QLOG_INFO()<<"GDataFactory get_query_result success!";
    else
        QLOG_WARN()<<"GDataFactory get_query_result failed!";
    return queryResult;
}

bool GDataFactory::truncate_change_info_table()
{
    QMutexLocker lock(&m_mutex);
    QString strTruncate = QString("truncate table public.%1").arg(constCurrentChangeInfoTable);
    QString tmpEr;
    if(GDataFactory::get_pgsql()->ExecSql(strTruncate,tmpEr))
    {
        GDataFactory::get_board_style_wgtA()->save_plan_info();
        GDataFactory::get_board_style_wgtB()->save_plan_info();
        return true;
    }
    else {
        QLOG_WARN()<<"truncate the change info table failed!";
        return false;
    }
}

bool GDataFactory::truncate_change_info_table_pure()
{
    QMutexLocker lock(&m_mutex);
    QString strTruncate = QString("truncate table public.%1").arg(constCurrentChangeInfoTable);
    QString tmpEr;
    if(GDataFactory::get_pgsql()->ExecSql(strTruncate,tmpEr))
    {
        QLOG_WARN()<<"truncate the change info table success!";
        return true;
    }
    else {
        QLOG_WARN()<<"truncate the change info table failed!";
        return false;
    }
}

QString GDataFactory::HandleStr(int i)
{
    QString str,sResult;
        if(i<10)
        {
            str = QString::number(i);
            return "0000"+str;
        }
        else if(i<100)
        {
            str = QString::number(i);
            return "000"+str;
        }
        else if(i<1000)
        {
            str = QString::number(i);
            return "00"+str;
        }
        else if(i<10000)
        {
            str = QString::number(i);
            return "0"+str;
        }
        else
        {
            str = QString::number(i);
            return str;
        }
}

QString GDataFactory::DecodeStr(QString s)
{
    int i,l;
    int m[10];
    QChar n[10];
    QString ss[10],str,sResult;
    int Seed[10]={8,13,19,11,9,21,8,13,19,11};
    l = s.length()/5;
    for (i=0;i<=l-1;i++)
    {
        ss[i]=s.mid(i*5,5);
    }
    for (i=0;i<=l-1;i++)
    {
        m[i]=ss[i].toInt();
        int tmpR = static_cast<int>(qSqrt(static_cast<qreal>(m[i]+Seed[i])))-static_cast<int>(Seed[i]);
        n[i]=tmpR;
    }
    for (i=0;i<=l-1;i++)
        sResult=sResult+n[i];
    return sResult;
}

void GDataFactory::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        changInfoEx = get_all_machine_change_info();
    }
}

void GDataFactory::start_query_timer()
{
    m_pTimerID = startTimer(1000);
}

QString GDataFactory::EncodeStr(QString password)
{
    int Seed[10]={8,13,19,11,9,21,8,13,19,11};
    int i,l;
    int n[10],m[10];
    QString ss[10],sResult;
    l = password.length();
    for (i=0;i<=l-1;i++)
    {
        n[i]=password[i].toLatin1();
        m[i]=(n[i]+Seed[i])*(n[i]+Seed[i])-Seed[i];
        ss[i]=HandleStr(m[i]);
    }
    for(i=0;i<=l-1;i++)
        sResult=sResult+ss[i];
    return sResult;
}

void GDataFactory::save_AB_change_product_info(QList<QString> changeinfo)
{
//    QMutexLocker lock(&m_mutex);
    if(changeinfo.count()==9)
    {
        //check machine name exist in db or not,if not exist insert the current info,if exist update the entry
//        changeinfo[3].prepend("\'");
//        changeinfo[3].append("\'");
        QString strSql = QString("select * from public.%1 where \"Station_Name\"=\'%2\'").arg(constCurrentChangeInfoTable).arg(changeinfo[3]);
        QSqlQuery queryResult;

        int maxID = get_biggest_id(constCurrentChangeInfoTable);
        maxID+=1;
        strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constCurrentChangeInfoTable).
                    arg(maxID).
                    arg(changeinfo[0]).
                    arg(changeinfo[1]).
                    arg(changeinfo[2]).
                    arg(changeinfo[3]).
                    arg(changeinfo[4]).
                    arg(changeinfo[5]).
                    arg(changeinfo[6]).
                    arg(changeinfo[7]).
                    arg(changeinfo[8]);
        QString strErrorMsg;
        if(get_pgsql()->ExecSql(strSql,strErrorMsg))
            QLOG_INFO()<<"insert change product info success!";
        else
            QLOG_WARN()<<"insert change product info failed!";
    }
    else
    {
        QLOG_WARN()<<"save AB change product info to DB failed!";
    }
}

void GDataFactory::set_AB_change_product_info(QList<QString> boardinfo)
{
    //after scan the serial code and request data from mes,save the change product info
    if(boardSign == 0)
    {
        ListBoardInfoA = boardinfo;
//        set_config_para_1("BOARD_A_STYLE",ListBoardInfoA[3]);
//        set_config_para_1("HALF_MATERIAL_NUMBER_A",ListBoardInfoA[1]);
    }
    else {
        ListBoardInfoB = boardinfo;
//        set_config_para_1("BOARD_B_STYLE",ListBoardInfoB[3]);
//        set_config_para_1("HALF_MATERIAL_NUMBER_B",ListBoardInfoB[1]);
    }
}

QList<QString> GDataFactory::get_A_change_product_info()
{
    return ListBoardInfoA;
}

QList<QString> GDataFactory::get_B_change_product_info()
{
    return ListBoardInfoB;
}

int GDataFactory::get_biggest_id(const QString &tablename)
{
    QString strSql = QString("select * from public.%1 order by \"ID\" desc").arg(tablename);
    QSqlQuery queryResult;
    int id = -1;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(id > queryResult.value(0).toInt())
                continue;
            else
                id = queryResult.value(0).toInt();
        }
        if(id == -1)
        {
            id = 1;
            return id;
        }
        else
        {
            id += 1;
            return id;
        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
        return -1;
    }
}

bool GDataFactory::check_mac_exist(const QString &macaddr,QString* machinename)
{
    QString strSql = QString("select * from public.%1").arg(constRegisterMachineTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(macaddr == queryResult.value(2).toString())
            {
                *machinename = queryResult.value(4).toString();
                return true;
            }
            else
                continue;
        }
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
        return false;
    }
    return false;
}

QString GDataFactory::register_new_mac_machine(QList<QString> lstpara)
{
    int id = get_biggest_id(constRegisterMachineTable);
    QString strResultName = QString(u8"机台%1").arg(id);

    QString strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6')").
            arg(constRegisterMachineTable).
            arg(id).
            arg(lstpara[0]).
            arg(lstpara[1]).
            arg(lstpara[2]).
            arg(strResultName);

    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        QLOG_TRACE()<<"insert into db success!";
        emit signal_register_new_station_success();
        return strResultName;
    }
    else
    {
        QLOG_WARN()<<"insert into db failed!";
        return "";
    }
}

QMap<QString,QList<QString>> GDataFactory::get_all_machine_name_from_db()
{
    QMap<QString,QList<QString>> mapInfo;

    QList<QString> lstname;
    QString strSql = QString("select * from public.%1").arg(constRegisterMachineTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
//        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(mapInfo.contains(queryResult.value(1).toString()))
                mapInfo[queryResult.value(1).toString()].push_back(queryResult.value(4).toString());
            else
            {
                mapInfo.insert(queryResult.value(1).toString(),lstname);
                mapInfo[queryResult.value(1).toString()].push_back(queryResult.value(4).toString());
            }
//            lstname.push_back(queryResult.value(4).toString());
        }
//        while(queryResult.next())
//            lstname.push_back(queryResult.value(4).toString());
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
    return mapInfo;
}

QString GDataFactory::get_line_name_by_machine_name(QString machinename)
{
    QMap<QString,QList<QString>> mapAll = get_all_machine_name_from_db();
    QMap<QString,QList<QString>>::Iterator itr = mapAll.begin();
    while(itr!= mapAll.end())
    {
        QList<QString> lstMachineName = itr.value();
        for (int i=0;i<lstMachineName.size();i++) {
            if(machinename == lstMachineName[i])
                return itr.key();
            else
                continue;
        }
        itr++;
    }
    return "";
}

QString GDataFactory::search_http_config_file()
{
    QString binDir=QApplication::applicationDirPath();
    QString fileName("httpconfig.ini");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using http ini file "<<fileName;
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        QLOG_WARN()<<dir<<fileName<<" not found";
    }
    QLOG_FATAL()<<"Cannot find http ini file "<<fileName;
    return nullptr;
}

void GDataFactory::slot_del_one_change_product_info(QString machinename)
{
//    QMutexLocker lock(&m_mutex);
    QString strSql = QString("DELETE FROM public.%1 WHERE \"Station_Name\" = \'%2\'").arg(constCurrentChangeInfoTable).arg(machinename);
    QString queryResult;
    QLOG_WARN()<<strSql;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,queryResult))
    {
        QLOG_WARN()<<u8"delete the entry "<<machinename<<" SUCESS!";
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
}

void GDataFactory::slot_rev_serial_number_tbox(const QByteArray &data)
{
    emit signal_product_sn_to_ui(QString(data));
//    get_mes_process()->exec_http_get_tbox(QString(data));
}

void GDataFactory::slot_rev_serial_number(const QByteArray &data)
{
    if(get_history_display_wgt()->isVisible())
    {
        get_history_display_wgt()->set_serial_number(QString(data));
        //when we get sn from serial port,send the get msg to MES to get the board with the current sn details
        GDataFactory::get_mes_process()->exec_http_get(QString(data));
        QLOG_INFO()<<"rev history display serial number:"<<QString(data);
    }
    else {
        QLOG_INFO()<<"the display log widget is not visible,do not set the edit text";
    }

    if(boardSign == 1)
    {
        if(get_board_style_wgtB()->isVisible())
        {
            get_board_style_wgtB()->set_serial_number(QString(data));
            board_B_serial_number = QString(data);
            //when we get sn from serial port,send the get msg to MES to get the board with the current sn details
            GDataFactory::get_mes_process()->exec_http_get(board_B_serial_number);
            QLOG_INFO()<<"rev board B serial number:"<<board_B_serial_number;
            boardSign++;
        }
        else {
            QLOG_INFO()<<"the boardB widget is not visible,do not set the edit text";
        }
    }

    if(boardSign == 0)
    {
        if(get_board_style_wgtA()->isVisible())
        {
            get_board_style_wgtA()->set_serial_number(QString(data));
            board_A_serial_number = QString(data);
            GDataFactory::get_mes_process()->exec_http_get(board_A_serial_number);
            QLOG_INFO()<<"rev board A serial number:"<<board_A_serial_number;
            boardSign++;
        }
        else {
            QLOG_INFO()<<"the boardA widget is not visible,do not set the edit text";
        }
    }
}

void GDataFactory::slot_rev_serial_number_light_bar(const QByteArray &sn)
{
    if(m_pFirstTimeSN == "")
    {
        //send product number to PLC without Mes
        m_pFirstTimeSN = sn;
        m_pFirstTimeSN.remove('\r');
        emit signal_product_sn_to_light_bar(1,m_pFirstTimeSN.toLatin1());
        //send product number to PLC without Mes
    }
    else if(m_pSecondTimeSN == ""){
        m_pSecondTimeSN = sn;
        m_pSecondTimeSN.remove('\r');
        if(m_pFirstTimeSN != m_pSecondTimeSN)
        {
            m_pSecondTimeSN = "";
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"第一次扫码内容与第二次扫码内容不一致,\r\n请重新扫码!");
            return;
        }
        emit signal_product_sn_to_light_bar(2,m_pSecondTimeSN.toLatin1());
//        get_light_bar_wgt()->set_product_sn(2,m_pSecondTimeSN);
    }
    else
        QLOG_WARN()<<"some product is on washing";
}

void GDataFactory::slot_change_production_failed(QString machinename, QString linename)
{
    Q_UNUSED(machinename);
    Q_UNUSED(linename);
    IMessageBox* msgBox = new IMessageBox(3);
    msgBox->warning(u8"当前板子配置文件不存在!");
    return;
}

static int testCount = 0;
void GDataFactory::slot_proc_fins_udp_data(QByteArray data)
{
    if(data.length() == 4)//light bar info
    {
        short a,b;
        a = static_cast<short>(data[1]);
        b = static_cast<short>(data[3]);
        if((a == 1) && (b != 0))
        {
            QByteArray clearData;
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10040,0x04,clearData);
            //finish washing
            emit signal_finish_washing(static_cast<int>(data[3]+data[2]));
        }
        else
            ;
    }
    else if(data.length() == 2)//fbj sign
    {
        if(data[0] || data[1])
        {
            QByteArray clearData;
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10010,0x02,clearData);
            QEventLoop eventloop;
            QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
            GDataFactory::get_fins_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D10000,0x14,nullptr);
        }
    }
    else if(data.length() == 20)//fbj SN
    {
        testCount++;
        QLOG_WARN()<<"the current SN is:"<<data;
        QByteArray qByteArray;
        qByteArray.append(data);
        qByteArray.append(static_cast<char>(0x00));
        qByteArray.append(static_cast<char>(0x01));
        GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10020,static_cast<short>(qByteArray.length()),qByteArray);
    }
    else
    {
        ;
    }
}

quint8 GDataFactory::process_ip_string(const QString ipstr)
{
    int nSize = ipstr.size();
    int nCount = 0;
    QString mData;
    mData.clear();
    QChar mChar;
    for(int idx = 0; idx<nSize; idx++)
    {
        mChar = ipstr[idx];
        if(mChar == '.')
        {
            nCount++;
        }
        else if(mChar != '.' && nCount == 3 && (mChar >= '0' || mChar <= '9'))
        {
            mData.append(mChar);
        }
    }
    return static_cast<quint8>(mData.toInt());
}
