#include "tcondataacquisitonthread.h"
#include "gdatafactory.h"
#include <QEventLoop>
TconDataAcquisitonThread::TconDataAcquisitonThread()
{

}

void TconDataAcquisitonThread::run()
{
    while(1)
    {
        QEventLoop eventloop;
        GDataFactory::get_fins_udp_service_tcon_test_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_TEST_STATION_W300,0x10,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataFactory::get_fins_udp_service_tcon_test_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_TEST_STATION_W310,0x12,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataFactory::get_fins_udp_service_tcon_test_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_TEST_STATION_W320,0x5C,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataFactory::get_fins_udp_service_tcon_test_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_TEST_STATION_W370,0x5C,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataFactory::get_fins_udp_service_tcon_test_station()->send_message_to_plc(READ_PLC,0xB1,ADDRESS_TCON_TEST_STATION_W420,0x02,nullptr);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
