#include "fpcpressfitwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "gdatafactory.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <zebraprinterservice.h>
FPCPressFitWgt* FPCPressFitWgt::m_pObj = nullptr;

FPCPressFitWgt::FPCPressFitWgt(QWidget *parent) : QWidget(parent)
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
    QLabel* labelScanMainBoard = new QLabel(u8"中框扫码");
    QLabel* labelScanKeyPart = new QLabel(u8"FPC扫码");
    QLabel* labelKeyPartCheck = new QLabel(u8"关键件校验");
    QLabel* labelPressFitResult = new QLabel(u8"压合结果");
    QLabel* labelFinalResult = new QLabel(u8"结果上传");
    QPushButton* btnUpdatebutton = new QPushButton(u8"重新开始");
    btnUpdatebutton->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,255);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnUpdatebutton,SIGNAL(clicked()),this,SLOT(slot_update_start_info()));
    hBox21->addStretch();
    hBox21->addWidget(m_pScanMainBoard);
    hBox21->addWidget(labelScanMainBoard);
    hBox21->addStretch();
    hBox21->addWidget(m_pScanKeyPart);
    hBox21->addWidget(labelScanKeyPart);
    hBox21->addStretch();
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
    connect(GDataFactory::get_station_config_wgt(),SIGNAL(signal_send_set_info(const int,const QString, const QString, const QString)),
            this,SLOT(slot_rev_set_info(const int,const QString,const QString,const QString)));
    connect(GDataFactory::get_mes_process(),SIGNAL(signal_to_common(const QByteArray&)),
            this,SLOT(slot_rev_mes_info(const QByteArray&)));
    pComm = nullptr;
}

QWidget *FPCPressFitWgt::createTableWidget()
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

void FPCPressFitWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pPressStationService = new PressStationSerialCom();
    //m_pPressStationService->connectToMCU();
    QString portName = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS");
    QString portRate = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS_BARD_RATE");
    m_pPressStationService->connectToMCU(portName,portRate);
    connect(m_pPressStationService, &PressStationSerialCom::notify_press_result, this, &FPCPressFitWgt::slot_recv_preas_result_info);

    //manometer
//    m_pManoMeterCom = new PressStationSerialCom();
    connect(m_pPressStationService,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
    m_pPressStationService->initManoMeter(GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER"),
                                   GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER_BARD_RATE"));
    //manometer

    stop_serial_port();
    read_serial_number();
    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_EQUIPMENT_NAME"));

    m_pCheckData.Source_System = (char*)("EQUIPMENT");
    m_pCheckData.Message_Type = (char*)("PASS_STATION_CHECK");

    m_pCheckData.Content_Data.Serial_Number = (char*)"";

    QByteArray stationCode = GDataFactory::get_factory()->
            get_config_para("PRESS_FIT_STATION_CODE").toLatin1();
    m_pCheckData.Content_Data.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Station_Code,stationCode.length()+1,stationCode.data());

    QByteArray equipmentCode = GDataFactory::get_factory()->
            get_config_para("PRESS_FIT_EQUIPMENT_CODE").toLatin1();
    m_pCheckData.Content_Data.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    QByteArray equipmentName = GDataFactory::get_factory()->
            get_config_para("PRESS_FIT_EQUIPMENT_NAME").toLatin1();
    m_pCheckData.Content_Data.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    m_pCheckData.Content_Data.Check_Type = (char*)("3");

    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("SAVE_YH_INFO");

    m_pResultData.Result_Content.Serial_Number = m_pCheckData.Content_Data.Serial_Number;
    m_pResultData.Result_Content.Station_Code = m_pCheckData.Content_Data.Station_Code;
    m_pResultData.Result_Content.Equipment_Code = m_pCheckData.Content_Data.Equipment_Code;
    m_pResultData.Result_Content.Equipment_Name = m_pCheckData.Content_Data.Equipment_Name;
}

