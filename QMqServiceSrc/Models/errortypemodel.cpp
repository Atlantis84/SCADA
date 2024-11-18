#include "errortypemodel.h"

ErrorTypeModel::ErrorTypeModel()
{

}

ErrorTypeModel* ErrorTypeModel::getModelByType(QList<ErrorTypeModel*> models,QString type)
{
    if(models.size()>0)
    {
        foreach (ErrorTypeModel* model, models) {
            if(model->getType()==type)
            {
                return model;
            }
        }
    }
    return 0;
}

QString ErrorTypeModel::getType() const
{
    return type;
}

void ErrorTypeModel::setType(const QString &value)
{
    type = value;
}

QString ErrorTypeModel::getName() const
{
    return name;
}

void ErrorTypeModel::setName(const QString &value)
{
    name = value;
}
