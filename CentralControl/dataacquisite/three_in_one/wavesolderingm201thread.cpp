#include "wavesolderingm201thread.h"
#include <QEventLoop>
#include "QsLog.h"
#include <QJsonObject>
#include <QJsonDocument>
WaveSolderingM201Thread* WaveSolderingM201Thread::m_pInstance = nullptr;
WaveSolderingM201Thread::WaveSolderingM201Thread()
{
    m_pTcpCommonUse = new TcpCommonUse();
    m_pChangeProductionSign = false;
}

void WaveSolderingM201Thread::slot_change_production(const bool changesign, const QString stationname, const QString testfilename)
{
    m_pChangeProductionSign = changesign;
    m_pStationName = stationname;
    m_pTestFileName = testfilename;
}

void WaveSolderingM201Thread::run()
{
    while(1)
    {
        QEventLoop eventloop;
        //change-production
        if(m_pChangeProductionSign)
        {
            m_pChangeProductionSign = false;
            QJsonObject  root;
            root.insert("stationName",m_pStationName);
            root.insert("testFileName",m_pTestFileName);
            QJsonDocument doc_data(root);
            QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
            m_pTcpCommonUse->send_data_to_server(request_data);
            QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
            eventloop.exec();
        }
        //change-production

        m_pTcpCommonUse->send_data_to_server("GetStates:");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pTcpCommonUse->send_data_to_server("GetParams:");
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pTcpCommonUse->send_data_to_server("GetStateValues:");
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
