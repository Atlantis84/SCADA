#include "userservice.h"
#include "usermodel.h"

#include <QDir>
#include <QList>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QJsonParseError>
#include <QCoreApplication>

QString UserService::JSON_FILE_PATH="";
UserService* UserService::instance=new UserService;

UserService::UserService()
{

}
UserService* UserService::getInstance()
{
    return UserService::instance;
}
void UserService::loadFromFile()
{
    if(UserService::JSON_FILE_PATH.isEmpty())
    {
        UserService::JSON_FILE_PATH=QCoreApplication::applicationDirPath()+QDir::separator()+"Users.json";
    }
    //从文件读取
    QFile file(UserService::JSON_FILE_PATH);
    if(file.exists()==false)
    {
        return;
    }
    file.open(QIODevice::ReadOnly);
    QTextStream fileStream(&file);
    QString jsonStr = fileStream.readAll();
    //解析
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonError);  // 转化为 JSON 文档
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (document.isArray()) { // JSON 文档为对象
            QJsonArray jsonArr = document.array();
            for(int i=0;i<jsonArr.size();i++)
            {
                QJsonValue jsonObjVal = jsonArr.at(i);
                if(jsonObjVal.isObject()){
                    QJsonObject jsonObj = jsonObjVal.toObject();
                    UserModel* model  = new UserModel();
                    if(jsonObj.contains("account"))
                    {
                        QJsonValue jsonVal = jsonObj.value("account");
                        if(jsonVal.isString())
                        {
                            model->setAccount(jsonVal.toString());
                        }
                    }
                    if(jsonObj.contains("name"))
                    {
                        QJsonValue jsonVal = jsonObj.value("name");
                        if(jsonVal.isString())
                        {
                            model->setName(jsonVal.toString());
                        }
                    }
                    if(jsonObj.contains("password"))
                    {
                        QJsonValue jsonVal = jsonObj.value("password");
                        if(jsonVal.isString())
                        {
                            model->setPassword(jsonVal.toString());
                        }
                    }
                    if(jsonObj.contains("admin"))
                    {
                        QJsonValue jsonVal = jsonObj.value("admin");
                        if(jsonVal.isBool())
                        {
                            model->setAdmin(jsonVal.toBool());
                        }
                    }
                    if(jsonObj.contains("enable"))
                    {
                        QJsonValue jsonVal = jsonObj.value("enable");
                        if(jsonVal.isBool())
                        {
                            model->setEnable(jsonVal.toBool());
                        }
                    }
                    //加入
                    models.append(model);
                }
            }
        }
    }
}
void UserService::saveToFile()
{
    QJsonArray jsonArr;
    for(int i=0;i<models.size();i++){
        QJsonObject jsonObj;
        jsonObj.insert("account",models[i]->getAccount());
        jsonObj.insert("name",models[i]->getName());
        jsonObj.insert("password",models[i]->getPassword());
        jsonObj.insert("admin",models[i]->isAdmin());
        jsonObj.insert("enable",models[i]->isEnable());
        jsonArr.append(jsonObj);
        qDebug(models[i]->getName().toLocal8Bit());
    }
    //保存到文件
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArr);
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
    QFile file(UserService::JSON_FILE_PATH);
    file.open(QIODevice::WriteOnly);
    QTextStream fileStream(&file);
    fileStream << QString::fromUtf8(byteArray);
    file.flush();
    file.close();
}
QList<UserModel*> UserService::getUserModels()
{
    return models;
}
void UserService::newUserModel(UserModel* model)
{
    models<<model;
    //保存到文件
    saveToFile();
}
void UserService::delUserModel(QString account)
{
    for(int i=0;i<models.size();i++)
    {
        if(QString::compare(account,models.at(i)->getAccount()))
        {
            models.removeAt(i);
            //保存到文件
            saveToFile();
            return;
        }
    }
}
UserModel* UserService::getUserModel(QString account)
{
    for(int i=0;i<models.size();i++)
    {
        if(QString::compare(account,models.at(i)->getAccount()))
        {
            return models.at(i);
        }
    }
    return 0;
}
UserModel* UserService::login(QString account,QString password)
{
    for(int i=0;i<models.size();i++)
    {
        if(QString::compare(account,models.at(i)->getAccount())==0)
        {
            if(QString::compare(password,models.at(i)->getPassword())==0)
            {
                return models.at(i);
            }
        }
    }
    return 0;
}
