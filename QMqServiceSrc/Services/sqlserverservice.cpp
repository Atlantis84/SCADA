#include "sqlserverservice.h"
#include "iniservice.h"
#include "boardmodel.h"
#include "componentmodel.h"
#include "errortypemodel.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QString>
#include <QList>
#include <QSqlError>

SqlserverService::SqlserverService()
{
    database = QSqlDatabase::addDatabase("QODBC");
    //Sql初始化：获取最新的PCB板
    _Sql_GetBoards += "SELECT  TOP 1 ";
    _Sql_GetBoards += " c.cModel,";
    _Sql_GetBoards += " c.StationId,";
    _Sql_GetBoards += " a.Pid,";
    _Sql_GetBoards += " a.Bid,";
    _Sql_GetBoards += " a.BoardSN,";
    _Sql_GetBoards += " a.imulti,";
    _Sql_GetBoards += " a.TopBtm,";
    _Sql_GetBoards += " b.DualLane,";
    _Sql_GetBoards += " a.ModifierDate,";
    _Sql_GetBoards += " a.Modifier,";
    _Sql_GetBoards += " a.Status,";
    _Sql_GetBoards += " a.ConfirmFlag,";
    _Sql_GetBoards += " a.RepairComp ";
    _Sql_GetBoards += "FROM %1.dbo.Board a ";
    _Sql_GetBoards += "left join  %1.dbo.panel b on a.Pid = b.Pid ";
    _Sql_GetBoards += "left join  TRI_GLOBAL.dbo.Model c on b.Mid = c.Mid ";
    _Sql_GetBoards += " where  (a.Pid= %2)  ";
    _Sql_GetBoards += "  and  (a.ConfirmFlag ='Y'   OR   (SELECT COUNT(*) from %1.dbo.Board d where a.Pid=d.Pid and d.ConfirmFlag ='N' and d.Status='F' ) =0) ";
    _Sql_GetBoards += "  ORDER BY a.Pid desc,a.RepairComp DESC,LEN(a.BoardSN) DESC,a.Bid ASC";

    _Sql_getPid+= "SELECT TOP 1 a.Pid  FROM %1.dbo.Board a WHERE (a.ConfirmFlag ='Y'   OR   (SELECT COUNT(*) from %1.dbo.Board d where a.Pid=d.Pid and d.ConfirmFlag ='N' and d.Status='F' ) =0) ORDER BY a.Pid desc";

    //Sql初始化：获取错误类型
    _Sql_GetErrorTypes += "SELECT ";
    _Sql_GetErrorTypes += " errType, ";
    _Sql_GetErrorTypes += " errName ";
    _Sql_GetErrorTypes += "FROM Error";
    //Sql初始化：获取异常元件

    _Sql_GetBoardComponents += "SELECT ";
    _Sql_GetBoardComponents += " a.Pid,";
    _Sql_GetBoardComponents += " b.BoardSN,";
    _Sql_GetBoardComponents += " a.Bid,";
    _Sql_GetBoardComponents += " a.CompName, ";
    //_Sql_GetBoardComponents += " a.WinDateTime, ";
    _Sql_GetBoardComponents += " c.errName, ";
    _Sql_GetBoardComponents += " MAX(a.errType) as errType, ";
    _Sql_GetBoardComponents += " a.realErr, ";
    _Sql_GetBoardComponents += " a.Cfirmflag, ";
    _Sql_GetBoardComponents += " MAX(a.iLeft) as iLeft, ";
    _Sql_GetBoardComponents += " MAX(a.iTop) as iTop, ";
    _Sql_GetBoardComponents += " MAX(a.iRight) as iRight, ";
    _Sql_GetBoardComponents += " MAX(a.iBottom) as iBottom ";
    _Sql_GetBoardComponents += "  FROM %1.dbo.FovWin a  ";
    _Sql_GetBoardComponents += "  LEFT JOIN %1.dbo.Board b on a.Bid=b.Bid and a.Pid=b.Pid  ";
    _Sql_GetBoardComponents += "  LEFT JOIN  TRI_GLOBAL.dbo.Error c on a.realErr=c.errType   ";
    _Sql_GetBoardComponents += "   WHERE a.Pid=%2 ";
    _Sql_GetBoardComponents += "   GROUP BY a.Pid,b.BoardSN,a.Bid,a.CompName,a.realErr,c.errName,a.Cfirmflag ORDER BY a.Bid , a.CompName DESC;";
    //Sql初始化：判断数据库是否存在
    _Sql_GetDataBases += "SELECT ";
    _Sql_GetDataBases += " * ";
    _Sql_GetDataBases += "FROM ";
    _Sql_GetDataBases += "master.dbo.sysdatabases ";
    _Sql_GetDataBases += "WHERE ";
    _Sql_GetDataBases += "name='%1'";
}

