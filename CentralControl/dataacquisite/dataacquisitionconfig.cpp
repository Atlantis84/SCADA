#include "dataacquisitionconfig.h"
#include "QsLog.h"
DataAcquisitionConfig* DataAcquisitionConfig::m_pInstance = nullptr;

DataAcquisitionConfig::DataAcquisitionConfig()
{

}

void DataAcquisitionConfig::load_json_config(char *filename)
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

        if(m_pRootObj.contains("THREE_IN_ONE_IP"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_IP"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("THREE_IN_ONE_DEVIDE_IP",obj.value("THREE_IN_ONE_DEVIDE_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_GLUE_IP",obj.value("THREE_IN_ONE_GLUE_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_WAVE_SOLDERING_1_IP",obj.value("THREE_IN_ONE_WAVE_SOLDERING_1_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_WAVE_SOLDERING_2_IP",obj.value("THREE_IN_ONE_WAVE_SOLDERING_2_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_WAVE_SOLDERING_3_IP",obj.value("THREE_IN_ONE_WAVE_SOLDERING_3_IP").toString());

                ConfigInfo.insert("THREE_IN_ONE_UP_BOARD_IP",obj.value("THREE_IN_ONE_UP_BOARD_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_UP_UTENSIL_IP",obj.value("THREE_IN_ONE_UP_UTENSIL_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_PICK_UPPER_COVER_IP",obj.value("THREE_IN_ONE_PICK_UPPER_COVER_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_DOWN_UTENSIL_IP",obj.value("THREE_IN_ONE_DOWN_UTENSIL_IP").toString());

                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE1_IP",obj.value("THREE_IN_ONE_INSERT_MACHINE1_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE2_IP",obj.value("THREE_IN_ONE_INSERT_MACHINE2_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE3_IP",obj.value("THREE_IN_ONE_INSERT_MACHINE3_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE4_IP",obj.value("THREE_IN_ONE_INSERT_MACHINE4_IP").toString());

                ConfigInfo.insert("THREE_IN_ONE_AOI_IP",obj.value("THREE_IN_ONE_AOI_IP").toString());

                ConfigInfo.insert("THREE_IN_ONE_MQTT_IP",obj.value("THREE_IN_ONE_MQTT_IP").toString());

                ConfigInfo.insert("THREE_IN_ONE_FIRST_SCANNER_IP",obj.value("THREE_IN_ONE_FIRST_SCANNER_IP").toString());
                ConfigInfo.insert("THREE_IN_ONE_SECOND_SCANNER_IP",obj.value("THREE_IN_ONE_SECOND_SCANNER_IP").toString());

                ConfigInfo.insert("THREE_IN_ONE_AOI_DEFECT_URL",obj.value("THREE_IN_ONE_AOI_DEFECT_URL").toString());
                ConfigInfo.insert("THREE_IN_ONE_AOI_PARAMS_URL",obj.value("THREE_IN_ONE_AOI_PARAMS_URL").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_PORT"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_PORT"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("THREE_IN_ONE_DEVIDE_PORT",obj.value("THREE_IN_ONE_DEVIDE_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_GLUE_PORT",obj.value("THREE_IN_ONE_GLUE_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_UDP_SERVER_PORT",obj.value("THREE_IN_ONE_UDP_SERVER_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_WAVE_SOLDERING_1_PORT",obj.value("THREE_IN_ONE_WAVE_SOLDERING_1_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_WAVE_SOLDERING_2_PORT",obj.value("THREE_IN_ONE_WAVE_SOLDERING_2_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_WAVE_SOLDERING_3_PORT",obj.value("THREE_IN_ONE_WAVE_SOLDERING_3_PORT").toString());

                ConfigInfo.insert("THREE_IN_ONE_UP_BOARD_PORT",obj.value("THREE_IN_ONE_UP_BOARD_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_UP_UTENSIL_PORT",obj.value("THREE_IN_ONE_UP_UTENSIL_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_PICK_UPPER_COVER_PORT",obj.value("THREE_IN_ONE_PICK_UPPER_COVER_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_DOWN_UTENSIL_PORT",obj.value("THREE_IN_ONE_DOWN_UTENSIL_PORT").toString());

                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE1_PORT",obj.value("THREE_IN_ONE_INSERT_MACHINE1_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE2_PORT",obj.value("THREE_IN_ONE_INSERT_MACHINE2_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE3_PORT",obj.value("THREE_IN_ONE_INSERT_MACHINE3_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_INSERT_MACHINE4_PORT",obj.value("THREE_IN_ONE_INSERT_MACHINE4_PORT").toString());

                ConfigInfo.insert("THREE_IN_ONE_AOI_PORT",obj.value("THREE_IN_ONE_AOI_PORT").toString());

                ConfigInfo.insert("THREE_IN_ONE_MQTT_PORT",obj.value("THREE_IN_ONE_MQTT_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_FIRST_SCANNER_PORT",obj.value("THREE_IN_ONE_FIRST_SCANNER_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_SECOND_SCANNER_PORT",obj.value("THREE_IN_ONE_SECOND_SCANNER_PORT").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_PARA"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_PARA"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("THREE_IN_ONE_DATA_ACQUISITION",obj.value("THREE_IN_ONE_DATA_ACQUISITION").toString());
                ConfigInfo.insert("THREE_IN_ONE_DB_ADDRESS",obj.value("THREE_IN_ONE_DB_ADDRESS").toString());
                ConfigInfo.insert("THREE_IN_ONE_DB_NAME",obj.value("THREE_IN_ONE_DB_NAME").toString());
                ConfigInfo.insert("THREE_IN_ONE_DB_PASSWORD",obj.value("THREE_IN_ONE_DB_PASSWORD").toString());
                ConfigInfo.insert("THREE_IN_ONE_DB_PORT",obj.value("THREE_IN_ONE_DB_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_DB_USR_NAME",obj.value("THREE_IN_ONE_DB_USR_NAME").toString());
                ConfigInfo.insert("THREE_IN_ONE_COM_PORT",obj.value("THREE_IN_ONE_COM_PORT").toString());
                ConfigInfo.insert("THREE_IN_ONE_BOARD_RATE",obj.value("THREE_IN_ONE_BOARD_RATE").toString());

                ConfigInfo.insert("THREE_IN_ONE_MQTT_USR_NAME",obj.value("THREE_IN_ONE_MQTT_USR_NAME").toString());
                ConfigInfo.insert("THREE_IN_ONE_MQTT_PASSWORD",obj.value("THREE_IN_ONE_MQTT_PASSWORD").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_DIVIDE_PARA"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_DIVIDE_PARA"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("MAIN_ROLL_SPEED_FLOOR",obj.value("MAIN_ROLL_SPEED_FLOOR").toString());
                ConfigInfo.insert("MAIN_ROLL_SPEED_UPPER",obj.value("MAIN_ROLL_SPEED_UPPER").toString());
                ConfigInfo.insert("NULL_KNIFE_SPEED_FLOOR",obj.value("NULL_KNIFE_SPEED_FLOOR").toString());
                ConfigInfo.insert("NULL_KNIFE_SPEED_UPPER",obj.value("NULL_KNIFE_SPEED_UPPER").toString());
                ConfigInfo.insert("DOWN_KNIFE_SPEED_FLOOR",obj.value("DOWN_KNIFE_SPEED_FLOOR").toString());
                ConfigInfo.insert("DOWN_KNIFE_SPEED_UPPER",obj.value("DOWN_KNIFE_SPEED_UPPER").toString());
                ConfigInfo.insert("PRODUCE_SPEED_FLOOR",obj.value("PRODUCE_SPEED_FLOOR").toString());
                ConfigInfo.insert("PRODUCE_SPEED_UPPER",obj.value("PRODUCE_SPEED_UPPER").toString());
                ConfigInfo.insert("HIGH_SPEED_FLOOR",obj.value("HIGH_SPEED_FLOOR").toString());
                ConfigInfo.insert("HIGH_SPEED_UPPER",obj.value("HIGH_SPEED_UPPER").toString());
                ConfigInfo.insert("LOW_SPEED_FLOOR",obj.value("LOW_SPEED_FLOOR").toString());
                ConfigInfo.insert("LOW_SPEED_UPPER",obj.value("LOW_SPEED_UPPER").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_WAVE_SOLDERING_PARA"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_WAVE_SOLDERING_PARA"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("MOTOR_1_FRE_FLOOR",obj.value("MOTOR_1_FRE_FLOOR").toString());
                ConfigInfo.insert("MOTOR_1_FRE_UPPER",obj.value("MOTOR_1_FRE_UPPER").toString());
                ConfigInfo.insert("MOTOR_2_FRE_FLOOR",obj.value("MOTOR_2_FRE_FLOOR").toString());
                ConfigInfo.insert("MOTOR_2_FRE_UPPER",obj.value("MOTOR_2_FRE_UPPER").toString());
                ConfigInfo.insert("POLDER_SPEED_FLOOR",obj.value("POLDER_SPEED_FLOOR").toString());
                ConfigInfo.insert("POLDER_SPEED_UPPER",obj.value("POLDER_SPEED_UPPER").toString());
                ConfigInfo.insert("PRE_HEAT_1_FLOOR",obj.value("PRE_HEAT_1_FLOOR").toString());
                ConfigInfo.insert("PRE_HEAT_1_UPPER",obj.value("PRE_HEAT_1_UPPER").toString());
                ConfigInfo.insert("PRE_HEAT_2_FLOOR",obj.value("PRE_HEAT_2_FLOOR").toString());
                ConfigInfo.insert("PRE_HEAT_2_UPPER",obj.value("PRE_HEAT_2_UPPER").toString());
                ConfigInfo.insert("PRE_HEAT_3_FLOOR",obj.value("PRE_HEAT_3_FLOOR").toString());
                ConfigInfo.insert("PRE_HEAT_3_UPPER",obj.value("PRE_HEAT_3_UPPER").toString());
                ConfigInfo.insert("SOLER_TEMP_FLOOR",obj.value("SOLER_TEMP_FLOOR").toString());
                ConfigInfo.insert("SOLER_TEMP_UPPER",obj.value("SOLER_TEMP_UPPER").toString());
                ConfigInfo.insert("DEFECT_RATE",obj.value("DEFECT_RATE").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_INSERT_MACHINE_PARA"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_INSERT_MACHINE_PARA"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("INSERT_MACHINE_1_THROW_RATE_STANDARD",obj.value("INSERT_MACHINE_1_THROW_RATE_STANDARD").toString());
                ConfigInfo.insert("INSERT_MACHINE_2_THROW_RATE_STANDARD",obj.value("INSERT_MACHINE_2_THROW_RATE_STANDARD").toString());
                ConfigInfo.insert("INSERT_MACHINE_3_THROW_RATE_STANDARD",obj.value("INSERT_MACHINE_3_THROW_RATE_STANDARD").toString());
                ConfigInfo.insert("INSERT_MACHINE_4_THROW_RATE_STANDARD",obj.value("INSERT_MACHINE_4_THROW_RATE_STANDARD").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_TEST_STATION_PARA"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_TEST_STATION_PARA"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("PASS_RATE",obj.value("PASS_RATE").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_ASSET_NUMBER"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_ASSET_NUMBER"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("UP_BOARD_ASSET_NUMBER",obj.value("UP_BOARD_ASSET_NUMBER").toString());
                ConfigInfo.insert("DIVIDE_BOARD_ASSET_NUMBER",obj.value("DIVIDE_BOARD_ASSET_NUMBER").toString());
                ConfigInfo.insert("UP_UTENSIL_ASSET_NUMBER",obj.value("UP_UTENSIL_ASSET_NUMBER").toString());
                ConfigInfo.insert("GLUE_DISPENSE_ASSET_NUMBER",obj.value("GLUE_DISPENSE_ASSET_NUMBER").toString());
                ConfigInfo.insert("INSERT_MACHINE_1_ASSET_NUMBER",obj.value("INSERT_MACHINE_1_ASSET_NUMBER").toString());
                ConfigInfo.insert("INSERT_MACHINE_2_ASSET_NUMBER",obj.value("INSERT_MACHINE_2_ASSET_NUMBER").toString());
                ConfigInfo.insert("INSERT_MACHINE_3_ASSET_NUMBER",obj.value("INSERT_MACHINE_3_ASSET_NUMBER").toString());
                ConfigInfo.insert("INSERT_MACHINE_4_ASSET_NUMBER",obj.value("INSERT_MACHINE_4_ASSET_NUMBER").toString());
                ConfigInfo.insert("WAVE_SOLDERING_ASSET_NUMBER",obj.value("WAVE_SOLDERING_ASSET_NUMBER").toString());
                ConfigInfo.insert("PICK_UP_COVER_ASSET_NUMBER",obj.value("PICK_UP_COVER_ASSET_NUMBER").toString());
                ConfigInfo.insert("AOI_ASSET_NUMBER",obj.value("AOI_ASSET_NUMBER").toString());
                ConfigInfo.insert("DOWN_UTENSIL_ASSET_NUMBER",obj.value("DOWN_UTENSIL_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_1_ASSET_NUMBER",obj.value("TEST_STATION_1_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_2_ASSET_NUMBER",obj.value("TEST_STATION_2_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_3_ASSET_NUMBER",obj.value("TEST_STATION_3_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_4_ASSET_NUMBER",obj.value("TEST_STATION_4_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_5_ASSET_NUMBER",obj.value("TEST_STATION_5_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_6_ASSET_NUMBER",obj.value("TEST_STATION_6_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_7_ASSET_NUMBER",obj.value("TEST_STATION_7_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_8_ASSET_NUMBER",obj.value("TEST_STATION_8_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_9_ASSET_NUMBER",obj.value("TEST_STATION_9_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_10_ASSET_NUMBER",obj.value("TEST_STATION_10_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_11_ASSET_NUMBER",obj.value("TEST_STATION_11_ASSET_NUMBER").toString());
                ConfigInfo.insert("TEST_STATION_12_ASSET_NUMBER",obj.value("TEST_STATION_12_ASSET_NUMBER").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_DEVICE_NAME"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_DEVICE_NAME"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("UP_BOARD_DEVICE_NAME",obj.value("UP_BOARD_DEVICE_NAME").toString());
                ConfigInfo.insert("DIVIDE_BOARD_DEVICE_NAME",obj.value("DIVIDE_BOARD_DEVICE_NAME").toString());
                ConfigInfo.insert("UP_UTENSIL_DEVICE_NAME",obj.value("UP_UTENSIL_DEVICE_NAME").toString());
                ConfigInfo.insert("GLUE_DISPENSE_DEVICE_NAME",obj.value("GLUE_DISPENSE_DEVICE_NAME").toString());
                ConfigInfo.insert("INSERT_MACHINE_1_DEVICE_NAME",obj.value("INSERT_MACHINE_1_DEVICE_NAME").toString());
                ConfigInfo.insert("INSERT_MACHINE_2_DEVICE_NAME",obj.value("INSERT_MACHINE_2_DEVICE_NAME").toString());
                ConfigInfo.insert("INSERT_MACHINE_3_DEVICE_NAME",obj.value("INSERT_MACHINE_3_DEVICE_NAME").toString());
                ConfigInfo.insert("INSERT_MACHINE_4_DEVICE_NAME",obj.value("INSERT_MACHINE_4_DEVICE_NAME").toString());
                ConfigInfo.insert("WAVE_SOLDERING_DEVICE_NAME",obj.value("WAVE_SOLDERING_DEVICE_NAME").toString());
                ConfigInfo.insert("PICK_UP_COVER_DEVICE_NAME",obj.value("PICK_UP_COVER_DEVICE_NAME").toString());
                ConfigInfo.insert("AOI_DEVICE_NAME",obj.value("AOI_DEVICE_NAME").toString());
                ConfigInfo.insert("DOWN_UTENSIL_DEVICE_NAME",obj.value("DOWN_UTENSIL_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_1_DEVICE_NAME",obj.value("TEST_STATION_1_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_2_DEVICE_NAME",obj.value("TEST_STATION_2_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_3_DEVICE_NAME",obj.value("TEST_STATION_3_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_4_DEVICE_NAME",obj.value("TEST_STATION_4_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_5_DEVICE_NAME",obj.value("TEST_STATION_5_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_6_DEVICE_NAME",obj.value("TEST_STATION_6_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_7_DEVICE_NAME",obj.value("TEST_STATION_7_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_8_DEVICE_NAME",obj.value("TEST_STATION_8_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_9_DEVICE_NAME",obj.value("TEST_STATION_9_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_10_DEVICE_NAME",obj.value("TEST_STATION_10_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_11_DEVICE_NAME",obj.value("TEST_STATION_11_DEVICE_NAME").toString());
                ConfigInfo.insert("TEST_STATION_12_DEVICE_NAME",obj.value("TEST_STATION_12_DEVICE_NAME").toString());
            }
        }

        if(m_pRootObj.contains("THREE_IN_ONE_TOPIC"))
        {
            QJsonValue objValue = m_pRootObj.value(QStringLiteral("THREE_IN_ONE_TOPIC"));
            if(objValue.isObject())
            {
                QJsonObject obj = objValue.toObject();
                ConfigInfo.insert("UP_BOARD_TOPIC",obj.value("UP_BOARD_TOPIC").toString());
                ConfigInfo.insert("DIVIDE_BOARD_TOPIC",obj.value("DIVIDE_BOARD_TOPIC").toString());
                ConfigInfo.insert("UP_UTENSIL_TOPIC",obj.value("UP_UTENSIL_TOPIC").toString());
                ConfigInfo.insert("GLUE_DISPENSE_TOPIC",obj.value("GLUE_DISPENSE_TOPIC").toString());
                ConfigInfo.insert("INSERT_MACHINE_1_TOPIC",obj.value("INSERT_MACHINE_1_TOPIC").toString());
                ConfigInfo.insert("INSERT_MACHINE_2_TOPIC",obj.value("INSERT_MACHINE_2_TOPIC").toString());
                ConfigInfo.insert("INSERT_MACHINE_3_TOPIC",obj.value("INSERT_MACHINE_3_TOPIC").toString());
                ConfigInfo.insert("INSERT_MACHINE_4_TOPIC",obj.value("INSERT_MACHINE_4_TOPIC").toString());
                ConfigInfo.insert("WAVE_SOLDERING_TOPIC",obj.value("WAVE_SOLDERING_TOPIC").toString());
                ConfigInfo.insert("PICK_UP_COVER_TOPIC",obj.value("PICK_UP_COVER_TOPIC").toString());
                ConfigInfo.insert("AOI_TOPIC",obj.value("AOI_TOPIC").toString());
                ConfigInfo.insert("DOWN_UTENSIL_TOPIC",obj.value("DOWN_UTENSIL_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_1_TOPIC",obj.value("TEST_STATION_1_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_2_TOPIC",obj.value("TEST_STATION_2_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_3_TOPIC",obj.value("TEST_STATION_3_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_4_TOPIC",obj.value("TEST_STATION_4_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_5_TOPIC",obj.value("TEST_STATION_5_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_6_TOPIC",obj.value("TEST_STATION_6_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_7_TOPIC",obj.value("TEST_STATION_7_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_8_TOPIC",obj.value("TEST_STATION_8_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_9_TOPIC",obj.value("TEST_STATION_9_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_10_TOPIC",obj.value("TEST_STATION_10_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_11_TOPIC",obj.value("TEST_STATION_11_TOPIC").toString());
                ConfigInfo.insert("TEST_STATION_12_TOPIC",obj.value("TEST_STATION_12_TOPIC").toString());
            }
        }
    }
}

QString DataAcquisitionConfig::get_config_para(const QString keyname)
{
    return ConfigInfo[keyname];
}

void DataAcquisitionConfig::set_config_para(const QString keyname, const QString keyvalue)
{
    ConfigInfo[keyname] = keyvalue;//update the config file,at the same time you have to update the info that has been loaded

    QJsonObject subObj;
    QJsonArray subArray;
}

void DataAcquisitionConfig::save_config_file()
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