void FPCPressFitWgt::closeEvent(QCloseEvent *event)
{
    stop_serial_port();
    disconnect(m_pPressStationService,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
    disconnect(m_pPressStationService, &PressStationSerialCom::notify_press_result, this, &FPCPressFitWgt::slot_recv_preas_result_info);
    m_pPressStationService->disconnectToMCU();
    delete m_pPressStationService;
    m_pPressStationService = nullptr;
    m_pScanMainBoard->set_color(0);
    m_pScanKeyPart->set_color(0);
    m_pKeyPartCheckState->set_color(0);
    m_pPressFitCheckState->set_color(0);
    m_pFinalCheckState->set_color(0);
    m_pLogTextEdit->clear();
    m_pListCode.clear();
    m_pListDesc.clear();
    this->tableWidget->clearContents();
    m_pCheckData.Key_Bar_Code->clear();
    m_pResultData.Result_Content.Content_Detail.clear();
    m_press_process = 0;
}

void FPCPressFitWgt::timerEvent(QTimerEvent *event)
{
    static int sign = 0;
    if(m_pTimerID == event->timerId()){

        if(sign%2 == 0){
            m_pPressFitCheckState->set_color(0);
        }else if(sign%2 == 1){
            sign = 1;
            m_pPressFitCheckState->set_color(1);
        }
        sign++;
    }
}

void FPCPressFitWgt::read_serial_number()
{
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

void FPCPressFitWgt::stop_serial_port()
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

QByteArray FPCPressFitWgt::format_check_json_info()
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
        if(*it == m_pCheckData.Content_Data.Serial_Number)
        {
            ++it;
            continue;
        }
        std::string amb = *it;
        ats_value.insert(i,amb.c_str());
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

QByteArray FPCPressFitWgt::format_result_json_info()
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
    content.insert("QM_STATUS",m_pResultData.Result_Content.QM_Status);

    std::list<ResultDetailData>::iterator it;
    it = m_pResultData.Result_Content.Content_Detail.begin();
    int i=0;

    while (it != m_pResultData.Result_Content.Content_Detail.end()) {
        ResultDetailData amb = *it;
        QJsonObject value;
        value.insert("KEY_BAR_CODE", amb.Key_Bar_Code);
        value.insert("BEFORE_VALUE", amb.Before_Value);
        value.insert("AFTER_VALUE", amb.After_Value);
        value.insert("ITEM_VALUE", amb.Item_Value);//ITEM_QM_STATUS
        value.insert("ITEM_QM_STATUS", amb.Item_QM_status);
        value.insert("FIELD1", "");
        value.insert("FIELD2", "");
        value.insert("FIELD3", "");

        ats_value.insert(i,value);

        ++it;
        ++i;
    }

    content.insert("DETAIL",ats_value);
    root.insert("CONTENT",content);

    doc.setObject(root);
    result = doc.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<"final result:"<<result;
    return result;
}

void FPCPressFitWgt::update_ui_according_to_mes_info()
{
    m_pKeyPartCheckState->set_color(1);
    set_table_data();
}

void FPCPressFitWgt::set_table_data()
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

void FPCPressFitWgt::log_add_text_edit(QString text)
{
    QString time_log = QDateTime::currentDateTime().toString(fmtDateTime);
    m_pLogTextEdit->append(time_log + " "+ text);
}

void FPCPressFitWgt::slot_rev_serial_number(const QByteArray &data)
{
    QString cd = data;
    if(cd.contains('\r'))
        cd.remove('\r');

    QByteArray bb = cd.toLatin1();
    char* bCode = new char[bb.length()+1];
    strcpy_s(bCode,bb.length()+1,bb.data());

    if(m_pCheckData.Key_Bar_Code[0].size() == 0){
        m_pScanMainBoard->set_color(1);
        m_pScanKeyPart->set_color(0);
        m_pKeyPartCheckState->set_color(0);
        m_pPressFitCheckState->set_color(0);
        m_pFinalCheckState->set_color(0);
        m_pLogTextEdit->clear();
        m_pListCode.clear();
        m_pListDesc.clear();
        m_printBARCODE.clear();
        this->tableWidget->clearContents();
        m_press_process = 0;
    }else if(m_pCheckData.Key_Bar_Code[0].size() == 2){
        m_pScanMainBoard->set_color(1);
        m_pScanKeyPart->set_color(1);
        m_pKeyPartCheckState->set_color(0);
        m_pPressFitCheckState->set_color(0);
        m_pFinalCheckState->set_color(0);
    }

    std::list<string>::iterator it = std::find(m_pCheckData.Key_Bar_Code[0].begin(),
            m_pCheckData.Key_Bar_Code[0].end(),bCode);

    if(it != m_pCheckData.Key_Bar_Code[0].end())
    {
        log_add_text_edit("当前扫描条码内容重复，请重新扫码");
        QLOG_INFO()<<"the value already exist!";
        return;
    }
    else {
        log_add_text_edit(u8"当前扫描条码内容为:"+cd);
        QLOG_INFO()<<"the value not exist,insert into the list!";
        m_pCheckData.Key_Bar_Code[0].push_back(bCode);
        if(m_pCheckData.Key_Bar_Code[0].size() == 3){
            m_pCheckData.Content_Data.Serial_Number = (char*)m_pCheckData.Key_Bar_Code[0].begin()->c_str();
            GDataFactory::get_mes_process()->exec_http_common_get(format_check_json_info(),1);
        }
    }
}

void FPCPressFitWgt::slot_station_config()
{
     GDataFactory::get_station_config_wgt()->set_station_characters(2);
     GDataFactory::get_station_config_wgt()->show();
}

void FPCPressFitWgt::slot_update_start_info()
{
    m_pCheckData.Key_Bar_Code[0].clear();

    m_pScanMainBoard->set_color(0);
    m_pScanKeyPart->set_color(0);
    m_pKeyPartCheckState->set_color(0);
    m_pPressFitCheckState->set_color(0);
    m_pFinalCheckState->set_color(0);
    m_pLogTextEdit->clear();
    m_pListCode.clear();
    m_pListDesc.clear();
    m_printBARCODE.clear();
    this->tableWidget->clearContents();
}

void FPCPressFitWgt::slot_rev_set_info(const int characterid,const QString EquipCode, const QString EquipName, const QString StationCode)
{
    if(characterid == 2)
    {
        m_pLineEditStationCode->setText(StationCode);
        m_pLineEditEquipCode->setText(EquipCode);
        m_pLineEditEquipName->setText(EquipName);
    }
}

void FPCPressFitWgt::slot_rev_mes_info(const QByteArray &data)
{
    if(this->isVisible())
        ;
    else
        return;
    QLOG_INFO()<<data;
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

    log_add_text_edit(u8"Mes返回结果描述为:"+eventdes);
    if(statusString == "S")
    {
        if(jsonObject.contains("EVENT_DATA"))
        {
            QJsonObject objA = jsonObject["EVENT_DATA"].toObject();
            if(!objA.isEmpty()){
                if(objA.contains("KEY_PARTS_PRINT"))
                {
                    m_printBARCODE.clear();
                    QJsonValue array = objA.value("KEY_PARTS_PRINT");
                    if(array.isArray())
                    {
                        QJsonArray iconArray = array.toArray();
                        for(int i=0;i<iconArray.size();i++)
                        {
                            QJsonValue iconValue = iconArray.at(i);
                            QJsonObject icon = iconValue.toObject();
                            m_printBARCODE = icon["BARCODE"].toString();
                        }
                    }
                }

                if(objA.contains("KEY_PARTS"))
                {
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

                m_pPressStationService->startScanRes(SCAN_RES_TYPE::ResScanOK);
                log_add_text_edit(u8"关键物料校验OK,工装可以压合");
                m_press_process = 1;
            }else{
                //上传结果成功
                m_pFinalCheckState->set_color(1);
                log_add_text_edit(u8"MES结果校验成功");
                m_pCheckData.Key_Bar_Code->clear();
                m_pResultData.Result_Content.Content_Detail.clear();
                set_table_data();
            }
        }
    }
    else {
        m_pCheckData.Key_Bar_Code[0].clear();
        m_pResultData.Result_Content.Content_Detail.clear();
        log_add_text_edit(u8"MES返回数据错误");
        QLOG_WARN()<<"MES status para is error!";
        return;
    }
}

void FPCPressFitWgt::slot_recv_preas_result_info(int code)
{
    QVector<std::string> barCodeList;

    ResultDetailDataApp data;
    char* bar_code[2];
    bar_code[0] = new char[50];
    bar_code[1] = new char[50];
    char* keyBarCode = new char[50];

    QLOG_INFO() << "receive press result slot" << code;
    if(m_press_process == 1){
        if(code == PRESS_RESULT::ResPressEnd){
            m_pPressStationService->readPressMeter();
            log_add_text_edit(u8"压合工装压合成功！");
//            killTimer(m_pTimerID);
//            m_pPressFitCheckState->set_color(1);//压合OK

//            std::list<std::string>::iterator it;
//            it = m_pCheckData.Key_Bar_Code[0].begin();
//            int i=0;
//            while (it != m_pCheckData.Key_Bar_Code[0].end()) {
//                std::string amb = *it;
//                QLOG_WARN() << "value" << amb.c_str();
//                if(*it == m_pCheckData.Content_Data.Serial_Number)
//                {
//                    ++it;
//                    continue;
//                }

//                strcpy_s(keyBarCode,50,amb.c_str());
//                //keyBarCode = (char*)(amb.c_str());
//                QLOG_WARN() << "keyBarCode" << keyBarCode;
//                ++it;
//                i++;
//            }
//            m_pResultData.Result_Content.Serial_Number = m_pCheckData.Content_Data.Serial_Number;
//            m_pResultData.Result_Content.QM_Status = (char*)"OK";
//            //ResultDetailData data;
//            data.Key_Bar_Code = keyBarCode;
//            data.Before_Value = NULL;
//            data.After_Value = NULL;
//            data.Item_Value = NULL;
//            data.Item_QM_status = (char*)"OK";
//            data.Field1 = (char*)"";
//            data.Field2 = (char*)"";
//            data.Field3 =(char*)"";
//            m_pResultData.Result_Content.Content_Detail.push_back(data);

//            QLOG_WARN() <<m_pResultData.Result_Content.Serial_Number;

//            if(!m_printBARCODE.isEmpty()){
//                zebraPrinterService* pZebraPrinter = new zebraPrinterService;
//                QString msgInfo;
//                log_add_text_edit(u8"打印主板条码："+m_printBARCODE);
//                if(pZebraPrinter->printExcuteFunc(m_printBARCODE, GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_MODEL_PATH"),msgInfo)){
//                    QLOG_WARN() << u8"打印完成";
//                    log_add_text_edit(u8"打印结果OK！");
//                }else{
//                    QLOG_WARN() << u8"打印失败！";
//                    log_add_text_edit(u8"打印失败");
//                }
//            }

//            GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),0);

        }else if(code == PRESS_RESULT::ResTimeOut){
            killTimer(m_pTimerID);
            log_add_text_edit(u8"压合工装超时，请重新扫码压合");
            m_pCheckData.Key_Bar_Code->clear();
            m_pResultData.Result_Content.Content_Detail.clear();
        }else if(code == PRESS_RESULT::ResPressStart){
            QLOG_INFO() << "receive start press signal";
            m_pTimerID = startTimer(500);
        }
    }
}

void FPCPressFitWgt::slot_rev_mano_meter_result(uint16_t r)
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

    QVector<std::string> barCodeList;

    char* bar_code[2];
    bar_code[0] = new char[50];
    bar_code[1] = new char[50];

    killTimer(m_pTimerID);
    m_pPressFitCheckState->set_color(1);//压合OK

    std::list<std::string>::iterator it;
    it = m_pCheckData.Key_Bar_Code[0].begin();
    int i=0;
    while (it != m_pCheckData.Key_Bar_Code[0].end()) {
        std::string amb = *it;
        QLOG_WARN() << "value" << amb.c_str();
        if(*it == m_pCheckData.Content_Data.Serial_Number)
        {
            ++it;
            continue;
        }

        ResultDetailDataApp data;
        char* keyBarCode = new char[50];
        strcpy_s(keyBarCode,50,amb.c_str());
        //keyBarCode = (char*)(amb.c_str());
        QLOG_WARN() << "keyBarCode" << keyBarCode;
        ++it;
        i++;

        //ResultDetailData data;
        data.Key_Bar_Code = keyBarCode;
        data.Before_Value = NULL;
        data.After_Value = NULL;
        data.Item_Value = NULL;
        data.Item_QM_status = (char*)"OK";
        data.Field1 = (char*)"";
        data.Field2 = (char*)"";
        data.Field3 =(char*)"";
        m_pResultData.Result_Content.Content_Detail.push_back(data);
    }

    m_pResultData.Result_Content.Serial_Number = m_pCheckData.Content_Data.Serial_Number;
    m_pResultData.Result_Content.QM_Status = (char*)"OK";

    QLOG_WARN() <<m_pResultData.Result_Content.Serial_Number;

    if(!m_printBARCODE.isEmpty()){
        zebraPrinterService* pZebraPrinter = new zebraPrinterService;
        QString msgInfo;
        log_add_text_edit(u8"打印主板条码："+m_printBARCODE);
        QLOG_INFO()<<u8"打印的主板条码:"<<m_printBARCODE;
        if(pZebraPrinter->printExcuteFunc(m_printBARCODE, GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_MODEL_PATH"),msgInfo)){
            QLOG_WARN() << u8"打印完成";
            log_add_text_edit(u8"打印结果OK！");
        }else{
            QLOG_WARN() << u8"打印失败！";
            log_add_text_edit(u8"打印失败");
        }
    }
    else {
        QLOG_WARN()<<u8"打印的条码为空!";
    }

    GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),0);
}

