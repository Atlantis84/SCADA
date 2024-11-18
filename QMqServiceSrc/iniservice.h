#ifndef INISERVICE_H
#define INISERVICE_H

#include <QString>

namespace Service {
class IniService;
}

class IniService
{
    private:
        IniService();
    public:
        static void loadFromFile();
        static void saveToFile();
        static void setWindowAutoRun(QString shortcut,bool autoRun);
        static bool disableRegedit();
        static QString getWindowUserName();
        static QList<QString> getWindowIPList();
        static QString getWindowIP();
        static QString INI_FILE_PATH;
        static QString INI_TOTAL_FILE_PATH;
    /**
    * 声明：系统配置
    */
    public:
        static bool App_EnableWindowsAutoStart;
        static bool App_EnableAutoStartService;
        static bool App_EnableTraceLogToFile;
        static bool App_EnableAutoHide;
        static QString App_WindowsAutoStartShortcut;
        static QString App_LineCode;
        static QString App_LineName;
//        static QString App_Code;
//        static QString App_Name;

//        static QString App_StationCode;
//        static QString App_StationName;
//        static QString App_SubLineCode_A;
//        static QString App_SubLineName_A;
//        static QString App_SubLineCode_B;
//        static QString App_SubLineName_B;

    /**
    * 声明：上板机
    */
    public:
        static QString UpperLoginUserName;
        static QString UpperLoginUserCode;
        static QString Upper_DeviceType;
        static QString Upper_DeviceCodeA;
        static QString Upper_DeviceNameA;
        static QString Upper_DeviceCodeB;
        static QString Upper_DeviceNameB;
        static QString UpperBoardCodeProfixDataLA;//上板机数据项代码前缀
        static QString UpperBoardAlarmProfixDataLA;//报警前缀
        static QString UpperBoardCodeProfixDataLB;//上板机数据项代码前缀
        static QString UpperBoardAlarmProfixDataLB;//报警前缀
    /**
    * 声明：分板机
    */
    public:
        static QString SubLoginUserName;
        static QString SubLoginUserCode;
        static QString Sub_DeviceType;
        static QString Sub_DeviceCodeA;
        static QString Sub_DeviceNameA;
        static QString Sub_DeviceCodeB;
        static QString Sub_DeviceNameB;
        static QString SubBoardCodeProfixDataLA;//分板机数据项代码前缀
        static QString SubBoardAlarmProfixDataLA;//报警前缀
        static QString SubBoardCodeProfixDataLB;//分板机数据项代码前缀
        static QString SubBoardAlarmProfixDataLB;//报警前缀

    /**
    * 声明：测试
    */
    public:
        static QString TestLoginUserName;
        static QString TestLoginUserCode;
        static QString Test_DeviceType;
        static QString Test_DeviceCodeA;
        static QString Test_DeviceNameA;
        static QString Test_DeviceCodeB;
        static QString Test_DeviceNameB;
        static QString Test_MachineCodeA;
        static QString Test_MachineNameA;
        static QString Test_MachineCodeB;
        static QString Test_MachineNameB;
        static QString TestBoardCodeProfixDataLA;//测试机台数据项代码前缀
        static QString TestBoardAlarmProfixDataLA;//报警前缀
        static QString TestBoardCodeProfixDataLB;//测试机台数据项代码前缀
        static QString TestBoardAlarmProfixDataLB;//报警前缀

    /**
    * 声明：AOI
    */
    public:
        static QString AOILoginUserName;
        static QString AOILoginUserCode;
        static QString AOI_DeviceType;
        static QString AOI_DeviceCode;
        static QString AOI_DeviceName;
        static QString AOIBoardCodeProfixData;//测试机台数据项代码前缀
        static QString AOIBoardAlarmProfixData;//报警前缀
    /**
    * 声明：摆盘机
    */
    public:
        static QString PacketLoginUserName;
        static QString PacketLoginUserCode;
        static QString Packet_DeviceCode;
        static QString Packet_DeviceType;
        static QString Packet_DeviceName;
        static QString PacketBoardCodeProfixData;//摆盘机数据项代码前缀
        static QString PacketBoardAlarmProfixData;//报警前缀

    /**
    * 声明：用户登录
    */
    public:
        static QString LoginUserName;
        static QString LoginUserCode;
    /**
    * 声明：霍尼RabbitMQ数据推送
    */
    public:
        static bool Push_HNMQ_EnableTraceLog;
        static QString Push_HNMQ_Host;
        static int Push_HNMQ_Port;
        static QString Push_HNMQ_UName;
        static QString Push_HNMQ_UPwd;
        static QString Push_HNMQ_ExchangeName;
        static QString Push_HNMQ_ExchangeType;
        static QString Push_HNMQ_VirtualHost;
        static bool Push_HNMQ_EnableAutoDelete;
        static bool Push_HNMQ_EnablePersistance;
        static QString Push_HNMQ_RouteKey;
        static QString Push_HNMQ_LA_DeviceName;
        static QString Push_HNMQ_LA_SystemNo;
        static QString Push_HNMQ_LB_DeviceName;
        static QString Push_HNMQ_LB_SystemNo;
        static bool Push_SettingUpdated;
        static QString Push_HNMQ_LA_WarnCodeProfix;
        static QString Push_HNMQ_LB_WarnCodeProfix;
        static QString Push_HNMQ_LA_DataItemProfix;
        static QString Push_HNMQ_LB_DataItemProfix;

};

#endif // INISERVICE_H
