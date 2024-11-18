#include "iniservice.h"

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QHostInfo>
#include <QHostAddress>
#include <QStandardPaths>
#include "QsLog.h"
QString IniService::INI_FILE_PATH="";
/**
* 初始化：系统配置
*/
bool IniService::App_EnableWindowsAutoStart=true;
bool IniService::App_EnableAutoStartService=true;
bool IniService::App_EnableTraceLogToFile=true;
bool IniService::App_EnableAutoHide=true;
QString IniService::App_WindowsAutoStartShortcut="DAC_AOI_Delv_TR7501";
//QString IniService::App_Code="L205_AOI";
//QString IniService::App_Name=u8"德律AOI数据采集";
QString IniService::App_LineCode="L205";
QString IniService::App_LineName="L205线";
//QString IniService::App_MachineCode="1";
//QString IniService::App_MachineName="1号机台";
//QString IniService::App_StationCode="1";
//QString IniService::App_StationName="1号工位";

/**
* 上板机
*/
QString IniService::UpperLoginUserName = "";
QString IniService::UpperLoginUserCode = "";
QString IniService::Upper_DeviceType = u8"上板机";
QString IniService::Upper_DeviceCodeA="";
QString IniService::Upper_DeviceNameA="";
QString IniService::Upper_DeviceCodeB="";
QString IniService::Upper_DeviceNameB="";
QString IniService::UpperBoardCodeProfixDataLA = "T110Z_TCON_UpperBoard";
QString IniService::UpperBoardAlarmProfixDataLA = "Alarm";
QString IniService::UpperBoardCodeProfixDataLB = "T110Z_TCON_UpperBoard";
QString IniService::UpperBoardAlarmProfixDataLB = "Alarm";
/**
* 分板机
*/
QString IniService::SubLoginUserName = "";
QString IniService::SubLoginUserCode = "";
QString IniService::Sub_DeviceType = u8"分板机";
QString IniService::Sub_DeviceCodeA="";
QString IniService::Sub_DeviceNameA="";
QString IniService::Sub_DeviceCodeB="";
QString IniService::Sub_DeviceNameB="";
QString IniService::SubBoardCodeProfixDataLA = "T110Z_TCON_SubBoard";
QString IniService::SubBoardAlarmProfixDataLA = "Alarm";
QString IniService::SubBoardCodeProfixDataLB = "T110Z_TCON_SubBoard";
QString IniService::SubBoardAlarmProfixDataLB = "Alarm";
/**
* 测试
*/
QString IniService::TestLoginUserName = "";
QString IniService::TestLoginUserCode = "";
QString IniService::Test_DeviceType = u8"测试";
QString IniService::Test_DeviceCodeA="";
QString IniService::Test_DeviceNameA="";
QString IniService::Test_DeviceCodeB="";
QString IniService::Test_DeviceNameB="";
QString IniService::Test_MachineCodeA = "";
QString IniService::Test_MachineNameA = "";
QString IniService::Test_MachineCodeB = "";
QString IniService::Test_MachineNameB = "";
QString IniService::TestBoardCodeProfixDataLA = "T110Z_TCON_TestBoard";
QString IniService::TestBoardAlarmProfixDataLA = "Alarm";
QString IniService::TestBoardCodeProfixDataLB = "T110Z_TCON_TestBoard";
QString IniService::TestBoardAlarmProfixDataLB = "Alarm";
/**
* AOI
*/
QString IniService::AOILoginUserName = "";
QString IniService::AOILoginUserCode = "";
QString IniService::AOI_DeviceType = u8"AOI";
QString IniService::AOI_DeviceCode="";
QString IniService::AOI_DeviceName="";
QString IniService::AOIBoardCodeProfixData = "T110Z_TCON_AOIBoard";
QString IniService::AOIBoardAlarmProfixData = "Alarm";
/**
* 摆盘机
*/
QString IniService::PacketLoginUserName = "";
QString IniService::PacketLoginUserCode = "";
QString IniService::Packet_DeviceType = u8"摆盘机";
QString IniService::Packet_DeviceCode="";
QString IniService::Packet_DeviceName="";
QString IniService::PacketBoardCodeProfixData = "T110Z_TCON_PacketBoard";
QString IniService::PacketBoardAlarmProfixData = "Alarm";
/**
* 初始化：用户登录
*/
QString IniService::LoginUserName="";
QString IniService::LoginUserCode="";

