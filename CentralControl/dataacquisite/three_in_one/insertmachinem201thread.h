#ifndef INSERTMACHINEM201THREAD_H
#define INSERTMACHINEM201THREAD_H

#include <QObject>
#include <QThread>
#include "modbusclientdll.h"
class InsertMachineM201Thread : public QThread
{

public:
    static InsertMachineM201Thread* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new InsertMachineM201Thread();
        return m_pInstance;
    }

    ModbusClientDll* get_insert_machine1_modbus_obj()
    {
        return m_pInsertMachine1ModbusObj;
    }

    ModbusClientDll* get_insert_machine2_modbus_obj()
    {
        return m_pInsertMachine2ModbusObj;
    }

    ModbusClientDll* get_insert_machine3_modbus_obj()
    {
        return m_pInsertMachine3ModbusObj;
    }

    ModbusClientDll* get_insert_machine4_modbus_obj()
    {
        return m_pInsertMachine4ModbusObj;
    }

protected:
    void run();
private:
    InsertMachineM201Thread();

    static InsertMachineM201Thread* m_pInstance;

    ModbusClientDll* m_pInsertMachine1ModbusObj;
    ModbusClientDll* m_pInsertMachine2ModbusObj;
    ModbusClientDll* m_pInsertMachine3ModbusObj;
    ModbusClientDll* m_pInsertMachine4ModbusObj;


};

#endif // INSERTMACHINEM201THREAD_H
