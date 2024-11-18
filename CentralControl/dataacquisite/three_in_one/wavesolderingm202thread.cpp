#include "wavesolderingm202thread.h"
#include <QEventLoop>
WaveSolderingM202Thread* WaveSolderingM202Thread::m_pInstance = nullptr;
WaveSolderingM202Thread::WaveSolderingM202Thread()
{
    m_pTcpCommonUse = new TcpCommonUse();
}

void WaveSolderingM202Thread::run()
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
