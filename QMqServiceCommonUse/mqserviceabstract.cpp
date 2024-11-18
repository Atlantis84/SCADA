#include "mqserviceabstract.h"
#include <QThreadPool>
#include <QDateTime>
MqServiceAbstract::MqServiceAbstract()
{
    //修改默认线程池大小
    QThreadPool::globalInstance()->setMaxThreadCount(100);

    channel = 1;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2;
    //禁用QRunnable自动删除
    this->setAutoDelete(false);
}

void MqServiceAbstract::run()
{
    stoped=false;
    while (started) {
        //线程等待
        taskSleep(500);
        //推送LA
        if(donePushAction==false)
        {            //推送
            donePushAction=true;
            this->pushObjectByJson();
        }
    }
    stoped=true;
}

void MqServiceAbstract::taskSleep(int sleepTimeMs)
{
    if(sleepTimeMs<1)
    {
        QThread::msleep(1);
    }
    qint64 outTimeStamp = QDateTime::currentMSecsSinceEpoch()+sleepTimeMs;
    while (outTimeStamp>QDateTime::currentMSecsSinceEpoch()&&started) {
        QThread::msleep(2);
    }
}

void MqServiceAbstract::startService()
{
    if(started==false)
    {
        this->started=true;
        QThreadPool::globalInstance()->start(this);
    }
}

void MqServiceAbstract::stopService()
{
    started=false;
}

void MqServiceAbstract::MqStartService(bool flag)
{
    if(flag){
        startService();
    }else{
        stopService();
    }
}

void MqServiceAbstract::disconnect()
{
    //断开连接
    if(disconnected==false)
    {
        amqp_channel_close(conn, channel, AMQP_REPLY_SUCCESS);
        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(conn);
        disconnected=true;
    }
}

bool MqServiceAbstract::isConnected()
{
    return connected;
}

QString MqServiceAbstract::getDataItemName(QString profix, QString name, bool enable)
{
    if(enable==false)
    {
        return name;
    }
    //启用
    QString dataItemName("");
    dataItemName.append(profix);
    dataItemName.append(name);
    return dataItemName;
}
