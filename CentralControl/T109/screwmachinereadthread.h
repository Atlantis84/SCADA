#ifndef SCREWMACHINEREADTHREAD_H
#define SCREWMACHINEREADTHREAD_H
#include <QThread>
class ScrewMachineReadThread : public QThread
{
public:
    ScrewMachineReadThread();
    void set_sign(bool sign){m_pRunSign = sign;}
private:
    void run();
    static bool m_pRunSign;
};

#endif // SCREWMACHINEREADTHREAD_H
