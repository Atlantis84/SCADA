#ifndef UPBOARDWGT_H
#define UPBOARDWGT_H

#include <QObject>
#include <QWidget>

class UpBoardWgt : public QWidget
{
    Q_OBJECT
public:
    static UpBoardWgt* get_instance()
    {
        if(m_pUpBoardWgt == nullptr)
            m_pUpBoardWgt = new UpBoardWgt();
        return m_pUpBoardWgt;
    }
private:
    explicit UpBoardWgt(QWidget *parent = nullptr);
    static UpBoardWgt* m_pUpBoardWgt;
signals:

    void signal_update_left_ui_state(int status);
    void signal_update_right_ui_state(int status);

    void signal_update_left_ui_warnings(QList<QString> lstInfo);
    void signal_update_right_ui_warnings(QList<QString> lstInfo);
public slots:
    void slot_warnings(QList<QString> lstInfo,int leftorright);
    void slot_equipment_state(int state,int leftorright);
};

#endif // UPBOARDWGT_H
