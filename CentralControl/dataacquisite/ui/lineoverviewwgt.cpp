#include "lineoverviewwgt.h"
#include "finsudpprocessbase.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QGroupBox>
#include "linestatewidget.h"
#include <QLineEdit>
LineOverViewWgt* LineOverViewWgt::m_pLineOverViewWgt = nullptr;

//int count_one(unsigned long n)
//{
//    n = ((n & 0xAAAAAAAA)>>1) + (n & 0x55555555);
//    n = ((n & 0xCCCCCCCC)>>2) + (n & 0x33333333);
//    n = ((n & 0xF0F0F0))
//}

LineOverViewWgt::LineOverViewWgt(QWidget *parent) : QWidget(parent)
{
    QList<QLineEdit*> lst;
    m_pBallWgtSB = new BallWidget();
    m_pBallWgtSB->set_color(0);
    m_pBallWgtFB = new BallWidget();
    m_pBallWgtFB->set_color(1);
    m_pBallWgtCS = new BallWidget();
    m_pBallWgtCS->set_color(0);
    m_pBallWgtAOI = new BallWidget();
    m_pBallWgtAOI->set_color(1);
    m_pBallWgtBP = new BallWidget();
    m_pBallWgtBP->set_color(0);
    m_pBallWgtLineState = new BallWidget();
    m_pBallWgtLineState->set_color(1);

    m_pLineEditProjId = new QLineEdit();
    m_pLineEditSchedQuantity = new QLineEdit();
    m_pLineEditProduceQuantity = new QLineEdit();
    m_pLineEditPassRate = new QLineEdit();
    m_pLineEditAlarmTimes = new QLineEdit();
    m_pLineEditProcessState = new QLineEdit();
    m_pLineEditLineNumber = new QLineEdit();
    m_pLineEditOperateMode = new QLineEdit();

    lst<<m_pLineEditProjId<<m_pLineEditSchedQuantity<<m_pLineEditProduceQuantity<<
         m_pLineEditPassRate<<m_pLineEditAlarmTimes<<m_pLineEditProcessState<<
         m_pLineEditLineNumber<<m_pLineEditOperateMode;
    for (int i=0;i<lst.size();i++) {
        lst[i]->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                          "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                                          "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
        lst[i]->setReadOnly(true);
    }

    QVBoxLayout* vAll = new QVBoxLayout();

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"线体组成示意图");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    LineStateWidget* lineStateWgt = new LineStateWidget();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(lineStateWgt);
    grpBox1->setLayout(vBox1);
    hBox1->addWidget(grpBox1);

    QGroupBox* grpBox2 = new QGroupBox(u8"线体状态");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox01 = new QHBoxLayout();
    hBox01->addSpacing(50);
    QLabel* labelLineNumber = new QLabel(u8"线体编号:");
    hBox01->addWidget(labelLineNumber);
    hBox01->addWidget(m_pLineEditLineNumber);
    QHBoxLayout* hBox02 = new QHBoxLayout();
    hBox02->addSpacing(50);
    QLabel* labelOperateMode = new QLabel(u8"操作模式:");
    hBox02->addWidget(labelOperateMode);
    hBox02->addWidget(m_pLineEditOperateMode);
    QHBoxLayout* hBox03 = new QHBoxLayout();
    hBox03->addSpacing(50);
    QLabel* labelRunMode = new QLabel(u8"运行状态:");
    hBox03->addWidget(labelRunMode);
    hBox03->addWidget(m_pBallWgtLineState);
    QHBoxLayout* hBox04 = new QHBoxLayout();
    hBox04->addSpacing(50);
    QLabel* labelAlarmTimes = new QLabel(u8"报警次数:");
    hBox04->addWidget(labelAlarmTimes);
    hBox04->addWidget(m_pLineEditAlarmTimes);
    QHBoxLayout* hBox05 = new QHBoxLayout();
    hBox05->addSpacing(50);
    QLabel* labelProduceState = new QLabel(u8"加工状态:");
    hBox05->addWidget(labelProduceState);
    hBox05->addWidget(m_pLineEditProcessState);
    hBox01->addSpacing(50);
    hBox02->addSpacing(50);
    hBox03->addSpacing(200);
    hBox04->addSpacing(50);
    hBox05->addSpacing(50);
    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addLayout(hBox01);
    vBox2->addLayout(hBox02);
    vBox2->addLayout(hBox03);
    vBox2->addLayout(hBox04);
    vBox2->addLayout(hBox05);
    grpBox2->setLayout(vBox2);
    hBox1->addWidget(grpBox2);

    hBox1->setStretch(0,1);
    hBox1->setStretch(1,1);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QGroupBox* grpBox3 = new QGroupBox(u8"运行状态");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox3 = new QVBoxLayout();
    QHBoxLayout* hBox11 = new QHBoxLayout();
    hBox11->addSpacing(100);
    QLabel* labelSBState = new QLabel(u8"  上板机运行状态:");
    hBox11->addWidget(labelSBState);
    hBox11->addWidget(m_pBallWgtSB);
    QHBoxLayout* hBox12 = new QHBoxLayout();
    hBox12->addSpacing(100);
    QLabel* labelFBState = new QLabel(u8"  分板机运行状态:");
    hBox12->addWidget(labelFBState);
    hBox12->addWidget(m_pBallWgtFB);
    QHBoxLayout* hBox13 = new QHBoxLayout();
    hBox13->addSpacing(100);
    QLabel* labelTestState = new QLabel(u8"测试机台运行状态:");
    hBox13->addWidget(labelTestState);
    hBox13->addWidget(m_pBallWgtCS);
    QHBoxLayout* hBox14 = new QHBoxLayout();
    hBox14->addSpacing(100);
    QLabel* labelAOIState = new QLabel(u8"    AOI运行状态:");
    hBox14->addWidget(labelAOIState);
    hBox14->addWidget(m_pBallWgtAOI);
    QHBoxLayout* hBox15 = new QHBoxLayout();
    hBox15->addSpacing(100);
    QLabel* labelBPState = new QLabel(u8"  摆盘机运行状态:");
    hBox15->addWidget(labelBPState);
    hBox15->addWidget(m_pBallWgtBP);
    hBox11->addSpacing(100);
    hBox12->addSpacing(100);
    hBox13->addSpacing(100);
    hBox14->addSpacing(100);
    hBox15->addSpacing(100);
    vBox3->addLayout(hBox11);
    vBox3->addLayout(hBox12);
    vBox3->addLayout(hBox13);
    vBox3->addLayout(hBox14);
    vBox3->addLayout(hBox15);
    grpBox3->setLayout(vBox3);
    hBox2->addWidget(grpBox3);

    QGroupBox* grpBox4 = new QGroupBox(u8"生产数据");
    grpBox4->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox4 = new QVBoxLayout();
    QHBoxLayout* hBox21 = new QHBoxLayout();
    QLabel* labelProjId = new QLabel(u8"  当前工单:");
    hBox21->addWidget(labelProjId);
    hBox21->addWidget(m_pLineEditProjId);
    QHBoxLayout* hBox22 = new QHBoxLayout();
    QLabel* labelSchedQuantity = new QLabel(u8"  计划产量:");
    hBox22->addWidget(labelSchedQuantity);
    hBox22->addWidget(m_pLineEditSchedQuantity);
    QHBoxLayout* hBox23 = new QHBoxLayout();
    QLabel* labelProduceQuantity = new QLabel(u8"  生产产量:");
    hBox23->addWidget(labelProduceQuantity);
    hBox23->addWidget(m_pLineEditProduceQuantity);
    QHBoxLayout* hBox24 = new QHBoxLayout();
    QLabel* labelPassRate = new QLabel(u8"一次通过率:");
    hBox24->addWidget(labelPassRate);
    hBox24->addWidget(m_pLineEditPassRate);
    vBox4->addLayout(hBox21);
    vBox4->addLayout(hBox22);
    vBox4->addLayout(hBox23);
    vBox4->addLayout(hBox24);
    grpBox4->setLayout(vBox4);
    hBox2->addWidget(grpBox4);

    hBox2->setStretch(0,1);
    hBox2->setStretch(1,1);

    vAll->addLayout(hBox1);
    vAll->addLayout(hBox2);

    this->setLayout(vAll);
}
