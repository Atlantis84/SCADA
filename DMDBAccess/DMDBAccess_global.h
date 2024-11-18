#ifndef DMDBACCESS_GLOBAL_H
#define DMDBACCESS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#if defined(DMDBACCESS_LIBRARY)
#  define DMDBACCESS_EXPORT Q_DECL_EXPORT
#else
#  define DMDBACCESS_EXPORT Q_DECL_IMPORT
#endif

//1、Info for DBAccess;
//2、for test,need to change for real use;
//3、through ODBC to conn DM Database on the vitural machine
#define  HOSTNAME           "127.0.0.1"
#define  DATABASENAME       "TestBackup"
#define  USERNAME           "postgres"
#define  PASSWORD           "a"
#define  PORTNUM             5432

#define  HOSTNAME_MYSQL           "127.0.0.1"
#define  DATABASENAME_MYSQL       "tv4th"
#define  USERNAME_MYSQL           "root"
#define  PASSWORD_MYSQL          "asdfasdf1"
#define  PORTNUM_MYSQL             3306

//style QODBC/QODBC3/QSQLITE/QPSQL/QPSQL7
#define  DATABASESTYLE       "QPSQL"
#define  DATABASESTYLE_MYSQL       "QMYSQL"

//data table defination
const QString constUserInfoTable        = QString("\"User_Table\"");         //用户数据信息表
const QString constCalibrationEntryTable= QString("\"Calibration_Entry_Table\"");
const QString constCalibrationDetailsTable = QString("\"Calibration_Details_Table\"");
const QString constRegisterMachineTable = QString("\"Machine_Register_Table\"");
const QString constCurrentChangeInfoTable = QString("\"Current_Product_Station_Info\"");
const QString constLightBarWashingResultTable = QString("\"Light_Bar_Results_Table\"");
const QString constLightBarProductTable = QString("\"Light_Bar_Product_Table\"");
const QString constRedFlagGlueInfoTable = QString("\"Red_Flag_Glue_Info\"");

//TCON Data-Acquisition
const QString constTconTestDataAcquisition = QString("\"Test_Data_Acquisition\"");
const QString constUpBoardWarningsTable = QString("\"Up_Board_Warnings_Table\"");
const QString constSubBoardWarningsTable = QString("\"Sub_Board_Warnings_Table\"");
const QString constTestStationWarningsTable = QString("\"Test_Station_Warnings_Table\"");
const QString constAOIWarningsTable = QString("\"AOI_Warnings_Table\"");
const QString constPackageWarningsTable = QString("\"Package_Warnings_Table\"");
const QString constUpABoardDataAcquisition = QString("\"UpA_Board_Data_Acquisition_Table\"");
const QString constUpBBoardDataAcquisition = QString("\"UpB_Board_Data_Acquisition_Table\"");
const QString constSubABoardDataAcquisition = QString("\"SubA_Board_Data_Acquisition_Table\"");
const QString constSubBBoardDataAcquisition = QString("\"SubB_Board_Data_Acquisition_Table\"");
const QString constTestAStationDataAcquisition = QString("\"TestA_Station_Data_Acquisition_Table\"");
const QString constTestBStationDataAcquisition = QString("\"TestB_Station_Data_Acquisition_Table\"");
const QString constAOIDataAcquisition = QString("\"AOI_Data_Acquisition_Table\"");
const QString constPacketDataAcquisition = QString("\"Packet_Data_Acquisition_Table\"");
//TCON Data-Acquisition

//three-in-one-M201 Data-Acquisition
const QString constM201WaveSolderingWarningsTable = QString("\"M201_Wave_Soldering_Warnings_Table\"");
const QString constM201GlueMachineWarningsTable = QString("\"M201_Glue_Machine_Warnings_Table\"");
const QString constM201DivideMachineWarningsTable = QString("\"M201_Divide_Machine_Warnings_Table\"");
const QString constM201UpBoardWarningsTable = QString("\"M201_Up_Board_Warnings_Table\"");
const QString constM201UpUtensilWarningsTable = QString("\"M201_Up_Utensil_Warnings_Table\"");
const QString constM201PickUpCoverWarningsTable = QString("\"M201_Pick_Up_Cover_Warnings_Table\"");
const QString constM201DownUtensilWarningsTable = QString("\"M201_Down_Utensil_Warnings_Table\"");
const QString constM201BoardStyleAndMaterialNumber = QString("\"M201_Board_Style_And_Material_Number\"");
const QString constM201InsertMachineErrors = QString("\"M201_Insert_Machine_Errors\"");
//three-in-one-M201 Data-Acquisition

const QString constProductStyleMapTable = QString("\"Product_Style_Map\"");
const QString constProductStylePowerTable = QString("\"Product_Style_Power_Map\"");
const QString constCountResultTable = QString("\"Count_Result_Table\"");
#endif // DMDBACCESS_GLOBAL_H
