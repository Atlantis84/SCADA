#ifndef DATAACQUISITIONDEFINES_H
#define DATAACQUISITIONDEFINES_H
#include <QString>

enum DATA_TYPE{
    Equipment_State=0,                  //设备状态
    Key_Part_Action_Times=1,            //关键部件
    Ok_Ng_Quantity=2,
    Equipment_Warnings=3,               //故障报警
    Product_ProjectID=4,                //工单
    Product_Type=5,                     //产品类型
    Work_Device_Ok_Ng_Ratio=6,          //工装OK\NG数及通过率
    Param_Set_Value=7,                  //工艺设置参数
    Param_Practical_Value=8,            //设备实际参数
    Param_Warnings=9                    //工艺参数报警
};

enum DATA_TYPE_M201{
    Equipment_State_M201=0,
    Param_Content_M201=1,
    Fault_Content_M201=2,
    Reject_Content_M201=3
};

typedef struct{
    QString Equipment_State;//设备状态
    QString Key_Part_Action_Times;//关键部件
    QString Ok_Ng_Quantity;
    QString Equipment_Warnings;
}DATA_VALUE;

enum DEV_TYPE{
    TconUpperBoardDevA = 0,
    TconUpperBoardDevB,
    TconSubBoardDevA,
    TconSubBoardDevB,
    TconTestDevA,
    TconTestDevB,
    TconAOIdev,
    TconPackeageDev,
};

enum DEV_TYPE_M201{
    M201UpBoard,
    M201DivideBoard,
    M201UpUtensil,
    M201GlueMachine,
    M201InsertMachineA,
    M201InsertMachineB,
    M201InsertMachineC,
    M201InsertMachineD,
    M201WaveSoldering,
    M201PickUpCover,
    M201AOI,
    M201DownUtensil,
    M201TestStationA,
    M201TestStationB,
    M201TestStationC,
    M201TestStationD,
    M201TestStationE,
    M201TestStationF,
    M201TestStationG,
    M201TestStationH,
    M201TestStationI,
    M201TestStationJ,
    M201TestStationK,
    M201TestStationL,
    M202WaveSoldering
};

const QString fmtDateTime("yyyy-MM-dd hh:mm:ss.zzz");

#define DEV_STATE_STOP      "1"
#define DEV_STATE_NOMAL     "2"
#define DEV_STATE_FREE      "3"

//故障与EAM代码映射
typedef struct{
    int DataType_id;
    uint16_t MelsecCode;
    QString MelsecAddr;
    uint16_t length;
}MelsecData;

#endif // DATAACQUISITIONDEFINES_H
