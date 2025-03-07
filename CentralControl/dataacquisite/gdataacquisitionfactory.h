#ifndef GDATAACQUISITIONFACTORY_H
#define GDATAACQUISITIONFACTORY_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "dataacquisiteheaders.h"
#include "datacommserial.h"
#include <QNetworkAccessManager>
#include <QScopedPointer>
#include <QJsonObject>
#include "httpresponse.h"
#include "httptvmsg.h"
#include "collectinsertmachineerrorsthread.h"
#include "httpbasedoncurl.h"
#include "finsudpservice.h"
class GDataAcquisitionFactory : public QObject
{
    Q_OBJECT
    //test;

public:
    static GDataAcquisitionFactory* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new GDataAcquisitionFactory();
        return m_pInstance;
    }

    static DataAcquisitionConfig* get_data_acquisition_config()
    {
        return DataAcquisitionConfig::get_instance();
    }

    static DMDBAccessInterface* get_pgsql_three_in_one_data_acquisition()
    {
        if(m_pDMDBAccessThreeInOne == nullptr)
            m_pDMDBAccessThreeInOne = DMDBAccessInterface::GetInterface() ;
        return m_pDMDBAccessThreeInOne;
    }

    static M201UdpBusinessObj* get_M201_udp_business_obj()
    {
        return M201UdpBusinessObj::get_instance();
    }

    static PlcBusinessM201Obj* get_plc_business_m201_obj()
    {
        return PlcBusinessM201Obj::get_instance();
    }

    static PlcBusinessM201Thread* get_plc_business_m201_thread()
    {
        return PlcBusinessM201Thread::get_instance();
    }

    static InsertMachineM201Thread* get_insert_machine_thread_of_m201()
    {
        return InsertMachineM201Thread::get_instance();
    }

    static InsertMachineBusinessM201Obj* get_insert_machine_obj_of_m201()
    {
        return InsertMachineBusinessM201Obj::get_instance();
    }

    static AOIBusinessM201Obj* get_aoi_business_m201_obj()
    {
        return AOIBusinessM201Obj::get_instance();
    }

    static CollectInsertMachineErrorsThread* get_collect_machine_error_thread()
    {
        return CollectInsertMachineErrorsThread::get_instance();
    }

    static InsertMachineErrorsWgt* get_insert_machine_errors_wgt()
    {
        return InsertMachineErrorsWgt::get_instance();
    }

    void start_data_acquisition_three_in_one();
    void connect_common_signals_slots();
    void read_serial_number_m201();
    void stop_serial_port_m201();

    bool isGreater(double a, double b, double epsilon = 1e-5);
    bool isLess(double a, double b, double epsilon = 1e-5);
    bool isEqual(double a, double b, double epsilon = 1e-15);

    void rev_tv_test_info(QJsonObject obj);
    QNetworkAccessManager* get_access_manager_params()
    {
        return m_pAccessManagerParams;
    }

    QNetworkAccessManager* get_access_manager_defect()
    {
        return m_pAccessManagerDefect;
    }

    void exec_http_get_method_to_get_aoi_defect();
    void exec_http_post_method_to_get_head_params(const QString sn);
    void exec_http_post_method_to_get_end_params(const QString sn);

    bool isTimeBetween730And830();
    bool isTimeBetween1830And2030();
    bool isTimeBetween1130And1230();

    HttpThreeInOneMsg* get_http_obj()
    {
        HttpThreeInOneMsg* hMsg = new HttpThreeInOneMsg();
        return hMsg;
    }

    void process_http_obj_tv(QJsonObject httpJsonObj,HttpResponse& response)
    {
        QScopedPointer<HttpTVMsg> hMsg(new HttpTVMsg());
        hMsg->process_http_msg(httpJsonObj,response);
    }

    static FinsUDPService* get_fins_upd_obj_m201_test_station(const int stationid)
    {
        FinsUDPService* m_pObj = nullptr;

        switch (stationid) {
        case 1:
            if(m_pFinsUdpService01 == nullptr)
                m_pFinsUdpService01 = new FinsUDPService();
            m_pObj = m_pFinsUdpService01;
            break;
        case 2:
            if(m_pFinsUdpService02 == nullptr)
                m_pFinsUdpService02 = new FinsUDPService();
            m_pObj = m_pFinsUdpService02;
            break;
        case 3:
            if(m_pFinsUdpService03 == nullptr)
                m_pFinsUdpService03 = new FinsUDPService();
            m_pObj = m_pFinsUdpService03;
            break;
        case 4:
            if(m_pFinsUdpService04 == nullptr)
                m_pFinsUdpService04 = new FinsUDPService();
            m_pObj = m_pFinsUdpService04;
            break;
        case 5:
            if(m_pFinsUdpService05 == nullptr)
                m_pFinsUdpService05 = new FinsUDPService();
            m_pObj = m_pFinsUdpService05;
            break;
        case 6:
            if(m_pFinsUdpService06 == nullptr)
                m_pFinsUdpService06 = new FinsUDPService();
            m_pObj = m_pFinsUdpService06;
            break;
        case 7:
            if(m_pFinsUdpService07 == nullptr)
                m_pFinsUdpService07 = new FinsUDPService();
            m_pObj = m_pFinsUdpService07;
            break;
        case 8:
            if(m_pFinsUdpService08 == nullptr)
                m_pFinsUdpService08 = new FinsUDPService();
            m_pObj = m_pFinsUdpService08;
            break;
        case 9:
            if(m_pFinsUdpService09 == nullptr)
                m_pFinsUdpService09 = new FinsUDPService();
            m_pObj = m_pFinsUdpService09;
            break;
        case 10:
            if(m_pFinsUdpService10 == nullptr)
                m_pFinsUdpService10 = new FinsUDPService();
            m_pObj = m_pFinsUdpService10;
            break;
        case 11:
            if(m_pFinsUdpService11 == nullptr)
                m_pFinsUdpService11 = new FinsUDPService();
            m_pObj = m_pFinsUdpService11;
            break;
        case 12:
            if(m_pFinsUdpService12 == nullptr)
                m_pFinsUdpService12 = new FinsUDPService();
            m_pObj = m_pFinsUdpService12;
            break;
        default:
            if(m_pFinsUdpService01 == nullptr)
                m_pFinsUdpService01 = new FinsUDPService();
            m_pObj = m_pFinsUdpService01;
            break;
        }
        return m_pObj;
    }

