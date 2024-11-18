#ifndef MQSERVICE_H
#define MQSERVICE_H

//#include "boardmodel.h"
//#include "componentmodel.h"
//#include "statusmodel.h"
//#include "usermodel.h"

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <QString>
#include <QDebug>
#include <QRunnable>
#include <QList>
#include <QMutex>
#include <qmqservicesrc_global.h>

//设备故障报警
#define WARNNING_NAME_INDEXOF       (0)
#define WARNNING_DISC_INDEXOF       (1)
#define WARNNING_CODE_INDEXOF       (2)
//设备状态
#define EquipmentStateListValueIndex    (0)
#define EquipmentStateListTimeMsIndex   (1)
#define EquipmentStateListTimeDiscIndex (2)
//OK与Ng数量
#define OK_NUM_LIST_INDEFOF         (1)
#define NG_NUM_LIST_INDEFOF         (2)
//工装动作次数

namespace Service {
class MQService;
}

class QMQSERVICESRCSHARED_EXPORT MQService : QRunnable
{
public:
    explicit MQService();
    ~MQService();
public:
    void run();
    void taskSleep(int sleepTimeMs);
    void startService();
    void stopService();
    bool isStarted();
    bool isStoped();
    void MqStartService(bool flag = true);
    void push(int devNo, QMap<int, QString>data);
    //void setLoginUserModel(UserModel* userModel);
private:
    void connect();
    void disconnect();
    bool isConnected();
    void pushMessage(QString msg);
    QString getDataItemName(QString profix,QString name,bool enable=true);
    void pushObjectByJson();
private:
    QMutex mqPushMutex;
    //UserModel* loginUserModel=0;
    bool started=false;
    bool stoped=true;
    bool connected=false;
    bool disconnected=true;
    bool donePushAction=true;
    int channel=1;
    amqp_connection_state_t conn;
    amqp_socket_t* socket;
    amqp_basic_properties_t props;

    int devNoCollect;//设备编号
    QMap<int, QString>mapCollectData;
    QString dataItemProfix;
    QMap<QString, QString>mapDevBaseInfo;

    enum DATA_TYPE{
        Equipment_State=0,//设备状态
        Key_Part_Action_Times=1,//关键部件
        Ok_Ng_Quantity=2,
        Equipment_Warnings=3,//故障报警
        Product_ProjectID=4,//工单
        Product_Type=5,//产品类型
        Work_Device_Ok_Ng_Ratio=6,//工装OK\NG数及通过率
    };

    enum DEV_TYPE{
        TconUpperBoardDevA = 0,
        TconUpperBoardDevB,
        TconSubBoardDevA,
        TconSubBoardDevB,
        TconTestDevA,
        TconTestDevB,
        TconAOIdev,
        TconPackeageDev
    };
};

#endif // HNMQCLIENT_H
