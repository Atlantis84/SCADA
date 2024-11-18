#ifndef TCONDATAACQUISITIONPACKAGETHREAD_H
#define TCONDATAACQUISITIONPACKAGETHREAD_H

#include <QObject>
#include <QThread>
class TconDataAcquisitionPackageThread : public QThread
{
    Q_OBJECT
public:
    TconDataAcquisitionPackageThread();
protected:
    void run();
};

#endif // TCONDATAACQUISITIONPACKAGETHREAD_H
