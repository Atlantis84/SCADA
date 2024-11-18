#ifndef LINEOVERVIEWWGT_H
#define LINEOVERVIEWWGT_H

#include <QObject>
#include <QWidget>
#include "ballwidget.h"
#include <QLineEdit>
class LineOverViewWgt : public QWidget
{
    Q_OBJECT
public:
    static LineOverViewWgt* get_instance()
    {
        if(m_pLineOverViewWgt == nullptr)
            m_pLineOverViewWgt = new LineOverViewWgt();
        return m_pLineOverViewWgt;
    }
private:
    explicit LineOverViewWgt(QWidget *parent = nullptr);
    static LineOverViewWgt* m_pLineOverViewWgt;

    BallWidget* m_pBallWgtLineState;
    QLineEdit* m_pLineEditAlarmTimes;
    QLineEdit* m_pLineEditProcessState;
    QLineEdit* m_pLineEditLineNumber;
    QLineEdit* m_pLineEditOperateMode;

    BallWidget* m_pBallWgtSB;
    BallWidget* m_pBallWgtFB;
    BallWidget* m_pBallWgtCS;
    BallWidget* m_pBallWgtAOI;
    BallWidget* m_pBallWgtBP;

    QLineEdit* m_pLineEditProjId;
    QLineEdit* m_pLineEditSchedQuantity;
    QLineEdit* m_pLineEditProduceQuantity;
    QLineEdit* m_pLineEditPassRate;

signals:

public slots:
};

#endif // LINEOVERVIEWWGT_H
