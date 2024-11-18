#ifndef HTTPTESTSEVER_H
#define HTTPTESTSEVER_H

#include <QObject>
#include "httpmsgprocess.h"
class HttpTestSever : public HttpMsgProcess
{
    Q_OBJECT
public:
    HttpTestSever();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);

signals:

public slots:
};

#endif // HTTPTESTSEVER_H
