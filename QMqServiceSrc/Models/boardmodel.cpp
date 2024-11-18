#include "boardmodel.h"
#include "componentmodel.h"

#include <QList>

BoardModel::BoardModel()
{

}

void BoardModel::copy(BoardModel *model)
{
    setCode(model->getCode());
    setConfirmer(model->getConfirmer());
    setEnable(model->getEnable());
    setFailComponentCount(model->getFailComponentCount());
    setHadConfirmed(model->getHadConfirmed());
    setId(model->getId());
    setPid(model->getPid());
    setIsFront(model->getIsFront());
    setIsLASubLine(model->getIsLASubLine());
    setMisjudgementComponentCount(model->getMisjudgementComponentCount());
    setTestSuccessed(model->getTestSuccessed());
    setTestTime(model->getTestTime());
    setNo(model->getNo());
    setStation(model->getStation());
    setType(model->getType());

    SetMesNewText(model->GetMesNewText());
}
QList<ComponentModel*> BoardModel::getComponentModels()
{
    return componentModels;
}
void BoardModel::appendComponentModel(ComponentModel* model)
{
    componentModels.append(model);
}

QString BoardModel::getFrontOrRearCode()
{
    if(getIsFront())
    {
        return "Front";
    }
    else
    {
        return "Rear";
    }
}

QString BoardModel::getNo() const
{
    return no;
}

void BoardModel::setNo(const QString &value)
{
    no = value;
}

void BoardModel::clearComponentModels()
{
    //删除
    qDeleteAll(componentModels);
    componentModels.clear();

}

QString BoardModel::getTestTimeDesc()
{
    return testTimeDesc;
}

int BoardModel::getSubLineNo()
{
    if(getIsLASubLine())
    {
        return 2;
    }
    else
    {
        return 4;
    }
}

QString BoardModel::getResultCode()
{
    if(getTestSuccessed())
    {
        return "OK";
    }
    else
    {
        return "ERR";
    }
}

QString BoardModel::getResultName()
{
    if(getTestSuccessed())
    {
        return "正常";
    }
    else
    {
        return "异常";
    }
}

int BoardModel::getMisjudgementComponentCount() const
{
    return misjudgementComponentCount;
}

void BoardModel::setMisjudgementComponentCount(int value)
{
    misjudgementComponentCount = value;
}

int BoardModel::getFailComponentCount() const
{
    return failComponentCount;
}

void BoardModel::setFailComponentCount(int value)
{
    failComponentCount = value;
}

bool BoardModel::getHadMisjudements()
{
    return misjudgementComponentCount>0;
}

bool BoardModel::getEnable() const
{
    return enable;
}

void BoardModel::setEnable(bool value)
{
    enable = value;
}

bool BoardModel::getTestSuccessed() const
{
    return testSuccessed;
}

void BoardModel::setTestSuccessed(bool value)
{
    testSuccessed = value;
}

bool BoardModel::getIsLASubLine() const
{
    return isLASubLine;
}

void BoardModel::setIsLASubLine(bool value)
{
    isLASubLine = value;
}

bool BoardModel::getHadConfirmed() const
{
    return hadConfirmed;
}

void BoardModel::setHadConfirmed(bool value)
{
    hadConfirmed = value;
}

QString BoardModel::getConfirmer() const
{
    return confirmer;
}

void BoardModel::setConfirmer(const QString &value)
{
    confirmer = value;
}

QDateTime BoardModel::getTestTime() const
{
    return testTime;
}

void BoardModel::setTestTime(const QDateTime &value)
{
    testTime = value;
    testTimeDesc = testTime.toString("yyyy-MM-dd hh:mm:ss");
}

bool BoardModel::getIsFront() const
{
    return isFront;
}

void BoardModel::setIsFront(bool value)
{
    isFront = value;
}

QString BoardModel::getType() const
{
    return type;
}

void BoardModel::setType(const QString &value)
{
    type = value;
}
QString BoardModel::getStation() const
{
    return station;
}

void BoardModel::setStation(const QString &value)
{
    station = value;
}

QString BoardModel::getCode() const
{
    return code;
}

void BoardModel::setCode(const QString &value)
{
    code = value;
}

QString BoardModel::getId() const
{
    return id;
}

void BoardModel::setId(const QString &value)
{
    id = value;
}
QString BoardModel::getPid() const
{
    return pid;
}

void BoardModel::setPid(const QString &value)
{
    pid = value;
}
QString BoardModel::GetMesNewText() const
{
    return MesNewText;
}

void BoardModel::SetMesNewText(const QString &value)
{
    MesNewText = value;
}

