#ifndef GDATAFACTORY_H
#define GDATAFACTORY_H

#include <QObject>
#include <QSerialPort>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "commonheaders.h"
#include "plcaddress.h"
#include "uiheaders.h"
#include "testdivideboardwgt.h"
#include "pressstationserialcom.h"
using namespace stefanfrings;
#define PUBLIC_KEY "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqyHeiIAoTZ1I3Nb9KE8DoSMPWEMdA4ilQGh0yGdUzqvbU2XfpK5aNIen9aWqDUb7hV5rjSc6ab/sw5Zp2EJtGKwZB/JzVaLIwlx7Dvijx9FojiQ1Ca+7qJF74b9CnDkIWrIsKBiHvFQQfgxKUACfXxwsLirfE8pcE309wcXlldBcGFY/UgyYi6MJZcP7Es9a44Fb3cH8iluEBw3x85BvGhAsFMp/1cx4FcUflgSr/L7qWIJbH90yuSaQEFJd8xMb2sJViZrogknD4KQzMd7gmdygaDtyxujh517U/vpgNGbxgpaCLt4u8EZDyP+Pbyxu4TI1LZlv3RJry95M7dn2mQIDAQAB"
class IResponseValidate_F1 : public IResponseValidate
{
public:
    virtual bool validate(const QByteArray& response)
    {
        Q_UNUSED(response);
        bool bValid = true;
        return bValid;
    }
};

enum LineInfoValue{
    TCON_LINE=0,
    ELEC_EQUIP_LINE=1,
    TV_LINE=2
};

class GDataFactory : public QObject
{
    Q_OBJECT
public:
    GDataFactory();
    ~GDataFactory();
    bool isGreater(double a, double b, double epsilon = 1e-5);

    bool isLess(double a, double b, double epsilon = 1e-5);

    TconDataAcquisitonThread ttr;
    TconDataAcquisitionAOIThread ttrAOI;
    TconDataAcquisitionPackageThread ttrPackage;
    void start_data_aquisiton();

    QByteArray alter_order_of_byte_array(QByteArray data);

    void read_serial_number();
    void stop_serial_port();

    //light-bar
    void read_serial_number_light_bar();
    void stop_serial_port_light_bar();
    //light-bar

    //TBOX
    void read_serial_number_tbox();
    void stop_serial_port_tbox();
    //TBOX

    void connect_common_signal_slot();
    void load_json_config_1(char* filename);
    QString get_config_para(const QString keyname);    
    void set_config_para_1(const QString keyname, const QString keyvalue);
    void save_config_file();

    //**after we change the production,we put the machine stations that change production into the
    //list,when we receive change production completely msg from tcon,we remove the according
    //station name from the list
    void add_change_production_machine_name(QString machinename){
        m_pMachineNamesOfChangeProduction.push_back(machinename);
    }
    void del_change_production_machine_name(QString machinename);
    //**after we change the production,we put the machine stations that change production into the
    //list,when we receive change production completely msg from tcon,we remove the according
    //station name from the list

    //after we change the production,we send the change info(11111222222,the first 5 bits is board-style
    //the last 5 bits is half-material-number)to PLC or AOI
    void add_change_production_info_for_PLC_or_AOI(QString machinename,QString boardtype,QString halfmaterialnumber);
    QMap<QString,QString> get_PLC_or_AOI_change_info(QString keyvalue);
    void clear_change_production_info_for_PLC_or_AOI()
    {
        mapOneButtonToVisionOrAOI.clear();
    }
    //after we change the production,we send the change info(11111222222,the first 5 bits is board-style
    //the last 5 bits is half-material-number)to PLC or AOI

    QString bytes_to_str(QByteArray data);
    quint8 get_last_ip(const int id);
    void init_http_service();
    void clear_alter_production();
    QList<QList<QString>> get_all_machine_change_info();
    QSqlQuery get_query_result(QString strSql);
    void save_AB_change_product_info(QList<QString> changeinfo);
    void set_AB_change_product_info(QList<QString> boardinfo);
    QList<QString> get_A_change_product_info();
    QList<QString> get_B_change_product_info();

    int get_biggest_id(const QString& tablename);
    bool check_mac_exist(const QString& macaddr,QString* machinename);
    QString register_new_mac_machine(QList<QString> lstpara);
    QMap<QString,QList<QString>> get_all_machine_name_from_db();
    QString get_line_name_by_machine_name(QString machinename);

