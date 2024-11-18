#include "messervice.h"
#include "iniservice.h"

#include <QDir>
#include <QThread>
#include <QThreadPool>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

MesService::MesService()
{
    textCodec = QTextCodec::codecForName(qUtf8Printable("UTF-8"));
}

void MesService::playWarnMedia(int volume)
{
    //音频文件目录
    QString mediaFilePath;
    mediaFilePath.append(QCoreApplication::applicationDirPath());
    mediaFilePath.append(QDir::separator());
    mediaFilePath.append("NG.wav");
    mediaFilePath=mediaFilePath.replace(QChar('/'),QChar('\\'),Qt::CaseInsensitive);
    //文件存在
    QFile mediaFile(mediaFilePath);
    if(mediaFile.exists())
    {
        //播放音乐
        if(mediaPlayer==0)
        {
            mediaPlayer=new QMediaPlayer();
            connect(mediaPlayer,//
                    SIGNAL(stateChanged(QMediaPlayer::State)),//
                    this,//
                    SLOT(playerStateChanged(QMediaPlayer::State)));
        }
        mediaPlayer->setMedia(QUrl::fromLocalFile(mediaFilePath));
        mediaPlayer->setVolume(volume);
        mediaPlayer->play();
    }
}

void MesService::playerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::State::StoppedState)
    {
        if(IniService::Push_Mes_EnableTraceLog)
        {
            qDebug("MES提示音播放停止");
        }
    }
    else if(state==QMediaPlayer::State::PlayingState)
    {
        if(IniService::Push_Mes_EnableTraceLog)
        {
            qDebug("MES提示音播放开始");
        }
    }
}

void MesService::push(BoardModel* boardModelLA,BoardModel* boardModelLB)
{
    if(IniService::Push_Mes_Enable==false)
    {
        return;
    }
        //LA
        if(boardModelLA!=0//
                &&boardModelLA->getEnable()//
                &&boardModelLA->getTestSuccessed()
                &&boardModelLA->getIsLASubLine()==true
                )
        {
                if(lastBoardIdLA!=boardModelLA->getId())
                {
                    doneNgPushActionLA=true;
                    lastBoardIdLA=boardModelLA->getId();
                                      QString code=boardModelLA->getCode();
                                       qDebug("%s",qUtf8Printable("code是："+code));
                                      if(code.isEmpty()==false){
                                          scannerCodeLA=code;
                                      }
                }

        }
        //LB
        if(boardModelLB!=0//
                &&boardModelLB->getEnable()//
                &&boardModelLB->getTestSuccessed()
                &&boardModelLB->getIsLASubLine()==false
                )
        {
                if(lastBoardIdLB!=boardModelLB->getId())
                {
                    doneNgPushActionLB=true;
                    lastBoardIdLB=boardModelLB->getId();
                                     QString code=boardModelLB->getCode();
                                     if(code.isEmpty()==false){
                                         scannerCodeLB=code;
                                     }
                }

        }
        //LA不良
        if(boardModelLA!=0//
                &&boardModelLA->getEnable()//
                &&boardModelLA->getIsLASubLine()==true
                &&boardModelLA->getTestSuccessed()==false)
        {
            if(lastNgBoardModelFirstIdLA!=boardModelLA->getPid())
            {
                //
                lastNgBoardModelFirstIdLA=boardModelLA->getPid();
                doneNgPushActionLA=false;
                        //
                //加锁
                boardModelLA->Mutex.lock();
                        for(int i=0;i<boardModelLA->getComponentModels().size();i++)
                        {
                            if(boardModelLA->getComponentModels().at(i)->getTestSuccess()==false)
                            {                                
                               //多拼板过站
                                if(lastNgBoardModelIdLA!=boardModelLA->getComponentModels().at(i)->getBoardId())
                                {
                                    lastNgBoardModelIdLA=boardModelLA->getComponentModels().at(i)->getBoardId();
                                //
                                jsonLANgPointsDesc=buildObjectJson(boardModelLA,boardModelLA->getComponentModels().at(i)->getBoardId(),IniService::Push_Mes_LA_UWorkcenterID,ngCodeReplaceString,true);
                                //
                                emit showNgCodeDialog("请输入A线不良品SN:",boardModelLA->getComponentModels().at(i)->getBoardCode(),true,buildNgPointsDesc(boardModelLA,boardModelLA->getComponentModels().at(i)->getBoardId()));

                                }
                            }
                        }
                        scannerNgCodeLA="";
                         waitingNgCodeLA=true;
                          boardModelLA->Mutex.unlock();
            }

        }
        //LB不良
         if(boardModelLB!=0//
                &&boardModelLB->getEnable()//
               &&boardModelLB->getIsLASubLine()==false//
                &&boardModelLB->getTestSuccessed()==false)
        {
            if(lastNgBoardModelFirstIdLB!=boardModelLB->getPid())
            {
                //
                //
                lastNgBoardModelFirstIdLB=boardModelLB->getPid();
                doneNgPushActionLB=false;
                //
                //加锁
                boardModelLB->Mutex.lock();
                for(int i=0;i<boardModelLB->getComponentModels().size();i++)
                {
                    if(boardModelLB->getComponentModels().at(i)->getTestSuccess()==false)
                    {
                       //多拼板过站
                        if(lastNgBoardModelIdLB!=boardModelLB->getComponentModels().at(i)->getBoardId())
                        {
                            lastNgBoardModelIdLB=boardModelLB->getComponentModels().at(i)->getBoardId();
                        //
                        jsonLBNgPointsDesc=buildObjectJson(boardModelLB,boardModelLB->getComponentModels().at(i)->getBoardId(),IniService::Push_Mes_LB_UWorkcenterID,ngCodeReplaceString,true);
                        //

                        emit showNgCodeDialog("请输入B线不良品SN:",boardModelLB->getComponentModels().at(i)->getBoardCode(),false,buildNgPointsDesc(boardModelLB,boardModelLB->getComponentModels().at(i)->getBoardId()));

                        }
                    }
                }
                scannerNgCodeLB="";
                waitingNgCodeLB=true;
                 boardModelLB->Mutex.unlock();
            }
        }
    }

