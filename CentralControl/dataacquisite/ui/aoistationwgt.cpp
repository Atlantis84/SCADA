#include "aoistationwgt.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "QsLog.h"
AOIStationWgt* AOIStationWgt::m_pAOIStationWgt = nullptr;
AOIStationWgt::AOIStationWgt(QWidget *parent) : QWidget(parent)
{
    m_pEquipStatus = new BallWidget();
    m_pLineEditOKQuantity = new QLineEdit();
    m_pLineEditOKQuantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pLineEditNGQuantity = new QLineEdit();
    m_pLineEditNGQuantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");
    m_pListWgt->addItem(u8"安全门报警");

    QVBoxLayout* vAll = new QVBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addStretch();
    hBox1->addWidget(new QLabel(u8" 设备状态:"));
    hBox1->addWidget(m_pEquipStatus);
    hBox1->addStretch();
    grpBox1->setLayout(hBox1);

    QGroupBox* grpBox2 = new QGroupBox(u8"OK|NG数量");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(new QLabel(u8"当前OK数:"));
    hBox2->addWidget(m_pLineEditOKQuantity);
    hBox2->addWidget(new QLabel(u8"当前NG数量:"));
    hBox2->addWidget(m_pLineEditNGQuantity);
    grpBox2->setLayout(hBox2);

    QGroupBox* grpBox3 = new QGroupBox(u8"报警信息");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox3 = new QHBoxLayout();
    hBox3->addWidget(m_pListWgt);
    grpBox3->setLayout(hBox3);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);
    vAll->addWidget(grpBox3);

    this->setLayout(vAll);
}

void AOIStationWgt::slot_warnings(QList<QString> lstInfo)
{
    for (int i=0;i<lstInfo.length();i++)
        m_pListWgt->addItem(lstInfo[i]);
}

void AOIStationWgt::slot_ok_ng(QList<QString> lstInfo, int machineID)
{
    if(lstInfo.length() < 5)
    {
        QLOG_WARN()<<"the list length is abnormal";
        return;
    }
    //the total OKs
    int oks = lstInfo[0].toInt() + lstInfo[3].toInt();

    //the total NGs
    int ngs = lstInfo[1].toInt() + lstInfo[4].toInt();
    m_pLineEditOKQuantity->setText(QString::number(oks));
    m_pLineEditNGQuantity->setText(QString::number(ngs));
}

void AOIStationWgt::slot_equipment_state(QByteArray data)
{
    if(data[0] & 0x01)
        m_pEquipStatus->set_color(1);
    else
        m_pEquipStatus->set_color(0);
}
