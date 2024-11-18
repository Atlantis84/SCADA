#include "statusmodel.h"

#include <QString>
#include <QDateTime>

const int StatusModel::STATUS_VALUE_Run = 2;
const int StatusModel::STATUS_VALUE_Wait = 4;
const int StatusModel::STATUS_VALUE_Fault = 8;
const int StatusModel::STATUS_VALUE_Stop = 16;
const QString StatusModel::STATUS_CODE_Run = "Run";
const QString StatusModel::STATUS_CODE_Wait = "Wait";
const QString StatusModel::STATUS_CODE_Fault = "Fault";
const QString StatusModel::STATUS_CODE_Stop = "Stop";
const QString StatusModel::STATUS_NAME_Run = "运行";
const QString StatusModel::STATUS_NAME_Wait = "待机";
const QString StatusModel::STATUS_NAME_Fault = "故障";
const QString StatusModel::STATUS_NAME_Stop = "停机";

StatusModel::StatusModel()
{
    DateTime=QDateTime::currentDateTime();
    StatusValue=StatusModel::STATUS_VALUE_Stop;
    StatusCode=StatusModel::STATUS_CODE_Stop;
    StatusName=StatusModel::STATUS_NAME_Stop;
    FaultCode="";
    FaultName="";
    FaultDesc="";
}
void StatusModel::setWait()
{
    DateTime=QDateTime::currentDateTime();
    StatusValue=StatusModel::STATUS_VALUE_Wait;
    StatusCode=StatusModel::STATUS_CODE_Wait;
    StatusName=StatusModel::STATUS_NAME_Wait;
    FaultCode="";
    FaultName="";
    FaultDesc="";
}
void StatusModel::copy(StatusModel* model)
{
    this->DateTime=model->DateTime;
    this->StatusValue=model->StatusValue;
    this->StatusCode=model->StatusCode;
    this->StatusName=model->StatusName;
    this->FaultCode=model->FaultCode;
    this->FaultName=model->FaultName;
    this->FaultDesc=model->FaultDesc;
}
bool StatusModel::isFaulting()
{
    return StatusValue==StatusModel::STATUS_VALUE_Fault;
}
QString StatusModel::getStatusNameByValue(int value)
{
    QString name = "";
    switch (value)
    {
        case STATUS_VALUE_Run: name = STATUS_NAME_Run; break;
        case STATUS_VALUE_Wait: name = STATUS_NAME_Wait; break;
        case STATUS_VALUE_Fault: name = STATUS_NAME_Fault; break;
        case STATUS_VALUE_Stop: name = STATUS_NAME_Stop; break;
        default:
        break;
    }
    return name;
}

QString StatusModel::getStatusCodeByValue(int value)
{
    QString code = "";
    switch (value)
    {
        case STATUS_VALUE_Run: code = STATUS_CODE_Run; break;
        case STATUS_VALUE_Wait: code = STATUS_CODE_Wait; break;
        case STATUS_VALUE_Fault: code = STATUS_CODE_Fault; break;
        case STATUS_VALUE_Stop: code = STATUS_CODE_Stop; break;
        default:
        break;
    }
    return code;
}
