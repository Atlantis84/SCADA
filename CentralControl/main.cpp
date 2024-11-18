#include "mainwindow.h"
#include <QApplication>
#include "gdatafactory.h"
#include <QDir>
#include "QsLog.h"
#include "QsLogDest.h"
#include <QFile>
#include <QTextCodec>
#include <zebraprinterservice.h>
#include "gdataacquisitionfactory.h"
#include "qmqservicecommonuse.h"
#include <QDateTime>
#include <QMessageBox>
#include <QSharedMemory>
using namespace QsLogging;

void init_config_file(QString fileName)
{
    QString binDir=QApplication::applicationDirPath();

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            QString tmpfileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using json config file"<<fileName;

            if(fileName == "config.json"){
                GDataFactory::get_factory()->load_json_config_1(QByteArray(tmpfileName.toLatin1()).data());
            }else if(fileName == "DataAcquistionConfig.json"){
                DataAcquisitionConfig::get_instance()->load_json_config(QByteArray(tmpfileName.toLatin1()).data());
            }
        }
        else
        {
            QLOG_WARN()<<"Json config file not exist"<<fileName;
            QApplication::exit(1);
        }
    }
}

void init_qslog()
{
    init_config_file("DataAcquistionConfig.json");
    init_config_file("config.json");
    Logger& logger = Logger::instance();

    QString ttl = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
    QString tmpStr = "\Log\\log.txt";
    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath(tmpStr));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(256*1024), MaxOldLogCount(5000)));
    logger.addDestination(fileDestination);

    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);

    DestinationPtr controlDestination(DestinationFactory::MakeFunctorDestination(GDataFactory::get_logs_widget(),SLOT(slot_rev_logs(const QString))));
    logger.addDestination(controlDestination);

//    DestinationPtr controlDestination1(DestinationFactory::MakeFunctorDestination(GDataFactory::get_tbox_wgt(),SLOT(slot_rev_logs(const QString))));
//    logger.addDestination(controlDestination1);

//    DestinationPtr controlDestination2(DestinationFactory::MakeFunctorDestination(GDataFactory::get_one_button_change_m201_wgt(),SLOT(slot_rev_logs(const QString))));
//    logger.addDestination(controlDestination2);

    QString logLevel = GDataFactory::get_factory()->get_config_para("LOG_LEVEL");
    if(logLevel == "0")
        logger.setLoggingLevel(QsLogging::TraceLevel);
    else if(logLevel == "1")
        logger.setLoggingLevel(QsLogging::DebugLevel);
    else if(logLevel == "2")
        logger.setLoggingLevel(QsLogging::InfoLevel);
    else if(logLevel == "3")
        logger.setLoggingLevel(QsLogging::WarnLevel);
    else if(logLevel == "4")
        logger.setLoggingLevel(QsLogging::ErrorLevel);
    else if(logLevel == "5")
        logger.setLoggingLevel(QsLogging::FatalLevel);
    else
        logger.setLoggingLevel(QsLogging::DebugLevel);

    QLOG_INFO()<<u8"QsLog init SUCCESS";
}

void set_system_style()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString qssFile = ":/icon/darkgray.qss";
    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

bool init_db_service()
{
    if(GDataFactory::get_pgsql()->ConnectDataBase())
    {
        QLOG_INFO()<<"connect to db success common using!";
    }
    else {
        return false;
    }
    return true;
}

