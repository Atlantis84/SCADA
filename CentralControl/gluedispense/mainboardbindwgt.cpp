#include "mainboardbindwgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "gdatafactory.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QSqlQuery>
MainBoardBindWgt* MainBoardBindWgt::m_pMainBoardBindWgt = nullptr;
static int sign = 0;

MainBoardBindWgt::MainBoardBindWgt(QWidget *parent) : QWidget(parent)
{
    m_pLogTextEdit = new QTextEdit();
    m_pLogTextEdit->setStyleSheet("QTextEdit{background-color:rgba(255,255,255,100);color:rgb(0,0,0);font-family:KaiTi;font-size:20px;}");
    ngCheckBox = new QCheckBox(u8"重新扫描胶料号");
    connect(ngCheckBox,SIGNAL(stateChanged(int)),this,SLOT(slot_state_changed(int)));
    ngCheckBox->setStyleSheet("QCheckBox{color:rgb(0,0,0);font-family:'KaiTi';font-size: 20px;font-weight:normal;}");

    m_pGlueCheckState = new BallWidget();
    m_pPasteGlueCheckState = new BallWidget();
    m_pKeyPartCheckState = new BallWidget();
    m_pPressFitCheckState = new BallWidget();
    m_pFinalCheckState = new BallWidget();

    m_pLineEditStationCode = new QLineEdit();
    m_pLineEditEquipCode = new QLineEdit();
    m_pLineEditEquipName= new QLineEdit();
    m_pLineEditGlueInfo = new QLineEdit();
    m_pLineEditGlueStandard = new QLineEdit();
    QList<QLineEdit*> lst;
    lst<<m_pLineEditStationCode<<m_pLineEditEquipCode<<m_pLineEditEquipName<<m_pLineEditGlueInfo<<m_pLineEditGlueStandard;
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

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QGroupBox* grpGlueInfo = new QGroupBox(u8"胶料信息");
    grpGlueInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QLabel* labelGlue = new QLabel(u8"当前胶料号信息:");
    hBox2->addWidget(labelGlue);
    hBox2->addWidget(m_pLineEditGlueInfo);
    hBox2->addWidget(ngCheckBox);
    grpGlueInfo->setLayout(hBox2);

    QGroupBox* grpFinalResult = new QGroupBox(u8"校验及结果上传信息");
    grpFinalResult->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox21 = new QHBoxLayout();
    QLabel* labelGlueResult = new QLabel(u8"胶料校验");
    QLabel* labelPasteGlueResult = new QLabel(u8"点胶结果");
    QLabel* labelKeyPartCheck = new QLabel(u8"关键件校验");
    QLabel* labelPressFitResult = new QLabel(u8"压合结果");
    QLabel* labelFinalResult = new QLabel(u8"结果上传");

    QPushButton* btnRestart = new QPushButton(u8"重新开始");
    btnRestart->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,255);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnRestart,SIGNAL(clicked()),this,SLOT(slot_restart()));
    hBox21->addStretch();
    hBox21->addWidget(m_pGlueCheckState);
    hBox21->addWidget(labelGlueResult);
    hBox21->addStretch();
    hBox21->addWidget(m_pPasteGlueCheckState);
    hBox21->addWidget(labelPasteGlueResult);
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
    hBox21->addWidget(btnRestart);
    hBox21->addStretch();
    grpFinalResult->setLayout(hBox21);

    QHBoxLayout* hBox3 = new QHBoxLayout();

    QGroupBox* grpCheckInfo = new QGroupBox(u8"物料描述及标准值");
    grpCheckInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox1 = new QVBoxLayout();
    QHBoxLayout* hBox11 = new QHBoxLayout();
    QLabel* labelGlueStandard = new QLabel(u8"胶量标准值:");
    hBox11->addWidget(labelGlueStandard);
    hBox11->addWidget(m_pLineEditGlueStandard);

    QGroupBox* grpKeyMaterialInfo = new QGroupBox(u8"关键物料描述");
    grpKeyMaterialInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox11 = new QVBoxLayout();
    vBox11->addWidget(createTableWidget());
    grpKeyMaterialInfo->setLayout(vBox11);
    vBox1->addLayout(hBox11);
    vBox1->addWidget(grpKeyMaterialInfo);
    grpCheckInfo->setLayout(vBox1);

    QVBoxLayout* vBox21 = new QVBoxLayout();
    QGroupBox* grpScanInfo = new QGroupBox(u8"扫描信息");
    grpScanInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox21->addWidget(m_pLogTextEdit);
    grpScanInfo->setLayout(vBox21);

    hBox3->addWidget(grpCheckInfo);
    hBox3->addWidget(grpScanInfo);

    vAll->addWidget(grpEquipInfo);
    vAll->addWidget(grpGlueInfo);
    vAll->addWidget(grpFinalResult);
    vAll->addLayout(hBox3);
    this->setLayout(vAll);
    connect(GDataFactory::get_station_config_wgt(),SIGNAL(signal_send_set_info(const int,const QString, const QString, const QString)),
            this,SLOT(slot_rev_set_info(const int,const QString,const QString,const QString)));
    connect(GDataFactory::get_mes_process(),SIGNAL(signal_to_common(const QByteArray&)),
            this,SLOT(slot_rev_mes_info(const QByteArray&)));

    pComm = nullptr;
    m_pAlreadyPressOver = false;

    connect(GDataFactory::get_glue_weight_comm_service(),SIGNAL(notify_result_weight(int, QString)),this,SLOT(slot_weight_result(int, QString)));
}

