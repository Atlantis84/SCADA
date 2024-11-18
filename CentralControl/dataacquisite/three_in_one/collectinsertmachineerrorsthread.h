#ifndef COLLECTINSERTMACHINEERRORSTHREAD_H
#define COLLECTINSERTMACHINEERRORSTHREAD_H

#include <QObject>
#include <QThread>
class CollectInsertMachineErrorsThread : public QThread
{
    Q_OBJECT

public:
    static CollectInsertMachineErrorsThread* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new CollectInsertMachineErrorsThread();
        return m_pInstance;
    }

    void set_para_value(bool sign,int stationid,qint64 day)
    {
        m_pInsertMachineSign = sign;
        m_pStationID = stationid;
        m_pSpecifyDay = day;
    }

private:
    CollectInsertMachineErrorsThread();

protected:
    void run();
    static CollectInsertMachineErrorsThread* m_pInstance;
private:
    bool m_pInsertMachineSign;
    int m_pStationID;
    qint64 m_pSpecifyDay;
signals:
    void signal_insert_errors_to_db(QStringList lstInfo);
    void signal_send_errors_to_ui(QList<QStringList> mapInfo);
};

#endif // COLLECTINSERTMACHINEERRORSTHREAD_H
