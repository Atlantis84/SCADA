#include "tcon_psql_data_acquisition.h"
#include "gdatafactory.h"
#include <QSqlQuery>

tcon_psql_data_acquisition::tcon_psql_data_acquisition(QObject *parent) : QObject(parent)
{

}

void tcon_psql_data_acquisition::slot_insert_tcon_psql_data_acquisition(QMap<int, QString> tconSubStationDataTable)
{

    QString strSql = QString("select max(id) from public.%1").
            arg(constTconTestDataAcquisition);

    int id = 0;
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql_tcon_data_acquisition()->GetQueryResultDataAcquisition(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }

    strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7')").
            arg(constTconTestDataAcquisition).arg(id+1).
            arg(tconSubStationDataTable[DATA_TYPE::Equipment_Warnings]).
            arg(tconSubStationDataTable[DATA_TYPE::Ok_Ng_Quantity]).
            arg(tconSubStationDataTable[DATA_TYPE::Equipment_State]).
            arg(tconSubStationDataTable[DATA_TYPE::Key_Part_Action_Times]).
            arg(QDateTime::currentDateTime().toString(fmtDateTime).append("_Sub_STATION"));
    QString strError;
    if(GDataFactory::get_pgsql_tcon_data_acquisition()->ExecSqlDataAcquisition(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!";
    }
}
