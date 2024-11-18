#ifndef QMQSERVICECOMMONUSE_H
#define QMQSERVICECOMMONUSE_H

#include "qmqservicecommonuse_global.h"
class QMQSERVICECOMMONUSESHARED_EXPORT QMqServiceCommonUse
{

public:
    QMqServiceCommonUse();
    void start_three_in_one_M201_mq_service();
    void push_three_in_one_M201_message(int devtype,QMap<int,QVariant> data);
};

#endif // QMQSERVICECOMMONUSE_H
