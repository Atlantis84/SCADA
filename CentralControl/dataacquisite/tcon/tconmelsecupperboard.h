#ifndef TCONMELSECUPPERBOARD_H
#define TCONMELSECUPPERBOARD_H

#include <QObject>
//#include "qmelsec.h"
#include <QTimer>
#include "dataacquisitiondefines.h"
#include <QThread>
#include "dmdbaccessinterface.h"
#include <qmelsecudpclient.h>
#include <mqservice.h>

#define TCON_UP_BOARD_NUMBER_OF_PLATES      (0)
#define TCON_UP_BOARD_WIDTH                 (1)
#define TCON_UP_BOARD_DEV_STATE             (2)
#define TCON_UP_BOARD_DEV_FREE              (3)
#define TCON_UP_BOARD_ALARM                 (4)

namespace UpperBoardData {
    const int counts = 5;
    static MelsecData config[counts]={
        {TCON_UP_BOARD_NUMBER_OF_PLATES,  32238,"D",2},
        {TCON_UP_BOARD_WIDTH, 974,"D",1},
        {TCON_UP_BOARD_DEV_STATE,0,"D",1},
        {TCON_UP_BOARD_DEV_FREE, 43,"X",1},
        {TCON_UP_BOARD_ALARM,     600,"R",1}
};
}

#define TCON_UP_DEV_NO_A                   (1)
#define TCON_UP_DEV_NO_B                   (2)

#define TCON_UP_MC_CODE                    (20)
#define TCON_UP_MC_ADDR                    ("DT")
#define TCON_UP_MC_LENGTH                  (11+4)

class tconMelsecUpperBoard : public QThread
{
Q_OBJECT
public:
    tconMelsecUpperBoard(int devNo);

    static tconMelsecUpperBoard* get_instance(int devNo)
    {
        if(m_pObj == nullptr)
            m_pObj = new tconMelsecUpperBoard(devNo);
        return m_pObj;
    }

    void connectToPLC(const QString ipPath, const quint16 port, const quint16 local_port = 0);

    //开始启动采集控制
    void isStartUpperBoard();
    void isStopUpperBoard();
    void insert_data_to_db();
    void load_data_from_db();

public slots:
    void timerExp();
    void slot_recv_melsec_udp_data(const QByteArray data_array);

private:
    static tconMelsecUpperBoard* m_pObj;

    QTimer *m_pTimer = nullptr;
    bool isEnabled = false;
    //QMelsec* m_pMelsecUpperBoard;
    QMap<int, QString> m_pTconUpStationDataTable;
    QMap<int, QString>m_getDBWarnAddrValues;

    DMDBAccessInterface *m_pgsq_data;
    QString dev_name;
    int m_nDevNo;

    MQService* m_pMqSerViceSrc;

    QMelsecUdpClient *m_melsec_udp_client;

signals:
    // QThread interface
    void signal_ui_warnning(QList<QString> lstInfo,int leftorright);
    void signal_ui_equipment_state(int state,int leftorright);
protected:
    void run();
};

#endif // TCONMELSECUPPERBOARD_H