bool SqlserverService::isDBNameExist(QString dbName)
{
    //重连
    if(isConnected()==false)
    {
        connect();
    }
    //配置更新
    if(IniService::Collect_Sqlserver_SettingUpdated)
    {
        disconnect();
        connect();
        IniService::Collect_Sqlserver_SettingUpdated=false;
    }
    //读取数据
    if(isConnected())
    {
        QSqlQuery result = database.exec(_Sql_GetDataBases.arg(dbName));
        //断开重连
        QSqlError error = result.lastError();
        if(error.type()!=QSqlError::ErrorType::NoError)
        {
            disconnect();
            qWarning("%s",qUtf8Printable("SQL执行异常："+error.text()));
            return false;
        }
        //数据封装
        if(result.next())
        {
            return true;
        }
    }
    return false;
}


bool SqlserverService:: syncLastBoardModels(BoardModel* lastBoardModel,int iPid)
{

    //重连
    if(isConnected()==false)
    {
        connect();
    }
    //配置更新
    if(IniService::Collect_Sqlserver_SettingUpdated)
    {
        disconnect();
        connect();
        IniService::Collect_Sqlserver_SettingUpdated=false;
    }
    //读取数据
    if(isConnected())
    {
        //qDebug("%s",qUtf8Printable(_Sql_GetBoards));
        qDebug("%d",iPid);
        QSqlQuery result = database.exec(_Sql_GetBoards.arg(IniService::Collect_Sqlserver_RecordDBName,QString::number(iPid)));
        //断开重连

        QSqlError error = result.lastError();
        if(error.type()!=QSqlError::ErrorType::NoError)
        {
            disconnect();
            qWarning("%s",qUtf8Printable("SQL执行异常："+error.text()));
            return false;
        }

        //读取
        if(result.next())
        {
            //解析
            QString cModel=result.value("cModel").isNull()?"":result.value("cModel").toString();
            QString StationId=result.value("StationId").isNull()?"":result.value("StationId").toString();
            int Pid=result.value("Pid").isNull()?0:result.value("Pid").toInt();
            int Bid=result.value("Bid").isNull()?0:result.value("Bid").toInt();
            QString BoardSN=result.value("BoardSN").isNull()?"":result.value("BoardSN").toString();
            QString imulti=result.value("imulti").isNull()?"":result.value("imulti").toString();
            QString TopBtm=result.value("TopBtm").isNull()?"":result.value("TopBtm").toString();
            int DualLane=result.value("DualLane").isNull()?0:result.value("DualLane").toInt();
            QDateTime ModifierDate=result.value("ModifierDate").isNull()?QDateTime::currentDateTime():result.value("ModifierDate").toDateTime();
            QString Modifier=result.value("Modifier").isNull()?"":result.value("Modifier").toString();
            //QString Status=result.value("Status").isNull()?"":result.value("Status").toString();
            int FailComp=result.value("RepairComp").isNull()?0:result.value("RepairComp").toInt();
            QString OnLineCheck=result.value("ConfirmFlag").isNull()?"":result.value("ConfirmFlag").toString();

                lastBoardModel->setEnable(true);
                lastBoardModel->setPid(QString::number(Pid));
                lastBoardModel->setId(QString::number(Bid));
                lastBoardModel->setNo(imulti);
                lastBoardModel->setCode(BoardSN);
                lastBoardModel->setType(cModel);
                lastBoardModel->setStation(StationId);
                lastBoardModel->setTestTime(ModifierDate);
                lastBoardModel->setConfirmer(Modifier);
                lastBoardModel->setIsFront(TopBtm.trimmed()=="T");
                lastBoardModel->setIsLASubLine(DualLane == 1);
                lastBoardModel->setFailComponentCount(FailComp);
                lastBoardModel->setTestSuccessed(FailComp==0);
                lastBoardModel->setHadConfirmed(OnLineCheck.trimmed()=="Y");
        }else{
           return false;
        }
        result.clear();
        return true;
    }
    return false;
}
int SqlserverService::getIPid()
{
    if(isConnected()==false)
    {
        connect();
    }
    //读取数据
    if(isConnected())
    {
        QSqlQuery result = database.exec(_Sql_getPid.arg(IniService::Collect_Sqlserver_RecordDBName));
        //读取
        if(result.next())
        {
            if(result.value("Pid").isNull()==false)
                       {
                           return result.value("Pid").toInt();
                       }
        }
    }
    return -1;
}

