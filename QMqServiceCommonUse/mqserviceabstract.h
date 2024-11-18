#ifndef MQSERVICEABSTRACT_H
#define MQSERVICEABSTRACT_H

#include <QRunnable>
#include <QMutex>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <QMap>
#include <QVariant>
class MqServiceAbstract : public QRunnable
{
public:
    MqServiceAbstract();

    void run();
    void taskSleep(int sleepTimeMs);
    void startService();
    void stopService();
    void MqStartService(bool flag = true);
    virtual void push(int devNo, QMap<int, QVariant>data) = 0;
    void disconnect();
    bool isConnected();

private:
    virtual void connect() = 0;
    virtual void pushMessage(QString msg) = 0;
    QString getDataItemName(QString profix,QString name,bool enable=true);
    virtual void pushObjectByJson() = 0;
protected:
    QMutex mqPushMutex;
    bool started=false;
    bool stoped=true;
    bool connected=false;
    bool disconnected=true;
    bool donePushAction=true;
    amqp_channel_t channel=1;
    amqp_connection_state_t conn;
    amqp_socket_t* socket;
    amqp_basic_properties_t props;

    int devNoCollect;//…Ë±∏±‡∫≈
    QMap<int, QVariant>mapCollectData;
    QString dataItemProfix;
    QMap<QString, QString>mapDevBaseInfo;
};

#endif // MQSERVICEABSTRACT_H