void MesService::when_scanner_getNewCode(QString code,bool isLA)
{
    if(isLA)
    {
        scannerCodeLA=code;
    }else
    {
        scannerCodeLB=code;
    }
}

void MesService::run()
{
    stoped=false;
    QString jsonMsg="";
    QString lastCodeLA="";
    QString lastCodeLB="";
    while (started) {
        //线程等待
        taskSleep(1000);
        //禁用
        if(IniService::Push_Mes_Enable==false)
        {
            continue;
        }
        //LA良品过栈
        if(lastCodeLA!=scannerCodeLA&&doneNgPushActionLA==true)
        {
            if(IniService::Push_Mes_LA_WorkcenterID.isEmpty()==false)
            {
            jsonMsg=buildObjectJson(0,0,IniService::Push_Mes_LA_WorkcenterID,scannerCodeLA,false);
            if(pushObjectJson(jsonMsg))
            {

            }
            }
            if(IniService::Push_Mes_LA_UWorkcenterID.isEmpty()==false)
            {
            jsonMsg=buildObjectJson(0,0,IniService::Push_Mes_LA_UWorkcenterID,scannerCodeLA,false);
            if(pushObjectJson(jsonMsg))
            {

            }
            }
            lastCodeLA=scannerCodeLA;
        }
        //LB良品过栈
        if(lastCodeLB!=scannerCodeLB&&doneNgPushActionLB==true)
        {
            if(IniService::Push_Mes_LB_WorkcenterID.isEmpty()==false)
             {
            jsonMsg=buildObjectJson(0,0,IniService::Push_Mes_LB_WorkcenterID,scannerCodeLB,false);
            if(pushObjectJson(jsonMsg))
            {

            }
            }
            if(IniService::Push_Mes_LB_UWorkcenterID.isEmpty()==false)
            {
            jsonMsg=buildObjectJson(0,0,IniService::Push_Mes_LB_UWorkcenterID,scannerCodeLB,false);
            if(pushObjectJson(jsonMsg))
            {

            }
            }
            lastCodeLB=scannerCodeLB;
        }
        //LA不良过栈
        if(doneNgPushActionLA==false)
        {
            //等待获取不良品SN
            if(waitingNgCodeLA)
            {
                //获取到不良条码
                if(scannerNgCodeLA.isEmpty()==false)
                {
                    //jsonMsg=buildObjectJson(ngBoardModelsLA,IniService::Push_Mes_LA_WorkcenterID,scannerNgCodeLA,true);
                    //if(pushObjectJson(jsonMsg))
                    //{

                    //}
                    jsonLANgPointsDesc=jsonLANgPointsDesc.replace(ngCodeReplaceString,scannerNgCodeLA);
                    if(pushObjectJson(jsonLANgPointsDesc))
                    {
                        //emit showMesMessage("Mes不良点位上传成功！！",false);
                    }
                    else
                    {
                        //emit showMesMessage("Mes不良点位上传失败！！",true);
                    }
                    scannerNgCodeLA="";
                    jsonLANgPointsDesc="";
                    doneNgPushActionLA=true;
                    waitingNgCodeLA=false;
                }
            }
        }
        //LB不良过栈
        if(doneNgPushActionLB==false)
        {
            //等待获取不良品SN
            if(waitingNgCodeLB)
            {
                //获取到不良条码
                if(scannerNgCodeLB.isEmpty()==false)
                {
                    //jsonMsg=buildObjectJson(ngBoardModelsLB,IniService::Push_Mes_LB_WorkcenterID,scannerNgCodeLB,true);
                    //if(pushObjectJson(jsonMsg))
                    //{

                    //}
                    jsonLBNgPointsDesc=jsonLBNgPointsDesc.replace(ngCodeReplaceString,scannerNgCodeLB);
                    if(pushObjectJson(jsonLBNgPointsDesc))
                    {
                        //emit showMesMessage("Mes不良点位上传成功！！",false);
                    }
                   else
                   {
                       //emit showMesMessage("Mes不良点位上传失败！！",true);
                   }
                    scannerNgCodeLB="";
                    jsonLBNgPointsDesc="";
                    doneNgPushActionLB=true;
                    waitingNgCodeLB=false;
                }
            }
        }
    }
    stoped=true;
}