/**
* 初始化：霍尼RabbitMQ
*/
bool IniService::Push_HNMQ_EnableTraceLog=false;
QString IniService::Push_HNMQ_Host="10.50.2.43";
int IniService::Push_HNMQ_Port=30802;
QString IniService::Push_HNMQ_UName="admin";
QString IniService::Push_HNMQ_UPwd="admin";
QString IniService::Push_HNMQ_ExchangeName="exchange.jg";
QString IniService::Push_HNMQ_ExchangeType="direct";
QString IniService::Push_HNMQ_VirtualHost="/";
bool IniService::Push_HNMQ_EnableAutoDelete=false;
bool IniService::Push_HNMQ_EnablePersistance=true;
QString IniService::Push_HNMQ_RouteKey="niagara";
QString IniService::Push_HNMQ_LA_DeviceName="L205_AOI_A";
QString IniService::Push_HNMQ_LA_SystemNo="JG20521";
QString IniService::Push_HNMQ_LB_DeviceName="L205_AOI_B";
QString IniService::Push_HNMQ_LB_SystemNo="JG20522";
QString IniService::Push_HNMQ_LA_WarnCodeProfix="AlarmJG20521a";
QString IniService::Push_HNMQ_LB_WarnCodeProfix="AlarmJG20522a";
QString IniService::Push_HNMQ_LA_DataItemProfix="L205_AOI_A_";
QString IniService::Push_HNMQ_LB_DataItemProfix="L205_AOI_B_";
bool IniService::Push_SettingUpdated=false;

IniService::IniService()
{

}

