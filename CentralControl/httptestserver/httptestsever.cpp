#include "httptestsever.h"
#include "QsLog.h"
#include <QJsonDocument>
HttpTestSever::HttpTestSever()
{

}

void HttpTestSever::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    if(instructionValue.toString() == "TEST_SERVER")
    {
        QJsonObject reJson;
        reJson.insert("SN","1307225S00HE65230411000086R3DSZ");
        reJson.insert("PCBCode","90130336-01");
        reJson.insert("CleanCount","1");
        reJson.insert("Type","0");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);

    }
}
