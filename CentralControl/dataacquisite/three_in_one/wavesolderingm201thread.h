#ifndef WAVESOLDERINGM201THREAD_H
#define WAVESOLDERINGM201THREAD_H

#include <QObject>
#include <QThread>
#include "tcpcommonuse.h"
class WaveSolderingM201Thread : public QThread
{
    Q_OBJECT
public:
    static WaveSolderingM201Thread* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new WaveSolderingM201Thread();
        return m_pInstance;
    }

    TcpCommonUse* get_tcp_common_use()
    {
        return m_pTcpCommonUse;
    }

protected:
    void run();

private:

    WaveSolderingM201Thread();
    TcpCommonUse* m_pTcpCommonUse;
    static WaveSolderingM201Thread* m_pInstance;

    bool m_pChangeProductionSign;
    QString m_pStationName;
    QString m_pTestFileName;

signals:

public slots:

    void slot_change_production(const bool changesign,const QString stationname,const QString testfilename);
};

#endif // WAVESOLDERINGM201THREAD_H