    QMutex m_mutex;
    QMutex m_mutex_PLC_AOI;
    bool truncate_change_info_table();
    bool truncate_change_info_table_pure();

    //login for process password
    QString EncodeStr(QString password);
    QString HandleStr(int i);
    QString DecodeStr(QString s);

    void set_current_line_info(LineInfoValue info)
    {
        m_pCurrentLineInfoValue = info;
    }

    LineInfoValue get_current_line_info()
    {
        return m_pCurrentLineInfoValue;
    }

    QString get_current_usr_name()
    {
        return m_pCurrentUsrName;
    }

    void set_current_usr_name(QString usrName)
    {
        m_pCurrentUsrName = usrName;
    }

    //light bar
    QString get_first_sn_light_bar()
    {
        return m_pFirstTimeSN;
    }

    void set_second_sn_light_bar(QString str)
    {
        m_pSecondTimeSN = str;
    }
    QString get_second_sn_light_bar()
    {
        return m_pSecondTimeSN;
    }

    void clear_1_2_light_bar_sn()
    {
        m_pFirstTimeSN = "";
        m_pSecondTimeSN = "";
    }
    //light bar
    //Driver not loaded
    void start_query_timer();
    QList<QList<QString>> get_all_info(){return changInfoEx;}
    //Driver not loaded
private:
    //Driver not loaded
    void timerEvent(QTimerEvent* event);
    int m_pTimerID;
    QList<QList<QString>> changInfoEx;

    //Driver not loaded
    LineInfoValue m_pCurrentLineInfoValue;
    QMap<QString,QString> ConfigInfo;
    quint8 process_ip_string(const QString ipstr);
    QString search_http_config_file();
    QList<QString> ListBoardInfoA;
    QList<QString> ListBoardInfoB;

    QList<QString> m_pMachineNamesOfChangeProduction;

    QFile m_pConfigFile;
    QJsonDocument m_pJsonDocument;
    QJsonObject m_pRootObj;
    QString m_pConfigFileName;
    QString board_A_serial_number;
    QString board_B_serial_number;
    int boardSign;
    DataCommSerial* pCommLightBar;//this obj is for light-bar cleaning
    DataCommSerial* pComm;
    DataCommSerial* pCommTBox;//this obj is for TBOX auto-test
    //notice PLC or AOI the current change production info
    QMap<QString,QMap<QString,QString>> mapOneButtonToVisionOrAOI;//QString:machineStationName QString:boardStyle QString:halfMaterialNum
    //notice PLC or AOI the current change production info

    //light bar
    QString m_pFirstTimeSN;
    QString m_pSecondTimeSN;
    //light bar

    QString m_pCurrentUsrName;
public:
    static LightBarCleanWgt* get_light_bar_wgt()
    {
        if(m_pLightBarCleanWgt == nullptr)
            m_pLightBarCleanWgt = new LightBarCleanWgt();
        return m_pLightBarCleanWgt;
    }

    static GDataFactory* get_factory()
    {
        if(m_pGDataFactory == nullptr)
            m_pGDataFactory = new GDataFactory();
        return m_pGDataFactory;
    }

    static MainWindow* get_main_window()
    {
        if(m_pMainWindow == nullptr)
            m_pMainWindow = new MainWindow();
        return m_pMainWindow;
    }

    static WelcomeWidget* get_welcome_wgt()
    {
        if(m_pWelcomeWgt == nullptr)
            m_pWelcomeWgt = new WelcomeWidget();
        return m_pWelcomeWgt;
    }

    static DMDBAccessInterface* get_pgsql()
    {
        if(m_pDMDBAccess == nullptr)
            m_pDMDBAccess = DMDBAccessInterface::GetInterface() ;
        return m_pDMDBAccess;
    }

    static DMDBAccessInterface* get_pgsql_tcon_data_acquisition()
    {
        if(m_pDMDBAccessTcon == nullptr)
            m_pDMDBAccessTcon = DMDBAccessInterface::GetInterface() ;
        return m_pDMDBAccessTcon;
    }

    static CentralWidget* get_central_wgt()
    {
        if(m_pCentralWgt == nullptr)
            m_pCentralWgt = new CentralWidget();
        return m_pCentralWgt;
    }

    static LeftWidget* get_left_wgt()
    {
        if(m_pLeftWgt == nullptr)
            m_pLeftWgt = new LeftWidget();
        return m_pLeftWgt;
    }