QWidget *MainBoardBindWgt::createTableWidget()
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

void MainBoardBindWgt::slot_rev_set_info(const int characterid,const QString EquipCode, const QString EquipName, const QString StationCode)
{
    if(characterid == 1)
    {
        m_pLineEditStationCode->setText(StationCode);
        m_pLineEditEquipCode->setText(EquipCode);
        m_pLineEditEquipName->setText(EquipName);
    }
}

static bool presssign = false;
void MainBoardBindWgt::slot_rev_mano_meter_result(uint16_t p)
{
    if(presssign == true)
        return;
    //rev info from manometer
    double tmpV = static_cast<double>(p/100.0);
    QString strR = u8"收到压合工装压力值:"+QString::number(tmpV,'f',2);
    m_pLogTextEdit->append(strR+"KG");

    m_pLogTextEdit->append(u8"压力上限值为:"+GDataFactory::get_factory()->get_config_para("PRESSURE_HIGH_VALUE")+"KG");
    m_pLogTextEdit->append(u8"压力下限值为:"+GDataFactory::get_factory()->get_config_para("PRESSURE_LOW_VALUE")+"KG");

    double realValue = p/100.0;
    double topValue = GDataFactory::get_factory()->get_config_para("PRESSURE_HIGH_VALUE").toDouble();
    double lowValue = GDataFactory::get_factory()->get_config_para("PRESSURE_LOW_VALUE").toDouble();

    if((GDataFactory::get_factory()->isGreater(realValue,lowValue)) && (GDataFactory::get_factory()->isLess(realValue,topValue)))
        ;
    else {
        presssign = true;
        m_pLogTextEdit->append(u8"压力值超限，请关闭窗口，调整设备重新开始!");
        return;
    }

    if(m_pAlreadyPressOver == true)
        m_pAlreadyPressOver = false;
    else
        return;

    //together
    killTimer(m_pTimerID);
    sign = 0;
    //together

    m_pPressFitCheckState->set_color(1);

    //upload info to MES
    GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),0);
    //upload info to MES
}

void MainBoardBindWgt::slot_press_result(int resultcode)
{
    if(resultcode == PRESS_RESULT::ResPressStart)
        m_pTimerID = startTimer(500);
    else if(resultcode == PRESS_RESULT::ResTimeOut)
    {
        QLOG_WARN()<<u8"压合超时!";
    }
    else
    {
        m_pAlreadyPressOver = true;
        m_pPressStationSerialCom->readPressMeter();
        m_pLogTextEdit->append(u8"压合结束!");
//        killTimer(m_pTimerID);
//        m_pPressFitCheckState->set_color(1);

//        //upload info to MES
//        GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),0);
//        //upload info to MES
    }
}

