#include "ftpservice.h"
#include "iniservice.h"
#include "Models/componentmodel.h"
#include <QDir>
#include <QFile>
#include <QThread>
#include <QThreadPool>
#include <QNetworkRequest>
#include <QNetworkReply>

FtpService::FtpService()
{
    textCodec = QTextCodec::codecForName(qUtf8Printable("UTF-8"));
}

void FtpService::push(BoardModel* boardModelLA,BoardModel* boardModelLB)
{
    if(IniService::Push_Ftp_Enable==false||started==false)
    {
        return;
    }
    //LA不良
       if(boardModelLA!=0//
               &&boardModelLA->getEnable()
               )
       {
           if(this->boardModelLA==0//
                   ||this->boardModelIdLA!=boardModelLA->getId())
           {
               //忽略第一块PCB
               if(this->boardModelLA!=0)
               {
                   doneLAPushAction=false;
                   if(IniService::Push_Ftp_EnableTraceLog)
                   {
                       qDebug("%s",qUtf8Printable("Ftp新增LA待上传数据："+boardModelLA->getId()));
                   }
                   upload(boardModelLA,IniService::Push_Ftp_LA_WorkcenterID);
               }
               //
               this->boardModelLA=boardModelLA;
               this->boardModelIdLA=boardModelLA->getId();
           }
       }
       //LB不良
       if(boardModelLB!=0//
               &&boardModelLB->getEnable())
       {
           if(this->boardModelLB==0//
                   ||this->boardModelIdLB!=boardModelLB->getId())
           {
               //忽略第一块PCB
               if(this->boardModelLB!=0)
               {
                   doneLBPushAction=false;
                   if(IniService::Push_Ftp_EnableTraceLog)
                   {
                       qDebug("%s",qUtf8Printable("Ftp新增LB待上传数据："+boardModelLB->getId()));
                   }
                   upload(boardModelLB,IniService::Push_Ftp_LB_WorkcenterID);
               }
               //
               this->boardModelLB=boardModelLB;
               this->boardModelIdLB=boardModelLB->getId();
           }
       }
   }