    static RightHeadWidget* get_right_head_wgt()
    {
        if(m_pRightHeadWgt == nullptr)
            m_pRightHeadWgt = new RightHeadWidget();
        return m_pRightHeadWgt;
    }

    static RightMainWindow* get_right_main_window()
    {
        if(m_pRightMainWindow == nullptr)
            m_pRightMainWindow = new RightMainWindow();
        return m_pRightMainWindow;
    }

    static RightMainWidget* get_right_main_wgt()
    {
        if(m_pRightMainWgt == nullptr)
            m_pRightMainWgt = new RightMainWidget();
        return m_pRightMainWgt;
    }

    static RightQueryWidget* get_right_query_wgt()
    {
        if(m_pRightQueryWgt == nullptr)
            m_pRightQueryWgt = new RightQueryWidget();
        return m_pRightQueryWgt;
    }

    static TestWidget* get_test_wgt()
    {
        if(m_pTestWgt == nullptr)
            m_pTestWgt = new TestWidget();
        return m_pTestWgt;
    }

    static MesProcess* get_mes_process()
    {
        if(m_pMesProcess == nullptr)
            m_pMesProcess = new MesProcess();
        return m_pMesProcess;
    }

    static TestPlanWgt* get_test_plan_wgt()
    {
        if(m_pTestPlanWgt == nullptr)
            m_pTestPlanWgt = new TestPlanWgt();
        return m_pTestPlanWgt;
    }

    static BoardStyleWidget* get_board_style_wgtA()
    {
        if(m_pBoardStyleWgtA == nullptr)
            m_pBoardStyleWgtA = new BoardStyleWidget(nullptr,u8"板A");
        return m_pBoardStyleWgtA;
    }

    static BoardStyleWidget* get_board_style_wgtB()
    {
        if(m_pBoardStyleWgtB == nullptr)
            m_pBoardStyleWgtB = new BoardStyleWidget(nullptr,u8"板B");
        return m_pBoardStyleWgtB;
    }

    HttpMsgProcess* get_http_process_obj(QString msgType)
    {
        HttpMsgProcess* httpMsgProcess = HttpMsgProcess::get_interface_obj(msgType);
        return httpMsgProcess;
    }

    static QsLogWidget* get_logs_widget(){
        if(m_pQsLogWgt == nullptr)
            m_pQsLogWgt = new QsLogWidget();
        return m_pQsLogWgt;
    }

//    static TestPlanThread* get_test_plan_thread()
//    {
//        if(m_pTestPlanThread == nullptr)
//            m_pTestPlanThread = new TestPlanThread();
//        return m_pTestPlanThread;
//    }

//    static TestDBThread* get_test_db_thread()
//    {
//        if(m_pTestDBThread == nullptr)
//            m_pTestDBThread = new TestDBThread();
//        return m_pTestDBThread;
//    }

    static HistoryDisplayWgt* get_history_display_wgt()
    {
        if(m_pHistoryDisplayWgt == nullptr)
            m_pHistoryDisplayWgt = new HistoryDisplayWgt();
        return m_pHistoryDisplayWgt;
    }

    static ElecEquipTestPlanWgt* get_elec_equip_test_plan_wgt()
    {
        if(m_pElecEquipWgt == nullptr)
            m_pElecEquipWgt = new ElecEquipTestPlanWgt();
        return m_pElecEquipWgt;
    }

    static StationTabWgt* get_station_tab_wgt()
    {
        if(m_pStationTabWgt == nullptr)
            m_pStationTabWgt = new StationTabWgt();
        return m_pStationTabWgt;
    }

    static UserTabWgt* get_user_tab_wgt()
    {
        if(m_pUserTabWgt == nullptr)
            m_pUserTabWgt = new UserTabWgt();
        return m_pUserTabWgt;
    }

    static StationMonitorWgt* get_station_monitor_wgt()
    {
        if(m_pStationMonitorWgt == nullptr)
            m_pStationMonitorWgt = new StationMonitorWgt();
        return m_pStationMonitorWgt;
    }

    static TvPlanWgt* get_tv_plan_wgt()
    {
        if(m_pTvPlanWgt == nullptr)
            m_pTvPlanWgt = new TvPlanWgt();
        return m_pTvPlanWgt;
    }

    static PlcWakeUpDlg* get_plc_wake_up_dlg()
    {
        if(m_pPlcWakeUpDlg == nullptr)
            m_pPlcWakeUpDlg = new PlcWakeUpDlg();
        return m_pPlcWakeUpDlg;
    }

