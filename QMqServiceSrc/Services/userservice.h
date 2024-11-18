#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "usermodel.h"

#include <QList>

namespace Service {
class UserService;
}

class UserService
{
public:
    static QString JSON_FILE_PATH;
    static UserService* getInstance();
private:
    UserService();
    static UserService* instance;
public:
    void loadFromFile();
    void saveToFile();
    QList<UserModel*> getUserModels();
    void newUserModel(UserModel* model);
    void delUserModel(QString account);
    UserModel* getUserModel(QString account);
    UserModel* login(QString account,QString password);
private:
    QList<UserModel*> models;
};

#endif // USERSERVICE_H
