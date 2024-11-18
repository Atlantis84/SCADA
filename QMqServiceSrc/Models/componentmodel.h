#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include <QString>
#include <QDateTime>

class ComponentModel
{
public:
    ComponentModel();
    void copy(ComponentModel* model);
    QString getErrorTypeForEAM();
private:
    QString boardId;
    QString boardPid;
    QString boardNo;
    QString boardCode;
    QString boardType;
    QString name;
    QString errorType;
    QString errorName;
    QDateTime winTime;
    QString errorTypeName;
    bool isMisjudgement;
    bool testSuccess;
    bool hadConfirmed;
    int positionX1;
    int positionY1;
    int positionX2;
    int positionY2;
public:
    QString getBoardId() const;
    void setBoardId(const QString &value);
    QString getBoardPid() const;
    void setBoardPid(const QString &value);
    QString getBoardCode() const;
    void setBoardCode(const QString &value);
    QString getBoardType() const;
    void setBoardType(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QString getErrorType() const;
    void setErrorType(const QString &value);
    QDateTime getWinTime() const;
    void setWinTime(const QDateTime &value);
    bool getIsMisjudgement() const;
    void setIsMisjudgement(bool value);
    int getPositionX1() const;
    void setPositionX1(int value);
    int getPositionY1() const;
    void setPositionY1(int value);
    int getPositionX2() const;
    void setPositionX2(int value);
    int getPositionY2() const;
    void setPositionY2(int value);
    QString getErrorTypeName() const;
    void setErrorTypeName(const QString &value);
    QString geterrName() const;
    void seterrName(const QString &value);
    bool getTestSuccess() const;
    void setTestSuccess(bool value);
    bool getHadConfirmed() const;
    void setHadConfirmed(bool value);
    QString getBoardNo() const;
    void setBoardNo(const QString &value);
};

#endif // COMPONENTMODEL_H
