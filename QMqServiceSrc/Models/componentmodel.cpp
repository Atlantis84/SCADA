#include "componentmodel.h"

ComponentModel::ComponentModel()
{

}

QString ComponentModel::getErrorTypeForEAM()
{
    QString eamErrorType="DL";
    eamErrorType+=errorType;
    return eamErrorType;
}
void ComponentModel::copy(ComponentModel* model)
{
    setBoardCode(model->getBoardCode());
    setBoardId(model->getBoardId());
    setBoardPid(model->getBoardPid());
    setBoardNo(model->getBoardNo());
    setBoardType(model->getBoardType());
    setErrorType(model->getErrorType());
    setErrorTypeName(model->getErrorTypeName());
    setHadConfirmed(model->getHadConfirmed());
    setIsMisjudgement(model->getIsMisjudgement());
    setName(model->getName());
    setPositionX1(model->getPositionX1());
    setPositionX2(model->getPositionX2());
    setPositionY1(model->getPositionY1());
    setPositionY2(model->getPositionY2());
    setWinTime(model->getWinTime());
    seterrName(model->geterrName());
    setTestSuccess(model->getTestSuccess());

}
QString ComponentModel::getBoardNo() const
{
    return boardNo;
}

void ComponentModel::setBoardNo(const QString &value)
{
    boardNo = value;
}

bool ComponentModel::getHadConfirmed() const
{
    return hadConfirmed;
}

void ComponentModel::setHadConfirmed(bool value)
{
    hadConfirmed = value;
}

bool ComponentModel::getTestSuccess() const
{
    return testSuccess;
}

void ComponentModel::setTestSuccess(bool value)
{
    testSuccess = value;
}

QString ComponentModel::getErrorTypeName() const
{
    return errorTypeName;
}

void ComponentModel::setErrorTypeName(const QString &value)
{
    errorTypeName = value;
}

int ComponentModel::getPositionY2() const
{
    return positionY2;
}

void ComponentModel::setPositionY2(int value)
{
    positionY2 = value;
}

int ComponentModel::getPositionX2() const
{
    return positionX2;
}

void ComponentModel::setPositionX2(int value)
{
    positionX2 = value;
}

int ComponentModel::getPositionY1() const
{
    return positionY1;
}

void ComponentModel::setPositionY1(int value)
{
    positionY1 = value;
}

int ComponentModel::getPositionX1() const
{
    return positionX1;
}

void ComponentModel::setPositionX1(int value)
{
    positionX1 = value;
}

bool ComponentModel::getIsMisjudgement() const
{
    return isMisjudgement;
}

void ComponentModel::setIsMisjudgement(bool value)
{
    isMisjudgement = value;
}
QDateTime ComponentModel::getWinTime() const
{
    return winTime;
}

void ComponentModel::setWinTime(const QDateTime &value)
{
    winTime = value;
}

QString ComponentModel::geterrName() const
{
    return errorName;
}

void ComponentModel::seterrName(const QString &value)
{
    errorName = value;
}

QString ComponentModel::getErrorType() const
{
    return errorType;
}

void ComponentModel::setErrorType(const QString &value)
{
    errorType = value;
}

QString ComponentModel::getName() const
{
    return name;
}

void ComponentModel::setName(const QString &value)
{
    name = value;
}

QString ComponentModel::getBoardType() const
{
    return boardType;
}

void ComponentModel::setBoardType(const QString &value)
{
    boardType = value;
}

QString ComponentModel::getBoardCode() const
{
    return boardCode;
}

void ComponentModel::setBoardCode(const QString &value)
{
    boardCode = value;
}

QString ComponentModel::getBoardId() const
{
    return boardId;
}

void ComponentModel::setBoardId(const QString &value)
{
    boardId = value;
}
QString ComponentModel::getBoardPid() const
{
    return boardPid;
}

void ComponentModel::setBoardPid(const QString &value)
{
    boardPid = value;
}
