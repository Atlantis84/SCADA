#include "upboardwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "gdatafactory.h"
UpBoardWgt* UpBoardWgt::m_pUpBoardWgt = nullptr;
UpBoardWgt::UpBoardWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    QHBoxLayout* hAll = new QHBoxLayout();

    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"上板机(左)");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox1->addWidget(GDataFactory::get_left_up_board_wgt());
    connect(this,SIGNAL(signal_update_left_ui_state(int)),GDataFactory::get_left_up_board_wgt(),SLOT(slot_left_update_ui_info_state(int)));
    connect(this,SIGNAL(signal_update_left_ui_warnings(QList<QString>)),GDataFactory::get_left_up_board_wgt(),SLOT(slot_left_update_ui_info_warnings(QList<QString>)));

    grpBox1->setLayout(vBox1);
    hAll->addWidget(grpBox1);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    QGroupBox* grpBox2 = new QGroupBox(u8"上板机(右)");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox2->addWidget(GDataFactory::get_right_up_board_wgt());
    connect(this,SIGNAL(signal_update_right_ui_state(int)),GDataFactory::get_right_up_board_wgt(),SLOT(slot_right_update_ui_info_state(int)));
    connect(this,SIGNAL(signal_update_right_ui_warnings(QList<QString>)),GDataFactory::get_right_up_board_wgt(),SLOT(slot_right_update_ui_info_warnings(QList<QString>)));

    grpBox2->setLayout(vBox2);
    hAll->addWidget(grpBox2);

    vAll->addLayout(hAll);

    this->setLayout(vAll);
}

void UpBoardWgt::slot_warnings(QList<QString> lstInfo, int leftorright)
{
    if(leftorright == 1)
        emit signal_update_left_ui_warnings(lstInfo);
    else
        emit signal_update_right_ui_warnings(lstInfo);
}

void UpBoardWgt::slot_equipment_state(int state, int leftorright)
{
    if(leftorright == 1)
        emit signal_update_left_ui_state(state);
    else
        emit signal_update_right_ui_state(state);
}
