#ifndef TCONDATAACQUISITONTHREAD_H
#define TCONDATAACQUISITONTHREAD_H

#include <QThread>
class TconDataAcquisitonThread : public QThread
{
    Q_OBJECT
public:
    TconDataAcquisitonThread();
protected:
    void run();

};

#endif // TCONDATAACQUISITONTHREAD_H
