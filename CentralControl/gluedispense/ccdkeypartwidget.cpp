#include "ccdkeypartwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "gdatafactory.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
CCDKeyPartWidget* CCDKeyPartWidget::m_pCCDKeyPartWidget = nullptr;
CCDKeyPartWidget::CCDKeyPartWidget(QWidget *parent) : QWidget(parent)
{
    m_pLogTextEdit = new QTextEdit();
    m_pLogTextEdit->setStyleSheet("QTextEdit{background-color:rgba(255,255,255,100);color:rgb(0,0,0);font-family:KaiTi;font-size:20px;}");

    m_pScanMainBoard = new BallWidget();
    m_pScanKeyPart = new BallWidget();
    m_pKeyPartCheckState = new BallWidget();
    m_pPressFitCheckState = new BallWidget();
    m_pFinalCheckState = new BallWidget();

    m_pLineEditStationCode = new QLineEdit();
    m_pLineEditEquipCode = new QLineEdit();
    m_pLineEditEquipName= new QLineEdit();
    QList<QLineEdit*> lst;
    lst<<m_pLineEditStationCode<<m_pLineEditEquipCode<<m_pLineEditEquipName;
    for(int i=0;i<lst.size();i++)
    {
        lst[i]->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
        lst[i]->setReadOnly(true);
        lst[i]->setAlignment(Qt::AlignCenter);
    }

    QVBoxLayout* vAll = new QVBoxLayout();

    QGroupBox* grpEquipInfo = new QGroupBox(u8"设备信息");
    grpEquipInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();

    QLabel* labelStationCode = new QLabel(u8"工位编码:");
    QLabel* labelEquipCode = new QLabel(u8"设备编码:");
    QLabel* labelEquipName = new QLabel(u8"设备名称:");
    QPushButton* btnChangeInfo = new QPushButton(u8"修改设备信息");
    btnChangeInfo->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,255);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnChangeInfo,SIGNAL(clicked()),this,SLOT(slot_station_config()));

    hBox1->addWidget(labelStationCode);
    hBox1->addWidget(m_pLineEditStationCode);
    hBox1->addWidget(labelEquipCode);
    hBox1->addWidget(m_pLineEditEquipCode);
    hBox1->addWidget(labelEquipName);
    hBox1->addWidget(m_pLineEditEquipName);
    hBox1->addWidget(btnChangeInfo);
    grpEquipInfo->setLayout(hBox1);

    QGroupBox* grpFinalResult = new QGroupBox(u8"校验及结果上传信息");
    grpFinalResult->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox21 = new QHBoxLayout();
    QLabel* labelScanMainBoard = new QLabel(u8"主板扫码");
//    QLabel* labelScanKeyPart = new QLabel(u8"关键件校验");
    QLabel* labelKeyPartCheck = new QLabel(u8"关键件校验");
    QLabel* labelPressFitResult = new QLabel(u8"压合结果");
    QLabel* labelFinalResult = new QLabel(u8"结果上传");
    QPushButton* btnUpdatebutton = new QPushButton(u8"重新开始");
    btnUpdatebutton->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,255);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnUpdatebutton,SIGNAL(clicked()),this,SLOT(slot_restart()));
    hBox21->addStretch();
    hBox21->addWidget(m_pScanMainBoard);
    hBox21->addWidget(labelScanMainBoard);
    hBox21->addStretch();
