#ifndef BOARDTYPEMODEL_H
#define BOARDTYPEMODEL_H

#include <QString>

class BoardTypeModel
{
public:
    BoardTypeModel();
private:
    QString id;
    QString name;
    QString version;
    int totalComp;
public:
    QString getId() const;
    void setId(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QString getVersion() const;
    void setVersion(const QString &value);
    int getTotalComp() const;
    void setTotalComp(int value);
};

#endif // BOARDTYPEMODEL_H
