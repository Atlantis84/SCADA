#ifndef HTTPBASEDONCURL_H
#define HTTPBASEDONCURL_H

#include "httpbasedoncurl_global.h"
#include <string>
#include <QObject>
class HTTPBASEDONCURLSHARED_EXPORT HttpBasedOnCurl:public QObject
{
    Q_OBJECT

public:
    HttpBasedOnCurl();

public:
    void get(const std::string& url);
    void post(const std::string& url, const std::string& data, const int& sign);//sign=1:get the head para of line;sign = 2:get the end para of line
    std::string post(const std::string& url, const std::string& data);

signals:
    void signal_rev_get_reply(const std::string& response);
    void signal_rev_head_post_reply(const std::string& response);
    void signal_rev_end_post_reply(const std::string& response);

private:

    static HttpBasedOnCurl* m_pInstance;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void performRequest(const std::string& url, const std::string& data, bool isPost, const int& sign);
    std::string performRequest(const std::string& url, const std::string& data, bool isPost);
};

#endif // HTTPBASEDONCURL_H
