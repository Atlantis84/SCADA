#include "threeinoneiniconfig.h"
#include <QApplication>
#include <QDir>
#include "QsLog.h"
#include <QSettings>
ThreeInOneIniConfig* ThreeInOneIniConfig::m_pInstance = nullptr;

ThreeInOneIniConfig::ThreeInOneIniConfig()
{

}

void ThreeInOneIniConfig::load_ini_config()
{
    if(INI_FILE_PATH.isEmpty())
    {
        INI_FILE_PATH.append(QCoreApplication::applicationDirPath());
        INI_FILE_PATH.append(QDir::separator());
        INI_FILE_PATH.append("ThreeInOne.ini");
    }

    QFileInfo file(INI_FILE_PATH);
    if(file.exists()==false){
        QLOG_WARN()<<"ThreeInOne.ini is not existed!";
        return;
    }

    QSettings * qSettings = new QSettings(INI_FILE_PATH, QSettings::IniFormat);
    qSettings->setIniCodec("UTF8");
    QString groupName="";

    groupName="System/";
    App_LineCode = qSettings->value(groupName+"App_LineCode",App_LineCode).toString();
    App_LineName = qSettings->value(groupName+"App_LineName",App_LineName).toString();

    groupName = "UpBoard/";
    QString UpBoard_DeviceType,UpBoard_DeviceCode,UpBoard_DeviceName,UpBoardCodeProfixData,UpBoardAlarmProfixData;
    UpBoard_DeviceType = qSettings->value(groupName+"UpBoard_DeviceType",UpBoard_DeviceType).toString();
    UpBoard_DeviceCode = qSettings->value(groupName+"UpBoard_DeviceCode",UpBoard_DeviceCode).toString();
    UpBoard_DeviceName = qSettings->value(groupName+"UpBoard_DeviceName",UpBoard_DeviceName).toString();
    UpBoardCodeProfixData = qSettings->value(groupName+"UpBoardCodeProfixData",UpBoardCodeProfixData).toString();
    UpBoardAlarmProfixData = qSettings->value(groupName+"UpBoardAlarmProfixData",UpBoardAlarmProfixData).toString();
    m_pMapIniInfo.insert("UpBoard_DeviceType",UpBoard_DeviceType);
    m_pMapIniInfo.insert("UpBoard_DeviceCode",UpBoard_DeviceCode);
    m_pMapIniInfo.insert("UpBoard_DeviceName",UpBoard_DeviceName);
    m_pMapIniInfo.insert("UpBoardCodeProfixData",UpBoardCodeProfixData);
    m_pMapIniInfo.insert("UpBoardAlarmProfixData",UpBoardAlarmProfixData);

    groupName = "SubBoard/";
    QString Sub_DeviceType,Sub_DeviceCode,Sub_DeviceName,SubBoardCodeProfixData,SubBoardAlarmProfixData;
    Sub_DeviceType = qSettings->value(groupName+"Sub_DeviceType",Sub_DeviceType).toString();
    Sub_DeviceCode = qSettings->value(groupName+"Sub_DeviceCode",Sub_DeviceCode).toString();
    Sub_DeviceName = qSettings->value(groupName+"Sub_DeviceName",Sub_DeviceName).toString();
    SubBoardCodeProfixData = qSettings->value(groupName+"SubBoardCodeProfixData",SubBoardCodeProfixData).toString();
    SubBoardAlarmProfixData = qSettings->value(groupName+"SubBoardAlarmProfixData",SubBoardAlarmProfixData).toString();
    m_pMapIniInfo.insert("Sub_DeviceType",Sub_DeviceType);
    m_pMapIniInfo.insert("Sub_DeviceCode",Sub_DeviceCode);
    m_pMapIniInfo.insert("Sub_DeviceName",Sub_DeviceName);
    m_pMapIniInfo.insert("SubBoardCodeProfixData",SubBoardCodeProfixData);
    m_pMapIniInfo.insert("SubBoardAlarmProfixData",SubBoardAlarmProfixData);

    groupName = "UpUtensil/";
    QString UpUtensil_DeviceType,UpUtensil_DeviceCode,UpUtensil_DeviceName,UpUtensilCodeProfixData,UpUtensilAlarmProfixData;
    UpUtensil_DeviceType = qSettings->value(groupName+"UpUtensil_DeviceType",UpUtensil_DeviceType).toString();
    UpUtensil_DeviceCode = qSettings->value(groupName+"UpUtensil_DeviceCode",UpUtensil_DeviceCode).toString();
    UpUtensil_DeviceName = qSettings->value(groupName+"UpUtensil_DeviceName",UpUtensil_DeviceName).toString();
    UpUtensilCodeProfixData = qSettings->value(groupName+"UpUtensilCodeProfixData",UpUtensilCodeProfixData).toString();
    UpUtensilAlarmProfixData = qSettings->value(groupName+"UpUtensilAlarmProfixData",UpUtensilAlarmProfixData).toString();
    m_pMapIniInfo.insert("UpUtensil_DeviceType",Sub_DeviceType);
    m_pMapIniInfo.insert("UpUtensil_DeviceCode",Sub_DeviceCode);
    m_pMapIniInfo.insert("UpUtensil_DeviceName",Sub_DeviceName);
    m_pMapIniInfo.insert("UpUtensilCodeProfixData",SubBoardCodeProfixData);
    m_pMapIniInfo.insert("UpUtensilAlarmProfixData",SubBoardAlarmProfixData);

    groupName = "GlueDispenser/";
    QString GlueDispenser_DeviceType,GlueDispenser_DeviceCode,
            GlueDispenser_DeviceName,GlueDispenserCodeProfixData,GlueDispenserAlarmProfixData;
    GlueDispenser_DeviceType = qSettings->value(groupName+"GlueDispenser_DeviceType",GlueDispenser_DeviceType).toString();
    GlueDispenser_DeviceCode = qSettings->value(groupName+"GlueDispenser_DeviceCode",GlueDispenser_DeviceCode).toString();
    GlueDispenser_DeviceName = qSettings->value(groupName+"GlueDispenser_DeviceName",GlueDispenser_DeviceName).toString();
    GlueDispenserCodeProfixData = qSettings->value(groupName+"GlueDispenserCodeProfixData",GlueDispenserCodeProfixData).toString();
    GlueDispenserAlarmProfixData = qSettings->value(groupName+"GlueDispenserAlarmProfixData",GlueDispenserAlarmProfixData).toString();
    m_pMapIniInfo.insert("GlueDispenser_DeviceType",GlueDispenser_DeviceType);
    m_pMapIniInfo.insert("GlueDispenser_DeviceCode",GlueDispenser_DeviceCode);
    m_pMapIniInfo.insert("GlueDispenser_DeviceName",GlueDispenser_DeviceName);
    m_pMapIniInfo.insert("GlueDispenserCodeProfixData",GlueDispenserCodeProfixData);
    m_pMapIniInfo.insert("GlueDispenserAlarmProfixData",GlueDispenserAlarmProfixData);

    groupName = "InsertionMachine/";
    QString InsertionMachine_DeviceType,
            InsertionMachine_DeviceCodeA,
            InsertionMachine_DeviceNameA,
            InsertionMachine_MachineCodeA,
            InsertionMachine_MachineNameA,
            InsertionMachineCodeProfixDataA,
            InsertionMachineAlarmProfixDataA;
    InsertionMachine_DeviceType = qSettings->value(groupName+"InsertionMachine_DeviceType",InsertionMachine_DeviceType).toString();
    InsertionMachine_DeviceCodeA = qSettings->value(groupName+"InsertionMachine_DeviceCodeA",InsertionMachine_DeviceCodeA).toString();
    InsertionMachine_DeviceNameA = qSettings->value(groupName+"InsertionMachine_DeviceNameA",InsertionMachine_DeviceNameA).toString();
    InsertionMachine_MachineCodeA = qSettings->value(groupName+"InsertionMachine_MachineCodeA",InsertionMachine_MachineCodeA).toString();
    InsertionMachine_MachineNameA = qSettings->value(groupName+"InsertionMachine_MachineNameA",InsertionMachine_MachineNameA).toString();
    InsertionMachineCodeProfixDataA = qSettings->value(groupName+"InsertionMachineCodeProfixDataA",InsertionMachineCodeProfixDataA).toString();
    InsertionMachineAlarmProfixDataA = qSettings->value(groupName+"InsertionMachineAlarmProfixDataA",InsertionMachineAlarmProfixDataA).toString();
    m_pMapIniInfo.insert("InsertionMachine_DeviceType",InsertionMachine_DeviceType);
    m_pMapIniInfo.insert("InsertionMachine_DeviceCodeA",InsertionMachine_DeviceCodeA);
    m_pMapIniInfo.insert("InsertionMachine_DeviceNameA",InsertionMachine_DeviceNameA);
    m_pMapIniInfo.insert("InsertionMachine_MachineCodeA",InsertionMachine_MachineCodeA);
    m_pMapIniInfo.insert("InsertionMachine_MachineNameA",InsertionMachine_MachineNameA);
    m_pMapIniInfo.insert("InsertionMachineCodeProfixDataA",InsertionMachineCodeProfixDataA);
    m_pMapIniInfo.insert("InsertionMachineAlarmProfixDataA",InsertionMachineAlarmProfixDataA);

    QString InsertionMachine_DeviceCodeB,
            InsertionMachine_DeviceNameB,
            InsertionMachine_MachineCodeB,
            InsertionMachine_MachineNameB,
            InsertionMachineCodeProfixDataB,
            InsertionMachineAlarmProfixDataB;
    InsertionMachine_DeviceCodeB = qSettings->value(groupName+"InsertionMachine_DeviceCodeB",InsertionMachine_DeviceCodeB).toString();
    InsertionMachine_DeviceNameB = qSettings->value(groupName+"InsertionMachine_DeviceNameB",InsertionMachine_DeviceNameB).toString();
    InsertionMachine_MachineCodeB = qSettings->value(groupName+"InsertionMachine_MachineCodeB",InsertionMachine_MachineCodeB).toString();
    InsertionMachine_MachineNameB = qSettings->value(groupName+"InsertionMachine_MachineNameB",InsertionMachine_MachineNameB).toString();
    InsertionMachineCodeProfixDataB = qSettings->value(groupName+"InsertionMachineCodeProfixDataB",InsertionMachineCodeProfixDataB).toString();
    InsertionMachineAlarmProfixDataB = qSettings->value(groupName+"InsertionMachineAlarmProfixDataB",InsertionMachineAlarmProfixDataB).toString();
    m_pMapIniInfo.insert("InsertionMachine_DeviceCodeB",InsertionMachine_DeviceCodeB);
    m_pMapIniInfo.insert("InsertionMachine_DeviceNameB",InsertionMachine_DeviceNameB);
    m_pMapIniInfo.insert("InsertionMachine_MachineCodeB",InsertionMachine_MachineCodeB);
    m_pMapIniInfo.insert("InsertionMachine_MachineNameB",InsertionMachine_MachineNameB);
    m_pMapIniInfo.insert("InsertionMachineCodeProfixDataB",InsertionMachineCodeProfixDataB);
    m_pMapIniInfo.insert("InsertionMachineAlarmProfixDataB",InsertionMachineAlarmProfixDataB);

    QString InsertionMachine_DeviceCodeC,
            InsertionMachine_DeviceNameC,
            InsertionMachine_MachineCodeC,
            InsertionMachine_MachineNameC,
            InsertionMachineCodeProfixDataC,
            InsertionMachineAlarmProfixDataC;
    InsertionMachine_DeviceCodeC = qSettings->value(groupName+"InsertionMachine_DeviceCodeC",InsertionMachine_DeviceCodeC).toString();
    InsertionMachine_DeviceNameC = qSettings->value(groupName+"InsertionMachine_DeviceNameC",InsertionMachine_DeviceNameC).toString();
    InsertionMachine_MachineCodeC = qSettings->value(groupName+"InsertionMachine_MachineCodeC",InsertionMachine_MachineCodeC).toString();
    InsertionMachine_MachineNameC = qSettings->value(groupName+"InsertionMachine_MachineNameC",InsertionMachine_MachineNameC).toString();
    InsertionMachineCodeProfixDataC = qSettings->value(groupName+"InsertionMachineCodeProfixDataC",InsertionMachineCodeProfixDataC).toString();
    InsertionMachineAlarmProfixDataC = qSettings->value(groupName+"InsertionMachineAlarmProfixDataC",InsertionMachineAlarmProfixDataC).toString();
    m_pMapIniInfo.insert("InsertionMachine_DeviceCodeC",InsertionMachine_DeviceCodeC);
    m_pMapIniInfo.insert("InsertionMachine_DeviceNameC",InsertionMachine_DeviceNameC);
    m_pMapIniInfo.insert("InsertionMachine_MachineCodeC",InsertionMachine_MachineCodeC);
    m_pMapIniInfo.insert("InsertionMachine_MachineNameC",InsertionMachine_MachineNameC);
    m_pMapIniInfo.insert("InsertionMachineCodeProfixDataC",InsertionMachineCodeProfixDataC);
    m_pMapIniInfo.insert("InsertionMachineAlarmProfixDataC",InsertionMachineAlarmProfixDataC);

    QString InsertionMachine_DeviceCodeD,
            InsertionMachine_DeviceNameD,
            InsertionMachine_MachineCodeD,
            InsertionMachine_MachineNameD,
            InsertionMachineCodeProfixDataD,
            InsertionMachineAlarmProfixDataD;
    InsertionMachine_DeviceCodeD = qSettings->value(groupName+"InsertionMachine_DeviceCodeD",InsertionMachine_DeviceCodeD).toString();
    InsertionMachine_DeviceNameD = qSettings->value(groupName+"InsertionMachine_DeviceNameD",InsertionMachine_DeviceNameD).toString();
    InsertionMachine_MachineCodeD = qSettings->value(groupName+"InsertionMachine_MachineCodeD",InsertionMachine_MachineCodeD).toString();
    InsertionMachine_MachineNameD = qSettings->value(groupName+"InsertionMachine_MachineNameD",InsertionMachine_MachineNameD).toString();
    InsertionMachineCodeProfixDataD = qSettings->value(groupName+"InsertionMachineCodeProfixDataD",InsertionMachineCodeProfixDataD).toString();
    InsertionMachineAlarmProfixDataD = qSettings->value(groupName+"InsertionMachineAlarmProfixDataD",InsertionMachineAlarmProfixDataD).toString();
    m_pMapIniInfo.insert("InsertionMachine_DeviceCodeD",InsertionMachine_DeviceCodeD);
    m_pMapIniInfo.insert("InsertionMachine_DeviceNameD",InsertionMachine_DeviceNameD);
    m_pMapIniInfo.insert("InsertionMachine_MachineCodeD",InsertionMachine_MachineCodeD);
    m_pMapIniInfo.insert("InsertionMachine_MachineNameD",InsertionMachine_MachineNameD);
    m_pMapIniInfo.insert("InsertionMachineCodeProfixDataD",InsertionMachineCodeProfixDataD);
    m_pMapIniInfo.insert("InsertionMachineAlarmProfixDataD",InsertionMachineAlarmProfixDataD);

    groupName = "WaveSoldering/";
    QString WaveSoldering_DeviceType,
            WaveSoldering_DeviceCode,
            WaveSoldering_DeviceName,
            WaveSolderingBoardCodeProfixData,
            WaveSolderingBoardAlarmProfixData;
    WaveSoldering_DeviceType = qSettings->value(groupName+"WaveSoldering_DeviceType",WaveSoldering_DeviceType).toString();
    WaveSoldering_DeviceCode = qSettings->value(groupName+"WaveSoldering_DeviceCode",WaveSoldering_DeviceCode).toString();
    WaveSoldering_DeviceName = qSettings->value(groupName+"WaveSoldering_DeviceName",WaveSoldering_DeviceName).toString();
    WaveSolderingBoardCodeProfixData = qSettings->value(groupName+"WaveSolderingBoardCodeProfixData",WaveSolderingBoardCodeProfixData).toString();
    WaveSolderingBoardAlarmProfixData = qSettings->value(groupName+"WaveSolderingBoardAlarmProfixData",WaveSolderingBoardAlarmProfixData).toString();
    m_pMapIniInfo.insert("WaveSoldering_DeviceType",WaveSoldering_DeviceType);
    m_pMapIniInfo.insert("WaveSoldering_DeviceCode",WaveSoldering_DeviceCode);
    m_pMapIniInfo.insert("WaveSoldering_DeviceName",WaveSoldering_DeviceName);
    m_pMapIniInfo.insert("WaveSolderingCodeProfixData",WaveSolderingBoardCodeProfixData);
    m_pMapIniInfo.insert("WaveSolderingAlarmProfixData",WaveSolderingBoardAlarmProfixData);

    groupName = "PickUpCover/";
    QString PickUpCover_DeviceType,
            PickUpCover_DeviceCode,
            PickUpCover_DeviceName,
            PickUpCoverCodeProfixData,
            PickUpCoverAlarmProfixData;
    PickUpCover_DeviceType = qSettings->value(groupName+"PickUpCover_DeviceType",PickUpCover_DeviceType).toString();
    PickUpCover_DeviceCode = qSettings->value(groupName+"PickUpCover_DeviceCode",PickUpCover_DeviceCode).toString();
    PickUpCover_DeviceName = qSettings->value(groupName+"PickUpCover_DeviceName",PickUpCover_DeviceName).toString();
    PickUpCoverCodeProfixData = qSettings->value(groupName+"PickUpCoverCodeProfixData",PickUpCoverCodeProfixData).toString();
    PickUpCoverAlarmProfixData = qSettings->value(groupName+"PickUpCoverAlarmProfixData",PickUpCoverAlarmProfixData).toString();
    m_pMapIniInfo.insert("PickUpCover_DeviceType",PickUpCover_DeviceType);
    m_pMapIniInfo.insert("PickUpCover_DeviceCode",PickUpCover_DeviceCode);
    m_pMapIniInfo.insert("PickUpCover_DeviceName",PickUpCover_DeviceName);
    m_pMapIniInfo.insert("PickUpCoverCodeProfixData",PickUpCoverCodeProfixData);
    m_pMapIniInfo.insert("PickUpCoverAlarmProfixData",PickUpCoverAlarmProfixData);

    groupName = "AOI/";
    QString AOI_DeviceType,
            AOI_DeviceCode,
            AOI_DeviceName,
            AOICodeProfixData,
            AOIAlarmProfixData;
    AOI_DeviceType = qSettings->value(groupName+"AOI_DeviceType",AOI_DeviceType).toString();
    AOI_DeviceCode = qSettings->value(groupName+"AOI_DeviceCode",AOI_DeviceCode).toString();
    AOI_DeviceName = qSettings->value(groupName+"AOI_DeviceName",AOI_DeviceName).toString();
    AOICodeProfixData = qSettings->value(groupName+"AOICodeProfixData",AOICodeProfixData).toString();
    AOIAlarmProfixData = qSettings->value(groupName+"AOIAlarmProfixData",AOIAlarmProfixData).toString();
    m_pMapIniInfo.insert("AOI_DeviceType",AOI_DeviceType);
    m_pMapIniInfo.insert("AOI_DeviceCode",AOI_DeviceCode);
    m_pMapIniInfo.insert("AOI_DeviceName",AOI_DeviceName);
    m_pMapIniInfo.insert("AOICodeProfixData",AOICodeProfixData);
    m_pMapIniInfo.insert("AOIAlarmProfixData",AOIAlarmProfixData);

    groupName = "TestStation/";
    QString TestStation_DeviceType ,
            TestStation_DeviceCodeA,
            TestStation_DeviceNameA,
            TestStation_MachineCodeA,
            TestStation_MachineNameA,
            TestStationCodeProfixDataA,
            TestStationAlarmProfixDataA;
    TestStation_DeviceType = qSettings->value(groupName+"TestStation_DeviceType",TestStation_DeviceType).toString();
    TestStation_DeviceCodeA = qSettings->value(groupName+"TestStation_DeviceCodeA",TestStation_DeviceCodeA).toString();
    TestStation_DeviceNameA = qSettings->value(groupName+"TestStation_DeviceNameA",TestStation_DeviceNameA).toString();
    TestStation_MachineCodeA = qSettings->value(groupName+"TestStation_MachineCodeA",TestStation_MachineCodeA).toString();
    TestStation_MachineNameA = qSettings->value(groupName+"TestStation_MachineNameA",TestStation_MachineNameA).toString();
    TestStationCodeProfixDataA = qSettings->value(groupName+"TestStationCodeProfixDataA",TestStationCodeProfixDataA).toString();
    TestStationAlarmProfixDataA = qSettings->value(groupName+"TestStationAlarmProfixDataA",TestStationAlarmProfixDataA).toString();
    m_pMapIniInfo.insert("TestStation_DeviceType",TestStation_DeviceType);
    m_pMapIniInfo.insert("TestStation_DeviceCodeA",TestStation_DeviceCodeA);
    m_pMapIniInfo.insert("TestStation_DeviceNameA",TestStation_DeviceNameA);
    m_pMapIniInfo.insert("TestStation_MachineCodeA",TestStation_MachineCodeA);
    m_pMapIniInfo.insert("TestStation_MachineNameA",TestStation_MachineNameA);
    m_pMapIniInfo.insert("TestStationCodeProfixDataA",TestStationCodeProfixDataA);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataA",TestStationAlarmProfixDataA);

    QString TestStation_DeviceCodeB,
            TestStation_DeviceNameB,
            TestStation_MachineCodeB,
            TestStation_MachineNameB,
            TestStationCodeProfixDataB,
            TestStationAlarmProfixDataB;
    TestStation_DeviceCodeB = qSettings->value(groupName+"TestStation_DeviceCodeB",TestStation_DeviceCodeB).toString();
    TestStation_DeviceNameB = qSettings->value(groupName+"TestStation_DeviceNameB",TestStation_DeviceNameB).toString();
    TestStation_MachineCodeB = qSettings->value(groupName+"TestStation_MachineCodeB",TestStation_MachineCodeB).toString();
    TestStation_MachineNameB = qSettings->value(groupName+"TestStation_MachineNameB",TestStation_MachineNameB).toString();
    TestStationCodeProfixDataB = qSettings->value(groupName+"TestStationCodeProfixDataB",TestStationCodeProfixDataB).toString();
    TestStationAlarmProfixDataB = qSettings->value(groupName+"TestStationAlarmProfixDataB",TestStationAlarmProfixDataB).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeB",TestStation_DeviceCodeB);
    m_pMapIniInfo.insert("TestStation_DeviceNameB",TestStation_DeviceNameB);
    m_pMapIniInfo.insert("TestStation_MachineCodeB",TestStation_MachineCodeB);
    m_pMapIniInfo.insert("TestStation_MachineNameB",TestStation_MachineNameB);
    m_pMapIniInfo.insert("TestStationCodeProfixDataB",TestStationCodeProfixDataB);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataB",TestStationAlarmProfixDataB);

    QString TestStation_DeviceCodeC,
            TestStation_DeviceNameC,
            TestStation_MachineCodeC,
            TestStation_MachineNameC,
            TestStationCodeProfixDataC,
            TestStationAlarmProfixDataC;
    TestStation_DeviceCodeC = qSettings->value(groupName+"TestStation_DeviceCodeC",TestStation_DeviceCodeC).toString();
    TestStation_DeviceNameC = qSettings->value(groupName+"TestStation_DeviceNameC",TestStation_DeviceNameC).toString();
    TestStation_MachineCodeC = qSettings->value(groupName+"TestStation_MachineCodeC",TestStation_MachineCodeC).toString();
    TestStation_MachineNameC = qSettings->value(groupName+"TestStation_MachineNameC",TestStation_MachineNameC).toString();
    TestStationCodeProfixDataC = qSettings->value(groupName+"TestStationCodeProfixDataC",TestStationCodeProfixDataC).toString();
    TestStationAlarmProfixDataC = qSettings->value(groupName+"TestStationAlarmProfixDataC",TestStationAlarmProfixDataC).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeC",TestStation_DeviceCodeC);
    m_pMapIniInfo.insert("TestStation_DeviceNameC",TestStation_DeviceNameC);
    m_pMapIniInfo.insert("TestStation_MachineCodeC",TestStation_MachineCodeC);
    m_pMapIniInfo.insert("TestStation_MachineNameC",TestStation_MachineNameC);
    m_pMapIniInfo.insert("TestStationCodeProfixDataC",TestStationCodeProfixDataC);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataC",TestStationAlarmProfixDataC);

    QString TestStation_DeviceCodeD,
            TestStation_DeviceNameD,
            TestStation_MachineCodeD,
            TestStation_MachineNameD,
            TestStationCodeProfixDataD,
            TestStationAlarmProfixDataD;
    TestStation_DeviceCodeD = qSettings->value(groupName+"TestStation_DeviceCodeD",TestStation_DeviceCodeD).toString();
    TestStation_DeviceNameD = qSettings->value(groupName+"TestStation_DeviceNameD",TestStation_DeviceNameD).toString();
    TestStation_MachineCodeD = qSettings->value(groupName+"TestStation_MachineCodeD",TestStation_MachineCodeD).toString();
    TestStation_MachineNameD = qSettings->value(groupName+"TestStation_MachineNameD",TestStation_MachineNameD).toString();
    TestStationCodeProfixDataD = qSettings->value(groupName+"TestStationCodeProfixDataD",TestStationCodeProfixDataD).toString();
    TestStationAlarmProfixDataD = qSettings->value(groupName+"TestStationAlarmProfixDataD",TestStationAlarmProfixDataD).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeD",TestStation_DeviceCodeD);
    m_pMapIniInfo.insert("TestStation_DeviceNameD",TestStation_DeviceNameD);
    m_pMapIniInfo.insert("TestStation_MachineCodeD",TestStation_MachineCodeD);
    m_pMapIniInfo.insert("TestStation_MachineNameD",TestStation_MachineNameD);
    m_pMapIniInfo.insert("TestStationCodeProfixDataD",TestStationCodeProfixDataD);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataD",TestStationAlarmProfixDataD);

    QString TestStation_DeviceCodeE,
            TestStation_DeviceNameE,
            TestStation_MachineCodeE,
            TestStation_MachineNameE,
            TestStationCodeProfixDataE,
            TestStationAlarmProfixDataE;
    TestStation_DeviceCodeE = qSettings->value(groupName+"TestStation_DeviceCodeE",TestStation_DeviceCodeE).toString();
    TestStation_DeviceNameE = qSettings->value(groupName+"TestStation_DeviceNameE",TestStation_DeviceNameE).toString();
    TestStation_MachineCodeE = qSettings->value(groupName+"TestStation_MachineCodeE",TestStation_MachineCodeE).toString();
    TestStation_MachineNameE = qSettings->value(groupName+"TestStation_MachineNameE",TestStation_MachineNameE).toString();
    TestStationCodeProfixDataE = qSettings->value(groupName+"TestStationCodeProfixDataE",TestStationCodeProfixDataE).toString();
    TestStationAlarmProfixDataE = qSettings->value(groupName+"TestStationAlarmProfixDataE",TestStationAlarmProfixDataE).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeE",TestStation_DeviceCodeE);
    m_pMapIniInfo.insert("TestStation_DeviceNameE",TestStation_DeviceNameE);
    m_pMapIniInfo.insert("TestStation_MachineCodeE",TestStation_MachineCodeE);
    m_pMapIniInfo.insert("TestStation_MachineNameE",TestStation_MachineNameE);
    m_pMapIniInfo.insert("TestStationCodeProfixDataE",TestStationCodeProfixDataE);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataE",TestStationAlarmProfixDataE);

    QString TestStation_DeviceCodeF,
            TestStation_DeviceNameF,
            TestStation_MachineCodeF,
            TestStation_MachineNameF,
            TestStationCodeProfixDataF,
            TestStationAlarmProfixDataF;
    TestStation_DeviceCodeF = qSettings->value(groupName+"TestStation_DeviceCodeF",TestStation_DeviceCodeF).toString();
    TestStation_DeviceNameF = qSettings->value(groupName+"TestStation_DeviceNameF",TestStation_DeviceNameF).toString();
    TestStation_MachineCodeF = qSettings->value(groupName+"TestStation_MachineCodeF",TestStation_MachineCodeF).toString();
    TestStation_MachineNameF = qSettings->value(groupName+"TestStation_MachineNameF",TestStation_MachineNameF).toString();
    TestStationCodeProfixDataF = qSettings->value(groupName+"TestStationCodeProfixDataF",TestStationCodeProfixDataF).toString();
    TestStationAlarmProfixDataF = qSettings->value(groupName+"TestStationAlarmProfixDataF",TestStationAlarmProfixDataF).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeF",TestStation_DeviceCodeF);
    m_pMapIniInfo.insert("TestStation_DeviceNameF",TestStation_DeviceNameF);
    m_pMapIniInfo.insert("TestStation_MachineCodeF",TestStation_MachineCodeF);
    m_pMapIniInfo.insert("TestStation_MachineNameF",TestStation_MachineNameF);
    m_pMapIniInfo.insert("TestStationCodeProfixDataF",TestStationCodeProfixDataF);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataF",TestStationAlarmProfixDataF);

    QString TestStation_DeviceCodeG,
            TestStation_DeviceNameG,
            TestStation_MachineCodeG,
            TestStation_MachineNameG,
            TestStationCodeProfixDataG,
            TestStationAlarmProfixDataG;
    TestStation_DeviceCodeG = qSettings->value(groupName+"TestStation_DeviceCodeG",TestStation_DeviceCodeG).toString();
    TestStation_DeviceNameG = qSettings->value(groupName+"TestStation_DeviceNameG",TestStation_DeviceNameG).toString();
    TestStation_MachineCodeG = qSettings->value(groupName+"TestStation_MachineCodeG",TestStation_MachineCodeG).toString();
    TestStation_MachineNameG = qSettings->value(groupName+"TestStation_MachineNameG",TestStation_MachineNameG).toString();
    TestStationCodeProfixDataG = qSettings->value(groupName+"TestStationCodeProfixDataG",TestStationCodeProfixDataG).toString();
    TestStationAlarmProfixDataG = qSettings->value(groupName+"TestStationAlarmProfixDataG",TestStationAlarmProfixDataG).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeG",TestStation_DeviceCodeG);
    m_pMapIniInfo.insert("TestStation_DeviceNameG",TestStation_DeviceNameG);
    m_pMapIniInfo.insert("TestStation_MachineCodeG",TestStation_MachineCodeG);
    m_pMapIniInfo.insert("TestStation_MachineNameG",TestStation_MachineNameG);
    m_pMapIniInfo.insert("TestStationCodeProfixDataG",TestStationCodeProfixDataG);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataG",TestStationAlarmProfixDataG);

    QString TestStation_DeviceCodeH,
            TestStation_DeviceNameH,
            TestStation_MachineCodeH,
            TestStation_MachineNameH,
            TestStationCodeProfixDataH,
            TestStationAlarmProfixDataH;
    TestStation_DeviceCodeH = qSettings->value(groupName+"TestStation_DeviceCodeH",TestStation_DeviceCodeH).toString();
    TestStation_DeviceNameH = qSettings->value(groupName+"TestStation_DeviceNameH",TestStation_DeviceNameH).toString();
    TestStation_MachineCodeH = qSettings->value(groupName+"TestStation_MachineCodeH",TestStation_MachineCodeH).toString();
    TestStation_MachineNameH = qSettings->value(groupName+"TestStation_MachineNameH",TestStation_MachineNameH).toString();
    TestStationCodeProfixDataH = qSettings->value(groupName+"TestStationCodeProfixDataH",TestStationCodeProfixDataH).toString();
    TestStationAlarmProfixDataH = qSettings->value(groupName+"TestStationAlarmProfixDataH",TestStationAlarmProfixDataH).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeH",TestStation_DeviceCodeH);
    m_pMapIniInfo.insert("TestStation_DeviceNameH",TestStation_DeviceNameH);
    m_pMapIniInfo.insert("TestStation_MachineCodeH",TestStation_MachineCodeH);
    m_pMapIniInfo.insert("TestStation_MachineNameH",TestStation_MachineNameH);
    m_pMapIniInfo.insert("TestStationCodeProfixDataH",TestStationCodeProfixDataH);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataH",TestStationAlarmProfixDataH);

    QString TestStation_DeviceCodeI,
            TestStation_DeviceNameI,
            TestStation_MachineCodeI,
            TestStation_MachineNameI,
            TestStationCodeProfixDataI,
            TestStationAlarmProfixDataI;
    TestStation_DeviceCodeI = qSettings->value(groupName+"TestStation_DeviceCodeI",TestStation_DeviceCodeI).toString();
    TestStation_DeviceNameI = qSettings->value(groupName+"TestStation_DeviceNameI",TestStation_DeviceNameI).toString();
    TestStation_MachineCodeI = qSettings->value(groupName+"TestStation_MachineCodeI",TestStation_MachineCodeI).toString();
    TestStation_MachineNameI = qSettings->value(groupName+"TestStation_MachineNameI",TestStation_MachineNameI).toString();
    TestStationCodeProfixDataI = qSettings->value(groupName+"TestStationCodeProfixDataI",TestStationCodeProfixDataI).toString();
    TestStationAlarmProfixDataI = qSettings->value(groupName+"TestStationAlarmProfixDataI",TestStationAlarmProfixDataI).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeI",TestStation_DeviceCodeI);
    m_pMapIniInfo.insert("TestStation_DeviceNameI",TestStation_DeviceNameI);
    m_pMapIniInfo.insert("TestStation_MachineCodeI",TestStation_MachineCodeI);
    m_pMapIniInfo.insert("TestStation_MachineNameI",TestStation_MachineNameI);
    m_pMapIniInfo.insert("TestStationCodeProfixDataI",TestStationCodeProfixDataI);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataI",TestStationAlarmProfixDataI);

    QString TestStation_DeviceCodeJ,
            TestStation_DeviceNameJ,
            TestStation_MachineCodeJ,
            TestStation_MachineNameJ,
            TestStationCodeProfixDataJ,
            TestStationAlarmProfixDataJ;
    TestStation_DeviceCodeJ = qSettings->value(groupName+"TestStation_DeviceCodeJ",TestStation_DeviceCodeJ).toString();
    TestStation_DeviceNameJ = qSettings->value(groupName+"TestStation_DeviceNameJ",TestStation_DeviceNameJ).toString();
    TestStation_MachineCodeJ = qSettings->value(groupName+"TestStation_MachineCodeJ",TestStation_MachineCodeJ).toString();
    TestStation_MachineNameJ = qSettings->value(groupName+"TestStation_MachineNameJ",TestStation_MachineNameJ).toString();
    TestStationCodeProfixDataJ = qSettings->value(groupName+"TestStationCodeProfixDataJ",TestStationCodeProfixDataJ).toString();
    TestStationAlarmProfixDataJ = qSettings->value(groupName+"TestStationAlarmProfixDataJ",TestStationAlarmProfixDataJ).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeJ",TestStation_DeviceCodeJ);
    m_pMapIniInfo.insert("TestStation_DeviceNameJ",TestStation_DeviceNameJ);
    m_pMapIniInfo.insert("TestStation_MachineCodeJ",TestStation_MachineCodeJ);
    m_pMapIniInfo.insert("TestStation_MachineNameJ",TestStation_MachineNameJ);
    m_pMapIniInfo.insert("TestStationCodeProfixDataJ",TestStationCodeProfixDataJ);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataJ",TestStationAlarmProfixDataJ);

    QString TestStation_DeviceCodeK,
            TestStation_DeviceNameK,
            TestStation_MachineCodeK,
            TestStation_MachineNameK,
            TestStationCodeProfixDataK,
            TestStationAlarmProfixDataK;
    TestStation_DeviceCodeK = qSettings->value(groupName+"TestStation_DeviceCodeK",TestStation_DeviceCodeK).toString();
    TestStation_DeviceNameK = qSettings->value(groupName+"TestStation_DeviceNameK",TestStation_DeviceNameK).toString();
    TestStation_MachineCodeK = qSettings->value(groupName+"TestStation_MachineCodeK",TestStation_MachineCodeK).toString();
    TestStation_MachineNameK = qSettings->value(groupName+"TestStation_MachineNameK",TestStation_MachineNameK).toString();
    TestStationCodeProfixDataK = qSettings->value(groupName+"TestStationCodeProfixDataK",TestStationCodeProfixDataK).toString();
    TestStationAlarmProfixDataK = qSettings->value(groupName+"TestStationAlarmProfixDataK",TestStationAlarmProfixDataK).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeK",TestStation_DeviceCodeK);
    m_pMapIniInfo.insert("TestStation_DeviceNameK",TestStation_DeviceNameK);
    m_pMapIniInfo.insert("TestStation_MachineCodeK",TestStation_MachineCodeK);
    m_pMapIniInfo.insert("TestStation_MachineNameK",TestStation_MachineNameK);
    m_pMapIniInfo.insert("TestStationCodeProfixDataK",TestStationCodeProfixDataK);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataK",TestStationAlarmProfixDataK);

    QString TestStation_DeviceCodeL,
            TestStation_DeviceNameL,
            TestStation_MachineCodeL,
            TestStation_MachineNameL,
            TestStationCodeProfixDataL,
            TestStationAlarmProfixDataL;
    TestStation_DeviceCodeL = qSettings->value(groupName+"TestStation_DeviceCodeL",TestStation_DeviceCodeL).toString();
    TestStation_DeviceNameL = qSettings->value(groupName+"TestStation_DeviceNameL",TestStation_DeviceNameL).toString();
    TestStation_MachineCodeL = qSettings->value(groupName+"TestStation_MachineCodeL",TestStation_MachineCodeL).toString();
    TestStation_MachineNameL = qSettings->value(groupName+"TestStation_MachineNameL",TestStation_MachineNameL).toString();
    TestStationCodeProfixDataL = qSettings->value(groupName+"TestStationCodeProfixDataL",TestStationCodeProfixDataL).toString();
    TestStationAlarmProfixDataL = qSettings->value(groupName+"TestStationAlarmProfixDataL",TestStationAlarmProfixDataL).toString();
    m_pMapIniInfo.insert("TestStation_DeviceCodeL",TestStation_DeviceCodeL);
    m_pMapIniInfo.insert("TestStation_DeviceNameL",TestStation_DeviceNameL);
    m_pMapIniInfo.insert("TestStation_MachineCodeL",TestStation_MachineCodeL);
    m_pMapIniInfo.insert("TestStation_MachineNameL",TestStation_MachineNameL);
    m_pMapIniInfo.insert("TestStationCodeProfixDataL",TestStationCodeProfixDataL);
    m_pMapIniInfo.insert("TestStationAlarmProfixDataL",TestStationAlarmProfixDataL);

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
}
