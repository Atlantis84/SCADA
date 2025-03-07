#ifndef TVTESTBUSINESSM201THREAD_H
#define TVTESTBUSINESSM201THREAD_H

#include <QObject>
#include <QThread>
class TvTestBusinessM201Thread : public QThread
{
public:
    static TvTestBusinessM201Thread* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new TvTestBusinessM201Thread();
        return m_pObj;
    }

private:
    TvTestBusinessM201Thread();

    static TvTestBusinessM201Thread* m_pObj;
protected:
    void run();
};

#endif // TVTESTBUSINESSM201THREAD_H
