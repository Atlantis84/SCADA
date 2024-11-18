#include "wavesolderingm201thread.h"
#include <QEventLoop>
#include "QsLog.h"
WaveSolderingM201Thread* WaveSolderingM201Thread::m_pInstance = nullptr;
WaveSolderingM201Thread::WaveSolderingM201Thread()
{
    m_pTcpCommonUse = new TcpCommonUse();
}

void WaveSolderingM201Thread::run()
{
    while(1)
    {
        QEventLoop eventloop;
        m_pTcpCommonUse->send_data_to_server("GetStates:");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pTcpCommonUse->send_data_to_server("GetParams:");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pTcpCommonUse->send_data_to_server("GetStatevValues:");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pTcpCommonUse->send_data_to_server("GetAlarms:");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pTcpCommonUse->send_data_to_server("GetOutput:2024;0;0;");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
