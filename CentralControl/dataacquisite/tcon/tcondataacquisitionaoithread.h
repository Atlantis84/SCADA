#ifndef TCONDATAACQUISITIONTHREAD_H
#define TCONDATAACQUISITIONTHREAD_H

#include <QObject>
#include <QThread>
class TconDataAcquisitionAOIThread : public QThread
{
    Q_OBJECT
public:
    TconDataAcquisitionAOIThread();
protected:
    void run();
};

#endif // TCONDATAACQUISITIONTHREAD_H