void IniService::loadFromFile()
{
    if(INI_FILE_PATH.isEmpty())
    {
        INI_FILE_PATH.append(QCoreApplication::applicationDirPath());
        INI_FILE_PATH.append(QDir::separator());
        INI_FILE_PATH.append("System.ini");
    }
    /**
    * 读取配置文件
    */
    QFileInfo file(INI_FILE_PATH);
    if(file.exists()==false){
        saveToFile();
        return;
    }
    QSettings * qSettings = new QSettings(INI_FILE_PATH, QSettings::IniFormat);
    qSettings->setIniCodec("UTF8");
    QString groupName="System/";
    /**
    * 系统配置
    */
    groupName="System/";
    App_EnableAutoStartService = qSettings->value(groupName+"App_EnableAutoStartService",App_EnableAutoStartService).toBool();
    App_EnableTraceLogToFile = qSettings->value(groupName+"App_EnableTraceLogToFile",App_EnableTraceLogToFile).toBool();
    App_EnableWindowsAutoStart = qSettings->value(groupName+"App_EnableWindowsAutoStart",App_EnableWindowsAutoStart).toBool();
    App_EnableAutoHide  = qSettings->value(groupName+"App_EnableAutoHide",App_EnableAutoHide).toBool();
    App_WindowsAutoStartShortcut = qSettings->value(groupName+"App_WindowsAutoStartShortcut",App_WindowsAutoStartShortcut).toString();
//    App_Code = qSettings->value(groupName+"App_Code",App_Code).toString();
//    App_Name = qSettings->value(groupName+"App_Name",App_Name).toString();
    App_LineCode = qSettings->value(groupName+"App_LineCode",App_LineCode).toString();
    App_LineName = qSettings->value(groupName+"App_LineName",App_LineName).toString();
//    App_StationCode = qSettings->value(groupName+"App_StationCode",App_StationCode).toString();
//    App_StationName = qSettings->value(groupName+"App_StationName",App_StationName).toString();
//    App_SubLineCode_A = qSettings->value(groupName+"App_SubLineCode_A",App_SubLineCode_A).toString();
//    App_SubLineName_A = qSettings->value(groupName+"App_SubLineName_A",App_SubLineName_A).toString();
//    App_SubLineCode_B = qSettings->value(groupName+"App_SubLineCode_B",App_SubLineCode_B).toString();
//    App_SubLineName_B = qSettings->value(groupName+"App_SubLineName_B",App_SubLineName_B).toString();

    groupName="UpperBoard/";
    Upper_DeviceType = qSettings->value(groupName+"Upper_DeviceType",Upper_DeviceType).toString();
    Upper_DeviceCodeA = qSettings->value(groupName+"Upper_DeviceCodeA",Upper_DeviceCodeA).toString();
    Upper_DeviceNameA = qSettings->value(groupName+"Upper_DeviceNameA",Upper_DeviceNameA).toString();
    Upper_DeviceCodeB = qSettings->value(groupName+"Upper_DeviceCodeB",Upper_DeviceCodeB).toString();
    Upper_DeviceNameB = qSettings->value(groupName+"Upper_DeviceNameB",Upper_DeviceNameB).toString();
    UpperBoardCodeProfixDataLA = qSettings->value(groupName+"UpperBoardCodeProfixDataLA",UpperBoardCodeProfixDataLA).toString();
    UpperBoardAlarmProfixDataLA = qSettings->value(groupName+"UpperBoardAlarmProfixDataLA",UpperBoardAlarmProfixDataLA).toString();
    UpperBoardCodeProfixDataLB = qSettings->value(groupName+"UpperBoardCodeProfixDataLB",UpperBoardCodeProfixDataLB).toString();
    UpperBoardAlarmProfixDataLB = qSettings->value(groupName+"UpperBoardAlarmProfixDataLB",UpperBoardAlarmProfixDataLB).toString();

    groupName="SubBoard/";
    Sub_DeviceType = qSettings->value(groupName+"Sub_DeviceType",Sub_DeviceType).toString();
    Sub_DeviceCodeA = qSettings->value(groupName+"Sub_DeviceCodeA",Sub_DeviceCodeA).toString();
    Sub_DeviceNameA = qSettings->value(groupName+"Sub_DeviceNameA",Sub_DeviceNameA).toString();
    Sub_DeviceCodeB = qSettings->value(groupName+"Sub_DeviceCodeB",Sub_DeviceCodeB).toString();
    Sub_DeviceNameB = qSettings->value(groupName+"Sub_DeviceNameB",Sub_DeviceNameB).toString();
    SubBoardCodeProfixDataLA = qSettings->value(groupName+"SubBoardCodeProfixDataLA",SubBoardCodeProfixDataLA).toString();
    SubBoardAlarmProfixDataLA = qSettings->value(groupName+"SubBoardAlarmProfixDataLA",SubBoardAlarmProfixDataLA).toString();
    SubBoardCodeProfixDataLB = qSettings->value(groupName+"SubBoardCodeProfixDataLB",SubBoardCodeProfixDataLB).toString();
    SubBoardAlarmProfixDataLB = qSettings->value(groupName+"SubBoardAlarmProfixDataLB",SubBoardAlarmProfixDataLB).toString();

    groupName="TestBoard/";
    Test_DeviceType = qSettings->value(groupName+"Test_DeviceType",Test_DeviceType).toString();
    Test_DeviceCodeA = qSettings->value(groupName+"Test_DeviceCodeA",Test_DeviceCodeA).toString();
    Test_DeviceNameA = qSettings->value(groupName+"Test_DeviceNameA",Test_DeviceNameA).toString();
    Test_DeviceCodeB = qSettings->value(groupName+"Test_DeviceCodeB",Test_DeviceCodeB).toString();
    Test_DeviceNameB = qSettings->value(groupName+"Test_DeviceNameB",Test_DeviceNameB).toString();
    Test_MachineCodeA = qSettings->value(groupName+"Test_MachineCodeA",Test_MachineCodeA).toString();
    Test_MachineNameA = qSettings->value(groupName+"Test_MachineNameA",Test_MachineNameA).toString();
    Test_MachineCodeB = qSettings->value(groupName+"Test_MachineCodeB",Test_MachineCodeB).toString();
    Test_MachineNameB = qSettings->value(groupName+"Test_MachineNameB",Test_MachineNameB).toString();
    TestBoardCodeProfixDataLA = qSettings->value(groupName+"TestBoardCodeProfixDataLA",TestBoardCodeProfixDataLA).toString();
    TestBoardAlarmProfixDataLA = qSettings->value(groupName+"TestBoardAlarmProfixDataLA",TestBoardAlarmProfixDataLA).toString();
    TestBoardCodeProfixDataLB = qSettings->value(groupName+"TestBoardCodeProfixDataLB",TestBoardCodeProfixDataLB).toString();
    TestBoardAlarmProfixDataLB = qSettings->value(groupName+"TestBoardAlarmProfixDataLB",TestBoardAlarmProfixDataLB).toString();

    groupName="AOIBoard/";
    AOI_DeviceType = qSettings->value(groupName+"AOI_DeviceType",AOI_DeviceType).toString();
    AOI_DeviceCode = qSettings->value(groupName+"AOI_DeviceCode",AOI_DeviceCode).toString(); 
    AOI_DeviceName = qSettings->value(groupName+"AOI_DeviceName",AOI_DeviceName).toString();
    AOIBoardCodeProfixData = qSettings->value(groupName+"AOIBoardCodeProfixData",AOIBoardCodeProfixData).toString();
    AOIBoardAlarmProfixData = qSettings->value(groupName+"AOIBoardAlarmProfixData",AOIBoardAlarmProfixData).toString();

    groupName="PacketBoard/";
    Packet_DeviceType = qSettings->value(groupName+"Packet_DeviceType",Packet_DeviceType).toString();
    Packet_DeviceCode = qSettings->value(groupName+"Packet_DeviceCode",Packet_DeviceCode).toString();
    Packet_DeviceName = qSettings->value(groupName+"Packet_DeviceName",Packet_DeviceName).toString();
    PacketBoardCodeProfixData = qSettings->value(groupName+"PacketBoardCodeProfixData",PacketBoardCodeProfixData).toString();
    PacketBoardAlarmProfixData = qSettings->value(groupName+"PacketBoardAlarmProfixData",PacketBoardAlarmProfixData).toString();

    /**
    * 用户登录
    */
    groupName="Login/";
    LoginUserName = qSettings->value(groupName+"LoginUserName",LoginUserName).toString();
    LoginUserCode = qSettings->value(groupName+"LoginUserCode",LoginUserCode).toString();
    /**
    * 霍尼RabbitMQ推送
    */
    groupName="HNRabbitMQ/";
    Push_HNMQ_EnableTraceLog = qSettings->value(groupName+"Push_HNMQ_EnableTraceLog",Push_HNMQ_EnableTraceLog).toBool();
    Push_HNMQ_Host = qSettings->value(groupName+"Push_HNMQ_Host",Push_HNMQ_Host).toString();
    Push_HNMQ_Port = qSettings->value(groupName+"Push_HNMQ_Port",Push_HNMQ_Port).toInt();
    Push_HNMQ_UName = qSettings->value(groupName+"Push_HNMQ_UName",Push_HNMQ_UName).toString();
    Push_HNMQ_UPwd = qSettings->value(groupName+"Push_HNMQ_UPwd",Push_HNMQ_UPwd).toString();
    Push_HNMQ_ExchangeName = qSettings->value(groupName+"Push_HNMQ_ExchangeName",Push_HNMQ_ExchangeName).toString();
    Push_HNMQ_ExchangeType = qSettings->value(groupName+"Push_HNMQ_ExchangeType",Push_HNMQ_ExchangeType).toString();
    Push_HNMQ_VirtualHost = qSettings->value(groupName+"Push_HNMQ_VirtualHost",Push_HNMQ_VirtualHost).toString();
    Push_HNMQ_EnableAutoDelete = qSettings->value(groupName+"Push_HNMQ_EnableAutoDelete",Push_HNMQ_EnableAutoDelete).toBool();
    Push_HNMQ_EnablePersistance = qSettings->value(groupName+"Push_HNMQ_EnablePersistance",Push_HNMQ_EnablePersistance).toBool();
    Push_HNMQ_RouteKey = qSettings->value(groupName+"Push_HNMQ_RouteKey",Push_HNMQ_RouteKey).toString();
    Push_HNMQ_LA_DeviceName = qSettings->value(groupName+"Push_HNMQ_LA_DeviceName",Push_HNMQ_LA_DeviceName).toString();
    Push_HNMQ_LA_SystemNo = qSettings->value(groupName+"Push_HNMQ_LA_SystemNo",Push_HNMQ_LA_SystemNo).toString();
    Push_HNMQ_LB_DeviceName = qSettings->value(groupName+"Push_HNMQ_LB_DeviceName",Push_HNMQ_LB_DeviceName).toString();
    Push_HNMQ_LB_SystemNo = qSettings->value(groupName+"Push_HNMQ_LB_SystemNo",Push_HNMQ_LB_SystemNo).toString();
    //
    Push_HNMQ_LA_WarnCodeProfix = qSettings->value(groupName+"Push_HNMQ_LA_WarnCodeProfix",Push_HNMQ_LA_WarnCodeProfix).toString();
    Push_HNMQ_LB_WarnCodeProfix = qSettings->value(groupName+"Push_HNMQ_LB_WarnCodeProfix",Push_HNMQ_LB_WarnCodeProfix).toString();
    //
    Push_HNMQ_LA_DataItemProfix = qSettings->value(groupName+"Push_HNMQ_LA_DataItemProfix",Push_HNMQ_LA_DataItemProfix).toString();
    Push_HNMQ_LB_DataItemProfix = qSettings->value(groupName+"Push_HNMQ_LB_DataItemProfix",Push_HNMQ_LB_DataItemProfix).toString();
    /**
    * 更新
    */
    saveToFile();
    /**
    * 回收
    */
    delete qSettings;
}