bool SqlserverService::syncComponentModels(BoardModel* boardModel)
{
    if(boardModel==0)
    {
        return false;
    }
    //清空
    boardModel->clearComponentModels();
    if(isConnected()==false)
    {
        connect();
    }
    //读取数据
    if(isConnected())
    {
        QSqlQuery result = database.exec(_Sql_GetBoardComponents.arg(IniService::Collect_Sqlserver_RecordDBName,//
                                                                     boardModel->getPid()));      
        int compMisjudgementCount=0;
        int compFailCount=0;
        bool testSuccessed=true;
        //加锁
        boardModel->Mutex.lock();
        //读取
        while (result.next()) {
            //解析
            int Pid=result.value("Pid").isNull()?0:result.value("Pid").toInt();
            int Bid=result.value("Bid").isNull()?0:result.value("Bid").toInt();
            QString BoardSN=result.value("BoardSN").isNull()?"":result.value("BoardSN").toString();
            QString errType=result.value("errType").isNull()?"":result.value("errType").toString();
            QString CompName=result.value("CompName").isNull()?"":result.value("CompName").toString();
            int iWinx=result.value("iLeft").isNull()?0:result.value("iLeft").toInt();
            int iWiny=result.value("iTop").isNull()?0:result.value("iTop").toInt();
            int iWinx1=result.value("iRight").isNull()?0:result.value("iRight").toInt();
            int iWiny1=result.value("iBottom").isNull()?0:result.value("iBottom").toInt();
           // QDateTime WinDateTime=result.value("WinDateTime").isNull()?QDateTime::currentDateTime():result.value("WinDateTime").toDateTime();
            QString errName=result.value("errName").isNull()?"":result.value("errName").toString();
            //QString Modifier=result.value("Modifier").isNull()?"":result.value("Modifier").toString();
            QString realErr=result.value("realErr").isNull()?"":result.value("realErr").toString();

            //过滤：误判
            if(realErr!=errType)
            {
                compMisjudgementCount++;
            }
            //过滤：良品
            if(realErr.isEmpty())
            {
                continue;
            }
            compFailCount++;
            testSuccessed=false;
            //封装
            ComponentModel* model=new ComponentModel();
            model->setBoardPid(QString::number(Pid));
            model->setBoardId(QString::number(Bid));
            model->setBoardNo(boardModel->getNo());
            model->setBoardCode(BoardSN);
            model->setBoardType("");
            model->setName(CompName);
            model->setPositionX1(iWinx);
            model->setPositionY1(iWiny);
            model->setPositionX2(iWinx1);
            model->setPositionY2(iWiny1);
           // model->setWinTime(WinDateTime);
            model->seterrName(errName);
            model->setErrorType(realErr);
            model->setTestSuccess(realErr.isEmpty());
            model->setIsMisjudgement(realErr!=errType);
            model->setHadConfirmed(true);
            qDebug("%s",qUtf8Printable("X"));
            //
            if(realErr.isEmpty()==false)
            {
                model->setErrorType(realErr);
            }
            boardModel->appendComponentModel(model);
        }
        //
        boardModel->setMisjudgementComponentCount(compMisjudgementCount);
        boardModel->setTestSuccessed(testSuccessed);
        boardModel->setFailComponentCount(compFailCount);
        //解锁
        boardModel->Mutex.unlock();
        //

        result.clear();
        return true;
    }
    return false;
}
void SqlserverService::connect()
{
    if(isConnected())
    {
        database.close();
    }
    //本地模式
    if(IniService::Collect_Sqlserver_EnableLocalDSN)
    {
        database.setDatabaseName(IniService::Collect_Sqlserver_LocalDSNName);
    }
    //远程模式
    else
    {
        QString dsn("DRIVER={SQL SERVER};SERVER=");
        dsn.append(IniService::Collect_Sqlserver_Host);
        // 注意,对于express版本数据库, 一定要加\\sqlexpress这种后缀
        if(IniService::Collect_Sqlserver_IsExpressVersion)
        {
            dsn.append("\\sqlexpress");
        }
        dsn.append(";PORT=");
        dsn.append(QString::number(IniService::Collect_Sqlserver_Port));
        dsn.append(";DATABASE=");
        dsn.append(IniService::Collect_Sqlserver_DBName);
        database.setDatabaseName(dsn);
        database.setUserName(IniService::Collect_Sqlserver_UName);
        database.setPassword(IniService::Collect_Sqlserver_UPwd);
    }
    database.open();
    if(database.isOpenError())
    {
        qWarning("%s",qUtf8Printable("数据库打开异常："+database.lastError().text()));
    }
}
void SqlserverService::disconnect()
{
    if(database.isOpen()&&database.isValid())
    {
        database.close();
    }
}
bool SqlserverService::isConnected()
{
    return database.isOpen()&&database.isValid()&&!database.isOpenError();
}
