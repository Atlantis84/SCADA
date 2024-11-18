#include "redflagtestcommandwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QDateTime>
#include "gdatafactory.h"
RedFlagTestCommandWgt* RedFlagTestCommandWgt::m_pInstance = nullptr;
RedFlagTestCommandWgt::RedFlagTestCommandWgt(QWidget *parent) : QWidget(parent)
{
    m_pPressStationSerialCom = new PressStationSerialCom();

    m_pLogTextEdit = new QTextEdit();
    m_pLogTextEdit->setStyleSheet("QTextEdit{background-color:rgba(255,255,255,100);color:rgb(0,0,0);font-family:KaiTi;font-size:20px;}");
    QVBoxLayout* vAll = new QVBoxLayout();
    QPushButton* btnToPress = new QPushButton(u8"通知工装进行压合");
    btnToPress->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,255);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    QPushButton* btnToManoMeter = new QPushButton(u8"读取压力表压力值");
    btnToManoMeter->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,255);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnToPress,SIGNAL(clicked()),this,SLOT(slot_btn_to_press()));
    connect(btnToManoMeter,SIGNAL(clicked()),this,SLOT(slot_btn_to_read()));

    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addWidget(btnToPress);
    hBox1->addWidget(btnToManoMeter);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(m_pLogTextEdit);

    QGroupBox* grpBoxOperation = new QGroupBox(u8"操作");
    grpBoxOperation->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    grpBoxOperation->setLayout(hBox1);
    QGroupBox* grpBoxLog = new QGroupBox(u8"日志");
    grpBoxLog->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    grpBoxLog->setLayout(hBox2);
    vAll->addWidget(grpBoxOperation);
    vAll->addWidget(grpBoxLog);
    vAll->setStretch(0,1);
    vAll->setStretch(1,7);

    this->setLayout(vAll);
}

void RedFlagTestCommandWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    connect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_from_press(int)));
    QString portName = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS");
    QString portRate = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS_BARD_RATE");
    m_pPressStationSerialCom->connectToMCU(portName,portRate);

    connect(m_pPressStationSerialCom,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_from_mano_meter(uint16_t)));
    m_pPressStationSerialCom->initManoMeter(GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER"),
                                   GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER_BARD_RATE"));

}

void RedFlagTestCommandWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
    m_pPressStationSerialCom->disconnectToMCU();

    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
}

QString RedFlagTestCommandWgt::get_current_time()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString r = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
    return r;
}

void RedFlagTestCommandWgt::slot_btn_to_press()
{
    m_pLogTextEdit->append(get_current_time()+ u8":通知压合设备进行压合");
    m_pPressStationSerialCom->startScanRes(SCAN_RES_TYPE::ResScanOK);
}

void RedFlagTestCommandWgt::slot_btn_to_read()
{
    m_pLogTextEdit->append(get_current_time()+u8":读取压力表压力值");
    m_pPressStationSerialCom->readPressMeter();
}

void RedFlagTestCommandWgt::slot_from_press(int code)
{
    if(code == PRESS_RESULT::ResPressStart)
        m_pLogTextEdit->append(u8"压合开始");
    else if(code == PRESS_RESULT::ResTimeOut)
        m_pLogTextEdit->append(u8"压合超时");
    else
        m_pLogTextEdit->append(u8"压合结束!");
}

void RedFlagTestCommandWgt::slot_from_mano_meter(uint16_t value)
{
    double tmpV = static_cast<double>(value/100.0);
    QString strR = u8"收到压合工装压力值:"+QString::number(tmpV,'f',2);
    m_pLogTextEdit->append(strR+"KG");
}
