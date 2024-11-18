#ifndef LEFTDIVIDEBOARDWGT_H
#define LEFTDIVIDEBOARDWGT_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "ballwidget.h"
#include <QLineEdit>
class LeftDivideBoardWgt : public QWidget
{
    Q_OBJECT

public:
    static LeftDivideBoardWgt* get_instance()
    {
        if(m_pLeftDivideBoardWgt == nullptr)
            m_pLeftDivideBoardWgt = new LeftDivideBoardWgt();
        return m_pLeftDivideBoardWgt;
    }
private:
    explicit LeftDivideBoardWgt(QWidget *parent = nullptr);
private:
    QListWidget* m_pListWgt;
    BallWidget* m_pEquipState;
    QLineEdit* m_pLineEditTimes;

    static LeftDivideBoardWgt* m_pLeftDivideBoardWgt;


signals:

public slots:
    void slot_update_left_divide_ui_state(int status);
    void slot_update_left_divide_ui_warnings(QList<QString> lstInfo);
    void slot_update_left_divide_ui_key_part_action_times(int times);
};

#endif // LEFTDIVIDEBOARDWGT_H
