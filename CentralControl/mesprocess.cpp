#include "mesprocess.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "QsLog.h"
#include "gdatafactory.h"
#include "imessagebox.h"
#include <QEventLoop>
MesProcess::MesProcess(QObject *parent) : QObject(parent)
{
    accessManager = new QNetworkAccessManager(this);
    accessManagerLightBar = new QNetworkAccessManager(this);
    accessManagerTBox = new QNetworkAccessManager(this);
    accessManagerCommon = new QNetworkAccessManager(this);
    accessManagerForGetMesInfoBySN = new QNetworkAccessManager(this);
    accessManagerM201 = new QNetworkAccessManager(this);

    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply(QNetworkReply*)));
    connect(accessManagerLightBar,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply_light_bar(QNetworkReply*)));
    connect(accessManagerTBox,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply_tbox(QNetworkReply*)));
    connect(accessManagerCommon,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply_common(QNetworkReply*)));
    connect(accessManagerM201,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply_m201(QNetworkReply*)));
}

void MesProcess::exec_http_post(int desSign)//0:Mes 1:AOI 2:register 3:visuallead
{
    QString urlDes;
    if(desSign == 0)//mes
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
        urlDes.append("/post");
    }
    else if(desSign == 1)//AOI
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("AOI_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("AOI_PORT"));
        urlDes.append("/post");
    }
    else if(desSign == 2)//register
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("REGISTER_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("REGISTER_PORT"));
        urlDes.append("/post");
    }
    else if(desSign == 3)//visuallead
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("VISUAL_LEAD_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("VISUAL_LEAD_PORT"));
        urlDes.append("/post");
    }
    else
        urlDes = "http://127.0.0.1:8080/post";

    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root, data_value,ats_models;
    QJsonArray   ats_value;

    ats_models.insert("itemId","");
    ats_models.insert("itemName","C1");
    ats_models.insert("itemL_Limit","");
    ats_models.insert("itemH_Limit","");
    ats_models.insert("itemTsValue","");
    ats_models.insert("itemLength","");
    ats_models.insert("itemRes","OK");
    ats_models.insert("errCodes","SX;LB");

    ats_value.append(ats_models);
    root.insert("atsTsItemsModels", ats_value);

    root.insert("iType", "1");
    root.insert("pcbCode", "56CTA69");
    root.insert("workStationSn", "WS101A01");
    root.insert("tsRes","OK");
    root.insert("tsLength", "");
    root.insert("pcName", "sys001");
    root.insert("pcIp", "127.0.0.1");
    root.insert("devName",u8"德律AOI");
    root.insert("devModel", "DL2000");
    root.insert("devNo", "ZD_AOI001");
    root.insert("toolNo", "");
    root.insert("toolBitSn", "");
    root.insert("logData", "");

    data_value.insert("imeiInfo","11");
    data_value.insert("meidInfo","22");
    data_value.insert("macInfo","22");
    root.insert("data",data_value);

    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    accessManager->post(request, request_data);
}

void MesProcess::exec_http_get(QString productsn)
{
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
        productsn.remove('\r');
    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
    tmpurl.append(productsn);
    QLOG_WARN()<<"the SN after processing is:"<<tmpurl;

    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request->setRawHeader("appId","CenterController");
    request->setRawHeader("publicKey",PUBLIC_KEY);
    reply = accessManager->get(*request);
    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";;
    }
}

void MesProcess::exec_http_post_light_bar(const int timesign,QByteArray data)
{
    QLOG_WARN()<<u8"灯条发送至Mes信息为:"<<data;
    if(timesign == 1)
    {
        QString urlDes;
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("LIGHT_BAR_MES_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("LIGHT_BAR_MES_PORT"));
        urlDes.append("/LedPcbClean/SaveLedPcbCleanError");

        QNetworkReply* reply;
        QEventLoop eventloop;
        QNetworkRequest request;
        request.setUrl(QUrl(urlDes));
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        reply = accessManagerLightBar->post(request, data);
        connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
        QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        if(reply->isFinished())
        {
            QLOG_INFO()<<"light-bar http post request reply is NORMAL!";
        }
        else
        {
            QLOG_WARN()<<"light-bar http post request reply is TIMEOUT!";;
        }
    }
    else
    {
        QString urlDes;
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("LIGHT_BAR_MES_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("LIGHT_BAR_MES_PORT"));
        urlDes.append("/LedPcbClean/SaveLedPcbCleanRepair");

        QNetworkReply* reply;
        QEventLoop eventloop;
        QNetworkRequest request;
        request.setUrl(QUrl(urlDes));
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        reply = accessManagerLightBar->post(request, data);
        connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
        QTimer::singleShot(15000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        if(reply->isFinished())
        {
            QLOG_INFO()<<"light-bar http post request reply is NORMAL!";
        }
        else
        {
            QLOG_WARN()<<"light-bar http post request reply is TIMEOUT!";;
        }
    }
}

void MesProcess::exec_http_get_tbox(QString data)
{
//    data = "ZS21UP426D0143";
    data.remove('\r');
    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
    tmpurl.append(data);
    QLOG_WARN()<<tmpurl;

    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));/*
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request->setRawHeader("appId","CenterController");
    request->setRawHeader("publicKey",PUBLIC_KEY);*/
    reply = accessManagerTBox->get(*request);
    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";
    }
}

