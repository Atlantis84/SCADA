#ifndef INICONFIGABSTRACT_H
#define INICONFIGABSTRACT_H

#include <QString>
#include <QMap>
class IniConfigAbstract
{
public:
    IniConfigAbstract();
    virtual ~IniConfigAbstract();

    virtual void load_ini_config() = 0;
    QString get_ini_info(QString key)
    {
        if(m_pMapIniInfo.contains(key))
            return m_pMapIniInfo[key];
        return "";
    }

    static QString INI_FILE_PATH;
    static QString INI_TOTAL_FILE_PATH;
    static QString App_LineCode;
    static QString App_LineName;
public:
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


    QMap<QString,QString> m_pMapIniInfo;
};

#endif // INICONFIGABSTRACT_H
