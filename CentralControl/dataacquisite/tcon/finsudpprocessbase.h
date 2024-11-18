#ifndef FINSUDPPROCESSBASE_H
#define FINSUDPPROCESSBASE_H

#include <QObject>

class FinsUDPProcessBase : public QObject
{
    Q_OBJECT
public:
    explicit FinsUDPProcessBase(QObject *parent = nullptr);
    virtual void insert_data_to_db() =0;
    virtual void load_data_from_db()=0;
    virtual void process_warnings_data(QByteArray data)=0;

signals:

public slots:
    virtual void process_plc_data(QByteArray data) = 0;
};

#endif // FINSUDPPROCESSBASE_H
