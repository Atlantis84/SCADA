#ifndef HTTPPACKAGEMSG_H
#define HTTPPACKAGEMSG_H
#include "httpmsgprocess.h"

class HttpPackageMsg : public HttpMsgProcess
{
public:
    HttpPackageMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);
};

#endif // HTTPPACKAGEMSG_H
