#ifndef SCANNERSERVICE_H
#define SCANNERSERVICE_H

#include <QList>
#include <QString>
#include <QObject>
#include <QRunnable>
#include <QSerialPort>
#include <QQueue>
#include <QMutex>
#include <QDateTime>

class ScannerService : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit ScannerService();
signals:
    void scannerQueueChanged(QString codesDesc,bool isLA);
    void scannerGetNewCode(QString code,bool isLA);
public:
    void run();
    void taskSleep(int sleepTimeMs);
    void startService();
    void stopService();
    bool isStarted();
    bool isStoped();
    static bool openSerialPort(QSerialPort* com,QString portName,int baudRate,int dataBits,QString parityBit,int stopBits);
    static bool closeSerialPort(QSerialPort* com);
    QString getLastLACode();
    QString getLastLBCode();
    void showScannerQueueDesc();
    void clearQueue();
    void clearQueueLA();
    void clearQueueLB();
private:
    bool started=false;
    bool stoped=true;
    QMutex lastLACodesMutex;
    QMutex lastLBCodesMutex;
    QQueue<QString> lastLACodes;
    QQueue<QString> lastLBCodes;
    QString lastLACode="";
    QString lastLBCode="";
    QDateTime lastLACodeTime;
    QDateTime lastLBCodeTime;
};

#endif // SCANNERSERVICE_H
