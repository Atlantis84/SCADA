#ifndef DIVIDEBOARDWGT_H
#define DIVIDEBOARDWGT_H

#include <QObject>
#include <QWidget>

class DivideBoardWgt : public QWidget
{
    Q_OBJECT
public:

    static DivideBoardWgt* get_instance()
    {
        if(m_pDivideBoardWgt == nullptr)
            m_pDivideBoardWgt = new DivideBoardWgt();
        return m_pDivideBoardWgt;
    }

private:
    explicit DivideBoardWgt(QWidget *parent = nullptr);
    static DivideBoardWgt* m_pDivideBoardWgt;

signals:
    void signal_left_divide_state(int status);
    void signal_left_divide_warnings(QList<QString> lstInfo);
    void signal_left_divide_times(int times);

    void signal_right_divide_state(int status);
    void signal_right_divide_warnings(QList<QString> lstInfo);
    void signal_right_divide_times(int times);


public slots:
    void slot_warnings(QList<QString> lstInfo,int leftorright);
    void slot_equipment_state(int state,int leftorright);
    void slot_key_part_action_times(int times,int leftorright);
};

#endif // DIVIDEBOARDWGT_H