//    hBox21->addWidget(m_pScanKeyPart);
//    hBox21->addWidget(labelScanKeyPart);
//    hBox21->addStretch();
    hBox21->addWidget(m_pKeyPartCheckState);
    hBox21->addWidget(labelKeyPartCheck);
    hBox21->addStretch();
    hBox21->addWidget(m_pPressFitCheckState);
    hBox21->addWidget(labelPressFitResult);
    hBox21->addStretch();
    hBox21->addWidget(m_pFinalCheckState);
    hBox21->addWidget(labelFinalResult);
    hBox21->addStretch();
    hBox21->addWidget(btnUpdatebutton);
    grpFinalResult->setLayout(hBox21);

    QHBoxLayout* hBox3 = new QHBoxLayout();

    QGroupBox* grpKeyMaterialInfo = new QGroupBox(u8"关键物料描述");
    grpKeyMaterialInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox11 = new QVBoxLayout();
    vBox11->addWidget(createTableWidget());
    grpKeyMaterialInfo->setLayout(vBox11);

    QVBoxLayout* vBox21 = new QVBoxLayout();
    QGroupBox* grpScanInfo = new QGroupBox(u8"扫描信息");
    grpScanInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox21->addWidget(m_pLogTextEdit);
    grpScanInfo->setLayout(vBox21);

    hBox3->addWidget(grpKeyMaterialInfo);
    hBox3->addWidget(grpScanInfo);

    vAll->addWidget(grpEquipInfo);
    vAll->addWidget(grpFinalResult);
    vAll->addLayout(hBox3);
    this->setLayout(vAll);

    connect(GDataFactory::get_mes_process(),SIGNAL(signal_to_common(const QByteArray&)),
            this,SLOT(slot_rev_mes_info(const QByteArray&)));

    connect(GDataFactory::get_station_config_wgt(),SIGNAL(signal_send_set_info(const int,const QString, const QString, const QString)),
            this,SLOT(slot_rev_set_info(const int,const QString,const QString,const QString)));

    pComm = nullptr;
    m_pPressStationSerialCom = nullptr;
}

QWidget *CCDKeyPartWidget::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(100,2);
//    connect(tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slot_select_item(QTableWidgetItem*)));

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"物料号"<<u8"物料描述";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    return tableWidget;
}

void CCDKeyPartWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    //press
    m_pPressStationSerialCom = new PressStationSerialCom();
    connect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
    QString portName = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS");
    QString portRate = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS_BARD_RATE");
    m_pPressStationSerialCom->connectToMCU(portName,portRate);
    //press

    //manometer
//    m_pManoMeterCom = new PressStationSerialCom();
    connect(m_pPressStationSerialCom,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
    m_pPressStationSerialCom->initManoMeter(GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER"),
                                   GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER_BARD_RATE"));
    //manometer

    read_serial_number();
    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_EQUIPMENT_NAME"));

    m_pCheckData.Source_System = (char*)("EQUIPMENT");
    m_pCheckData.Message_Type = (char*)("PASS_STATION_CHECK");

    m_pCheckData.Content_Data.Serial_Number = (char*)"";

    QByteArray stationCode = GDataFactory::get_factory()->
            get_config_para("CCD_KEY_PART_STATION_CODE").toLatin1();
    m_pCheckData.Content_Data.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Station_Code,stationCode.length()+1,stationCode.data());

    QByteArray equipmentCode = GDataFactory::get_factory()->
            get_config_para("CCD_KEY_PART_EQUIPMENT_CODE").toLatin1();
    m_pCheckData.Content_Data.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    QByteArray equipmentName = GDataFactory::get_factory()->
            get_config_para("CCD_KEY_PART_EQUIPMENT_NAME").toLatin1();
    m_pCheckData.Content_Data.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    m_pCheckData.Content_Data.Check_Type = (char*)("2");

    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("SAVE_YH_INFO");

    m_pResultData.Result_Content.Serial_Number = m_pCheckData.Content_Data.Serial_Number;
    m_pResultData.Result_Content.Station_Code = m_pCheckData.Content_Data.Station_Code;
    m_pResultData.Result_Content.Equipment_Code = m_pCheckData.Content_Data.Equipment_Code;
    m_pResultData.Result_Content.Equipment_Name = m_pCheckData.Content_Data.Equipment_Name;
}

void CCDKeyPartWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    //press
    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
    m_pPressStationSerialCom->disconnectToMCU();
    delete m_pPressStationSerialCom;
    m_pPressStationSerialCom = nullptr;
    //press
    killTimer(m_pTimerID);
    reset_station_status();
    m_pCheckData.Key_Bar_Code[0].clear();
    m_pResultData.Result_Content.Content_Detail[0].clear();
    stop_serial_port();
}

void CCDKeyPartWidget::slot_station_config()
{
    GDataFactory::get_station_config_wgt()->set_station_characters(3);
    GDataFactory::get_station_config_wgt()->show();
}

