#ifndef RIGHTUPBOARDWGT_H
#define RIGHTUPBOARDWGT_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "ballwidget.h"
class RightUpBoardWgt : public QWidget
{
    Q_OBJECT
public:
    static RightUpBoardWgt* get_instance()
    {
        if(m_pRightUpBoardWgt == nullptr)
            m_pRightUpBoardWgt = new RightUpBoardWgt();
        return m_pRightUpBoardWgt;
    }

private:
    explicit RightUpBoardWgt(QWidget *parent = nullptr);

private:
    QListWidget* m_pListWgt;
    BallWidget* m_pEquipState;

    static RightUpBoardWgt* m_pRightUpBoardWgt;

signals:

public slots:
    void slot_right_update_ui_info_state(int status);
    void slot_right_update_ui_info_warnings(QList<QString> lstInfo);
};

#endif // RIGHTUPBOARDWGT_H
