#include "boardtypemodel.h"

BoardTypeModel::BoardTypeModel()
{

}

int BoardTypeModel::getTotalComp() const
{
    return totalComp;
}

void BoardTypeModel::setTotalComp(int value)
{
    totalComp = value;
}

QString BoardTypeModel::getVersion() const
{
    return version;
}

void BoardTypeModel::setVersion(const QString &value)
{
    version = value;
}

QString BoardTypeModel::getName() const
{
    return name;
}

void BoardTypeModel::setName(const QString &value)
{
    name = value;
}

QString BoardTypeModel::getId() const
{
    return id;
}

void BoardTypeModel::setId(const QString &value)
{
    id = value;
}
