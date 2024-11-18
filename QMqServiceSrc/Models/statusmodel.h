#ifndef STATUSMODEL_H
#define STATUSMODEL_H

#include <QDateTime>
#include <QString>

class StatusModel
{
public:
    StatusModel();
public:
    QDateTime DateTime;
    int StatusValue;
    QString StatusCode;
    QString StatusName;
    QString FaultCode;
    QString FaultName;
    QString FaultDesc;
public:
    bool isFaulting();
    void copy(StatusModel* model);
    void setWait();
public:
    static QString getStatusNameByValue(int value);
    static QString getStatusCodeByValue(int value);
public:
    const static int STATUS_VALUE_Run;
    const static int STATUS_VALUE_Wait;
    const static int STATUS_VALUE_Fault;
    const static int STATUS_VALUE_Stop;
    const static QString STATUS_CODE_Run;
    const static QString STATUS_CODE_Wait;
    const static QString STATUS_CODE_Fault;
    const static QString STATUS_CODE_Stop;
    const static QString STATUS_NAME_Run;
    const static QString STATUS_NAME_Wait;
    const static QString STATUS_NAME_Fault;
    const static QString STATUS_NAME_Stop;
};

#endif // STATUSMODEL_H
