#include "tcondataacquisitionaoithread.h"
#include "gdatafactory.h"
#include <QTimer>
#include <QEventLoop>
TconDataAcquisitionAOIThread::TconDataAcquisitionAOIThread()
{

}

void TconDataAcquisitionAOIThread::run()
{
    while (1) {
        QEventLoop eventloop;
        GDataFactory::get_fins_udp_service_tcon_AOI_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_AOI_W300,0x06,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        GDataFactory::get_fins_udp_service_tcon_AOI_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_AOI_W320,0x18,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        GDataFactory::get_fins_udp_service_tcon_AOI_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_AOI_W420,0x02,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
