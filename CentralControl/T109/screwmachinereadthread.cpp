#include "screwmachinereadthread.h"
#include "gdatafactory.h"
#include <QEventLoop>
bool ScrewMachineReadThread::m_pRunSign = false;
ScrewMachineReadThread::ScrewMachineReadThread()
{

}

void ScrewMachineReadThread::run()
{
    while(1)
    {
        if(m_pRunSign == false)
        {
            GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(READ_PLC,ADDRESS_SCREW_MACHINE_D10100,0xCC,nullptr);
            m_pRunSign = true;
        }
        QEventLoop eventloop;
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
