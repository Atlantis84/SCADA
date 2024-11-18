#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include "componentmodel.h"

#include <QList>
#include <QString>
#include <QMutex>
#include <QDateTime>

class BoardModel
{
public:
    BoardModel();
public:
    QMutex Mutex;
    int getSubLineNo();
    QString getResultCode();
    QString getResultName();
    bool getHadMisjudements();
    void clearComponentModels();
    void appendComponentModel(ComponentModel* model);
    QList<ComponentModel*> getComponentModels();
    QString getTestTimeDesc();
    QString getConfirmTimeDesc();
    QString getFrontOrRearCode();
    void copy(BoardModel* model);
private:
    QList<ComponentModel*> componentModels;
    QString testTimeDesc="";
private:
    bool enable;
    QString id;
    QString pid;
    QString no;
    QString code;
    QString type;
    QString station;
    bool isFront;
    QDateTime testTime;
    QString confirmer;
    bool hadConfirmed;
    bool hadPass;
    bool isLASubLine;
    bool testSuccessed;
    int failComponentCount;
    int misjudgementComponentCount;
    QString MesNewText;
public:
    QString getId() const;
    void setId(const QString &value);
    QString getPid() const;
    void setPid(const QString &value);
    QString getCode() const;
    void setCode(const QString &value);
    QString getType() const;
    void setType(const QString &value);
    QString getStation() const;
    void setStation(const QString &value);
    bool getIsFront() const;
    void setIsFront(bool value);
    QDateTime getTestTime() const;
    void setTestTime(const QDateTime &value);
    QDateTime getConfirmTime() const;
    void setConfirmTime(const QDateTime &value);
    QString getConfirmer() const;
    void setConfirmer(const QString &value);
    bool getHadConfirmed() const;
    void setHadConfirmed(bool value);
    bool getHadPass() const;
    void setHadPass(bool value);
    bool getIsLASubLine() const;
    void setIsLASubLine(bool value);
    bool getTestSuccessed() const;
    void setTestSuccessed(bool value);
    int getFailComponentCount() const;
    void setFailComponentCount(int value);
    int getMisjudgementComponentCount() const;
    void setMisjudgementComponentCount(int value);
    bool getEnable() const;
    void setEnable(bool value);
    QString getNo() const;
    void setNo(const QString &value);

    QString GetMesNewText()const;
    void SetMesNewText(const QString &value);
};

#endif // BOARDMODEL_H
