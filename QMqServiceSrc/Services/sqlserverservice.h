#ifndef MYSQLSERVICE_H
#define MYSQLSERVICE_H

#include "boardmodel.h"
#include "componentmodel.h"
#include "errortypemodel.h"
#include "boardtypemodel.h"

#include <QString>
#include <QList>
#include <QDateTime>
#include <QSqlDatabase>

namespace Service {
class SqlserverService;
}

class SqlserverService
{
public:
    SqlserverService();
private:
    void connect();
    void disconnect();
    bool isConnected();
public:
    bool syncLastBoardModels(BoardModel* boardModel,int iPid);
    bool syncComponentModels(BoardModel* boardModel);
    int getIPid();
    bool isDBNameExist(QString dbName);
private:
    QSqlDatabase database;
    QString _Sql_GetLastConfirmedBoardTestTime="";
    QString _Sql_GetErrorTypes="";
    QString _Sql_GetBoardTypes="";
    QString _Sql_GetBoards="";
    QString _Sql_getPid="";
    QString _Sql_GetOldBoards="";
    QString _Sql_getBaordTime="";
    QString _Sql_GetBoardComponents="";
    QString _Sql_GetDataBases="";
};

#endif // MYSQLSERVICE_H
