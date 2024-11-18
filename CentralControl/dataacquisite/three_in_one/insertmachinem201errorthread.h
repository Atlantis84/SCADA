#ifndef INSERTMACHINEM201ERRORTHREAD_H
#define INSERTMACHINEM201ERRORTHREAD_H

#include <QObject>
#include <QThread>
class InsertMachineM201ErrorThread : public QThread
{
    Q_OBJECT
public:
    static InsertMachineM201ErrorThread* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new InsertMachineM201ErrorThread();
        return m_pInstance;
    }

private:
    InsertMachineM201ErrorThread();
    static InsertMachineM201ErrorThread* m_pInstance;
    QString traverseFolder(const QString &path,const QString &month,const QString &day);

protected:
    void run();
signals:
    void signal_current_error(const int stationid, const QString errorcode);
};

#endif // INSERTMACHINEM201ERRORTHREAD_H
