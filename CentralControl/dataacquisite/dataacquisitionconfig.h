#ifndef DATAACQUISITIONCONFIG_H
#define DATAACQUISITIONCONFIG_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

class DataAcquisitionConfig : public QObject
{
    Q_OBJECT
public:
    static DataAcquisitionConfig* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new DataAcquisitionConfig();
        return m_pInstance;
    }

    //config file set
    void load_json_config(char* filename);
    QString get_config_para(const QString keyname);
    void set_config_para(const QString keyname, const QString keyvalue);

private:
    DataAcquisitionConfig();

    static DataAcquisitionConfig* m_pInstance;

    void save_config_file();

private:
    QFile m_pConfigFile;
    QMap<QString,QString> ConfigInfo;
    QString m_pConfigFileName;
    QJsonDocument m_pJsonDocument;
    QJsonObject m_pRootObj;


};

#endif // DATAACQUISITIONCONFIG_H
