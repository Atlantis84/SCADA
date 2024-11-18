#ifndef COMMONWAITINGWGT_H
#define COMMONWAITINGWGT_H
#include <QDialog>
class CommonWaitingWgt : public QDialog
{
    Q_OBJECT
public:
    explicit CommonWaitingWgt();

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent* event);

signals:

public slots:
    void slot_close();
};

#endif // COMMONWAITINGWGT_H
