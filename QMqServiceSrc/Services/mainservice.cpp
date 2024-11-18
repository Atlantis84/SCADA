#include "mainservice.h"
#include "iniservice.h"
#include "sqlserverservice.h"
#include "boardmodel.h"
#include "componentmodel.h"
#include "statusmodel.h"

#include <QList>
#include <QThreadPool>
#include <QStandardPaths>

MainService::MainService()
{
    statusModel = new StatusModel();
    sqlService = new SqlserverService();
    mqService = new MQService();
    scannerService = new ScannerService();
    mesService = new MesService();
    mesService->sqlService=sqlService;
    ftpService = new FtpService();
    lastBoardModel = new BoardModel();
        statusModel = new StatusModel();
        lastBoardModelLA = new BoardModel();
        statusModelLA = new StatusModel();
        lastBoardModelLB = new BoardModel();
        statusModelLB = new StatusModel();
    //禁用QRunnable自动删除
    this->setAutoDelete(false);
}
void MainService::setLoginUserModel(UserModel* userModel)
{
    loginUserModel=userModel;
    mqService->setLoginUserModel(userModel);
}
void MainService::run()
{
    stoped=false;
    bool hadNewBoard=false;
    bool isFaulting=false;
    bool doNextPush=false;
    //获取最新记录时间
    QDateTime nextWaitTime=QDateTime::currentDateTime();
    QString lastBaordId="";
    int lastPId=-1;
    QString laScannerCode="";
    QString lbScannerCode="";
    QString dbName="AOI_"+QDateTime::currentDateTime().toString("yyyyMM01");
    //
    while (started) {
        //线程等待
        taskSleep(IniService::Collect_Sqlserver_SyncIntervalMS);
        //生产记录数据源
        dbName="AOI_"+QDateTime::currentDateTime().toString("yyyyMM01");
        if(dbName!=IniService::Collect_Sqlserver_RecordDBName)
        {
            //判断数据库存在则进行切换
            qDebug("%s",qUtf8Printable("查询数据库是否存在："+dbName));
            if(sqlService->isDBNameExist(dbName))
            {
                IniService::Collect_Sqlserver_RecordDBName=dbName;
                IniService::saveToFile();
                qDebug("%s",qUtf8Printable("切换数据库："+IniService::Collect_Sqlserver_RecordDBName));
            }
            else
            {
                qDebug("%s",qUtf8Printable("数据库不存在："+IniService::Collect_Sqlserver_RecordDBName));
            }
        }
        if(lastPId==-1){
                 lastPId=sqlService->getIPid();
               }
        //获取最新记录
        if(sqlService->syncLastBoardModels(lastBoardModel,lastPId))//获取最新id
                {
            //初次启动
             qDebug("%d",lastPId);
                            if(lastBaordId.isEmpty())
                            {
                                //更新点位
                                if(sqlService->syncComponentModels(lastBoardModel)==false)
                                {
                                    continue;
                                }
                                lastBaordId=lastBoardModel->getPid();
                                lastPId=lastPId+1;
                            }
                            //新PCB生产记录
                            hadNewBoard=lastBaordId!=lastBoardModel->getPid();
                            if(hadNewBoard)
                            {
                                lastBaordId=lastBoardModel->getPid();
                                lastPId=lastPId+1;
                            }

            //条码枪
            if(hadNewBoard)
            {
                if(IniService::Scanner_LA_Enable&&lastBoardModel->getIsLASubLine())
                {
                    laScannerCode=scannerService->getLastLACode();
                }
                if(IniService::Scanner_LB_Enable&&lastBoardModel->getIsLASubLine()==false)
                {
                    lbScannerCode=scannerService->getLastLBCode();
                }
            }
            //更新点位
            if(hadNewBoard)
            {
                        if(sqlService->syncComponentModels(lastBoardModel)==false)
                        {
                            continue;
                        }

            }
            //空闲
            if(hadNewBoard==false//
                    &&QDateTime::currentDateTime()>=nextWaitTime//
                    &&isFaulting==false)
            {
                //状态
                statusModel->DateTime=QDateTime::currentDateTime();
                statusModel->StatusValue=StatusModel::STATUS_VALUE_Wait;
                statusModel->StatusCode=StatusModel::STATUS_CODE_Wait;
                statusModel->StatusName=StatusModel::STATUS_NAME_Wait;
                statusModel->FaultCode="";
                statusModel->FaultName="";
                statusModel->FaultDesc="";
                //
                doNextPush=true;
            }
            else
            {

                //测试合格
                if(lastBoardModel->getTestSuccessed())
                {
                    //设备运行
                    statusModel->DateTime=QDateTime::currentDateTime();
                    statusModel->StatusValue=StatusModel::STATUS_VALUE_Run;
                    statusModel->StatusCode=StatusModel::STATUS_CODE_Run;
                    statusModel->StatusName=StatusModel::STATUS_NAME_Run;
                    statusModel->FaultCode="";
                    statusModel->FaultName="";
                    statusModel->FaultDesc="";
                    //
                    doNextPush=true;
                    isFaulting=false;
                    //
                    if(hadNewBoard)
                    {
                        nextWaitTime=QDateTime::currentDateTime().addMSecs(IniService::Collect_Stauts_RunTimeoutMS);
                    }
                }
                //测试不合格
                else
                {
                    //人工已确认
                    if(lastBoardModel->getHadConfirmed())
                    {
                        //设备运行
                        statusModel->DateTime=QDateTime::currentDateTime();
                        statusModel->StatusValue=StatusModel::STATUS_VALUE_Run;
                        statusModel->StatusCode=StatusModel::STATUS_CODE_Run;
                        statusModel->StatusName=StatusModel::STATUS_NAME_Run;
                        statusModel->FaultCode="";
                        statusModel->FaultName="";
                        statusModel->FaultDesc="";
                        //
                        doNextPush=true;
                        isFaulting=false;
                        //
                        if(hadNewBoard)
                        {
                            nextWaitTime=QDateTime::currentDateTime().addMSecs(IniService::Collect_Stauts_RunTimeoutMS);
                        }
                    }
                    //等待人工处理
                    else
                    {
                        statusModel->DateTime=QDateTime::currentDateTime();
                        statusModel->StatusValue=StatusModel::STATUS_VALUE_Fault;
                        statusModel->StatusCode=StatusModel::STATUS_CODE_Fault;
                        statusModel->StatusName=StatusModel::STATUS_NAME_Fault;
                        statusModel->FaultCode=IniService::Collect_WarnCode_WaitConfirm;
                        statusModel->FaultName=IniService::Collect_WarnName_WaitConfirm;
                        statusModel->FaultDesc=IniService::Collect_WarnDesc_WaitConfirm;
                        //
                        doNextPush=true;
                        isFaulting=true;
                    }
                }
            }
        }
        //推送
        if(doNextPush)
        {
            //拆分复制
            if(lastBoardModel->getIsLASubLine())
            {
                lastBoardModelLA->setEnable(true);
                //加锁
                lastBoardModelLA->Mutex.lock();
                //复制
                lastBoardModelLA->copy(lastBoardModel);
                statusModelLA->copy(statusModel);
                //LB空闲
                statusModelLB->setWait();
                //清空
                lastBoardModelLA->clearComponentModels();
                //复制
                foreach(ComponentModel* model,lastBoardModel->getComponentModels())
                {
                    ComponentModel* newModel=new ComponentModel();
                    newModel->copy(model);
                    lastBoardModelLA->appendComponentModel(newModel);
                }
                //解锁
                lastBoardModelLA->Mutex.unlock();
                //重置计数LA
                if(lastBoardModelLA->getType()!=IniService::Total_LA_ProductType)
                {
                    IniService::Total_LA_ProductID="";
                    IniService::Total_LA_ProductType=lastBoardModelLA->getType();
                    IniService::Total_LA_TotalCount=0;
                    IniService::Total_LA_YieldCount=0;
                    IniService::Total_LA_DefectCount=0;
                    IniService::Total_LA_MisjudgementCount=0;
                    //保存
                    IniService::saveTotalToFile();
                }
                //计数LA
                if(lastBoardModelLA->getId()!=IniService::Total_LA_ProductID)
                {
                    IniService::Total_LA_ProductID=lastBoardModelLA->getId();
                    IniService::Total_LA_TotalCount++;
                    if(lastBoardModelLA->getTestSuccessed())
                    {
                        IniService::Total_LA_YieldCount++;
                    }
                    else
                    {
                        IniService::Total_LA_DefectCount++;
                    }
                    if(lastBoardModelLA->getHadMisjudements())
                    {
                        IniService::Total_LA_MisjudgementCount++;
                    }
                    //保存
                    IniService::saveTotalToFile();
                }
            }
            else
            {
                //初始化
                lastBoardModelLB->setEnable(true);
                //加锁
                lastBoardModelLB->Mutex.lock();
                //复制
                lastBoardModelLB->copy(lastBoardModel);
                statusModelLB->copy(statusModel);
                //LA空闲
                statusModelLA->setWait();
                //清空
                lastBoardModelLB->clearComponentModels();
                //复制
                foreach(ComponentModel* model,lastBoardModel->getComponentModels())
                {
                    ComponentModel* newModel=new ComponentModel();
                    newModel->copy(model);
                    lastBoardModelLB->appendComponentModel(newModel);
                }
                //解锁
                lastBoardModelLB->Mutex.unlock();
                //重置计数LB
                if(lastBoardModelLB->getType()!=IniService::Total_LB_ProductType)
                {
                    IniService::Total_LB_ProductID="";
                    IniService::Total_LB_ProductType=lastBoardModelLB->getType();
                    IniService::Total_LB_TotalCount=0;
                    IniService::Total_LB_YieldCount=0;
                    IniService::Total_LB_DefectCount=0;
                    IniService::Total_LB_MisjudgementCount=0;
                    //保存
                    IniService::saveTotalToFile();
                }
                //计数LB
                if(lastBoardModelLB->getId()!=IniService::Total_LB_ProductID)
                {
                    IniService::Total_LB_ProductID=lastBoardModelLB->getId();
                    IniService::Total_LB_TotalCount++;
                    if(lastBoardModelLB->getTestSuccessed())
                    {
                        IniService::Total_LB_YieldCount++;
                    }
                    else
                    {
                        IniService::Total_LB_DefectCount++;
                    }
                    if(lastBoardModelLB->getHadMisjudements())
                    {
                        IniService::Total_LB_MisjudgementCount++;
                    }
                    //保存
                    IniService::saveTotalToFile();
                }
            }
            //显示
            if(lastBoardModel->getIsLASubLine())
            {
                emit showUiData(lastBoardModelLA,statusModelLA);

            }
            else
            {
                emit showUiData(lastBoardModelLB,statusModelLB);

            }
            //推送
            mqService->push(lastBoardModelLA,//
                            statusModelLA,//
                            lastBoardModelLB,//
                            statusModelLB);
            //ftpService->push(lastBoardModelLA,//
                            // lastBoardModelLB);
            mesService->push(lastBoardModelLA,//
                             lastBoardModelLB);

        }
    }
    stoped=true;
}
void MainService::taskSleep(int sleepTimeMs)
{
    if(sleepTimeMs<1)
    {
        QThread::msleep(1);
    }
    qint64 outTimeStamp = QDateTime::currentMSecsSinceEpoch()+sleepTimeMs;
    while (outTimeStamp>QDateTime::currentMSecsSinceEpoch()&&started)
    {
        QThread::msleep(2);
    }
}
void MainService::startService()
{
    if(started==false)
    {
        started=true;
        QThreadPool::globalInstance()->start(this);
        mqService->startService();
        //ftpService->startService();
        mesService->startService();        
        scannerService->startService();
    }
}
void MainService::stopService()
{
    if(started)
    {
        mqService->stopService();
        //ftpService->stopService();
        mesService->stopService();        
        scannerService->stopService();
    }
    started=false;
}
bool MainService::isStarted()
{
    return started;
}
bool MainService::isStoped()
{
    return started;
}
