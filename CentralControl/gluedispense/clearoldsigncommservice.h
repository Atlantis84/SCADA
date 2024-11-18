#ifndef CLEAROLDSIGNCOMMSERVICE_H
#define CLEAROLDSIGNCOMMSERVICE_H

#include <QObject>
#include <QSerialPort>

class ClearOldSignCommService : public QObject
{
    Q_OBJECT
public:
    explicit ClearOldSignCommService(QObject *parent = nullptr);
    void connect_to_mcu_port(QString port_name);
    void sendClearOldSignCommand();

signals:

public slots:
    void slot_rev_serial_number();

private:
    QSerialPort* m_serial = nullptr;
    bool m_is_sync = false;
    const uchar CMD_END[2] = {0xCC,0xDD};//帧尾

    bool checkClearOldSignResult(QByteArray data);
};

#endif // CLEAROLDSIGNCOMMSERVICE_H