void CCDKeyPartWidget::slot_restart()
{
//    //press
//    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
//    m_pPressStationSerialCom->disconnectToMCU();
//    delete m_pPressStationSerialCom;
//    m_pPressStationSerialCom = nullptr;
//    //press
    killTimer(m_pTimerID);
    reset_station_status();
    m_pCheckData.Key_Bar_Code[0].clear();
    m_pResultData.Result_Content.Content_Detail[0].clear();
//    stop_serial_port();

//    //press
//    m_pPressStationSerialCom = new PressStationSerialCom();
//    connect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
//    QString portName = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS");
//    QString portRate = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS_BARD_RATE");
//    m_pPressStationSerialCom->connectToMCU(portName,portRate);
//    //press

//    stop_serial_port();
//    read_serial_number();
    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("CCD_KEY_PART_EQUIPMENT_NAME"));

    //to fill check key-part info
    m_pCheckData.Source_System = (char*)("EQUIPMENT");
    m_pCheckData.Message_Type = (char*)("PASS_STATION_CHECK");
    m_pCheckData.Content_Data.Serial_Number = (char*)"";

    QByteArray stationCode = GDataFactory::get_factory()->
            get_config_para("CCD_KEY_PART_STATION_CODE").toLatin1();
    m_pCheckData.Content_Data.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Station_Code,stationCode.length()+1,stationCode.data());

    QByteArray equipmentCode = GDataFactory::get_factory()->
            get_config_para("CCD_KEY_PART_EQUIPMENT_CODE").toLatin1();
    m_pCheckData.Content_Data.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    QByteArray equipmentName = GDataFactory::get_factory()->
            get_config_para("CCD_KEY_PART_EQUIPMENT_NAME").toLatin1();
    m_pCheckData.Content_Data.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    m_pCheckData.Content_Data.Check_Type = (char*)("2");
    //to fill check key-part info

    //to fill result info
    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("SAVE_YH_INFO");

    m_pResultData.Result_Content.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pResultData.Result_Content.Station_Code,stationCode.length()+1,stationCode.data());

    m_pResultData.Result_Content.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pResultData.Result_Content.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    m_pResultData.Result_Content.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pResultData.Result_Content.Equipment_Name,equipmentName.length()+1,equipmentName.data());
}

void CCDKeyPartWidget::set_table_data()
{
    this->tableWidget->clearContents();
    for (int i=0;i<m_pListDesc.size();i++) {
        tableWidget->setItem(i,0,new QTableWidgetItem(m_pListCode[i]));
        tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->item(i,0)->setFlags(Qt::ItemIsSelectable);
        tableWidget->setItem(i,1,new QTableWidgetItem(m_pListDesc[i]));
        tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->item(i,1)->setFlags(Qt::ItemIsSelectable);
    }
}

void CCDKeyPartWidget::notify_fpc_to_press()
{
    m_pPressStationSerialCom->startScanRes(SCAN_RES_TYPE::ResScanOK);
    m_pLogTextEdit->append(u8"通知压合设备进行压合！");
}

void CCDKeyPartWidget::update_ui_according_to_mes_info()
{
    m_pScanMainBoard->set_color(1);
    set_table_data();
    if(m_pCheckData.Key_Bar_Code[0].size() == (m_pListDesc.length() + 1))
    {
        m_pKeyPartCheckState->set_color(1);
        notify_fpc_to_press();
    }
}

void CCDKeyPartWidget::reset_check_result_data_after_over()
{
    m_pFinalCheckState->set_color(1);
    m_pCheckData.Key_Bar_Code[0].clear();
    m_pResultData.Result_Content.Content_Detail[0].clear();
}

