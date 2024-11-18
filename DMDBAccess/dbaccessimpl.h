#ifndef DBACCESSIMPL_H
#define DBACCESSIMPL_H

#include "dmdbaccessinterface.h"

class DBAccessImpl : public DMDBAccessInterface
{
public:
    DBAccessImpl();
    virtual bool ConnectDataBase();
    virtual bool ConnectDataBase(const QString addr,
                                       const QString dbname,
                                       const QString usrname,
                                       const QString password,
                                       const int port);
    virtual bool ConnectDataBase(const QString addr,
                                 const QString dbname,
                                 const QString usrname,
                                 const QString password,
                                 const int port,
                                 const QString connectionName);

    virtual bool ConnectDataBaseMySQL();
    virtual bool PingReadDB();
    virtual QSqlDatabase& GetReadDataBase();
//    virtual bool GetQueryModel(const QString& strSql, QSqlQueryModel& model);
//    virtual bool GetQueryModel(const QString& strSql, QSqlQueryModel* model);
    virtual bool GetQueryResult(const QString& strSql, QSqlQuery& sqlQuery);
    virtual bool GetQueryResultDataAcquisition(const QString& strSql, QSqlQuery& sqlQuery);
    virtual bool GetQueryResultWithConnectionName(const QString& strSql, QSqlQuery& sqlQuery);
    virtual bool ExecSql(QString& strsql, QString& strErrMsg);
    virtual bool ExecSqlDataAcquisition(QString& strsql, QString& strErrMsg);
    virtual bool ExecSqlWithConnectionName(QString& strsql, QString& strErrMsg);
private:
    QSqlDatabase m_pDatabase;
    QSqlDatabase m_pDatabaseDataAcquisition;
    void FormatSql(QString& strSql);
};

#endif // DBACCESSIMPL_H