QString MesService::buildNgPointsDesc(BoardModel *boardModel,QString boardId)
{
    QString result="";
        if(boardModel!=0)
        {
            //
            if(boardModel->getEnable())
            {
                //
                for(int i=0;i<boardModel->getComponentModels().size();i++)
                {
                    if(boardModel->getComponentModels().at(i)->getTestSuccess()==false)
                    {
                        if(boardModel->getComponentModels().at(i)->getBoardId()==boardId)
                        {
                        //去除下划线后缀
                        QString compName="";
                        QStringList compNames=boardModel->getComponentModels().at(i)->getName().split("_");
                        for(int i=0;i<compNames.size()-1;i++)
                        {
                            compName+=compNames.at(i);
                        }
                        if(compName.isEmpty())
                        {
                            compName=boardModel->getComponentModels().at(i)->getName();
                        }
                        //
                        result.append("不良拼板: [ ");
                        result.append(boardModel->getComponentModels().at(i)->getBoardNo());
                        result.append(" ] 不良点位: [ ");
                        result.append(compName);
                        result.append(" ] 不良代码: [ ");
                        result.append(getDefectTypeCode(boardModel->getComponentModels().at(i)->getErrorType().toInt()));
                        result.append(" ] \r\n");
                        }
                    }
                }
            }
        }
    return result;
}

