#ifndef WAVESOLDERINGM202THREAD_H
#define WAVESOLDERINGM202THREAD_H

#include <QObject>
#include <QThread>
#include "tcpcommonuse.h"
class WaveSolderingM202Thread : public QThread
{
    Q_OBJECT
public:
    static WaveSolderingM202Thread* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new WaveSolderingM202Thread();
        return m_pInstance;
    }

    TcpCommonUse* get_tcp_common_use()
    {
        return m_pTcpCommonUse;
    }
private:
    WaveSolderingM202Thread();
    TcpCommonUse* m_pTcpCommonUse;
    static WaveSolderingM202Thread* m_pInstance;
protected:
    void run();
};

#endif // WAVESOLDERINGM202THREAD_H