void MainBoardBindWgt::slot_weight_result(int nWorkId, QString weight)
{
//    if(m_pCheckData.Key_Bar_Code[0].size() != 2)
//    {
//        QLOG_WARN()<<"the count of sn is not correct!";
//        m_pLogTextEdit->append(u8"点胶前条码数量错误，无法进行点胶!");
//        return;
//    }

    if(nWorkId == GLUE_BEFORE_WEIGHT)
    {
        QLOG_INFO()<<"glue before value is:"<<weight;
        m_pLogTextEdit->append(u8"点胶前重量为:"+weight);
        m_pGlueBeforeValue = weight.toDouble();
    }
    else
    {
        QLOG_INFO()<<"glue after value is:"<<weight;
        m_pLogTextEdit->append(u8"点胶后重量为:"+weight);
        m_pGlueAfterValue = weight.toDouble();

        double middleValue = m_pGlueAfterValue - m_pGlueBeforeValue;
        QString r = "";
        if((middleValue > m_pGlueLowerValue) && (middleValue < m_pGlueUpperValue))
            r = "OK";
        else
            r = "NG";

        QList<QString> lst;
        std::list<std::string>::iterator it;
        it = m_pCheckData.Key_Bar_Code[0].begin();
        while (it != m_pCheckData.Key_Bar_Code[0].end())
        {
            lst.push_back(QString::fromStdString(*it));
            ++it;
        }

        QList<QString> lstglueinfo;
        lstglueinfo.push_back(QString::number(m_pGlueBeforeValue));
        lstglueinfo.push_back(QString::number(m_pGlueAfterValue));
        lstglueinfo.push_back(QString::number(middleValue));
        insert_glue_result_to_db(lst,r,lstglueinfo);

        if(r == "OK")
        {
            m_pLogTextEdit->append(u8"点胶成功，请扫描关键件(马达等)条码!");
            m_pPasteGlueCheckState->set_color(1);
        }
        else
            m_pLogTextEdit->append(u8"点胶失败!");
    }
}

void MainBoardBindWgt::slot_restart()
{
    m_pAlreadyPressOver = false;
    truncate_data_in_db();

    m_pCurrentMiddleFrameNumber = "";
//    //glue
//    GDataFactory::get_glue_weight_comm_service()->startReadModbusSignal(false);
//    //glue

//    //press
//    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
//    m_pPressStationSerialCom->disconnectToMCU();
//    delete m_pPressStationSerialCom;
//    m_pPressStationSerialCom = nullptr;
//    //press

    //together
    killTimer(m_pTimerID);
    sign = 0;
    //together

    reset_station_status();
    m_pCheckData.Key_Bar_Code[0].clear();
    m_pResultData.Result_Content.Content_Detail.clear();
//    stop_serial_port();

//    //glue
//    GDataFactory::get_glue_weight_comm_service()->startReadModbusSignal(true);
//    //glue

//    //press
//    m_pPressStationSerialCom = new PressStationSerialCom();
//    connect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
//    QString portName = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS");
//    QString portRate = GDataFactory::get_factory()->get_config_para("COM_PORT_RED_FLAG_PRESS_BARD_RATE");
//    m_pPressStationSerialCom->connectToMCU(portName,portRate);
//    //press

//    //manometer
////    m_pManoMeterCom = new PressStationSerialCom();
//    connect(m_pPressStationSerialCom,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
//    m_pPressStationSerialCom->initManoMeter(GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER"),
//                                   GDataFactory::get_factory()->get_config_para("COM_PORT_MANO_METER_BARD_RATE"));
//    //manometer

//    stop_serial_port();
//    read_serial_number();

    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_EQUIPMENT_NAME"));
    m_pLineEditGlueInfo->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_GLUE_NUMBER"));

    //to fill check key-part info
    m_pCheckData.Source_System = (char*)("EQUIPMENT");
    m_pCheckData.Message_Type = (char*)("PASS_STATION_CHECK");
    m_pCheckData.Content_Data.Serial_Number = (char*)"";

    std::string sgCode = GDataFactory::get_factory()->get_config_para("MAIN_BOARD_GLUE_NUMBER").toStdString();
    m_pCheckData.Key_Bar_Code[0].push_back(sgCode);

    QByteArray stationCode = GDataFactory::get_factory()->
            get_config_para("MAIN_BOARD_STATION_CODE").toLatin1();
    m_pCheckData.Content_Data.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Station_Code,stationCode.length()+1,stationCode.data());

    QByteArray equipmentCode = GDataFactory::get_factory()->
            get_config_para("MAIN_BOARD_EQUIPMENT_CODE").toLatin1();
    m_pCheckData.Content_Data.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    QByteArray equipmentName = GDataFactory::get_factory()->
            get_config_para("MAIN_BOARD_EQUIPMENT_NAME").toLatin1();
    m_pCheckData.Content_Data.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    m_pCheckData.Content_Data.Check_Type = (char*)("2");
    //to fill check key-part info

    //to fill result info
    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("SAVE_DJ_INFO");

    m_pResultData.Result_Content.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pResultData.Result_Content.Station_Code,stationCode.length()+1,stationCode.data());

    m_pResultData.Result_Content.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pResultData.Result_Content.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    m_pResultData.Result_Content.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pResultData.Result_Content.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    //to fill result info
}

