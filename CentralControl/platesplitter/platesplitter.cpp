#include "platesplitter.h"
#include "TitleBar.h"
#include <QVBoxLayout>
#include <QPainter>
#include "gdatafactory.h"
#include <QGroupBox>
PlateSplitter::PlateSplitter(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget|Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/icon/24x24.png"));
//    resize(1100,500);
//    TitleBar *pTitleBar = new TitleBar(this);
//    pTitleBar->setTitle(u8"分板机上下料");
//    pTitleBar->setFixedWidth(1100);
//    installEventFilter(pTitleBar);

    m_pLogEdit = new QTextEdit();
    m_pLogEdit->setReadOnly(true);
    m_pLogEdit->setStyleSheet("color:rgb(0,0,0);background-color:rgb(0,0,0);font: 10pt 'LiSu'");
    QGroupBox* grpLog = new QGroupBox(u8"实时日志");
    grpLog->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(m_pLogEdit);
    grpLog->setLayout(vBox1);

    m_pPassWgt = new PassWidget();
    QGroupBox* grpResult = new QGroupBox(u8"Mes过站结果");
    grpResult->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addWidget(m_pPassWgt);
    grpResult->setLayout(vBox2);

    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addWidget(grpResult);
//    hBox1->addWidget(grpLog);
//    hBox1->setStretch(0,1);
//    hBox1->setStretch(1,1);

    QVBoxLayout* vAll = new QVBoxLayout();
//    vAll->addSpacing(pTitleBar->height());
    vAll->addLayout(hBox1);
    this->setLayout(vAll);
}

void PlateSplitter::slot_rev_logs(const QString msg)
{
    this->m_pLogEdit->append(msg);
}

void PlateSplitter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void PlateSplitter::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pTimerID = startTimer(1000);
}

void PlateSplitter::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    killTimer(m_pTimerID);
    m_pTimerID = -1;
}

void PlateSplitter::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        GDataFactory::get_fins_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D10010,0x02,nullptr);
    }
}