protected:

    QNetworkAccessManager* m_pAccessManagerAOIDefect;
    QNetworkAccessManager* m_pAccessManagerHeadParams;
    QNetworkAccessManager* m_pAccessManagerEndParams;
    QNetworkAccessManager* m_pAccessManagerParams;
    QNetworkAccessManager* m_pAccessManagerDefect;

    QString m_pGetUrl;
    QString m_pPostUrlHead;
    QString m_pPostUrlEnd;

    //plc-test-station
    static FinsUDPService* m_pFinsUdpService01;
    static FinsUDPService* m_pFinsUdpService02;
    static FinsUDPService* m_pFinsUdpService03;
    static FinsUDPService* m_pFinsUdpService04;
    static FinsUDPService* m_pFinsUdpService05;
    static FinsUDPService* m_pFinsUdpService06;
    static FinsUDPService* m_pFinsUdpService07;
    static FinsUDPService* m_pFinsUdpService08;
    static FinsUDPService* m_pFinsUdpService09;
    static FinsUDPService* m_pFinsUdpService10;
    static FinsUDPService* m_pFinsUdpService11;
    static FinsUDPService* m_pFinsUdpService12;
    //plc-test-station

private:
    explicit GDataAcquisitionFactory(QObject *parent = nullptr);
    void timerEvent(QTimerEvent* event);

    static GDataAcquisitionFactory* m_pInstance;
    static DMDBAccessInterface* m_pDMDBAccessThreeInOne;

    HttpBasedOnCurl* m_pHttpBasedOnCurl;

    QTimer *m_pTimer;
    QTimer *m_pTimerM202;
    QTimer *m_pTimerAOIM201;
    int m_pTimerCollectErrors;

    QTimer *m_pTimerFirstScanner;
    QTimer *m_pTimerSecondScanner;
    TcpCommonUse* m_pTcpCommonUseOfFirstScanner;
    TcpCommonUse* m_pTcpCommonUseOfSecondScanner;

    DataCommSerial* m_pDataCommSerial;

    void process_divide_board_standard_paras(QJsonArray array);
    void process_wave_soldering_standard_paras(QJsonArray array);
    void process_insert_machine_standard_paras(QJsonArray array);
    void process_aoi_defect_standard_paras(QJsonArray array);
    void process_test_station_pass_rate_paras(QJsonArray array);

signals:
    void signal_connect_state_M201(int state);
    void signal_connect_state_M202(int state);
    void signal_connect_state_M203(int state);
    void signal_send_serial_number_to_ui(const QByteArray& data);
    void signal_send_tv_info_to_business_process(const QJsonObject obj);
public slots:
    void slot_monitor_tcp_connect_state_M201();
    void slot_monitor_tcp_connect_state_M202();
    void slot_monitor_tcp_connect_state_aoi_M201();
    void slot_monitor_tcp_connect_state_first_scanner();
    void slot_monitor_tcp_connect_state_second_scanner();
    void slot_acquisite_data_on_time();
    void slot_rev_serial_number_m201(const QByteArray& data);
    void slot_to_start_acquisition_after_confirm(const DEV_TYPE_M201 dType);

    void process_info_from_tcp_server_first_scanner(const QByteArray& data);
    void process_info_from_tcp_server_second_scanner(const QByteArray& data);

    void slot_rev_http_get_aoi_defect(QNetworkReply* reply);
    void slot_rev_http_post_head_params(QNetworkReply* reply);
    void slot_rev_http_post_end_params(QNetworkReply* reply);

    void slot_rev_http_get_aoi_defect(const std::string& data);
    void slot_rev_http_post_head_params(const std::string& data);
    void slot_rev_http_post_end_params(const std::string& data);

    void slot_insert_errors_to_db(QStringList lstInfo);

    void slot_rev_info_from_test_station_plc(QByteArray data);
};

#endif // GDATAACQUISITIONFACTORY_H
