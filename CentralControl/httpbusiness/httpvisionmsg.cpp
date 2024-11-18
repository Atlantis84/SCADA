#include "httpvisionmsg.h"
#include "QsLog.h"
#include <QJsonDocument>
#include "gdatafactory.h"
#include <QEventLoop>
#include <QNetworkReply>
#include "imessagebox.h"
#include <QJsonArray>
#include <QSslKey>
#include <QSslConfiguration>
HttpVisionMsg::HttpVisionMsg()
{

}

void HttpVisionMsg::process_http_msg(QJsonObject httpJsonObj,HttpResponse& response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));

    QNetworkRequest request;
//    QNetworkAccessManager* tmpaccessManager = new QNetworkAccessManager();
    QLOG_INFO()<<"rev msg from VISION!";

    QString bsA,hfA,bsB,hfB;
    bsA=hfA=bsB=hfB="";
    for (int i=0;i<5;i++) {
        bsA.append(GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE")[i]);
        bsB.append(GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE")[i]);
    }

    for (int i=0;i<6;i++) {
        hfA.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_A")[i]);
        hfB.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B")[i]);
    }

    QString cameraID = httpJsonObj.value(QStringLiteral("cameraID")).toString();

    QJsonObject reJson;
    if(cameraID == "1")
    {
        reJson.insert("code","OK");
        reJson.insert("projectID","");
        reJson.insert("halfMaterialNumber",hfA);
        reJson.insert("machineStyleName","");
        reJson.insert("boardStyle",bsA);
        reJson.insert("boardSize","");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
    else {
        reJson.insert("code","OK");
        reJson.insert("projectID","");
        reJson.insert("halfMaterialNumber",hfB);
        reJson.insert("machineStyleName","");
        reJson.insert("boardStyle",bsB);
        reJson.insert("boardSize","");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }


#if 0
    try
    {
        QNetworkReply* reply;
        QEventLoop eventloop;
        if((instructionValue.isString()) && (instructionValue.toString() != ""))
        {
            QString productsn = httpJsonObj.value(QStringLiteral("productSN")).toString();
            //the SN is changed by shixiangkeji,process the SN
            if(productsn.contains('/'))
            {
                productsn.remove('\r');
                QStringList lst = productsn.split('/');
                if(lst.size() == 2)
                    productsn = lst[1];
                else
                {
                    QLOG_WARN()<<"the SN format is ABNORMAL!";
                    return;
                }
            }
            else
                ;
            //the SN is changed by shixiangkeji,process the SN
            QLOG_INFO()<<"the SN of VISION request is:"<<productsn;
            QString tmpurl;
            tmpurl = "http://";
            tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
            tmpurl.append(":");
            tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
            tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
            tmpurl.append(productsn);

            request.setUrl(QUrl(tmpurl));
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            request.setRawHeader("appId","CenterController");
            request.setRawHeader("publicKey",PUBLIC_KEY);

            reply = GDataFactory::get_access_manager_vision()->get(request);
            connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
            //set get request time out
            QTimer::singleShot(2000,&eventloop,&QEventLoop::quit);
            eventloop.exec();

            if(reply->isFinished())
            {
                QByteArray responseByte = reply->readAll();
//                QLOG_INFO()<<"the response of MES is:" << QString(responseByte);
                if (reply->error() == QNetworkReply::NoError){
                    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);

                    QJsonObject jsonObject = jsonDocument.object();
                    QString statusString = "";
                    if(jsonObject.contains("status"))
                    {
                        QJsonValue instructionValue = jsonObject.value(QStringLiteral("status"));
                        if(instructionValue.isString())
                            statusString = instructionValue.toString();
                        else
                        {
                            QLOG_WARN()<<"MES status para data format is ABNORMAL!";
                            QJsonObject reJsonError;
                            reJsonError.insert("code","ERROR");
                            reJsonError.insert("data",u8"");
                            QJsonDocument doc_data(reJsonError);
                            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                            response.write(response_data,true);
                        }
                    }

                    if(statusString == "success")
                    {
                        if(jsonObject.contains("dataInfo"))
                        {
                            QJsonValue arrayValue = jsonObject.value(QStringLiteral("dataInfo"));
                            if(arrayValue.isArray())
                            {
                                QJsonArray array = arrayValue.toArray();
                                for (int i=0;i<array.size();i++)
                                {
                                    QJsonValue iconArray = array.at(i);
                                    QJsonObject icon = iconArray.toObject();
                                    QJsonObject reJson;
                                    QString tmpBoardStyle="";
                                    for (int i=0;i<5;i++)
                                    {
                                        tmpBoardStyle.append(icon["tpS_TEMPLATE"].toString()[i]);
                                    }
                                    reJson.insert("code","OK");
                                    reJson.insert("projectID",icon["projecT_ID"].toString());
                                    reJson.insert("halfMaterialNumber",icon["modeL_CODE"].toString());
                                    reJson.insert("machineStyleName",icon["mP_MODEL_CODE"].toString());
                                    reJson.insert("boardStyle",tmpBoardStyle);
                                    reJson.insert("boardSize",icon["boarD_SIZE"].toString());
                                    QJsonDocument doc_data(reJson);
                                    QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
    //                                QLOG_INFO()<<"reple the VISION data is:"<<response_data;
                                    response.write(response_data,true);
                                }
                            }
                        }
                    }
                    else
                    {
                        QLOG_WARN()<<"MES status para is error!";
                        QJsonObject reJsonError;
                        reJsonError.insert("code","ERROR");
                        reJsonError.insert("data",u8"");
                        QJsonDocument doc_data(reJsonError);
                        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                        response.write(response_data,true);
                    }
                }
                else
                {
                    QLOG_WARN()<<"MES reply exist ERROR!";
                    QJsonObject reJsonError;
                    reJsonError.insert("code","ERROR");
                    reJsonError.insert("data",u8"");
                    QJsonDocument doc_data(reJsonError);
                    QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                    response.write(response_data,true);
                }
            }
            else
            {
                QLOG_WARN()<<"MES reply TIMEOUT!"<<"reply error:"<<reply->errorString();
                QJsonObject reJsonError;
                reJsonError.insert("code","ERROR");
                reJsonError.insert("data",u8"");
                QJsonDocument doc_data(reJsonError);
                QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                response.write(response_data,true);
                return;
            }            
        }
        else
        {
            QLOG_WARN()<<"vision request msg data exist ERROR!";
            QJsonObject reJsonError;
            reJsonError.insert("code","ERROR");
            reJsonError.insert("data",u8"");

            QJsonDocument doc_data(reJsonError);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        disconnect(reply,&QNetworkReply::finished,&eventloop,&QEventLoop::quit);
        reply->abort();
        reply->deleteLater();
    }
    catch(...)
    {
        QLOG_WARN()<<"VISION FLIP EXIT DETECTION";
    }
#endif
}
