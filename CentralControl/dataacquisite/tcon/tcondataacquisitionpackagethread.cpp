#include "tcondataacquisitionpackagethread.h"
#include "gdatafactory.h"
#include <QTimer>
#include <QEventLoop>
TconDataAcquisitionPackageThread::TconDataAcquisitionPackageThread()
{

}

void TconDataAcquisitionPackageThread::run()
{
    while (1) {
        QEventLoop eventloop;
        GDataFactory::get_fins_udp_service_tcon_package_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_PACKAGE_W300,0x0A,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        GDataFactory::get_fins_udp_service_tcon_package_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_PACKAGE_W420,0x02,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
