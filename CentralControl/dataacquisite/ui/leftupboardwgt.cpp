#include "leftupboardwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
LeftUpBoardWgt* LeftUpBoardWgt::m_pLeftUpBoardWgt = nullptr;
LeftUpBoardWgt::LeftUpBoardWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");
//    m_pListWgt->addItem(u8"安全门报警");
    m_pEquipState = new BallWidget();

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    hBox1->addWidget(new QLabel(u8"当前设备状态:"));
    hBox1->addWidget(m_pEquipState);
    grpBox1->setLayout(hBox1);

    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpBox2 = new QGroupBox(u8"报警信息");
    vBox1->addWidget(m_pListWgt);
    grpBox2->setLayout(vBox1);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);

    this->setLayout(vAll);
}


void LeftUpBoardWgt::slot_left_update_ui_info_state(int status)
{
    if(status == 1)
        m_pEquipState->set_color(0);
    else if(status == 2)
        m_pEquipState->set_color(1);
    else if(status == 3)
        m_pEquipState->set_color(2);
    else
        m_pEquipState->set_color(0);
}

void LeftUpBoardWgt::slot_left_update_ui_info_warnings(QList<QString> lstInfo)
{
    for (int i=0;i<lstInfo.length();i++)
        m_pListWgt->addItem(lstInfo[i]);
}
