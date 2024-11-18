#include "wavesolderingwgt.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "QsLog.h"
#include "gdatafactory.h"
WaveSolderingWgt::WaveSolderingWgt(QWidget *parent) : QWidget(parent)
{
    m_pEquipStatus = new BallWidget();
    m_pTcpConnectStatus = new BallWidget();
    m_pLineEditOKQuantity = new QLineEdit();
    m_pLineEditOKQuantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pLineEditOKQuantity->setAlignment(Qt::AlignCenter);
    m_pLineEditOKQuantity->setReadOnly(true);


    m_pWave1SV = new QLineEdit();
    m_pWave1SV->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                              "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                              "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pWave1SV->setAlignment(Qt::AlignCenter);
    m_pWave1SV->setReadOnly(true);
    m_pWave2SV = new QLineEdit();
    m_pWave2SV->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                              "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                              "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pWave2SV->setAlignment(Qt::AlignCenter);
    m_pWave2SV->setReadOnly(true);

    m_pPlolderSpd = new WorkStationInfoWgt(nullptr,u8"链速");
    m_pB1PreHeat = new WorkStationInfoWgt(nullptr,u8"底部预热1");
    m_pB2PreHeat = new WorkStationInfoWgt(nullptr,u8"底部预热2");
    m_pB3PreHeat = new WorkStationInfoWgt(nullptr,u8"底部预热3");
    m_pFBPreHeat = new WorkStationInfoWgt(nullptr,u8"喷雾区底部预热");
    m_pT1PreHeat = new WorkStationInfoWgt(nullptr,u8"顶部预热1");
    m_pT2PreHeat = new WorkStationInfoWgt(nullptr,u8"顶部预热2");
    m_pT3PreHeat = new WorkStationInfoWgt(nullptr,u8"顶部预热3");
    m_pFTPreHeat = new WorkStationInfoWgt(nullptr,u8"喷雾区顶部预热");
    m_pSolderPot = new WorkStationInfoWgt(nullptr,u8"锡炉");

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");
//    m_pListWgt->addItem(u8"安全门报警");

    QVBoxLayout* vAll = new QVBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addStretch();
    hBox1->addWidget(new QLabel(u8"Tcp连接状态:"));
    hBox1->addWidget(m_pTcpConnectStatus);
    hBox1->addSpacing(50);
    hBox1->addWidget(new QLabel(u8" 设备状态:"));
    hBox1->addWidget(m_pEquipStatus);
    hBox1->addStretch();
    grpBox1->setLayout(hBox1);

    QGroupBox* grpBox2 = new QGroupBox(u8"设备产量");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(new QLabel(u8"当前产量:"));
    hBox2->addWidget(m_pLineEditOKQuantity);
    grpBox2->setLayout(hBox2);

    QGroupBox* grpBox3 = new QGroupBox(u8"报警信息");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox3 = new QHBoxLayout();
    hBox3->addWidget(m_pListWgt);
    grpBox3->setLayout(hBox3);

    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpBox4 = new QGroupBox(u8"关键参数信息");
    grpBox4->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");

    QHBoxLayout* hBox4 = new QHBoxLayout();
    QGroupBox* grpBoxPara1 = new QGroupBox(u8"波峰马达设定值");
    grpBoxPara1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox11 = new QVBoxLayout();
    QLabel* labelWave1SV = new QLabel(u8"波峰1(Hz):");
    QLabel* labelWave2SV = new QLabel(u8"波峰2(Hz):");
    QHBoxLayout* hBox11 = new QHBoxLayout();
    hBox11->addWidget(labelWave1SV);
    hBox11->addWidget(m_pWave1SV);
    QHBoxLayout* hBox12 = new QHBoxLayout();
    hBox12->addWidget(labelWave2SV);
    hBox12->addWidget(m_pWave2SV);
    vBox11->addLayout(hBox11);
    vBox11->addLayout(hBox12);
    grpBoxPara1->setLayout(vBox11);
    hBox4->addWidget(grpBoxPara1);
    hBox4->addWidget(m_pPlolderSpd);
    hBox4->addWidget(m_pSolderPot);
//    hBox4->addWidget(m_pFBPreHeat);

    QHBoxLayout* hBox5 = new QHBoxLayout();
    hBox5->addWidget(m_pB1PreHeat);
    hBox5->addWidget(m_pB2PreHeat);
    hBox5->addWidget(m_pB3PreHeat);
//    hBox5->addWidget(m_pFTPreHeat);

//    QHBoxLayout* hBox6 = new QHBoxLayout();
//    hBox6->addWidget(m_pT1PreHeat);
//    hBox6->addWidget(m_pT2PreHeat);
//    hBox6->addWidget(m_pT3PreHeat);
//    hBox6->addWidget(m_pSolderPot);

    vBox1->addLayout(hBox4);
    vBox1->addLayout(hBox5);
//    vBox1->addLayout(hBox6);
    grpBox4->setLayout(vBox1);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);
    vAll->addWidget(grpBox4);
    vAll->addWidget(grpBox3);

    this->setLayout(vAll);
}
