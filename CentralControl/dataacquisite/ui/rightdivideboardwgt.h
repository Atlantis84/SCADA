#ifndef RIGHTDIVIDEBOARDWGT_H
#define RIGHTDIVIDEBOARDWGT_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "ballwidget.h"
#include <QLineEdit>
class RightDivideBoardWgt : public QWidget
{
    Q_OBJECT
public:
    static RightDivideBoardWgt* get_instance()
    {
        if(m_pRightDivideBoardWgt == nullptr)
            m_pRightDivideBoardWgt = new RightDivideBoardWgt();
        return m_pRightDivideBoardWgt;
    }
private:
    explicit RightDivideBoardWgt(QWidget *parent = nullptr);
private:
    QListWidget* m_pListWgt;
    BallWidget* m_pEquipState;
    QLineEdit* m_pLineEditTimes;

    static RightDivideBoardWgt* m_pRightDivideBoardWgt;

signals:

public slots:
    void slot_update_right_divide_ui_state(int status);
    void slot_update_right_divide_ui_warnings(QList<QString> lstInfo);
    void slot_update_right_divide_ui_key_part_action_times(int times);
};

#endif // RIGHTDIVIDEBOARDWGT_H