void IniService::saveToFile()
{
    QSettings* qSettings = new QSettings(INI_FILE_PATH,QSettings::IniFormat);
    qSettings->setIniCodec("UTF8");
    /**
    * 系统配置
    */
    qSettings->beginGroup("System");
    qSettings->setValue("App_EnableAutoStartService",App_EnableAutoStartService);
    qSettings->setValue("App_EnableTraceLogToFile",App_EnableTraceLogToFile);
    qSettings->setValue("App_EnableWindowsAutoStart",App_EnableWindowsAutoStart);
    qSettings->setValue("App_EnableAutoHide",App_EnableAutoHide);
    qSettings->setValue("App_WindowsAutoStartShortcut",App_WindowsAutoStartShortcut);
//    qSettings->setValue("App_Code",App_Code);
//    qSettings->setValue("App_Name",App_Name);
    qSettings->setValue("App_LineCode",App_LineCode);
    qSettings->setValue("App_LineName",App_LineName);
//    qSettings->setValue("App_StationCode",App_StationCode);
//    qSettings->setValue("App_StationName",App_StationName);
//    qSettings->setValue("App_SubLineCode_A",App_SubLineCode_A);
//    qSettings->setValue("App_SubLineName_A",App_SubLineName_A);
//    qSettings->setValue("App_SubLineCode_B",App_SubLineCode_B);
//    qSettings->setValue("App_SubLineCode_B",App_SubLineCode_B);
    qSettings->endGroup();

    /**
    * 上板机
    */
    qSettings->beginGroup("UpperBoard");
    qSettings->setValue("Upper_DeviceType",Upper_DeviceType);
    qSettings->setValue("Upper_DeviceCodeA",Upper_DeviceCodeA);
    qSettings->setValue("Upper_DeviceNameA",Upper_DeviceNameA);
    qSettings->setValue("Upper_DeviceCodeB",Upper_DeviceCodeB);
    qSettings->setValue("Upper_DeviceNameB",Upper_DeviceNameB);
    qSettings->setValue("UpperBoardCodeProfixDataLA",UpperBoardCodeProfixDataLA);
    qSettings->setValue("UpperBoardAlarmProfixDataLA",UpperBoardAlarmProfixDataLA);
    qSettings->setValue("UpperBoardCodeProfixDataLB",UpperBoardCodeProfixDataLB);
    qSettings->setValue("UpperBoardAlarmProfixDataLB",UpperBoardAlarmProfixDataLB);
    qSettings->endGroup();

    /**
    * 分板机
    */
    qSettings->beginGroup("SubBoard");
    qSettings->setValue("Sub_DeviceType",Sub_DeviceType);
    qSettings->setValue("Sub_DeviceCodeA",Sub_DeviceCodeA);
    qSettings->setValue("Sub_DeviceNameA",Sub_DeviceNameA);
    qSettings->setValue("Sub_DeviceCodeB",Sub_DeviceCodeB);
    qSettings->setValue("Sub_DeviceNameB",Sub_DeviceNameB);
    qSettings->setValue("SubBoardCodeProfixDataLA",SubBoardCodeProfixDataLA);
    qSettings->setValue("SubBoardAlarmProfixDataLA",SubBoardAlarmProfixDataLA);
    qSettings->setValue("SubBoardCodeProfixDataLB",SubBoardCodeProfixDataLB);
    qSettings->setValue("SubBoardAlarmProfixDataLB",UpperBoardAlarmProfixDataLB);
    qSettings->endGroup();

    /**
    * 测试机台
    */
    qSettings->beginGroup("TestBoard");
    qSettings->setValue("Test_DeviceType",Test_DeviceType);
    qSettings->setValue("Test_DeviceCodeA",Test_DeviceCodeA);
    qSettings->setValue("Test_DeviceNameA",Test_DeviceNameA);
    qSettings->setValue("Test_DeviceCodeB",Test_DeviceCodeB);
    qSettings->setValue("Test_DeviceNameB",Test_DeviceNameB);
    qSettings->setValue("Test_MachineCodeA",Test_MachineCodeA);
    qSettings->setValue("Test_MachineNameA",Test_MachineNameA);
    qSettings->setValue("Test_MachineCodeB",Test_MachineCodeB);
    qSettings->setValue("Test_MachineNameB",Test_MachineNameB);
    qSettings->setValue("TestBoardCodeProfixDataLA",TestBoardCodeProfixDataLA);
    qSettings->setValue("TestBoardAlarmProfixDataLA",TestBoardAlarmProfixDataLA);
    qSettings->setValue("TestBoardCodeProfixDataLB",TestBoardCodeProfixDataLB);
    qSettings->setValue("TestBoardAlarmProfixDataLB",UpperBoardAlarmProfixDataLB);
    qSettings->endGroup();
    /**
    * AOI
    */
    qSettings->beginGroup("AOIBoard");
    qSettings->setValue("AOI_DeviceType",AOI_DeviceType);
    qSettings->setValue("AOI_DeviceCode",AOI_DeviceCode);
    qSettings->setValue("AOI_DeviceName",AOI_DeviceName);
    qSettings->setValue("AOIBoardCodeProfixData",AOIBoardCodeProfixData);
    qSettings->setValue("AOIBoardAlarmProfixData",AOIBoardAlarmProfixData);
    qSettings->endGroup();
    /**
    * 摆盘机
    */
    qSettings->beginGroup("PacketBoard");
    qSettings->setValue("Packet_DeviceType",Packet_DeviceType);
    qSettings->setValue("Packet_DeviceCode",Packet_DeviceCode);
    qSettings->setValue("Packet_DeviceName",Packet_DeviceName);
    qSettings->setValue("PacketBoardCodeProfixData",PacketBoardCodeProfixData);
    qSettings->setValue("PacketBoardAlarmProfixData",PacketBoardAlarmProfixData);
    qSettings->endGroup();
    /**
    * 用户登录
    */
    qSettings->beginGroup("Login");
    qSettings->setValue("LoginUserName",LoginUserName);
    qSettings->setValue("LoginUserCode",LoginUserCode);
    qSettings->endGroup();
    /**
    * 霍尼RabbitMQ推送
    */
    qSettings->beginGroup("HNRabbitMQ");
    qSettings->setValue("Push_HNMQ_EnableTraceLog",Push_HNMQ_EnableTraceLog);
    qSettings->setValue("Push_HNMQ_Host",Push_HNMQ_Host);
    qSettings->setValue("Push_HNMQ_Port",Push_HNMQ_Port);
    qSettings->setValue("Push_HNMQ_UName",Push_HNMQ_UName);
    qSettings->setValue("Push_HNMQ_UPwd",Push_HNMQ_UPwd);
    qSettings->setValue("Push_HNMQ_ExchangeName",Push_HNMQ_ExchangeName);
    qSettings->setValue("Push_HNMQ_ExchangeType",Push_HNMQ_ExchangeType);
    qSettings->setValue("Push_HNMQ_VirtualHost",Push_HNMQ_VirtualHost);
    qSettings->setValue("Push_HNMQ_EnableAutoDelete",Push_HNMQ_EnableAutoDelete);
    qSettings->setValue("Push_HNMQ_EnablePersistance",Push_HNMQ_EnablePersistance);
    qSettings->setValue("Push_HNMQ_RouteKey",Push_HNMQ_RouteKey);
    qSettings->setValue("Push_HNMQ_LA_DeviceName",Push_HNMQ_LA_DeviceName);
    qSettings->setValue("Push_HNMQ_LA_SystemNo",Push_HNMQ_LA_SystemNo);
    qSettings->setValue("Push_HNMQ_LB_DeviceName",Push_HNMQ_LB_DeviceName);
    qSettings->setValue("Push_HNMQ_LB_SystemNo",Push_HNMQ_LB_SystemNo);
    //
    qSettings->setValue("Push_HNMQ_LA_WarnCodeProfix",Push_HNMQ_LA_WarnCodeProfix);
    qSettings->setValue("Push_HNMQ_LB_WarnCodeProfix",Push_HNMQ_LB_WarnCodeProfix);
    //
    qSettings->setValue("Push_HNMQ_LA_DataItemProfix",Push_HNMQ_LA_DataItemProfix);
    qSettings->setValue("Push_HNMQ_LB_DataItemProfix",Push_HNMQ_LB_DataItemProfix);
    qSettings->endGroup();
    //回收
    delete qSettings;
}