void MainBoardBindWgt::slot_station_config()
{
    GDataFactory::get_station_config_wgt()->set_station_characters(1);
    GDataFactory::get_station_config_wgt()->show();
}

void MainBoardBindWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pAlreadyPressOver = false;
    presssign = false;

    //glue
    GDataFactory::get_glue_weight_comm_service()->startReadModbusSignal(true);
    //glue

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

    stop_serial_port();
    read_serial_number();
//    stop_serial_port_glue();
//    read_serial_number_glue();
    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_EQUIPMENT_NAME"));
    m_pLineEditGlueInfo->setText(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_GLUE_NUMBER"));

    //to fill check key-part info
    m_pCheckData.Source_System = (char*)("EQUIPMENT");
    m_pCheckData.Message_Type = (char*)("PASS_STATION_CHECK");
    m_pCheckData.Content_Data.Serial_Number = (char*)"";

    std::string sgCode = GDataFactory::get_factory()->get_config_para("MAIN_BOARD_GLUE_NUMBER").toStdString();
    m_pCheckData.Key_Bar_Code[0].push_back(sgCode);

    QByteArray stationCode = GDataFactory::get_factory()->
            get_config_para("MAIN_BOARD_STATION_CODE").toLatin1();
    m_pCheckData.Content_Data.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Station_Code,stationCode.length()+1,stationCode.data());

    QByteArray equipmentCode = GDataFactory::get_factory()->
            get_config_para("MAIN_BOARD_EQUIPMENT_CODE").toLatin1();
    m_pCheckData.Content_Data.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    QByteArray equipmentName = GDataFactory::get_factory()->
            get_config_para("MAIN_BOARD_EQUIPMENT_NAME").toLatin1();
    m_pCheckData.Content_Data.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    m_pCheckData.Content_Data.Check_Type = (char*)("2");
    //to fill check key-part info

    //to fill result info
    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("SAVE_DJ_INFO");

    m_pResultData.Result_Content.Station_Code = new char[stationCode.length()+1];
    strcpy_s(m_pResultData.Result_Content.Station_Code,stationCode.length()+1,stationCode.data());

    m_pResultData.Result_Content.Equipment_Code = new char[equipmentCode.length()+1];
    strcpy_s(m_pResultData.Result_Content.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    m_pResultData.Result_Content.Equipment_Name = new char[equipmentName.length()+1];
    strcpy_s(m_pResultData.Result_Content.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    //to fill result info
}

void MainBoardBindWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    m_pCurrentMiddleFrameNumber = "";
    //glue
    GDataFactory::get_glue_weight_comm_service()->startReadModbusSignal(false);
    //glue

    //press
    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_result(int)),this,SLOT(slot_press_result(int)));
    disconnect(m_pPressStationSerialCom,SIGNAL(notify_press_meter(uint16_t)),this,SLOT(slot_rev_mano_meter_result(uint16_t)));
    m_pPressStationSerialCom->disconnectToMCU();
    delete m_pPressStationSerialCom;
    m_pPressStationSerialCom = nullptr;
    //press

    //together
    killTimer(m_pTimerID);
    sign = 0;
    //together

    reset_station_status();
    m_pCheckData.Key_Bar_Code[0].clear();
    m_pResultData.Result_Content.Content_Detail.clear();
    stop_serial_port();