    static QNetworkAccessManager* get_access_manager_vision()
    {
        if(m_pAccessManagerVision == nullptr)
            m_pAccessManagerVision = new QNetworkAccessManager();
        return m_pAccessManagerVision;
    }

    static QNetworkAccessManager* get_access_manager_package()
    {
        if(m_pAccessManagerPackage == nullptr)
            m_pAccessManagerPackage = new QNetworkAccessManager();
        return m_pAccessManagerPackage;
    }

    static QNetworkAccessManager* get_access_manager_aoi()
    {
        if(m_pAccessManagerAOI == nullptr)
            m_pAccessManagerAOI = new QNetworkAccessManager();
        return m_pAccessManagerAOI;
    }

    static QNetworkAccessManager* get_access_manager_plc()
    {
        if(m_pAccessManagerPLC == nullptr)
            m_pAccessManagerPLC = new QNetworkAccessManager();
        return m_pAccessManagerPLC;
    }

    static LightBarWashingResultWgt* get_light_bar_washing_result_wgt()
    {
        if(m_pLightBarWashingResultWgt == nullptr)
            m_pLightBarWashingResultWgt = new LightBarWashingResultWgt();
        return m_pLightBarWashingResultWgt;
    }

    static LightBarNewProductWgt* get_light_bar_new_product_wgt()
    {
        if(m_pLightBarNewProductWgt == nullptr)
            m_pLightBarNewProductWgt = new LightBarNewProductWgt();
        return m_pLightBarNewProductWgt;
    }

    static PlateSplitter* get_plate_splitter()
    {
        if(m_pPlateSplitter == nullptr)
            m_pPlateSplitter = new PlateSplitter();
        return m_pPlateSplitter;
    }

    static TBoxWidget* get_tbox_wgt()
    {
        if(m_pTBoxWgt == nullptr)
            m_pTBoxWgt = new TBoxWidget();
        return m_pTBoxWgt;
    }

    static CommonWaitingWgt* get_common_waiting_wgt()
    {
        if(m_pCommonWaitingWgt == nullptr)
            m_pCommonWaitingWgt = new CommonWaitingWgt();
        return m_pCommonWaitingWgt;
    }

    static FinsUDPService* get_fins_udp_service()
    {
        if(m_pFinsUDPService == nullptr)
            m_pFinsUDPService = new FinsUDPService();
        return m_pFinsUDPService;
    }

    static FinsUDPService* get_fins_udp_service_tcon_test_station()
    {
        if(m_pFinsUDPServiceTestStation == nullptr)
            m_pFinsUDPServiceTestStation = new FinsUDPService();
        return m_pFinsUDPServiceTestStation;
    }

    static FinsUDPService* get_fins_udp_service_tcon_AOI_station()
    {
        if(m_pFinsUDPServiceAOIStation == nullptr)
            m_pFinsUDPServiceAOIStation = new FinsUDPService();
        return m_pFinsUDPServiceAOIStation;
    }

    static FinsUDPService* get_fins_udp_service_tcon_package_station()
    {
        if(m_pFinsUDPServicePackageStation == nullptr)
            m_pFinsUDPServicePackageStation = new FinsUDPService();
        return m_pFinsUDPServicePackageStation;
    }

    static tconMelsecUpperBoard* get_melsec_tcp_tcon_upper_board_stationA()
    {
        if(m_pMelsecTCPClientUpperBoardStationA == nullptr)
            m_pMelsecTCPClientUpperBoardStationA = new tconMelsecUpperBoard(TCON_UP_DEV_NO_A);
        return m_pMelsecTCPClientUpperBoardStationA;
    }

    static tconMelsecUpperBoard* get_melsec_tcp_tcon_upper_board_stationB()
    {
        if(m_pMelsecTCPClientUpperBoardStationB == nullptr)
            m_pMelsecTCPClientUpperBoardStationB = new tconMelsecUpperBoard(TCON_UP_DEV_NO_B);
        return m_pMelsecTCPClientUpperBoardStationB;
    }


    static tconMelsecSubBoard* get_melsec_tcp_tcon_subboard_stationA(){
        if(m_pMelsecTCPClientSubBoardStationA == nullptr){
            m_pMelsecTCPClientSubBoardStationA = new tconMelsecSubBoard(TCON_SUB_DEV_NO_A);
        }
        return m_pMelsecTCPClientSubBoardStationA;
    }

