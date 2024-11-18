/**
  @file
  @author Stefan Frings
*/

#include "httprequesthandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include "gdatafactory.h"
#include "gdataacquisitionfactory.h"
#include "QsLog.h"
using namespace stefanfrings;

QString readFileToString(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    QTextStream textStream(&file);
    QString content = textStream.readAll();

    file.close();
    return content;
}

HttpRequestHandler::HttpRequestHandler(QObject* parent)
    : QObject(parent)
{}

HttpRequestHandler::~HttpRequestHandler()
{}

void HttpRequestHandler::service(HttpRequest& request, HttpResponse& response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(request.getBody().data());
    QJsonObject jsonObject = jsonDocument.object();
    //rev and process data from registerstation\AOI\visuallead-lilei20211112
    if(jsonObject.contains("reType"))
    {
        QJsonValue instructionValue = jsonObject.value("reType");
//        QLOG_ERROR()<<"instruction value is:"<<instructionValue.toString();

//        QJsonObject reJsonError;
//        reJsonError.insert("responseCode","OK");
//        reJsonError.insert("responseInfo",u8"³É¹¦");
//        QJsonDocument doc_data(reJsonError);
//        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
//        response.write(response_data,true);
//        return;

        if(instructionValue.toString() == "TV_TEST")
        {
            GDataAcquisitionFactory::get_instance()->process_http_obj_tv(jsonObject,response);
        }
        else
        {
            if(GDataFactory::get_factory()->get_http_process_obj(instructionValue.toString()) == nullptr)
            {
                QLOG_WARN()<<"get http obj failed!";
                QJsonObject reJson;
                reJson.insert("code","ERROR");
                reJson.insert("data","");
                QJsonDocument doc_data(reJson);
                QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                response.write(response_data,true);
            }
            else
            {
                GDataFactory::get_factory()->
                        get_http_process_obj(instructionValue.toString())->
                        process_http_msg(jsonObject,response);
            }
        }
    }
    else if(jsonObject.contains("workShopSection"))
    {
        GDataAcquisitionFactory::get_instance()->get_http_obj()->process_http_msg(jsonObject,response);
    }
    else
    {
        QLOG_WARN()<<"the client request exist ERROR!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("data","");

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        QString str = readFileToString("f:/ll.txt");
        response.write(str.toUtf8(),true);
    }
}