void CCDKeyPartWidget::slot_rev_mes_info(const QByteArray &data)
{
    if(this->isVisible())
        ;
    else
        return;
    QLOG_WARN()<<"the info of mes reply is:"<<data;
    QByteArray responseByte = data;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);

    QJsonObject jsonObject = jsonDocument.object();
    QString statusString = "";
    if(jsonObject.contains("EVENT_CODE"))
    {
        QJsonValue instructionValue = jsonObject.value(QStringLiteral("EVENT_CODE"));
        if(instructionValue.isString())
            statusString = instructionValue.toString();
        else
        {
            QLOG_WARN()<<"MES status para data format is ABNORMAL!";
            return;
        }
    }

    QString eventdes = "";
    if(jsonObject.contains("EVENT_DESCRIPTION"))
    {
        QJsonValue instructionValue = jsonObject.value(QStringLiteral("EVENT_DESCRIPTION"));
        eventdes = instructionValue.toString();
    }

    m_pLogTextEdit->append(u8"Mes返回结果描述为:"+eventdes);
    if(statusString == "S")
    {
        QLOG_WARN()<<"EVENT_DATA:"<<jsonObject["EVENT_DATA"].toString();
        if(jsonObject.contains("EVENT_DATA"))
        {
            QJsonObject objA = jsonObject["EVENT_DATA"].toObject();
            if(!objA.isEmpty()){
                if(objA.contains("KEY_PARTS"))
                {
                    m_pListDesc.clear();
                    QJsonValue array = objA.value("KEY_PARTS");
                    if(array.isArray())
                    {
                        QJsonArray iconArray = array.toArray();
                        for(int i=0;i<iconArray.size();i++)
                        {
                            QJsonValue iconValue = iconArray.at(i);
                            QJsonObject icon = iconValue.toObject();
                            m_pListCode.push_back(icon["MATERIAL_CODE"].toString());
                            m_pListDesc.push_back(icon["MATERIAL_DESC"].toString());
                        }
                    }
                }
                update_ui_according_to_mes_info();
            }else{
                reset_check_result_data_after_over();
                m_pFinalCheckState->set_color(1);
            }
        }
    }
    else {
        m_pCheckData.Key_Bar_Code[0].pop_back();
        QLOG_WARN()<<"MES status para is error!";
        return;
    }
}

void CCDKeyPartWidget::read_serial_number()
{
    if(pComm != nullptr)
        return;

    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = GDataFactory::get_factory()->get_config_para("RED_FLAG_SCANNER_PORT");
        QString portRate = GDataFactory::get_factory()->get_config_para("RED_FLAG_SCANNER_BARD_RATE");
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));
        pComm->recvResponse_only();
        pComm->start();
    }
}

void CCDKeyPartWidget::stop_serial_port()
{
    if(pComm == nullptr)
        return;
    pComm->terminate();
    pComm->quit();
    pComm->closeSerialPort();
    if(pComm)
    {
        delete pComm;
        pComm = nullptr;
    }
}

void CCDKeyPartWidget::slot_press_result(int resultcode)
{
    if(resultcode == PRESS_RESULT::ResPressStart)
        m_pTimerID = startTimer(500);
    else if(resultcode == PRESS_RESULT::ResTimeOut)
    {
        QLOG_WARN()<<u8"压合超时";
    }
    else
    {
        m_pPressStationSerialCom->readPressMeter();
//        m_pLogTextEdit->append(u8"压合结束!");
//        killTimer(m_pTimerID);
//        m_pPressFitCheckState->set_color(1);

//        //upload info to MES
//        GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),0);
//        //upload info to MES
    }
}

void CCDKeyPartWidget::slot_rev_mano_meter_result(uint16_t r)
{
    double tmpV = static_cast<double>(r/100.0);
    QString strR = u8"收到压合工装压力值:"+QString::number(tmpV,'f',2);
    m_pLogTextEdit->append(strR+"KG");

    m_pLogTextEdit->append(u8"压力上限值为:"+GDataFactory::get_factory()->get_config_para("PRESSURE_HIGH_VALUE")+"KG");
    m_pLogTextEdit->append(u8"压力下限值为:"+GDataFactory::get_factory()->get_config_para("PRESSURE_LOW_VALUE")+"KG");

    double realValue = r/100.0;
    double topValue = GDataFactory::get_factory()->get_config_para("PRESSURE_HIGH_VALUE").toDouble();
    double lowValue = GDataFactory::get_factory()->get_config_para("PRESSURE_LOW_VALUE").toDouble();

    if((GDataFactory::get_factory()->isGreater(realValue,lowValue)) && (GDataFactory::get_factory()->isLess(realValue,topValue)))
        ;
    else {
        m_pLogTextEdit->append(u8"压力值超限，请关闭窗口，调整设备重新开始!");
        return;
    }

    killTimer(m_pTimerID);
    m_pPressFitCheckState->set_color(1);

    //upload info to MES
    GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),0);
    //upload info to MES
}

