#ifndef LEFTUPBOARDWGT_H
#define LEFTUPBOARDWGT_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "ballwidget.h"
class LeftUpBoardWgt : public QWidget
{
    Q_OBJECT

public:
    static LeftUpBoardWgt* get_instance()
    {
        if(m_pLeftUpBoardWgt == nullptr)
            m_pLeftUpBoardWgt = new LeftUpBoardWgt();
        return m_pLeftUpBoardWgt;
    }

private:
    explicit LeftUpBoardWgt(QWidget *parent = nullptr);

private:
    QListWidget* m_pListWgt;
    BallWidget* m_pEquipState;

    static LeftUpBoardWgt* m_pLeftUpBoardWgt;

signals:

public slots:
    void slot_left_update_ui_info_state(int status);
    void slot_left_update_ui_info_warnings(QList<QString> lstInfo);
};

#endif // LEFTUPBOARDWGT_H
