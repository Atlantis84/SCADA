#ifndef PLCBUSINESSM201OBJ_H
#define PLCBUSINESSM201OBJ_H

#include <QObject>
#include <QMap>
#include "dataacquisitiondefines.h"
#include "qmqttclient.h"
#include <mutex>
#include <QMutex>
class PlcBusinessM201Obj : public QObject
{
    Q_OBJECT
public:
    static PlcBusinessM201Obj* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new PlcBusinessM201Obj();
        return m_pInstance;
    }
private:
    explicit PlcBusinessM201Obj(QObject *parent = nullptr);

    static PlcBusinessM201Obj* m_pInstance;

    int m_pUpBoardStatus;
    int m_pUpUtensilStatus;
    int m_pPickUpCoverStatus;
    int m_pDownUtensilStatus;

//    QMutex mutex_up_board;

    QMap<int,QString> m_pMapWarningsInfoOfUpBoard;
    QMap<int,QString> m_pMapWarningsInfoOfUtensil;
    QMap<int,QString> m_pMapWarningsInfoOfPickUpCover;
    QMap<int,QString> m_pMapWarningsInfoOfDownUtensil;

    void load_data_from_db();
    bool find_warnings_of_up_board(const int warningid);
    bool find_warnings_of_up_utensil(const int warningid);

    bool find_warnings_of_pick_up_cover(const int warningid);
    bool find_warnings_of_down_utensil(const int warningid);

protected:
    void timerEvent(QTimerEvent* event);
    int m_pUpBoardTimerID;
    int m_pUpUtensilTimerID;
    int m_pPickUpCoverTimerID;
    int m_pDownUtensilTimerID;

    QMqttClient* m_pUpBoardMqttClient;
    QMqttClient* m_pUpUtensilMqttClient;
    QMqttClient* m_pPickUpCoverMqttClient;
    QMqttClient* m_pDownUtensilMqttClient;

    QMap<DATA_TYPE_M201,QVariant> m_pUpBoardDataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pUpUtensilDataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pPickUpCoverDataTable;
    QMap<DATA_TYPE_M201,QVariant> m_pDownUtensilDataTable;

signals:

public slots:
    void slot_up_utensil_info(QVector<unsigned short> data);
    void slot_up_board_info(QVector<unsigned short> data);
    void slot_pick_up_cover_info(QVector<unsigned short> data);
    void slot_down_utensil_info(QVector<unsigned short> data);

    void updateLogStateChangeUpBoard();
    void brokerDisconnectedUpBoard();

    void updateLogStateChangeUpUtensil();
    void brokerDisconnectedUpUtensil();

    void updateLogStateChangePickUpCover();
    void brokerDisconnectedPickUpCover();

    void updateLogStateChangeDownUtensil();
    void brokerDisconnectedDownUtensil();
};

#endif // PLCBUSINESSM201OBJ_H