QString MesService::getDefectTypeCode(int type)
{
    QString val ="无效不良原因";
    if (type==1)
    {
        val="缺件";
    }
    else if (type==2)
    {
        val = "偏移";
    }
    else if (type==3)
    {
        val = "翻身";
    }
    else if (type==4)
    {
        val = "件损";
    }
    else if (type==5)
    {
        val = "立碑";
    }
    else if (type==6)
    {
        val = "极性";
    }
    else if (type==7)
    {
        val = "少锡";
    }
    else if (type==8)
    {
        val = "桥接";
    }
    else if (type==9)
    {
        val = "错件";
    }
    else if (type==10)
    {
        val = "浮起";
    }
    else if (type==11)
    {
        val = "侧立";
    }
    else if (type==12)
    {
        val = "多锡";
    }
    else if (type==13)
    {
        val = "翘腿";
    }
    else if (type==14)
    {
        val = "多件";
    }
    return val;
}
QString MesService::buildObjectJson(BoardModel *boardModel,QString boardId, QString workcenterId,QString code,bool isNG)
{
    //封装
    QJsonObject dataTestItemJson;
    QJsonArray dataTestItemsJson;
    QJsonObject dataJson;
    QJsonObject dataItemJson;
    dataJson.insert("iType","1");
    dataJson.insert("pcbCode",code);
    dataJson.insert("workStationSn",workcenterId);
    dataJson.insert("tsRes",isNG?"NG":"OK");
    dataJson.insert("tsLength","");
    dataJson.insert("pcName",IniService::getWindowUserName());
    dataJson.insert("pcIp",IniService::getWindowIP());
    dataJson.insert("devName",IniService::App_DeviceName);
    dataJson.insert("devModel",IniService::App_DeviceType);
    dataJson.insert("devNo",IniService::App_DeviceCode);
    dataJson.insert("toolNo","");
    dataJson.insert("toolBitSn","");
    dataJson.insert("logData","");
    dataJson.insert("data",dataItemJson);

    //不良点位
    QString errPoints="";
    QString errCodes="";

        if(boardModel!=0 && boardId!=0)
        {
                for(int i=0;i<boardModel->getComponentModels().size();i++)
                {
                    if(boardModel->getComponentModels().at(i)->getTestSuccess()==false)
                    {
                        if(boardModel->getComponentModels().at(i)->getBoardId()==boardId)
                        {
                        //去除下划线后缀
                        QString compName="";
                        QStringList compNames=boardModel->getComponentModels().at(i)->getName().split("_");
                        for(int i=0;i<compNames.size()-1;i++)
                        {
                            compName+=compNames.at(i);
                        }
                        if(compName.isEmpty())
                        {
                            compName=boardModel->getComponentModels().at(i)->getName();
                        }




//                        QByteArray imageData ;

//                        //元件根路径
//                                                  QString compFileDirPath=IniService::Push_Ftp_LocalCompDirPath;
//                                                  if(IniService::Push_Ftp_LocalCompDirPath.endsWith("\\")==false)
//                                                  {
//                                                      compFileDirPath+="\\";
//                                                  }
//                                                  compFileDirPath+=boardModel->getType();
//                                                  compFileDirPath+="\\";
//                                                  compFileDirPath+=boardModel->getTestTime().toString("yyyyMMdd");
//                                                  compFileDirPath+="\\";
//                                                   qDebug("%s",qUtf8Printable(compFileDirPath));
//                                                  //元件路径
//                                                  QDir compFileBaseDir(compFileDirPath);
//                                                  if(compFileBaseDir.exists())
//                                                  {
//                                                      qDebug("%s",qUtf8Printable(compFileDirPath));

//                                                              QString filePath=compFileDirPath;
//                                                              filePath+=boardModel->getStation();
//                                                              filePath+="_"+boardModel->getCode().replace("/", "");//去掉其中的/字符
//                                                              filePath+="_"+boardModel->getNo();
//                                                              filePath+="_"+boardModel->getComponentModels().at(i)->getWinTime().toString("yyyyMMddhhmmss");
//                                                              filePath+="_"+boardModel->getComponentModels().at(i)->getName();
//                                                              filePath+="_"+boardModel->getComponentModels().at(i)->geterrName();
//                                                              filePath+=".jpg";
//                                                              qDebug("%s",qUtf8Printable(filePath));
//                                                              QFile file(filePath);
//                                                              if (!file.open(QIODevice::ReadOnly)) {
//                                                                  // 处理打开失败情况
//                                                                  qDebug("文件打开失败");
//                                                              }
//                                                             if(file.exists()) {
//                                                                 imageData = file.readAll();
//                                                                 dataJson.insert("logData",QString(imageData.toBase64()));
//                                                                 file.remove(); // 删除文件
//                                                             }
//                                                  }






                        //解锁
                        errPoints.append(compName);
                        errCodes.append(boardModel->getComponentModels().at(i)->getErrorType());

                        errPoints.append("|");
                        errCodes.append("|");
                        }
                    }
                }
    }
        dataTestItemJson.insert("errPoints",errPoints);
        dataTestItemJson.insert("errCodes",errCodes);
        dataTestItemsJson.append(dataTestItemJson);
    dataJson.insert("atsTsItemsModels",dataTestItemsJson);
    //转换
    QJsonDocument jsonDoc;
    jsonDoc.setObject(dataJson);
    QByteArray bytes=jsonDoc.toJson(QJsonDocument::Compact);
    QString jsonMsg=QString::fromUtf8(bytes);
    //返回
    return jsonMsg;
}

bool MesService::pushObjectJson(QString jsonMsg)
{
    //推送
    if(IniService::Push_Mes_EnableTraceLog)
    {
        qDebug("%s",qUtf8Printable("Mes开始发起请求：\n"+jsonMsg));
    }
    if(pushBytes(jsonMsg.toUtf8()))
    {
        if(IniService::Push_Mes_EnableTraceLog)
        {
            qDebug("%s",qUtf8Printable("Mes成功数据上传"));
        }
        return true;
    }
    else if(IniService::Push_Mes_EnableTraceLog)
    {
        qWarning("%s",qUtf8Printable("Mes数据上传失败"));
    }
    return false;
}

