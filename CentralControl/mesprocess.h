#ifndef MESPROCESS_H
#define MESPROCESS_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
class MesProcess : public QObject
{
    Q_OBJECT
public:
    explicit MesProcess(QObject *parent = nullptr);

    void exec_http_post(int desSign);//0:Mes 1:AOI 2:register 3:visuallead
    void exec_http_get(QString productsn);//for change production scanning code
    void exec_http_post_light_bar(const int timesign,QByteArray data);//timesign is used to distinguish the Mes API
    void exec_http_get_tbox(QString data);
    void exec_http_get_m201(QString data);
    void exec_http_common_get(const QByteArray& data,int sign);

private:
    QNetworkAccessManager* accessManager;
    QNetworkAccessManager* accessManagerLightBar;
    QNetworkAccessManager* accessManagerTBox;
    QNetworkAccessManager* accessManagerCommon;
    QNetworkAccessManager* accessManagerM201;

    QNetworkAccessManager* accessManagerForGetMesInfoBySN;

signals:
    void signal_to_ui(QByteArray* data);
    void signal_to_ui_tbox(const QString mesInfo,const QString orderNumber,const QString partOfSN);
    void signal_to_light_bar_info(const QByteArray& data);
    void signal_to_ui_m201(const QByteArray& data);
    void signal_to_common(const QByteArray& data);
public slots:
    void slot_urlReply(QNetworkReply* reply);
    void slot_urlReply_light_bar(QNetworkReply* reply);
    void slot_urlReply_tbox(QNetworkReply* reply);
    void slot_urlReply_common(QNetworkReply* reply);
    void slot_urlReply_m201(QNetworkReply* reply);
};

#endif // MESPROCESS_H
