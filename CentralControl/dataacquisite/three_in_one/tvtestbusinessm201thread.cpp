#include "tvtestbusinessm201thread.h"
#include <QEventLoop>
#include <QTimer>
#include "gdataacquisitionfactory.h"
#include "plcaddress.h"
#include "QsLog.h"
TvTestBusinessM201Thread* TvTestBusinessM201Thread::m_pObj = nullptr;
TvTestBusinessM201Thread::TvTestBusinessM201Thread()
{

}

void TvTestBusinessM201Thread::run()
{
    while(1)
    {
        QEventLoop eventloop;
        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(1)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(2)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(3)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(4)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(5)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(6)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

//        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(7)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
//        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
//        eventloop.exec();

//        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(8)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
//        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
//        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(9)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(10)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(11)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        GDataAcquisitionFactory::get_fins_upd_obj_m201_test_station(12)->send_message_to_plc(READ_PLC,0x82,0x05DC,0x06,nullptr);
        QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
