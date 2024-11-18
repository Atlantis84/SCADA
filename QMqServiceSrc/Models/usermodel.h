#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>

namespace Model {
class UserModel;
}
class UserModel
{
public:
    UserModel();
private:
    QString account;
    QString name;
    QString password;
    bool admin;
    bool enable;
public:
    QString getAccount();
    void setAccount(QString account);
    QString getName();
    void setName(QString name);
    QString getPassword();
    void setPassword(QString password);
    bool isAdmin();
    void setAdmin(bool isAdmin);
    bool isEnable();
    void setEnable(bool enable);
};

#endif // USERMODEL_H