void CCDKeyPartWidget::push_key_bar_to_list(QString barcode)
{
    if(m_pCheckData.Key_Bar_Code[0].size() == 0)
    {
        m_pScanMainBoard->set_color(0);
        m_pKeyPartCheckState->set_color(0);
        m_pPressFitCheckState->set_color(0);
        m_pFinalCheckState->set_color(0);
    }
    QByteArray bb = barcode.toLatin1();
    char* bCode = new char[bb.length()+1];
    strcpy_s(bCode,static_cast<rsize_t>(bb.length()+1),bb.data());
    std::list<string>::iterator it = std::find(m_pCheckData.Key_Bar_Code[0].begin(),
            m_pCheckData.Key_Bar_Code[0].end(),bCode);
    if(it != m_pCheckData.Key_Bar_Code[0].end())
    {
        QLOG_INFO()<<"the value already exist!";
        m_pLogTextEdit->append(u8"关键件条码重复扫描! 请确认关键件并重新扫描!");
    }
    else {
        QLOG_INFO()<<"the value not exist,insert into the list!";
        if(m_pCheckData.Key_Bar_Code[0].size() == 3)
        {
            QLOG_WARN()<<"the quantity of key part is already full!";
            QLOG_WARN()<<u8"关键件条码数量已扫描完成! 请勿重复扫描";
        }
        else
        {
            m_pCheckData.Key_Bar_Code[0].push_back(bCode);
            if(m_pCheckData.Key_Bar_Code[0].size() == 1)
            {
                m_pCheckData.Content_Data.Serial_Number = bCode;
                m_pResultData.Result_Content.Serial_Number = bCode;
            }
        }
    }
}

QByteArray CCDKeyPartWidget::format_check_json_info()
{
    QByteArray result;
    QByteArray mTime = QDateTime::currentDateTime().toString(fmtDateTime).toLatin1();
    m_pCheckData.Message_Time = mTime.data();
    QUuid uuid = QUuid::createUuid();
    QByteArray mID = uuid.toString(QUuid::WithoutBraces).toLatin1();
    m_pCheckData.Message_ID = mID.data();


    QJsonDocument doc;
    QJsonObject  root,content;
    QJsonArray   ats_value;

    root.insert("MESSAGE_ID",m_pCheckData.Message_ID);
    root.insert("SOURCE_SYSTEM",m_pCheckData.Source_System);
    root.insert("MESSAGE_TYPE",m_pCheckData.Message_Type);
    root.insert("MESSAGE_TIME",m_pCheckData.Message_Time);

    content.insert("SERIAL_NUMBER",m_pCheckData.Content_Data.Serial_Number);
    content.insert("STATION_CODE",m_pCheckData.Content_Data.Station_Code);
    content.insert("EQUIPMENT_CODE",m_pCheckData.Content_Data.Equipment_Code);
    content.insert("EQUIPMENT_NAME",m_pCheckData.Content_Data.Equipment_Name);
    content.insert("CHECK_TYPE",m_pCheckData.Content_Data.Check_Type);
    content.insert("FIELD1","");
    content.insert("FIELD2","");
    content.insert("FIELD3","");

    std::list<std::string>::iterator it;
    it = m_pCheckData.Key_Bar_Code[0].begin();
    int i=0;
    while (it != m_pCheckData.Key_Bar_Code[0].end()) {
        std::string amb = *it;
        if(i==0)
        {
            ++i;
            ++it;
            continue;
        }
        ats_value.insert(i-1,amb.c_str());
        ++it;
        ++i;
    }


    content.insert("KEY_BAR_CODE",ats_value);
    root.insert("CONTENT",content);

    doc.setObject(root);
    result = doc.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<"check json is:"<<result;
    return result;
}