void MesProcess::exec_http_get_m201(QString data)
{
    data.remove('\r');
    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
    tmpurl.append(data);

    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));
    reply = accessManagerM201->get(*request);
    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";
    }
}

void MesProcess::exec_http_common_get(const QByteArray &data,int sign)
{
    QLOG_INFO() << data;
    QString urlDes;
    if(sign == 1)
        urlDes = GDataFactory::get_factory()->get_config_para("MES_CHECK_URL")/*"http://172.16.123.79:8082/api/EqProduct/CheckEqPassStationAndAssembly"*/;
    else if(sign == 0)
        urlDes = GDataFactory::get_factory()->get_config_para("MES_SAVE_URL")/*"http://172.16.123.79:8082/api/EqProduct/CheckAndSaveEqProductInfo"*/;
    else if(sign == 2)//摩尔MES校验接口
        urlDes = "http://172.16.123.202:8092/api/Ats/AtsCkRun";
    else if(sign == 3)//摩尔MES上传结果接口
        urlDes = "http://172.16.123.202:8092/api/Ats/AtsTsRun";

    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QEventLoop eventloop;
    QNetworkReply* reply = accessManagerCommon->post(request, data);

    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";;
    }
}

void MesProcess::slot_urlReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        if(GDataFactory::get_test_plan_wgt()->isVisible())
            QLOG_WARN()<<"rev change production mes reply,the info is:" << QString(responseByte);
        else {
            QLOG_WARN()<<"rev history display mes reply,the info is:" << QString(responseByte);
        }
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
                IMessageBox* msgBox = new IMessageBox(3);
                if(GDataFactory::get_test_plan_wgt()->isVisible())
                    msgBox->warning(u8"MES返回换产信息数据格式错误,换产失败!");
                else {
                    msgBox->warning(u8"MES返回换产信息数据格式错误,历史浏览无法进行!");
                }
                return;
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
                        QList<QString> tmpInfo;
                        tmpInfo.push_back(icon["projecT_ID"].toString());
                        tmpInfo.push_back(icon["modeL_CODE"].toString());//half material
                        tmpInfo.push_back(icon["mP_MODEL_CODE"].toString());
                        tmpInfo.push_back(icon["tpS_TEMPLATE"].toString());//board type
                        tmpInfo.push_back(icon["boarD_SIZE"].toString());
                        //when rev the board info from serial port,put the info into the tmp list
                        //in the data factory,when we press the save button on the UI,we save
                        //the formal serial port info to the formal list
                        if(GDataFactory::get_test_plan_wgt()->isVisible())
                            GDataFactory::get_factory()->set_AB_change_product_info(tmpInfo);
                        if(GDataFactory::get_history_display_wgt()->isVisible())
                            GDataFactory::get_history_display_wgt()->set_boardtype_halfmaterialnumber(icon["tpS_TEMPLATE"].toString(),icon["modeL_CODE"].toString());
                    }
                }
            }
        }
        else {
            QLOG_WARN()<<"MES status para is error!";
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"MES status error!");
            return;
        }
//    reply->deleteLater();
    }
}

void MesProcess::slot_urlReply_light_bar(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        QLOG_INFO()<<"the Mes reply of light-bar is:"<<responseByte;
        emit signal_to_light_bar_info(responseByte);
    }
    else {
        QLOG_WARN()<<"light-bar Mes reply exist ERROR!";
    }
}

void MesProcess::slot_urlReply_tbox(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
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
                return;
            }
        }

        if(statusString == "success")
        {
            QString partOfSN = "";
            QString orderNumber = "";
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
                        QList<QString> tmpInfo;
                        tmpInfo.push_back(icon["projecT_ID"].toString());
                        tmpInfo.push_back(icon["modeL_CODE"].toString());//half material
                        tmpInfo.push_back(icon["mP_MODEL_CODE"].toString());
                        tmpInfo.push_back(icon["tpS_TEMPLATE"].toString());//board type
                        tmpInfo.push_back(icon["boarD_SIZE"].toString());
                        partOfSN = icon["mP_MODEL_CODE"].toString();
                        orderNumber = icon["ordeR_NUMBER"].toString();
                    }
                }
            }
            emit signal_to_ui_tbox(QString(responseByte),orderNumber,partOfSN);
        }
        else {
            QLOG_WARN()<<"MES status para is error!";
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"MES status error!");
            return;
        }
    }
    else
        QLOG_WARN()<<"Mes reply exist ERROR!";
}

void MesProcess::slot_urlReply_common(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        emit signal_to_common(reply->readAll());
    }
    else
    {
        QLOG_WARN()<<reply->errorString();
        QLOG_WARN()<<"Mes reply exist ERROR!";
    }
}

void MesProcess::slot_urlReply_m201(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        emit signal_to_ui_m201(reply->readAll());
    }
    else
        QLOG_WARN()<<"Mes reply exist ERROR!";
}

