#include "httpbasedoncurl.h"
#include <curl.h>
#include <QsLog.h>
using namespace QsLogging;
HttpBasedOnCurl* HttpBasedOnCurl::m_pInstance = nullptr;
HttpBasedOnCurl::HttpBasedOnCurl()
{
}

void HttpBasedOnCurl::get(const std::string &url)
{
    performRequest(url, "", false,0);
}

void HttpBasedOnCurl::post(const std::string &url, const std::string &data, const int& sign)
{
    performRequest(url, data, true,sign);
}

size_t HttpBasedOnCurl::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    (static_cast<std::string*>(userp))->append(static_cast<char*>(contents), size * nmemb);
       return size * nmemb;
}

void HttpBasedOnCurl::performRequest(const std::string &url, const std::string &data, bool isPost, const int& sign)
{
    CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            if (isPost) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

                struct curl_slist* headers = nullptr;
                headers = curl_slist_append(headers, "Content-Type: application/json");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            }
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                QLOG_WARN()<<"curl_easy_perform() failed: " << curl_easy_strerror(res);
            } else {
                if(isPost)
                {
                    if(sign == 1)
                        emit signal_rev_head_post_reply(readBuffer);
                    else
                        emit signal_rev_end_post_reply(readBuffer);
                }
                else
                    emit signal_rev_get_reply(readBuffer);
            }
            curl_easy_cleanup(curl);
        }
}
