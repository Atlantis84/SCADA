#ifndef PLCBUSINESSM201THREAD_H
#define PLCBUSINESSM201THREAD_H
#include <QThread>
#include "modbusclientdll.h"
class PlcBusinessM201Thread : public QThread
{
public:
    static PlcBusinessM201Thread* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new PlcBusinessM201Thread();
        return m_pInstance;
    }

    ModbusClientDll* get_up_board_modbus_obj()
    {
        return m_pUpBoardModbusObj;
    }

    ModbusClientDll* get_up_utensil_modbus_obj()
    {
        return m_pUpUtensilModbusObj;
    }

    ModbusClientDll* get_pick_up_cover_modbus_obj()
    {
        return m_pPickUpCoverModbusObj;
    }

    ModbusClientDll* get_down_utensil_modbus_obj()
    {
        return m_pDownUtensilModbusObj;
    }

protected:
    void run();
private:
    PlcBusinessM201Thread();

    static PlcBusinessM201Thread* m_pInstance;

    ModbusClientDll* m_pUpBoardModbusObj;
    ModbusClientDll* m_pUpUtensilModbusObj;
    ModbusClientDll* m_pPickUpCoverModbusObj;
    ModbusClientDll* m_pDownUtensilModbusObj;
};

#endif // PLCBUSINESSM201THREAD_H