QByteArray CCDKeyPartWidget::format_result_json_info()
{
    QByteArray result;
    QByteArray mTime = QDateTime::currentDateTime().toString(fmtDateTime).toLatin1();
    m_pResultData.Message_Time = mTime.data();
    QUuid uuid = QUuid::createUuid();
    QByteArray mID = uuid.toString(QUuid::WithoutBraces).toLatin1();
    m_pResultData.Message_ID = mID.data();


    QJsonDocument doc;
    QJsonObject  root,content,detail_list;
    QJsonArray   ats_value;

    root.insert("MESSAGE_ID",m_pResultData.Message_ID);
    root.insert("SOURCE_SYSTEM",m_pResultData.Source_System);
    root.insert("MESSAGE_TYPE",m_pResultData.Message_Type);
    root.insert("MESSAGE_TIME",m_pResultData.Message_Time);

    content.insert("SERIAL_NUMBER",m_pResultData.Result_Content.Serial_Number);
    content.insert("STATION_CODE",m_pResultData.Result_Content.Station_Code);
    content.insert("EQUIPMENT_CODE",m_pResultData.Result_Content.Equipment_Code);
    content.insert("EQUIPMENT_NAME",m_pResultData.Result_Content.Equipment_Name);

    QByteArray qmstatus = "OK";
    m_pResultData.Result_Content.QM_Status = new char[qmstatus.length()+1];
    strcpy_s(m_pResultData.Result_Content.QM_Status,qmstatus.length()+1,qmstatus.data());
    content.insert("QM_STATUS",m_pResultData.Result_Content.QM_Status);

    std::list<ResultDetailData>::iterator it;
    if(m_pCheckData.Key_Bar_Code[0].size() == m_pListDesc.length()+1)
    {
        std::list<std::string>::iterator it;
        it = m_pCheckData.Key_Bar_Code[0].begin();
        while (it != m_pCheckData.Key_Bar_Code[0].end())
        {
            if(*it == m_pResultData.Result_Content.Serial_Number)
            {
                ++it;
                continue;
            }
            QJsonObject value;
            value.insert("KEY_BAR_CODE", QString::fromStdString(*it));
            value.insert("BEFORE_VALUE", NULL);
            value.insert("AFTER_VALUE", NULL);
            value.insert("ITEM_VALUE", NULL);//ITEM_QM_STATUS
            value.insert("ITEM_QM_STATUS", "OK");
            value.insert("FIELD1", "");
            value.insert("FIELD2", "");
            value.insert("FIELD3", "");
            ats_value.insert(ats_value.size(),value);
            ++it;
        }
    }
    else {
        QLOG_WARN()<<"the quantity of key bar code is not CORRECT";
    }

    content.insert("DETAIL",ats_value);
    root.insert("CONTENT",content);

    doc.setObject(root);
    result = doc.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<"final result:"<<result;
    return result;
}

void CCDKeyPartWidget::slot_update_start_info()
{
    reset_station_status();
}

void CCDKeyPartWidget::slot_rev_serial_number(const QByteArray &data)
{
    QString cd = data;
    if(cd.contains('\r'))
        cd.remove('\r');

    m_pLogTextEdit->append(u8"收到的扫码内容为:"+cd);
    push_key_bar_to_list(cd);

    GDataFactory::get_mes_process()->exec_http_common_get(format_check_json_info(),1);
}

void CCDKeyPartWidget::slot_rev_set_info(const int characterid, const QString EquipCode, const QString EquipName, const QString StationCode)
{
    Q_UNUSED(characterid);
    m_pLineEditStationCode->setText(StationCode);
    m_pLineEditEquipCode->setText(EquipCode);
    m_pLineEditEquipName->setText(EquipName);
}

static int sign = 0;
void CCDKeyPartWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(sign%2 == 0)
        m_pPressFitCheckState->set_color(1);
    else
        m_pPressFitCheckState->set_color(0);
    sign++;

}

void CCDKeyPartWidget::reset_station_status()
{
    m_pScanMainBoard->set_color(0);
    m_pScanKeyPart->set_color(0);
    m_pKeyPartCheckState->set_color(0);
    m_pPressFitCheckState->set_color(0);
    m_pFinalCheckState->set_color(0);
    tableWidget->clearContents();
    m_pLogTextEdit->clear();
}
