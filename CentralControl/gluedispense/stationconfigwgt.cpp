#include "stationconfigwgt.h"
#include "TitleBar.h"
#include <QIcon>
#include <QPainter>
#include <QVBoxLayout>
#include <QGroupBox>
#include "gdatafactory.h"
StationConfigWgt* StationConfigWgt::m_pObj = nullptr;
void StationConfigWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

StationConfigWgt::StationConfigWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(750,200);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"设备信息配置");
    pTitleBar->setFixedWidth(750);
    installEventFilter(pTitleBar);

    m_pLineEditStationCode = new QLineEdit();
    m_pLineEditEquipCode = new QLineEdit();
    m_pLineEditEquipName= new QLineEdit();

    QList<QLineEdit*> lst;
    lst<<m_pLineEditStationCode<<m_pLineEditEquipCode<<m_pLineEditEquipName;
    for(int i=0;i<lst.size();i++)
    {
        lst[i]->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
        lst[i]->setAlignment(Qt::AlignCenter);
    }

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpEquipInfo = new QGroupBox(u8"设备信息");
    grpEquipInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();

    QLabel* labelStationCode = new QLabel(u8"工位编码:");
    QLabel* labelEquipCode = new QLabel(u8"设备编码:");
    QLabel* labelEquipName = new QLabel(u8"设备名称:");
    hBox1->addWidget(labelStationCode);
    hBox1->addWidget(m_pLineEditStationCode);
    hBox1->addWidget(labelEquipCode);
    hBox1->addWidget(m_pLineEditEquipCode);
    hBox1->addWidget(labelEquipName);
    hBox1->addWidget(m_pLineEditEquipName);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QPushButton* btnSave = new QPushButton(u8"保存");
    btnSave->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);max-width:200;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnSave,SIGNAL(clicked()),this,SLOT(slot_save()));
    hBox2->addWidget(btnSave);

    vBox1->addLayout(hBox1);
    vBox1->addLayout(hBox2);
    grpEquipInfo->setLayout(vBox1);

    vAll->addSpacing(40);
    vAll->addWidget(grpEquipInfo);
    this->setLayout(vAll);
}

void StationConfigWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if(m_pCharacterID == 0)
    {
        m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("MIDDLE_FRAME_STATION_CODE"));
        m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("MIDDLE_FRAME_EQUIPMENT_CODE"));
        m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("MIDDLE_FRAME_EQUIPMENT_NAME"));
    }
    else if(m_pCharacterID == 1)
    {
        m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_STATION_CODE"));
        m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_EQUIPMENT_CODE"));
        m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_EQUIPMENT_NAME"));
    }
    else if(m_pCharacterID == 2){
        m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_STATION_CODE"));
        m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_EQUIPMENT_CODE"));
        m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_EQUIPMENT_NAME"));
    }
    else if(m_pCharacterID == 3)
    {
        m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_STATION_CODE"));
        m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_EQUIPMENT_CODE"));
        m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_EQUIPMENT_NAME"));
    }
}

void StationConfigWgt::slot_save()
{
    if(m_pCharacterID == 0)
    {
        GDataFactory::get_factory()->set_config_para_1("MIDDLE_FRAME_STATION_CODE",m_pLineEditStationCode->text());
        GDataFactory::get_factory()->set_config_para_1("MIDDLE_FRAME_EQUIPMENT_CODE",m_pLineEditEquipCode->text());
        GDataFactory::get_factory()->set_config_para_1("MIDDLE_FRAME_EQUIPMENT_NAME",m_pLineEditEquipName->text());
        emit signal_send_set_info(0,m_pLineEditEquipCode->text(),m_pLineEditEquipName->text(),m_pLineEditStationCode->text());
    }
    else if(m_pCharacterID == 1)
    {
        GDataFactory::get_factory()->set_config_para_1("MAIN_BOARD_STATION_CODE",m_pLineEditStationCode->text());
        GDataFactory::get_factory()->set_config_para_1("MAIN_BOARD_EQUIPMENT_CODE",m_pLineEditEquipCode->text());
        GDataFactory::get_factory()->set_config_para_1("MAIN_BOARD_EQUIPMENT_NAME",m_pLineEditEquipName->text());
        emit signal_send_set_info(1,m_pLineEditEquipCode->text(),m_pLineEditEquipName->text(),m_pLineEditStationCode->text());
    }
    else if(m_pCharacterID == 2){
        GDataFactory::get_factory()->set_config_para_1("PRESS_FIT_STATION_CODE",m_pLineEditStationCode->text());
        GDataFactory::get_factory()->set_config_para_1("PRESS_FIT_EQUIPMENT_CODE",m_pLineEditEquipCode->text());
        GDataFactory::get_factory()->set_config_para_1("PRESS_FIT_EQUIPMENT_NAME",m_pLineEditEquipName->text());
        emit signal_send_set_info(2,m_pLineEditEquipCode->text(),m_pLineEditEquipName->text(),m_pLineEditStationCode->text());
    }
    else if(m_pCharacterID == 3){
        GDataFactory::get_factory()->set_config_para_1("CCD_KEY_PART_STATION_CODE",m_pLineEditStationCode->text());
        GDataFactory::get_factory()->set_config_para_1("CCD_KEY_PART_EQUIPMENT_CODE",m_pLineEditEquipCode->text());
        GDataFactory::get_factory()->set_config_para_1("CCD_KEY_PART_EQUIPMENT_NAME",m_pLineEditEquipName->text());
        emit signal_send_set_info(3,m_pLineEditEquipCode->text(),m_pLineEditEquipName->text(),m_pLineEditStationCode->text());
    }
    GDataFactory::get_factory()->save_config_file();
    this->hide();
}