//    stop_serial_port_glue();
}

QString MainBoardBindWgt::get_current_time()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString r = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
    return r;
}

void MainBoardBindWgt::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(sign%2 == 0)
        m_pPressFitCheckState->set_color(1);
    else
        m_pPressFitCheckState->set_color(0);

    if(sign == 3)//2s
    {
        m_pLogTextEdit->append(get_current_time()+u8":2s压力值读取");
        m_pPressStationSerialCom->readPressMeter();
    }

    if(sign == 9)//5s
    {
        m_pLogTextEdit->append(get_current_time()+u8":5s压力值读取");
        m_pPressStationSerialCom->readPressMeter();
    }

    if(sign == 15)//8s
    {
        m_pLogTextEdit->append(get_current_time()+u8":8s压力值读取");
        m_pPressStationSerialCom->readPressMeter();
    }

    sign++;
}

void MainBoardBindWgt::read_serial_number()
{
    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = GDataFactory::get_factory()->get_config_para("RED_FLAG_SCANNER_PORT");
        QString portRate = GDataFactory::get_factory()->get_config_para("RED_FLAG_SCANNER_BARD_RATE");
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));
        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0x04));
        aa.append(static_cast<char>(0xE4));
        aa.append(static_cast<char>(0x04));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0xFF));
        aa.append(static_cast<char>(0x14));
        datas.append(aa);
        IResponseValidate* pF1 = new IResponseValidate_F1();
        pComm->recvResponse_only();
        pComm->start();
    }
}

void MainBoardBindWgt::stop_serial_port()
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

void MainBoardBindWgt::push_glue_info_to_list()
{
    QString strSql = QString("select * from public.%1").arg(constRedFlagGlueInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            ResultDetailData rd;
            QByteArray keybarcode = queryResult.value(1).toString().toLatin1();
            rd.Key_Bar_Code = new char[keybarcode.length()+1];
            strcpy_s(rd.Key_Bar_Code,keybarcode.length()+1,keybarcode.data());

            QByteArray beforevalue = queryResult.value(3).toString().toLatin1();
            rd.Before_Value = new char[beforevalue.length()+1];
            strcpy_s(rd.Before_Value,beforevalue.length()+1,beforevalue.data());

            QByteArray aftervalue = queryResult.value(4).toString().toLatin1();
            rd.After_Value = new char[aftervalue.length()+1];
            strcpy_s(rd.After_Value,aftervalue.length()+1,aftervalue.data());

            QByteArray itemvalue = queryResult.value(5).toString().toLatin1();
            rd.Item_Value = new char[itemvalue.length()+1];
            strcpy_s(rd.Item_Value,itemvalue.length()+1,itemvalue.data());

            QByteArray itemqmstatus = queryResult.value(6).toString().toLatin1();
            rd.Item_QM_status = new char[itemqmstatus.length()+1];
            strcpy_s(rd.Item_QM_status,itemqmstatus.length()+1,itemqmstatus.data());

            rd.Field1 = (char*)"";
            rd.Field2 = (char*)"";
            rd.Field3 = (char*)"";

            m_pResultData.Result_Content.Content_Detail.push_back(rd);
        }
    }
   else
   {
       QLOG_WARN()<<u8"query database failed!";
   }
}

