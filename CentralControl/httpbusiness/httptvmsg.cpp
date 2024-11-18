#include "httptvmsg.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "gdataacquisitionfactory.h"
HttpTVMsg::HttpTVMsg()
{

}

void HttpTVMsg::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
//    QLOG_ERROR()<<"process tv msg";
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    if(instructionValue.toString() == "TV_TEST")
    {
        GDataAcquisitionFactory::get_instance()->rev_tv_test_info(httpJsonObj);
        QJsonObject reJsonError;
        reJsonError.insert("responseCode","OK");
        reJsonError.insert("responseInfo",u8"³É¹¦");
        QJsonDocument doc_data(reJsonError);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