    static tconMelsecSubBoard* get_melsec_tcp_tcon_subboard_stationB(){
        if(m_pMelsecTCPClientSubBoardStationB == nullptr){
            m_pMelsecTCPClientSubBoardStationB = new tconMelsecSubBoard(TCON_SUB_DEV_NO_B);
        }
        return m_pMelsecTCPClientSubBoardStationB;
    }

    static GlueDispenseWgt* get_glue_dispense_wgt()
    {
        return GlueDispenseWgt::get_instance();
    }

    static LineOverViewWgt* get_line_overview_wgt()
    {
        return LineOverViewWgt::get_instance();
    }

    static UpBoardWgt* get_up_board_wgt()
    {
        return UpBoardWgt::get_instance();
    }

    static DivideBoardWgt* get_divide_board_wgt()
    {
        return DivideBoardWgt::get_instance();
    }

    static LeftUpBoardWgt* get_left_up_board_wgt()
    {
        return LeftUpBoardWgt::get_instance();
    }

    static RightUpBoardWgt* get_right_up_board_wgt()
    {
        return RightUpBoardWgt::get_instance();
    }

    static LeftDivideBoardWgt* get_left_divide_board_wgt()
    {
        return LeftDivideBoardWgt::get_instance();
    }

    static RightDivideBoardWgt* get_right_divide_board_wgt()
    {
        return RightDivideBoardWgt::get_instance();
    }

    static TestStationWgt* get_test_station_wgt()
    {
        return TestStationWgt::get_instance();
    }

    static AOIStationWgt* get_aoi_station_wgt_tcon()
    {
        return AOIStationWgt::get_instance();
    }

    static PackageStationWgt* get_package_station_wgt()
    {
        return PackageStationWgt::get_instance();
    }

    static StationConfigWgt* get_station_config_wgt()
    {
        return StationConfigWgt::get_instance();
    }

    static MainBoardBindWgt* get_main_board_bind_wgt()
    {
        return MainBoardBindWgt::get_instance();
    }

    static FPCPressFitWgt* get_fpc_press_fit_wgt()
    {
        return FPCPressFitWgt::get_instance();
    }

    static TestLineGlueDispenseWgt* get_test_glue_dispense_wgt()
    {
        return TestLineGlueDispenseWgt::get_instance();
    }

    static glueWeighCommService* get_glue_weight_comm_service()
    {
        if(m_pGlueWeightCommService == nullptr){
            m_pGlueWeightCommService = new  glueWeighCommService();
        }
        return m_pGlueWeightCommService;
    }


    static PressStationSerialCom* get_glue_press_comm_service()
    {
        if(m_pPressCommServices == nullptr){
            m_pPressCommServices = new  PressStationSerialCom();
        }
        return m_pPressCommServices;
    }

    static RepairStationWgt* get_repair_station_wgt()
    {
        return  RepairStationWgt::get_instance();
    }

    static DataAcquisitonErrorConfigWgt* get_data_acq_error_wgt()
    {
        return DataAcquisitonErrorConfigWgt::get_instance();
    }

    static ScrewMachineWgt* get_screw_machine_wgt()
    {
        return ScrewMachineWgt::get_instance();
    }

    static TestDivideBoardWgt* get_test_divide_board_wgt()
    {
        return TestDivideBoardWgt::get_instance();
    }

    static FinsUDPService* get_fins_udp_service_tbox_screw_machine()
    {
        if(m_pFinsUDPServiceTBoxScrewMachine == nullptr)
            m_pFinsUDPServiceTBoxScrewMachine = new FinsUDPService();
        return m_pFinsUDPServiceTBoxScrewMachine;
    }

    static ThreeInOneGlueWgt* get_three_in_one_glue_wgt()
    {
        return ThreeInOneGlueWgt::get_instance();
    }

    static WaveSolderingWgt* get_wave_soldering_wgt_M201()
    {
        return WaveSolderingM201Wgt::get_instance();
    }

    static WaveSolderingWgt* get_wave_soldering_wgt_M202()
    {
        return WaveSolderingM202Wgt::get_instance();
    }

    static WaveSolderingWgt* get_wave_soldering_wgt_M203()
    {
        return WaveSolderingM203Wgt::get_instance();
    }

    static CCDKeyPartWidget* get_ccd_key_part_wgt()
    {
        return CCDKeyPartWidget::get_instance();
    }