QByteArray MainBoardBindWgt::format_check_json_info()
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
    QLOG_WARN()<<"check data is:"<<result;
    return result;
}

QByteArray MainBoardBindWgt::format_result_json_info()
{
    push_glue_info_to_list();
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

//    if(m_pCheckData.Key_Bar_Code[0].size() == 6)---20231218 lilei
    if(m_pCheckData.Key_Bar_Code[0].size() == m_pListDesc.length()+1)
    {
        std::list<std::string>::iterator it;
        it = m_pCheckData.Key_Bar_Code[0].begin();
        while (it != m_pCheckData.Key_Bar_Code[0].end())
        {
            if((*it == m_pResultData.Result_Content.Serial_Number)||(*it == m_pLineEditGlueInfo->text().trimmed().toStdString()))
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

void MainBoardBindWgt::update_ui_according_to_mes_info()
{
    m_pLineEditGlueStandard->setText(QString::number(m_pGlueStandardValue)+" g");
    m_pGlueCheckState->set_color(1);
    set_table_data();
//    if(m_pCheckData.Key_Bar_Code[0].size() == 6)---20231218 lilei
    if(m_pCheckData.Key_Bar_Code[0].size() == m_pListDesc.length()+1)
    {
        m_pKeyPartCheckState->set_color(1);
        notify_fpc_to_press();
    }
}

void MainBoardBindWgt::set_table_data()
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

void MainBoardBindWgt::reset_station_status()
{
    m_pGlueCheckState->set_color(0);
    m_pKeyPartCheckState->set_color(0);
    m_pPasteGlueCheckState->set_color(0);
    m_pPressFitCheckState->set_color(0);
    m_pFinalCheckState->set_color(0);
    m_pLineEditGlueStandard->setText("");
    tableWidget->clearContents();
    m_pLogTextEdit->clear();
}

void MainBoardBindWgt::check_glue_info_of_db(const QString middleframenumber, bool &midfexistsign, bool &oksign)
{
    QString strSql = QString("select * from public.%1").arg(constRedFlagGlueInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(middleframenumber == queryResult.value(2).toString())
                midfexistsign = true;
            if(queryResult.value(6).toString() == "OK")
                oksign = true;
        }
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
}

void MainBoardBindWgt::push_key_bar_to_list(QString barcode)
{
    //restart to scan another product
    if(m_pCheckData.Key_Bar_Code[0].size() == 1)
    {
        m_pLogTextEdit->clear();
        m_pGlueCheckState->set_color(0);
        m_pPasteGlueCheckState->set_color(0);
        m_pPressFitCheckState->set_color(0);
        m_pFinalCheckState->set_color(0);
        m_pKeyPartCheckState->set_color(0);

        QByteArray stationCode = barcode.toLatin1();
        m_pCheckData.Content_Data.Serial_Number = new char[stationCode.length()+1];
        strcpy_s(m_pCheckData.Content_Data.Serial_Number,stationCode.length()+1,stationCode.data());

        m_pResultData.Result_Content.Serial_Number = new char[stationCode.length()+1];
        strcpy_s(m_pResultData.Result_Content.Serial_Number,stationCode.length()+1,stationCode.data());
    }
    //restart to scan another product

    QByteArray bb = barcode.toLatin1();
    char* bCode = new char[bb.length()+1];
    strcpy_s(bCode,bb.length()+1,bb.data());
    std::list<string>::iterator it = std::find(m_pCheckData.Key_Bar_Code[0].begin(),
            m_pCheckData.Key_Bar_Code[0].end(),bCode);
    if(it != m_pCheckData.Key_Bar_Code[0].end())
    {
        m_pLogTextEdit->append(u8"当前关键料条码已扫，请重新扫码");
        QLOG_INFO()<<"the number value already exist!";
    }
    else {
        QLOG_INFO()<<"the value not exist,insert into the list!";
        m_pCheckData.Key_Bar_Code[0].push_back(bCode);
        if(m_pCheckData.Key_Bar_Code[0].size() == 2)
        {
            m_pCurrentMiddleFrameNumber = bCode;
            m_pCheckData.Content_Data.Serial_Number = bCode;
            QLOG_INFO()<<m_pCheckData.Content_Data.Serial_Number;
        }
    }
}

int MainBoardBindWgt::check_repeated_ok_result_of_glue(const QString middleframenumber)
{
    bool checkResult = false;
    QString strSql = QString("select * from public.%1").arg(constRedFlagGlueInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        QLOG_WARN()<<middleframenumber;
        while(queryResult.next())
        {
            QLOG_WARN()<<queryResult.value(6).toString();
            if((middleframenumber == queryResult.value(2).toString())&&
                   (queryResult.value(6).toString() == "OK"))
            {
                QLOG_WARN()<<"found middle frame number";
                checkResult = true;
                break;
            }
            else
                continue;

        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
        return -1;
    }
    if(checkResult)
        return 1;
    else
        return 0;
}

void MainBoardBindWgt::insert_glue_result_to_db(QList<QString> info, const QString result,QList<QString> lstglueinfo)
{
    int check = check_repeated_ok_result_of_glue(m_pCurrentMiddleFrameNumber);
    QLOG_WARN()<<"check result is:"<<check;
    if( check == -1)
    {
        QLOG_WARN()<<"check ok result from db failed!";
        return;
    }
    else if(check == 0)//insert
    {
        if((info.size() != 2) || (lstglueinfo.size() != 3))
        {
            QLOG_WARN()<<"the size of info or lstglueinfo is not correct,insert db failed";
            m_pLogTextEdit->append(u8"数组大小超限，请确认操作是否正常!");
            return;
        }
        int maxID = GDataFactory::get_factory()->get_biggest_id(constRedFlagGlueInfoTable);

        QString strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7','%8')").
                    arg(constRedFlagGlueInfoTable).
                    arg(maxID).
                    arg(info[0]).
                    arg(info[1]).
                    arg(lstglueinfo[0]).
                    arg(lstglueinfo[1]).
                    arg(lstglueinfo[2]).
                    arg(result);
        QString strErrorMsg;
        if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
            QLOG_INFO()<<"insert glue info success!";
        else
            QLOG_WARN()<<"insert glue info failed!";
    }
    else if(check == 1)//update
    {
        if((info.size() != 2) || (lstglueinfo.size() != 3))
        {
            QLOG_WARN()<<"the size of info or lstglueinfo is not correct,update db failed";
            m_pLogTextEdit->append(u8"数组大小超限，请确认操作是否正常!");
            return;
        }
        QString strSql = QString("update public.%1 set \"Glue_Number\"='%2',"
                                 "\"Middle_Frame_Number\"='%3',\"Before_Value\"='%4',"
                                 "\"After_Value\"='%5',\"Current_Value\"='%6',\"Glue_Result\"='%7' where \"Middle_Frame_Number\"='%8' and \"Glue_Result\"='OK'").
                    arg(constRedFlagGlueInfoTable).
                    arg(info[0]).
                    arg(info[1]).
                    arg(lstglueinfo[0]).
                    arg(lstglueinfo[1]).
                    arg(lstglueinfo[2]).
                    arg(result).arg(m_pCurrentMiddleFrameNumber);
        QString strErrorMsg;
        if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
            QLOG_INFO()<<"update glue info success!";
        else
            QLOG_WARN()<<"update glue info failed!";
    }
}

void MainBoardBindWgt::notify_fpc_to_press()
{
    m_pPressStationSerialCom->startScanRes(SCAN_RES_TYPE::ResScanOK);
    m_pLogTextEdit->append(u8"通知压合设备进行压合！");
}

void MainBoardBindWgt::reset_check_result_data_after_over()
{
    m_pFinalCheckState->set_color(1);
    m_pCheckData.Key_Bar_Code[0].clear();
    push_key_bar_to_list(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_GLUE_NUMBER"));

    m_pResultData.Result_Content.Content_Detail.clear();
    truncate_data_in_db();
}

void MainBoardBindWgt::truncate_data_in_db()
{
    QString strTruncate = QString("truncate table public.%1").arg(constRedFlagGlueInfoTable);
    QString tmpEr;
    if(GDataFactory::get_pgsql()->ExecSql(strTruncate,tmpEr))
    {
        m_pLogTextEdit->append(u8"缓存数据清空成功!");
        QLOG_INFO()<<"clear cached data SUCCESS!";
    }
    else {
        m_pLogTextEdit->append(u8"缓存数据清空失败!");
        QLOG_WARN()<<"clear cached data FAILED!";
    }
}

void MainBoardBindWgt::slot_rev_serial_number(const QByteArray &data)
{
    QString cd = data;
    if(cd.contains('\r'))
        cd.remove('\r');

    push_key_bar_to_list(cd);

    if(ngCheckBox->isChecked())
    {
        m_pCheckData.Key_Bar_Code[0].clear();
//        push_key_bar_to_list(cd);---20231218
        m_pLineEditGlueInfo->setText(cd);
        GDataFactory::get_factory()->set_config_para_1("MAIN_BOARD_GLUE_NUMBER",cd);
        GDataFactory::get_factory()->save_config_file();
        m_pLogTextEdit->append(u8"当前胶料条码内容为:"+cd);
    }
    else
    {
        bool middleframesign = false;
        bool oksign = false;
        check_glue_info_of_db(cd,middleframesign,oksign);
        if(middleframesign == true)
        {
            if(oksign == true)
            {
                m_pGlueCheckState->set_color(1);
                m_pPasteGlueCheckState->set_color(1);
                m_pLogTextEdit->append(u8"当前产品点胶完成，请继续进行下一步!");
            }
            else
                m_pLogTextEdit->append(u8"当前产品处于点胶NG状态，请重新点胶!");
        }

        m_pLogTextEdit->append(u8"当前关键件条码内容为:"+cd);
        GDataFactory::get_mes_process()->exec_http_common_get(format_check_json_info(),1);
    }
}

void MainBoardBindWgt::slot_state_changed(int state)
{
    if(state != 0)
    {
        m_pLineEditGlueInfo->setText("");
        truncate_data_in_db();
    }
}

void MainBoardBindWgt::slot_rev_mes_info(const QByteArray &data)
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
        if(jsonObject.contains("EVENT_DATA"))
        {
            QJsonObject objA = jsonObject["EVENT_DATA"].toObject();
            if(!objA.isEmpty()){
                if(objA.contains("STANDARD_VALUE"))
                {
                    QJsonValue array = objA.value("STANDARD_VALUE");
                    if(array.isArray())
                    {
                        QJsonArray iconArray = array.toArray();
                        for(int i=0;i<iconArray.size();i++)
                        {
                            QJsonValue iconValue = iconArray.at(i);
                            QJsonObject icon = iconValue.toObject();
                            m_pGlueUpperValue = icon["UPPER_VAL"].toDouble();
                            m_pGlueLowerValue = icon["LOWER_VAL"].toDouble();
                            m_pGlueStandardValue = icon["STANDARD_VAL"].toDouble();
                        }
                    }
                }

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
        if(jsonObject.contains("EVENT_DATA"))
        {
            QJsonObject objA = jsonObject["EVENT_DATA"].toObject();
            if(!objA.isEmpty())
                m_pCheckData.Key_Bar_Code[0].pop_back();
            else
            {
                m_pGlueCheckState->set_color(0);
                m_pKeyPartCheckState->set_color(0);
                m_pPasteGlueCheckState->set_color(0);
                m_pPressFitCheckState->set_color(0);
                m_pFinalCheckState->set_color(0);
                m_pCheckData.Key_Bar_Code[0].clear();
                push_key_bar_to_list(GDataFactory::get_factory()->get_config_para("MAIN_BOARD_GLUE_NUMBER"));
                m_pResultData.Result_Content.Content_Detail.clear();
            }
        }
        QLOG_WARN()<<"MES status para is error!";
    }
}