void FtpService::upload(BoardModel* boardModel,QString workcenterId)
{
    QList<QString> filePathes;
    QList<QString> ftpFileNames;
    //加锁
    boardModel->Mutex.lock();
    //不良图片上传
    if(boardModel->getTestSuccessed()==true)
    {
        //元件根路径
        QString compFileDirPath=IniService::Push_Ftp_LocalCompDirPath;
        if(IniService::Push_Ftp_LocalCompDirPath.endsWith("\\")==false)
        {
            compFileDirPath+="\\";
        }
        compFileDirPath+=boardModel->getType();
        compFileDirPath+="\\";
        compFileDirPath+=boardModel->getTestTime().toString("yyyyMMdd");
        compFileDirPath+="\\";
         qDebug("%s",qUtf8Printable(compFileDirPath));
        //元件路径
        QDir compFileBaseDir(compFileDirPath);
        if(compFileBaseDir.exists())
        {
            qDebug("%s",qUtf8Printable(compFileDirPath));
           // foreach(ComponentModel* compModel,boardModel->getComponentModels())
            //{
                //if(compModel->getTestSuccess()==true
                        //&&compModel->getName().isEmpty()==false
                 //       )
              //  {
                    QString filePath=compFileDirPath;
                    filePath+="L201_20230329155630_2_20230329155630_VD12_2_错件.jpg";
                    //filePath+=compModel->getName();
                    //filePath+="_"+compModel->getBoardNo()+"_";
                    //filePath+="_top.jpg";
                    //
                    qDebug("%s",qUtf8Printable(filePath));
                    QString ftpFileName="";
                    ftpFileName+=workcenterId;
                    ftpFileName+="_";
                    ftpFileName+=boardModel->getCode().replace("/","-").replace("\\","+");
                    ftpFileName+="_";
                    ftpFileName+=boardModel->getTestTime().toString("yyyyMMddhhmmss");
                   // ftpFileName+="_";
                   // ftpFileName+=compModel->getBoardNo();
                   // ftpFileName+="_";
                   // ftpFileName+=compModel->getName();
                   // ftpFileName+="_";
                   // ftpFileName+=compModel->getErrorTypeForEAM();
                    ftpFileName+=".jpg";
                    //
                    filePathes.append(filePath);
                    ftpFileNames.append(ftpFileName);
               // }
           // }
        }
    }
    //解锁
    boardModel->Mutex.unlock();
    //加锁
    this->uploadMutex.lock();
    //增加
    uploadFilePathes.append(filePathes);
    uploadFtpFileNames.append(ftpFileNames);
    //解锁
    this->uploadMutex.unlock();
}
void FtpService::run()
{
    qDebug("111111111111111111111111111111111111");
    stoped=false;
    QList<QString> filePathes;
    QList<QString> ftpFileNames;/*
    if(started){qDebug("true");}else{qDebug("false");}*/


    while (started) {
        //线程等待
        taskSleep(500);
        /*if(IniService::Push_Ftp_Enable)
        {
            continue;
            qDebug("%s",qUtf8Printable("T"));
        }*/
        //清空
        filePathes.clear();
        ftpFileNames.clear();
        //加锁
        uploadMutex.lock();
        //添加到缓存
        if(started)
        {
            filePathes.append(uploadFilePathes);
            ftpFileNames.append(uploadFtpFileNames);
            uploadFilePathes.clear();
            uploadFtpFileNames.clear();
        }
        //解锁
        uploadMutex.unlock();
        //上传
        for(int i=0;i<filePathes.size()&&i<ftpFileNames.size();i++)
        {
            if(started==false)
            {
                break;
            }
            qDebug("%s",qUtf8Printable("TT"));
            //
            QString filePath=filePathes[i];
            QString fileName=ftpFileNames[i];
            QFile file(filePath);
            if(file.exists())
            {
                qDebug("%s",qUtf8Printable("PP"));
                if(file.open(QFile::ReadOnly))
                {
                    //打开文件
                    if(IniService::Push_Ftp_EnableTraceLog)
                    {
                        qDebug("%s",qUtf8Printable("Ftp成功打开文件："+filePath));
                    }
                    QByteArray bytes=file.readAll();
                    if(eventLoop==0)
                    {
                        eventLoop=new QEventLoop();
                    }
                    if(uploadTimer==0)
                    {
                        uploadTimer = new QTimer();
                        connect(uploadTimer, SIGNAL(timeout()), this, SLOT(when_upload_timeout()));
                    }
                    //URL
                    QUrl url;
                    url.setScheme("ftp");
                    url.setHost(IniService::Push_Ftp_Host);
                    url.setPort(IniService::Push_Ftp_Port);
                    url.setUserName(IniService::Push_Ftp_UName);
                    url.setPassword(IniService::Push_Ftp_UPassword);
                    url.setPath("/"+IniService::Push_Ftp_Dir+"/"+fileName);
                    //构建请求
                    QNetworkAccessManager* manager=new QNetworkAccessManager();
                    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
                    connect(manager,SIGNAL(finished(QNetworkReply*)),eventLoop, SLOT(quit()));
                    QNetworkRequest request(url);
                    //同步请求
                    if(IniService::Push_Ftp_EnableTraceLog)
                    {
                        qDebug("%s",qUtf8Printable("Ftp开始上传文件到："+url.toString()));
                    }
                    //计时开始
                    uploadTimer->start(5000);
                    //开始上传
                    QNetworkReply* reply=manager->put(request,bytes);
                    eventLoop->exec();
                    //计时停止
                    if(uploadTimer->isActive())
                    {
                        //上传成功
                        if(reply->isFinished()//
                                &&reply->error()==QNetworkReply::NetworkError::NoError)
                        {
                            if(IniService::Push_Ftp_EnableTraceLog)
                            {
                                qDebug("%s",qUtf8Printable("Ftp成功上传文件："+filePath));
                            }
                        }
                        else if(IniService::Push_Ftp_EnableTraceLog)
                        {
                            qWarning("%s",qUtf8Printable("Ftp文件上传失败："+filePath));
                        }
                        uploadTimer->stop();
                    }
                    //回收
                    delete reply;
                    disconnect(manager,SIGNAL(finished(QNetworkReply*)),eventLoop, SLOT(quit()));
                    delete manager;
                }
                else
                {
                    if(IniService::Push_Ftp_EnableTraceLog)
                    {
                        qWarning("%s",qUtf8Printable("Ftp文件打开失败："+filePath));
                    }
                }
            }
            else
            {
                if(IniService::Push_Ftp_EnableTraceLog)
                {
                    qWarning("%s",qUtf8Printable("Ftp文件不存在："+filePath));
                }
            }
        }
    }
    stoped=true;
}

void FtpService::when_upload_timeout()
{
    //取消上传
    if(eventLoop!=0&&eventLoop->isRunning())
    {
        qDebug("%s",qUtf8Printable("Ftp上传超时"));
        eventLoop->exit(0);
    }
}

void FtpService::taskSleep(int sleepTimeMs)
{
    if(sleepTimeMs<1)
    {
        QThread::msleep(1);
    }
    qint64 outTimeStamp = QDateTime::currentMSecsSinceEpoch()+sleepTimeMs;
    while (outTimeStamp>QDateTime::currentMSecsSinceEpoch()&&started) {
        QThread::msleep(2);
    }
}

void FtpService::startService()
{
    if(started==false)
    {
        this->started=true;
        QThreadPool::globalInstance()->start(this);
    }
}

void FtpService::stopService()
{
    started=false;
}

bool FtpService::isStarted()
{
    return started;
}

bool FtpService::isStoped()
{
    return stoped;
}
