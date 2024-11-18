#include "qmqservicecommonuse.h"
#include "threeinonemqservice.h"

QMqServiceCommonUse::QMqServiceCommonUse()
{

}

void QMqServiceCommonUse::start_three_in_one_M201_mq_service()
{
    ThreeInOneMqService::get_instance()->MqStartService(true);
}

void QMqServiceCommonUse::push_three_in_one_M201_message(int devtype, QMap<int, QVariant> data)
{
    ThreeInOneMqService::get_instance()->push(devtype,data);
}
