#include "plcbusinessm201thread.h"
#include "gdataacquisitionfactory.h"
#include <QEventLoop>
PlcBusinessM201Thread* PlcBusinessM201Thread::m_pInstance = nullptr;

PlcBusinessM201Thread::PlcBusinessM201Thread()
{
    m_pUpBoardModbusObj = new ModbusClientDll();
    m_pUpBoardModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_UP_BOARD_IP"),
                                      GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_UP_BOARD_PORT").toInt());
    m_pUpUtensilModbusObj = new ModbusClientDll();
    m_pUpUtensilModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_UP_UTENSIL_IP"),
                                        GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_UP_UTENSIL_PORT").toInt());
    m_pPickUpCoverModbusObj = new ModbusClientDll();
    m_pPickUpCoverModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_IP"),
                                          GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_PORT").toInt());
    m_pDownUtensilModbusObj = new ModbusClientDll();
    m_pDownUtensilModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_IP"),
                                          GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_PORT").toInt());
}

void PlcBusinessM201Thread::run()
{
    while(1)
    {
        QEventLoop eventloop;

        m_pUpBoardModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x05DC,17);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pUpUtensilModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x05DC,17);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pPickUpCoverModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x05DC,17);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        m_pDownUtensilModbusObj->sendMessageToPLC(MODBUS_READ_SIGN,0x05DC,17);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
