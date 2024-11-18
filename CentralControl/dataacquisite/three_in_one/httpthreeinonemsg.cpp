#include "httpthreeinonemsg.h"
#include "dataacquisitionconfig.h"
#include "gdataacquisitionfactory.h"
#include <QNetworkReply>
HttpThreeInOneMsg::HttpThreeInOneMsg()
{

}

void HttpThreeInOneMsg::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
    QNetworkRequest request;

    request.setUrl(QUrl(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_AOI_PARAMS_URL")));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QNetworkReply* reply;

    if(httpJsonObj.contains("workShopSection"))
    {
        QJsonDocument doc_data(httpJsonObj);
        QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
        reply = GDataAcquisitionFactory::get_instance()->get_access_manager_params()->post(request,request_data);

        if(reply->isFinished())
        {
            QByteArray responseByte = reply->readAll();
            response.write(responseByte,true);
        }
    }
}
