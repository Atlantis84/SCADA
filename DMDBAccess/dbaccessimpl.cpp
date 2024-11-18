#include "dbaccessimpl.h"
#include <QSqlError>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "imessagebox.h"
DBAccessImpl::DBAccessImpl()
{
    m_pConnectionName = "";
}

bool DBAccessImpl::ConnectDataBase()
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE,"Common_Using");
    m_pDatabase.setDatabaseName(DATABASENAME);
    m_pDatabase.setPort(PORTNUM);
    m_pDatabase.setHostName(HOSTNAME);
    m_pDatabase.setUserName(USERNAME);
    m_pDatabase.setPassword(PASSWORD);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::ConnectDataBase(const QString addr, const QString dbname, const QString usrname, const QString password, const int port)
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE,"Data_Acquisition");
    m_pDatabase.setDatabaseName(dbname);
    m_pDatabase.setPort(port);
    m_pDatabase.setHostName(addr);
    m_pDatabase.setUserName(usrname);
    m_pDatabase.setPassword(password);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::ConnectDataBase(const QString addr, const QString dbname, const QString usrname, const QString password, const int port, const QString connectionName)
{
    m_pConnectionName = connectionName;
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE,m_pConnectionName);
    m_pDatabase.setDatabaseName(dbname);
    m_pDatabase.setPort(port);
    m_pDatabase.setHostName(addr);
    m_pDatabase.setUserName(usrname);
    m_pDatabase.setPassword(password);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::ConnectDataBaseMySQL()
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE_MYSQL);
    m_pDatabase.setDatabaseName(DATABASENAME_MYSQL);
    m_pDatabase.setPort(PORTNUM_MYSQL);
    m_pDatabase.setHostName(HOSTNAME_MYSQL);
    m_pDatabase.setUserName(USERNAME_MYSQL);
    m_pDatabase.setPassword(PASSWORD_MYSQL);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::PingReadDB()
{
    return 0;
}

QSqlDatabase &DBAccessImpl::GetReadDataBase()
{
    QSqlDatabase ss;
    return ss;
}

//bool DBAccessImpl::GetQueryModel(const QString &strSql, QSqlQueryModel &model)
//{
//    model.setQuery(strSql, m_pDatabase);
//    if (model.lastError().isValid())
//    {
//        QMessageBox* msgBox = new QMessageBox();
//        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
//        msgBox->critical(NULL, "ERROR", QString("GetQueryModel(const QString &strSql, QSqlQueryModel& model) failed. "
//                                                "query sql:%2. error info from database:%1")
//                        .arg(model.lastError().text())
//                        .arg(strSql));
//        return false;
//    }
//    return true;
//}

//bool DBAccessImpl::GetQueryModel(const QString &strSql, QSqlQueryModel *model)
//{
//    model->setQuery(strSql, m_pDatabase);
//    if (model->lastError().isValid())
//    {
//        QMessageBox* msgBox = new QMessageBox();
//        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
//        msgBox->critical(NULL, "ERROR", QString("GetQueryModel(const QString &strSql, QSqlQueryModel* model) failed. "
//                                                "query sql:%2. error info from database:%1")
//                        .arg(model->lastError().text())
//                        .arg(strSql));
//        return false;
//    }
//    return true;
//}

bool DBAccessImpl::GetQueryResult(const QString &strSql, QSqlQuery &sqlQuery)
{
    sqlQuery = QSqlQuery(QSqlDatabase::database("Common_Using"));
    if(!sqlQuery.exec(strSql))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                "query sql:%2. error info from database:%1")
                                 .arg(sqlQuery.lastError().text()).arg(strSql));
        return false;
    }
    return true;
}

bool DBAccessImpl::GetQueryResultDataAcquisition(const QString &strSql, QSqlQuery &sqlQuery)
{
    sqlQuery = QSqlQuery(QSqlDatabase::database("Data_Acquisition"));
    if(!sqlQuery.exec(strSql))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                "query sql:%2. error info from database:%1")
                                 .arg(sqlQuery.lastError().text()).arg(strSql));
        return false;
    }
    return true;
}

bool DBAccessImpl::GetQueryResultWithConnectionName(const QString &strSql, QSqlQuery &sqlQuery)
{
    sqlQuery = QSqlQuery(QSqlDatabase::database(m_pConnectionName));
    if(!sqlQuery.exec(strSql))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                "query sql:%2. error info from database:%1")
                                 .arg(sqlQuery.lastError().text()).arg(strSql));
        return false;
    }
    return true;
}

bool DBAccessImpl::ExecSql(QString &strsql, QString &strErrMsg)
{
    FormatSql(strsql);

    QSqlQuery sqlQuery = QSqlQuery(QSqlDatabase::database("Common_Using"));

    if(!sqlQuery.exec(strsql))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                "query sql:%2. error info from database:%1")
                                 .arg(sqlQuery.lastError().text()).arg(strsql));
        return false;
    }
    return true;

}

bool DBAccessImpl::ExecSqlDataAcquisition(QString &strsql, QString &strErrMsg)
{
    FormatSql(strsql);

    QSqlQuery sqlQuery = QSqlQuery(QSqlDatabase::database("Data_Acquisition"));

    if(!sqlQuery.exec(strsql))
    {
//        QMessageBox* msgBox = new QMessageBox();
//        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
//        msgBox->critical(NULL, "ERROR", QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
//                                                "query sql:%2. error info from database:%1")
//                        .arg(sqlQuery.lastError().text())
//                         .arg(strsql));
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                "query sql:%2. error info from database:%1")
                                 .arg(sqlQuery.lastError().text()).arg(strsql));
        return false;
    }
    return true;
}

bool DBAccessImpl::ExecSqlWithConnectionName(QString &strsql, QString &strErrMsg)
{
    qDebug()<<"the connection name is:"<<m_pConnectionName;
    FormatSql(strsql);

    QSqlQuery sqlQuery = QSqlQuery(QSqlDatabase::database(m_pConnectionName));

    if(!sqlQuery.exec(strsql))
    {
//        QMessageBox* msgBox = new QMessageBox();
//        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
//        msgBox->critical(NULL, "ERROR", QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
//                                                "query sql:%2. error info from database:%1")
//                        .arg(sqlQuery.lastError().text())
//                         .arg(strsql));
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
                                "query sql:%2. error info from database:%1")
                                 .arg(sqlQuery.lastError().text()).arg(strsql));
        return false;
    }
    return true;
}

void DBAccessImpl::FormatSql(QString& strSql)
{
    strSql.trimmed();
    if(strSql[strSql.size()-1] != QChar(';'))
    {
        strSql.append(QString(";"));
    }
}
