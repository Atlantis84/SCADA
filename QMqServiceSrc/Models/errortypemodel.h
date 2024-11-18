#ifndef COMPERRORTYPE_H
#define COMPERRORTYPE_H

#include <QString>
#include <QList>

class ErrorTypeModel
{
public:
    ErrorTypeModel();
private:
    QString type;
    QString name;
public:
    QString getType() const;
    void setType(const QString &value);
    QString getName() const;
    void setName(const QString &value);
public:
    static ErrorTypeModel* getModelByType(QList<ErrorTypeModel*> models,QString type);
};

#endif // COMPERRORTYPE_H