    static OneButtonChangeM201Wgt* get_one_button_change_m201_wgt()
    {
        return OneButtonChangeM201Wgt::get_instance();
    }

    static RedFlagTestCommandWgt* get_red_flag_test_command_wgt()
    {
        return RedFlagTestCommandWgt::get_instance();
    }

private:
    static GDataFactory* m_pGDataFactory;
    static MainWindow* m_pMainWindow;
    static WelcomeWidget* m_pWelcomeWgt;
    static DMDBAccessInterface* m_pDMDBAccess;
    static CentralWidget* m_pCentralWgt;
    static LeftWidget* m_pLeftWgt;
    static RightHeadWidget* m_pRightHeadWgt;
    static RightMainWindow* m_pRightMainWindow;
    static RightMainWidget* m_pRightMainWgt;
    static RightQueryWidget* m_pRightQueryWgt;
    static TestWidget* m_pTestWgt;
    static MesProcess* m_pMesProcess;
    static TestPlanWgt* m_pTestPlanWgt;
    static BoardStyleWidget* m_pBoardStyleWgtA;
    static BoardStyleWidget* m_pBoardStyleWgtB;
    static QsLogWidget* m_pQsLogWgt;
    static HistoryDisplayWgt* m_pHistoryDisplayWgt;
    static ElecEquipTestPlanWgt* m_pElecEquipWgt;
    static StationTabWgt* m_pStationTabWgt;
    static UserTabWgt* m_pUserTabWgt;
    static StationMonitorWgt* m_pStationMonitorWgt;
    static TvPlanWgt* m_pTvPlanWgt;
    static PlcWakeUpDlg* m_pPlcWakeUpDlg;
    static QNetworkAccessManager* m_pAccessManagerVision;
    static QNetworkAccessManager* m_pAccessManagerAOI;
    static QNetworkAccessManager* m_pAccessManagerPackage;
    static QNetworkAccessManager* m_pAccessManagerPLC;
    static LightBarCleanWgt* m_pLightBarCleanWgt;
    static LightBarWashingResultWgt* m_pLightBarWashingResultWgt;
    static LightBarNewProductWgt* m_pLightBarNewProductWgt;
    static PlateSplitter* m_pPlateSplitter;
    static TBoxWidget* m_pTBoxWgt;
    static CommonWaitingWgt* m_pCommonWaitingWgt;

    //common use finsUDP object
    static FinsUDPService* m_pFinsUDPService;
    //common use finsUDP object

    //TCON data acquisite using
    static FinsUDPService* m_pFinsUDPServiceTestStation;    //Test Station
    static FinsUDPService* m_pFinsUDPServiceAOIStation;     //AOI Station
    static FinsUDPService* m_pFinsUDPServicePackageStation; //Package Station
    static tconMelsecUpperBoard* m_pMelsecTCPClientUpperBoardStationA;    //upperBoard station
    static tconMelsecUpperBoard* m_pMelsecTCPClientUpperBoardStationB;    //upperBoard station
    static tconMelsecSubBoard* m_pMelsecTCPClientSubBoardStationA;      //SubBoard station
    static tconMelsecSubBoard* m_pMelsecTCPClientSubBoardStationB;      //SubBoard station

    static DMDBAccessInterface* m_pDMDBAccessTcon;
    //TCON data acquisite using

    static glueWeighCommService* m_pGlueWeightCommService;
    static PressStationSerialCom *m_pPressCommServices;

    //tbox-screw-machine
    static FinsUDPService* m_pFinsUDPServiceTBoxScrewMachine;
    //tbox-screw-machine

public:
signals:
    void signal_change_production_already_complete();
    void signal_register_new_station_success();
    //tbox sn->UI
    void signal_product_sn_to_ui(const QString sn);
    //tbox

    //light-bar sn->UI
    void signal_finish_washing(const int times);
    void signal_product_sn_to_light_bar(int codeSign,const QByteArray&);//codeSign=1 for the first time code scanning;codeSign=2 for the second time code scanning
    //light-bar sn->UI

    void signal_del_one_change_product_info(QString name);

public slots:
    void slot_rev_serial_number(const QByteArray&);
    void slot_rev_serial_number_light_bar(const QByteArray&);
    void slot_rev_serial_number_tbox(const QByteArray&);
    void slot_change_production_failed(QString machinename,QString linename);
    void slot_proc_fins_udp_data(QByteArray data);
    void slot_del_one_change_product_info(QString machinename);
};

#endif // GDATAFACTORY_H
