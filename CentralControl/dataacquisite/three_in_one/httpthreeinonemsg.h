#ifndef HTTPTHREEINONEMSG_H
#define HTTPTHREEINONEMSG_H
#include "httpmsgprocess.h"

class HttpThreeInOneMsg : public HttpMsgProcess
{
public:
    HttpThreeInOneMsg();

    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);
};

#endif // HTTPTHREEINONEMSG_H
