#include "teststationwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
TestStationWgt* TestStationWgt::m_pTestStationWgt = nullptr;
TestStationWgt::TestStationWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    m_pEquip1Status = new BallWidget();
    m_pEquip2Status = new BallWidget();
    m_pLineEditStation1Quantity = new QLineEdit();
    m_pLineEditStation1Quantity->setReadOnly(true);
    m_pLineEditStation1Quantity->setAlignment(Qt::AlignCenter);
    m_pLineEditStation2Quantity = new QLineEdit();
    m_pLineEditStation2Quantity->setReadOnly(true);
    m_pLineEditStation2Quantity->setAlignment(Qt::AlignCenter);
    m_pLineEditStation1Quantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pLineEditStation2Quantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");
//    m_pListWgt->addItem(u8"安全门报警");

    for (int i=1;i<11;i++) {
        if(i<10)
            m_pWorkStationInfoLst.push_back(new WorkStationInfoWgt(nullptr,QString(u8"工装0%1").arg(i)));
        else
            m_pWorkStationInfoLst.push_back(new WorkStationInfoWgt(nullptr,QString(u8"工装%1").arg(i)));
    }

    QVBoxLayout* vBox1 = new QVBoxLayout();

    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addStretch();
    hBox1->addWidget(new QLabel(u8"测试机台1状态:"));
    hBox1->addWidget(m_pEquip1Status);
    hBox1->addSpacing(80);
    hBox1->addWidget(new QLabel(u8"测试机台2状态:"));
    hBox1->addWidget(m_pEquip2Status);
    hBox1->addStretch();
    grpBox1->setLayout(hBox1);

    QGroupBox* grpBox2 = new QGroupBox(u8"工装信息");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox2 = new QHBoxLayout();

    QHBoxLayout* hBox11 = new QHBoxLayout();
    QGroupBox* grpBox3= new QGroupBox(u8"机台产量信息");
    hBox11->addWidget(new QLabel(u8"机台1产量:"));
    hBox11->addWidget(m_pLineEditStation1Quantity);
    QHBoxLayout* hBox22 = new QHBoxLayout();
    hBox22->addWidget(new QLabel(u8"机台2产量:"));
    hBox22->addWidget(m_pLineEditStation2Quantity);
    QVBoxLayout* vBox22 = new QVBoxLayout();
    vBox22->addLayout(hBox11);
    vBox22->addLayout(hBox22);
    grpBox3->setLayout(vBox22);

    hBox2->addWidget(grpBox3);

    for (int i=0;i<2;i++) {
        hBox2->addWidget(m_pWorkStationInfoLst[i]);
    }

    QHBoxLayout* hBox3 = new QHBoxLayout();
    for (int i=2;i<6;i++) {
        hBox3->addWidget(m_pWorkStationInfoLst[i]);
    }

    QHBoxLayout* hBox4 = new QHBoxLayout();
    for (int i=6;i<10;i++) {
        hBox4->addWidget(m_pWorkStationInfoLst[i]);
    }

    vBox1->addLayout(hBox2);
    vBox1->addLayout(hBox3);
    vBox1->addLayout(hBox4);
    grpBox2->setLayout(vBox1);

    QGroupBox* grpBox4 = new QGroupBox(u8"报警信息");
    grpBox4->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox5 = new QHBoxLayout();
    hBox5->addWidget(m_pListWgt);
    grpBox4->setLayout(hBox5);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);
    vAll->addWidget(grpBox4);
    this->setLayout(vAll);
}

void TestStationWgt::slot_warnings(QList<QString> lstInfo)
{
    for (int i=0;i<lstInfo.length();i++)
        m_pListWgt->addItem(lstInfo[i]);
}

void TestStationWgt::slot_ok_ng(QList<QString> lstInfo, int machineID)
{
    if(machineID == 31)//the test station1
    {
        int j=0;
        for (int i=0;i<lstInfo.length()-2;i++)
        {
            if(((i+1)%4) == 0)
            {
                m_pWorkStationInfoLst[j]->set_OKs(lstInfo[i-3]);
                m_pWorkStationInfoLst[j]->set_NGs(lstInfo[i-2]);
                j++;
            }
        }
        m_pLineEditStation1Quantity->setText(lstInfo[lstInfo.length()-3]);
    }
    else
    {
        int j=5;
        for (int i=0;i<lstInfo.length()-2;i++)
        {
            if(((i+1)%4) == 0)
            {
                m_pWorkStationInfoLst[j]->set_OKs(lstInfo[i-3]);
                m_pWorkStationInfoLst[j]->set_NGs(lstInfo[i-2]);
                j++;
            }
        }
        m_pLineEditStation2Quantity->setText(lstInfo[lstInfo.length()-3]);
    }
}

void TestStationWgt::slot_equipment_state(QByteArray data)
{
    if(data[0] & 0x01)
        m_pEquip1Status->set_color(1);
    if(data[0] & 0x02)
        m_pEquip1Status->set_color(2);
    if(data[0] & 0x04)
        m_pEquip1Status->set_color(0);
    if(data[0] & 0x08)
        m_pEquip2Status->set_color(1);
    if(data[0] & 0x10)
        m_pEquip2Status->set_color(2);
    if(data[0] & 0x20)
        m_pEquip2Status->set_color(0);
}
