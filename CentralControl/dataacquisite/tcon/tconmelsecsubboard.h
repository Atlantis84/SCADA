#ifndef TCONMELSECSUBBOARD_H
#define TCONMELSECSUBBOARD_H

#include <QObject>
//#include "qmelsec.h"
#include <QTimer>
#include "dataacquisitiondefines.h"
#include <QThread>
#include <mqservice.h>
#include <qmelsecudpclient.h>

#define TCON_SUB_MAJOR_PART                 (0)
#define TCON_SUB_STATE_RUN                  (1)
#define TCON_SUB_STATE_FREE                 (2)
#define TCON_SUB_URGENT_STOP                (3)
#define TCON_SUB_ALARM1                     (4)
#define TCON_SUB_ALARM2                     (5)
#define TCON_SUB_ALARM3                     (6)

#define TCON_SUB_DEV_NO_A                   (1)
#define TCON_SUB_DEV_NO_B                   (2)

namespace SubBoardData {
    const int counts = 8;
    static MelsecData config[counts]={
        {TCON_SUB_MAJOR_PART,        554,"D",2},
        {TCON_SUB_STATE_RUN,160,"M",1},
        {TCON_SUB_STATE_FREE, 131,"M",1},
        {TCON_SUB_URGENT_STOP, 301,"M", 1},
        {TCON_SUB_ALARM1,     130,"TS",1},
        {TCON_SUB_ALARM2,     146,"TS",1},
        {TCON_SUB_ALARM3,     162,"TS",2}
};
}

class tconMelsecSubBoard : public QThread
{
Q_OBJECT
public:
    tconMelsecSubBoard(int devNo);

    static tconMelsecSubBoard* get_instance(int devNo)
    {
        if(m_pObj == nullptr)
            m_pObj = new tconMelsecSubBoard(devNo);
        return m_pObj;
    }

    void connectToPLC(const QString ipPath, const quint16 port, const quint16 local_port = 0);
    void isStartSubBoard();//start data acquisition
    void isStopSubBoard();
    void insert_data_to_db();
    void load_data_from_db();

public slots:
    void timerExp();
    void slot_melsec_udp_data(const QByteArray data_array);

signals:
    void signal_ui_warnning(QList<QString> lstInfo,int leftorright);
    void signal_ui_equipment_state(int state,int leftorright);
    void signal_ui_key_part_times(int times,int leftorright);

private:
    static tconMelsecSubBoard *m_pObj;

    QTimer *m_pTimer = nullptr;
    bool isEnabled = false;
    //QMelsec* m_pMelsecSubBoard;
    QMap<int,QString> m_pTconSubStationDataTable;
    QMap<int, QString>m_getDBWarnAddrValues;

    MQService* m_pMqSerViceSrc;

    int m_nDevNo;
    QString dev_name;

    int beforeFreeState = -1;
    int currentFreeState = -1;
    int m_nFreeStateCount = 0;

    int data_type_id = -1;

    enum ORDER_RECV_ID{
        X0,
        X1,
        stop_state,
        run_state,
        free_state,
        key_move_count
    };

    DATA_VALUE dataStruct;

    QMelsecUdpClient *m_pMelsecUdpSubBoard;

    // QThread interface
protected:
    void run();
};

#endif // TCONMELSECSUBBOARD_H
