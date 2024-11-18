#ifndef STATIONCONFIGWGT_H
#define STATIONCONFIGWGT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
class StationConfigWgt : public QWidget
{
    Q_OBJECT
public:
    static StationConfigWgt* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new StationConfigWgt();
        return m_pObj;
    }
    void set_station_characters(int id)
    {
        m_pCharacterID = id;
    }
protected:

    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
private:
    explicit StationConfigWgt(QWidget *parent = nullptr);

    static StationConfigWgt* m_pObj;
    int m_pCharacterID;

    QLineEdit* m_pLineEditStationCode;
    QLineEdit* m_pLineEditEquipCode;
    QLineEdit* m_pLineEditEquipName;

signals:
    void signal_send_set_info(const int characterid,const QString EquipCode, const QString EquipName, const QString StationCode);
public slots:
    void slot_save();
};

#endif // STATIONCONFIGWGT_H
