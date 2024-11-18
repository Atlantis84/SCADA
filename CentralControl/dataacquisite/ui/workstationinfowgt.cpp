#include "workstationinfowgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
WorkStationInfoWgt::WorkStationInfoWgt(QWidget *parent,QString name) : QWidget(parent)
{
    m_pLineEditOKs = new QLineEdit();
    m_pLineEditNGs = new QLineEdit();
    m_pLineEditStatus = new QLineEdit();

    QList<QLineEdit*> lstEe;
    lstEe<<m_pLineEditOKs<<m_pLineEditNGs<<m_pLineEditStatus;
    for (int i=0;i<3;i++) {
        lstEe[i]->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                                "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
        lstEe[i]->setReadOnly(true);
        lstEe[i]->setAlignment(Qt::AlignCenter);
    }

    QVBoxLayout* vAll = new QVBoxLayout();

    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpBox = new QGroupBox(name);
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addWidget(new QLabel(u8"工装状态:"));

    hBox1->addWidget(m_pLineEditStatus);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    if(name.contains(u8"工装"))
        hBox2->addWidget(new QLabel(u8"  OK数量:"));
    else
        hBox2->addWidget(new QLabel(u8"设定值:"));
    hBox2->addWidget(m_pLineEditOKs);

    QHBoxLayout* hBox3 = new QHBoxLayout();
    if(name.contains(u8"工装"))
        hBox3->addWidget(new QLabel(u8"  NG数量:"));
    else
        hBox3->addWidget(new QLabel(u8"  PV值:"));
    hBox3->addWidget(m_pLineEditNGs);

//    vBox1->addLayout(hBox1);
    vBox1->addLayout(hBox2);
    vBox1->addLayout(hBox3);
    grpBox->setLayout(vBox1);

    vAll->addWidget(grpBox);
    vAll->setMargin(0);

    this->setLayout(vAll);
}