void IniService::setWindowAutoRun(QString shortcut,bool autoRun)
{
    //禁用注册表操作
    if(disableRegedit())
    {
        qDebug("%s",qUtf8Printable("已禁用注册表操作，尝试已快捷方式进行开机自启配置；若要修改请删除根目录下的disableRegedit.cmd文件"));
        //创建快捷方式
        QString appPath=QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        appPath=appPath.replace(QChar('/'),QChar('\\'),Qt::CaseInsensitive);
        QString appLink = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        appLink=appLink.replace(QChar('/'),QChar('\\'),Qt::CaseInsensitive);
        appLink+="\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
        appLink+=shortcut;
        appLink+=".lnk";
        //开机自启
        if(autoRun)
        {
            QFile appFile(appPath);
            if(appFile.link(appLink))
            {
                qDebug("%s",qUtf8Printable("成功添加开机启动项:"+appLink));
            }
        }
        else
        {
            QFile appFile(appPath);
            if(appFile.exists())
            {
                appFile.remove();
                qDebug("%s",qUtf8Printable("移除开机启动项:"+appLink));
            }
        }
        return;
    }
    //添加开机启动
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if (autoRun)
    {
        QString strAppPath=QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        strAppPath=strAppPath.replace(QChar('/'),QChar('\\'),Qt::CaseInsensitive);
        reg.setValue(shortcut,strAppPath);
        qDebug("%s",qUtf8Printable("添加开机启动项:"+strAppPath));
    }
    else
    {
        reg.remove(shortcut);
        qDebug("%s",qUtf8Printable("移除开机启动项"));
    }
}

bool IniService::disableRegedit()
{
    QString filePath="";
    filePath.append(QCoreApplication::applicationDirPath());
    filePath.append(QDir::separator());
    filePath.append("disableRegedit.cmd");
    QFileInfo file(filePath);
    return file.exists();
}

QString IniService::getWindowUserName()
{
    return QDir::home().dirName();
}

QList<QString> IniService::getWindowIPList()
{
    QList<QString> ipList;
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QList<QHostAddress> addresses = hostInfo.addresses();
    foreach(QHostAddress address,addresses)
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
            if(address.toString()!="127.0.0.1")
            {
                ipList.append(address.toString());
            }
        }
    }
    return ipList;
}

QString IniService::getWindowIP()
{
    QList<QString> ipList=getWindowIPList();
    if(ipList.size()>0)
    {
        return ipList[0];
    }
    return "127.0.0.1";
}