void MesService::exitEventLoop(int resultCode)
{
    if(eventLoop!=0&&eventLoop->isRunning())
    {
        eventLoop->exit(resultCode);
    }
}

bool MesService::pushBytes(QByteArray bytes)
{
    //
    QNetworkAccessManager* manager=new QNetworkAccessManager();
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    if(eventLoop==0)
    {
        eventLoop=new QEventLoop();
    }
    connect(manager, SIGNAL(finished(QNetworkReply*)), eventLoop, SLOT(quit()));
    //构建请求
    QNetworkRequest request(QUrl(qUtf8Printable(IniService::Push_Mes_Url_OK)));
    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    //发送请求
    QNetworkReply* reply=manager->post(request,bytes);
    eventLoop->exec();
    //获取返回
    bool result=false;
    if(started)
    {
        QString response="";
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
        if(reply->error()==QNetworkReply::NetworkError::NoError//
                &&reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>()==200)
        {
            QByteArray replyBytes=reply->readAll();
            response = textCodec->toUnicode(replyBytes);
            if(IniService::Push_Mes_EnableTraceLog)
            {
                qDebug("%s",qUtf8Printable("Mes上传返回："+response));
            }
            //解析结果
            QJsonParseError jsonError;
            QJsonDocument document = QJsonDocument::fromJson(response.toUtf8(), &jsonError);  // 转化为 JSON 文档
            if (!document.isNull() //
                    && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
                if (document.isObject()) { // JSON 文档为对象
                    QJsonObject jsonObj = document.object();
                    QString mesResultCode="";
                    QString mesResultMessage="";
                    QString mesPlanCode="";
                    int mesPlanCount=0;
                    int mesProductCount=0;
                    if(jsonObj.contains("code"))
                    {
                        QJsonValue jsonVal = jsonObj.value("code");
                        if(jsonVal.isString())
                        {
                            mesResultCode=jsonVal.toString();
                        }
                    }
                    if(jsonObj.contains("result"))
                    {
                        QJsonValue jsonVal = jsonObj.value("result");
                        if(jsonVal.isString())
                        {
                            mesResultMessage=jsonVal.toString();
                        }
                    }
                    if(jsonObj.contains("data"))
                    {
                        QJsonValue dataJsonVal = jsonObj.value("data");
                        QJsonObject dataJsonObj = dataJsonVal.toObject();
                        if(dataJsonObj.contains("moNum"))
                        {
                            QJsonValue jsonVal = dataJsonObj.value("moNum");
                            if(jsonVal.isString())
                            {
                                mesPlanCode=jsonVal.toString();
                            }
                        }
                        if(dataJsonObj.contains("planNum"))
                        {
                            QJsonValue jsonVal = dataJsonObj.value("planNum");
                            if(jsonVal.isString())
                            {
                                mesPlanCount=jsonVal.toString().toInt();
                            }
                        }
                        if(dataJsonObj.contains("realNum"))
                        {
                            QJsonValue jsonVal = dataJsonObj.value("realNum");
                            if(jsonVal.isString())
                            {
                                mesProductCount=jsonVal.toString().toInt();
                            }
                        }
                    }
                    //显示MES
                    if(IniService::Push_Mes_Enable && IniService::Push_Mes_Enable_Service )
                    {
                        emit showMesDialog(mesPlanCode,mesPlanCount,mesProductCount);
                    }
                    //异常提示
                    if(mesResultCode.toUpper()=="NG")
                    {
                        //
                        emit showMesMessage(mesResultMessage,mesResultCode,true);
                        //声音
                        if(IniService::Push_Mes_EnableShowErrorMessage)
                        {

                            playWarnMedia(80);
                        }
                    }
                    else
                    {
                        //
                         emit showMesMessage(mesResultMessage,mesResultCode,true);
                    }
                }
            }
            result=true;
        }
    }
    //回收
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), eventLoop, SLOT(quit()));
    delete reply;
    delete eventLoop;
    delete manager;
    eventLoop=0;
    return result;
}

void MesService::taskSleep(int sleepTimeMs)
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

void MesService::startService()
{
    if(started==false)
    {
        this->started=true;
        QThreadPool::globalInstance()->start(this);
    }
}

void MesService::stopService()
{
    started=false;
    exitEventLoop(0);
}

bool MesService::isStarted()
{
    return started;
}

bool MesService::isStoped()
{
    return stoped;
}

void MesService::setScannerNgCodeLA(QString code)
{
    scannerNgCodeLA=code;
}

void MesService::setScannerNgCodeLB(QString code)
{
    scannerNgCodeLB=code;
}
