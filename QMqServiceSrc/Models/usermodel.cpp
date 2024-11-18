#include "usermodel.h"

#include <QString>

UserModel::UserModel()
{

}
QString UserModel::getAccount() {
    return account;
}
void UserModel::setAccount(QString account) {
    this->account = account;
}
QString UserModel::getName() {
    return name;
}
void UserModel::setName(QString name) {
    this->name = name;
}
QString UserModel::getPassword() {
    return password;
}
void UserModel::setPassword(QString password) {
    this->password = password;
}
bool UserModel::isAdmin() {
    return admin;
}
void UserModel::setAdmin(bool isAdmin) {
    this->admin = isAdmin;
}
bool UserModel::isEnable() {
    return enable;
}
void UserModel::setEnable(bool enable) {
    this->enable = enable;
}
