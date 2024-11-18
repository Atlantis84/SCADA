#include "divideboardwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "gdatafactory.h"
DivideBoardWgt* DivideBoardWgt::m_pDivideBoardWgt = nullptr;
DivideBoardWgt::DivideBoardWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    QHBoxLayout* hAll = new QHBoxLayout();

    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"分板机(左)");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox1->addWidget(GDataFactory::get_left_divide_board_wgt());
    connect(this,SIGNAL(signal_left_divide_state(int)),GDataFactory::get_left_divide_board_wgt(),SLOT(slot_update_left_divide_ui_state(int)));
    connect(this,SIGNAL(signal_left_divide_warnings(QList<QString>)),GDataFactory::get_left_divide_board_wgt(),SLOT(slot_update_left_divide_ui_warnings(QList<QString>)));
    connect(this,SIGNAL(signal_left_divide_times(int)),GDataFactory::get_left_divide_board_wgt(),SLOT(slot_update_left_divide_ui_key_part_action_times(int)));
    grpBox1->setLayout(vBox1);
    hAll->addWidget(grpBox1);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    QGroupBox* grpBox2 = new QGroupBox(u8"分板机(右)");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox2->addWidget(GDataFactory::get_right_divide_board_wgt());
    connect(this,SIGNAL(signal_right_divide_state(int)),GDataFactory::get_right_divide_board_wgt(),SLOT(slot_update_right_divide_ui_state(int)));
    connect(this,SIGNAL(signal_right_divide_warnings(QList<QString>)),GDataFactory::get_right_divide_board_wgt(),SLOT(slot_update_right_divide_ui_warnings(QList<QString>)));
    connect(this,SIGNAL(signal_right_divide_times(int)),GDataFactory::get_right_divide_board_wgt(),SLOT(slot_update_right_divide_ui_key_part_action_times(int)));

    grpBox2->setLayout(vBox2);
    hAll->addWidget(grpBox2);

    vAll->addLayout(hAll);

    this->setLayout(vAll);
}

void DivideBoardWgt::slot_warnings(QList<QString> lstInfo, int leftorright)
{
    if(leftorright == 1)
        emit signal_left_divide_warnings(lstInfo);
    else
        emit signal_right_divide_warnings(lstInfo);
}

void DivideBoardWgt::slot_equipment_state(int state, int leftorright)
{
    if(leftorright == 1)
        emit signal_left_divide_state(state);
    else
        emit signal_right_divide_state(state);
}

void DivideBoardWgt::slot_key_part_action_times(int times, int leftorright)
{
    if(leftorright == 1)
        emit signal_left_divide_times(times);
    else
        emit signal_right_divide_times(times);
}
