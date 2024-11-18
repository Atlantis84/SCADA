#include "rightdivideboardwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
RightDivideBoardWgt* RightDivideBoardWgt::m_pRightDivideBoardWgt = nullptr;
RightDivideBoardWgt::RightDivideBoardWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");
//    m_pListWgt->addItem(u8"安全门报警");
    m_pEquipState = new BallWidget();
    m_pLineEditTimes = new QLineEdit();
    m_pLineEditTimes->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                      "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                                      "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pLineEditTimes->setReadOnly(true);

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    hBox1->addWidget(new QLabel(u8"当前设备状态:"));
    hBox1->addWidget(m_pEquipState);
    grpBox1->setLayout(hBox1);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QGroupBox* grpBox2 = new QGroupBox(u8"关键部件动作次数");
    hBox2->addWidget(new QLabel(u8"走刀使用次数:"));
    hBox2->addWidget(m_pLineEditTimes);
    grpBox2->setLayout(hBox2);

    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpBox3 = new QGroupBox(u8"报警信息");
    vBox1->addWidget(m_pListWgt);
    grpBox3->setLayout(vBox1);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);
    vAll->addWidget(grpBox3);

    this->setLayout(vAll);
}

void RightDivideBoardWgt::slot_update_right_divide_ui_state(int status)
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

void RightDivideBoardWgt::slot_update_right_divide_ui_warnings(QList<QString> lstInfo)
{
    for (int i=0;i<lstInfo.length();i++)
        m_pListWgt->addItem(lstInfo[i]);
}

void RightDivideBoardWgt::slot_update_right_divide_ui_key_part_action_times(int times)
{
    m_pLineEditTimes->setText(QString::number(times));
}
