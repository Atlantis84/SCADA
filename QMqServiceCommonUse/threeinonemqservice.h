#ifndef THREEINONEMQSERVICE_H
#define THREEINONEMQSERVICE_H
#include "mqserviceabstract.h"

class ThreeInOneMqService : public MqServiceAbstract
{
private:
    ThreeInOneMqService();

public:
    void push(int devNo, QMap<int, QVariant>data);

    static ThreeInOneMqService* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new ThreeInOneMqService();
        return m_pInstance;
    }
private:
    void connect();
    void pushMessage(QString msg);
    void pushObjectByJson();

    static ThreeInOneMqService* m_pInstance;

    enum DATA_TYPE_M201{
        Equipment_State_M201=0,
        Param_Content_M201=1,
        Fault_Content_M201=2,
        Reject_Content_M201=3
    };

    enum DEV_TYPE{
        M201UpBoard,
        M201DivideBoard,
        M201UpUtensil,
        M201GlueMachine,
        M201InsertMachineA,
        M201InsertMachineB,
        M201InsertMachineC,
        M201InsertMachineD,
        M201WaveSoldering,
        M201PickUpCover,
        M201AOI,
        M201DownUtensil,
        M201TestStationA,
        M201TestStationB,
        M201TestStationC,
        M201TestStationD,
        M201TestStationE,
        M201TestStationF,
        M201TestStationG,
        M201TestStationH,
        M201TestStationI,
        M201TestStationJ,
        M201TestStationK,
        M201TestStationL
    };
};

#endif // THREEINONEMQSERVICE_H
