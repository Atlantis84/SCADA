#include "insertmachinem201thread.h"
#include "gdataacquisitionfactory.h"
#include <QEventLoop>
InsertMachineM201Thread* InsertMachineM201Thread::m_pInstance = nullptr;

InsertMachineM201Thread::InsertMachineM201Thread()
{
    m_pInsertMachine1ModbusObj = new ModbusClientDll();
    m_pInsertMachine1ModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE1_IP"),
                                             DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE1_PORT").toInt());
    m_pInsertMachine2ModbusObj = new ModbusClientDll();
    m_pInsertMachine2ModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE2_IP"),
                                             DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE2_PORT").toInt());
    m_pInsertMachine3ModbusObj = new ModbusClientDll();
    m_pInsertMachine3ModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE3_IP"),
                                             DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE3_PORT").toInt());
    m_pInsertMachine4ModbusObj = new ModbusClientDll();
    m_pInsertMachine4ModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE4_IP"),
                                             DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE4_PORT").toInt());
}

void InsertMachineM201Thread::run()
{
    while(1)
    {
        QEventLoop eventloop;
        m_pInsertMachine1ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x14,20);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine1ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x0134,8);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine2ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x14,20);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine2ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x0134,8);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine3ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x14,20);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine3ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x0134,8);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine4ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x14,20);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pInsertMachine4ModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x0134,8);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