void init_data_acquisition()
{
    if(GDataFactory::get_factory()->get_config_para("TCON_DATA_ACQUISITION") == "1")
    {
        if(GDataFactory::get_pgsql_tcon_data_acquisition()->ConnectDataBase(GDataFactory::get_factory()->get_config_para("TCON_DB_ADDRESS"),
                                                                            GDataFactory::get_factory()->get_config_para("TCON_DB_NAME"),
                                                                            GDataFactory::get_factory()->get_config_para("TCON_DB_USR_NAME"),
                                                                            GDataFactory::get_factory()->get_config_para("TCON_DB_PASSWORD"),
                                                                            GDataFactory::get_factory()->get_config_para("TCON_DB_PORT").toInt()))
        {
            QLOG_INFO()<<"connect to db success data acquisition!";
        }
        else
            QLOG_WARN()<<"connect to db failed data acquisition!";

        GDataFactory::get_fins_udp_service_tcon_test_station()->init_ip_port(GDataFactory::get_factory()->get_config_para("TCON_TEST_STATION_PLC_IP"),
                                                                             GDataFactory::get_factory()->get_config_para("TCON_TEST_STATION_PLC_PORT").toInt());
        GDataFactory::get_fins_udp_service_tcon_AOI_station()->init_ip_port(GDataFactory::get_factory()->get_config_para("TCON_AOI_STATION_PLC_IP"),
                                                                             GDataFactory::get_factory()->get_config_para("TCON_AOI_STATION_PLC_PORT").toInt());
        GDataFactory::get_fins_udp_service_tcon_package_station()->init_ip_port(GDataFactory::get_factory()->get_config_para("TCON_PACKAGE_STATION_PLC_IP"),
                                                                             GDataFactory::get_factory()->get_config_para("TCON_PACKAGE_STATION_PLC_PORT").toInt());

    }

    //three-in-one
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->ConnectDataBase(
                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DB_ADDRESS"),
                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DB_NAME"),
                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DB_USR_NAME"),
                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DB_PASSWORD"),
                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DB_PORT").toInt(),"three-in-one"))
    {
        QLOG_INFO()<<"connect to three in one DB success";
    }
    else
        QLOG_WARN()<<"connect to three in one DB failed";

    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        GDataAcquisitionFactory::get_instance()->start_data_acquisition_three_in_one();
    }
}

int AssumeSingleInstance(const QString &key)
{
    static QSharedMemory shm(key);
    if (shm.create(100) == false)
    {
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QString  aa = GDataFactory::get_factory()->DecodeStr("04217043430474204345");

    if (AssumeSingleInstance("CentralControl.exe") < 0)
    {
        QMessageBox::information(nullptr, u8"提示", u8"程序已运行");
        return -1;
    }

    set_system_style();
    init_qslog();

//    qint64 timestamp = QDateTime::currentDateTime().toTime_t();
//    QLOG_INFO()<<timestamp;

//    QMqServiceCommonUse::get_instance()->start_three_in_one_M201_mq_service();

//    QJsonObject data;
//    data.insert("REJECTRATE",u8"测试");

//    QJsonDocument jsonDoc;
//    jsonDoc.setObject(data);

//    QTextCodec *tc = QTextCodec::codecForName("utf-8");

//    QString str = tc->toUnicode(jsonDoc.toJson());
//    QLOG_ERROR()<<"tv-test:"<<str;

    if(init_db_service())
    {
        QLOG_INFO()<<"init db service success!";
    }
    else
    {
        QLOG_WARN()<<"init db service failed!";
        QApplication::exit(1);
    }

    init_data_acquisition();

    GDataFactory::get_factory()->connect_common_signal_slot();
    GDataFactory::get_factory()->start_query_timer();
    GDataFactory::get_fins_udp_service()->init_ip_port(GDataFactory::get_factory()->get_config_para("PLC_IP"),
                                                       GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt());

    if(GDataFactory::get_factory()->get_config_para("PLC_WAKE_UP") == "1")
    {
        if(GDataFactory::get_plc_wake_up_dlg()->exec() == QDialog::Accepted)
            ;
        else
        {
            QApplication::exit(1);
            return 0;
        }
    }

//    GDataAcquisitionFactory::get_instance()->exec_http_get_method_to_get_aoi_defect();
    //insert-machine-error
//    InsertMachineM201ErrorThread::get_instance()->start();
    //insert-machine-error

    if(GDataFactory::get_welcome_wgt()->exec() == QDialog::Accepted )
    {
        GDataFactory::get_factory()->init_http_service();
        GDataFactory::get_main_window()->show();
        GDataFactory::get_factory()->start_data_aquisiton();//start data acquisition
    }
    else
        QApplication::exit(1);
    return a.exec();
}
