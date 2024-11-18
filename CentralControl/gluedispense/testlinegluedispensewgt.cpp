#include "testlinegluedispensewgt.h"
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
#include <QHostInfo>
TestLineGlueDispenseWgt* TestLineGlueDispenseWgt::m_pTestGlueDispenseWgt = nullptr;
TestLineGlueDispenseWgt::TestLineGlueDispenseWgt(QWidget *parent) : QWidget(parent)
{
    m_pLogTextEdit = new QTextEdit();
    m_pLogTextEdit->setStyleSheet("QTextEdit{background-color:rgba(255,255,255,100);color:rgb(0,0,0);font-family:KaiTi;font-size:20px;}");
    ngCheckBox = new QCheckBox(u8"重新扫描胶料号");
    connect(ngCheckBox,SIGNAL(stateChanged(int)),this,SLOT(slot_state_changed(int)));
    ngCheckBox->setStyleSheet("QCheckBox{color:rgb(0,0,0);font-family:'KaiTi';font-size: 20px;font-weight:normal;}");

    m_pGlueCheckState = new BallWidget();
    m_pPasteGlueCheckState = new BallWidget();
    m_pSNCheckState = new BallWidget();
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
    QLabel* labelMainSNCheck = new QLabel(u8"主板条码校验");
    QLabel* labelFinalResult = new QLabel(u8"结果上传");
    hBox21->addStretch();
    hBox21->addWidget(m_pGlueCheckState);
    hBox21->addWidget(labelGlueResult);
    hBox21->addStretch();
    hBox21->addWidget(m_pPasteGlueCheckState);
    hBox21->addWidget(labelPasteGlueResult);
    hBox21->addStretch();
    hBox21->addWidget(m_pSNCheckState);
    hBox21->addWidget(labelMainSNCheck);
    hBox21->addStretch();
    hBox21->addWidget(m_pFinalCheckState);
    hBox21->addWidget(labelFinalResult);
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
//    pCommGlue = nullptr;

    connect(GDataFactory::get_glue_weight_comm_service(),SIGNAL(notify_result_weight(int, QString)),this,SLOT(slot_weight_result(int, QString)));
}

QWidget *TestLineGlueDispenseWgt::createTableWidget()
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

void TestLineGlueDispenseWgt::slot_rev_set_info(const int characterid,const QString EquipCode, const QString EquipName, const QString StationCode)
{
    if(characterid == 1)
    {
        m_pLineEditStationCode->setText(StationCode);
        m_pLineEditEquipCode->setText(EquipCode);
        m_pLineEditEquipName->setText(EquipName);
    }
}

void TestLineGlueDispenseWgt::slot_weight_result(int nWorkId, QString weight)
{
    m_pGlueLowerValue = GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_LOWER_VALUE").toDouble();
    m_pGlueUpperValue = GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_UPPER_VALUE").toDouble();
    if(key_bar_code.size() != 2)
    {
        QLOG_WARN()<<"the count of sn is not correct!";
        log_add_text_edit(u8"点胶前条码数量错误，无法进行点胶!");
        return;
    }

    if(nWorkId == GLUE_BEFORE_WEIGHT)
    {
        QLOG_INFO()<<"glue before value is:"<<weight;
        log_add_text_edit(u8"点胶前重量为:"+weight);
        m_pGlueBeforeValue = weight.toDouble();
    }
    else
    {
        QLOG_INFO()<<"glue after value is:"<<weight;
        log_add_text_edit(u8"点胶后重量为:"+weight);
        m_pGlueAfterValue = weight.toDouble();

        double middleValue = m_pGlueAfterValue - m_pGlueBeforeValue;
        QString r = "";
        if((middleValue > m_pGlueLowerValue) && (middleValue < m_pGlueUpperValue))
            r = "OK";
        else
            r = "NG";

        QList<QString> lst;
//        std::list<std::string>::iterator it;
//        it = m_pCheckData.Key_Bar_Code[0].begin();
//        while (it != m_pCheckData.Key_Bar_Code[0].end())
//        {
//            lst.push_back(QString::fromStdString(*it));
//            ++it;
//        }
        lst.push_back(key_bar_code[CODE_ORDER::glue_number]);
        lst.push_back(key_bar_code[CODE_ORDER::mainBoardSN]);
        lst.push_back(QString::number(m_pGlueBeforeValue));
        lst.push_back(QString::number(m_pGlueAfterValue));
        lst.push_back(QString::number(middleValue));
        insert_glue_result_to_db(lst,r);

        glue_result_slst = lst;

        if(r == "OK")
        {
            log_add_text_edit(u8"点胶成功！");
            log_add_text_edit(u8"请扫描主板条码校验!");
            m_pPasteGlueCheckState->set_color(1);
            m_gl_glue_flags = GLUE_RESULT_OK;
        }
        else{
            log_add_text_edit(u8"点胶失败!");
            m_gl_glue_flags = GLUE_RESULT_NG;
        }
    }
}

void TestLineGlueDispenseWgt::slot_station_config()
{
    GDataFactory::get_station_config_wgt()->set_station_characters(1);
    GDataFactory::get_station_config_wgt()->show();
}

void TestLineGlueDispenseWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pGlueStandardValue = GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_STANDARD_VALUE").toDouble();
    m_pLineEditGlueStandard->setText(QString::number(m_pGlueStandardValue)+" g");

    //glue
    GDataFactory::get_glue_weight_comm_service()->startReadModbusSignal(true);
    //glue

    stop_serial_port();
    read_serial_number();
//    stop_serial_port_glue();
//    read_serial_number_glue();
    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_EQUIPMENT_NAME"));
    m_pLineEditGlueInfo->setText(GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_GLUE_NUMBER"));

    //to fill check key-part info
    m_pCheckData.Source_System = (char*)("EQUIPMENT");
    m_pCheckData.Message_Type = (char*)("GET_PASS_STATION");
    m_pCheckData.Content_Data.Serial_Number = (char*)"";

    QString sgCode = GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_GLUE_NUMBER");
    key_bar_code.insert(CODE_ORDER::glue_number,sgCode);

    QByteArray stationCode = GDataFactory::get_factory()->
            get_config_para("GLUE_DISPENSE_STATION_CODE").toLatin1();
    m_pCheckData.Content_Data.Station_Code = new char[stationCode.length()];
    strcpy_s(m_pCheckData.Content_Data.Station_Code,stationCode.length()+1,stationCode.data());

    QByteArray equipmentCode = GDataFactory::get_factory()->
            get_config_para("GLUE_DISPENSE_EQUIPMENT_CODE").toLatin1();
    m_pCheckData.Content_Data.Equipment_Code = new char[equipmentCode.length()];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    QByteArray equipmentName = GDataFactory::get_factory()->
            get_config_para("GLUE_DISPENSE_EQUIPMENT_NAME").toLatin1();
    m_pCheckData.Content_Data.Equipment_Name = new char[equipmentName.length()];
    strcpy_s(m_pCheckData.Content_Data.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    m_pCheckData.Content_Data.Check_Type = (char*)("2");
    //to fill check key-part info

    //to fill result info
    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("DJ");

    m_pResultData.Result_Content.Station_Code = new char[stationCode.length()];
    strcpy_s(m_pResultData.Result_Content.Station_Code,stationCode.length()+1,stationCode.data());

    m_pResultData.Result_Content.Equipment_Code = new char[equipmentCode.length()];
    strcpy_s(m_pResultData.Result_Content.Equipment_Code,equipmentCode.length()+1,equipmentCode.data());

    m_pResultData.Result_Content.Equipment_Name = new char[equipmentName.length()];
    strcpy_s(m_pResultData.Result_Content.Equipment_Name,equipmentName.length()+1,equipmentName.data());

    //to fill result info
}

void TestLineGlueDispenseWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QLOG_INFO()<<"close wgt";
    //glue
    GDataFactory::get_glue_weight_comm_service()->startReadModbusSignal(false);
    //glue

    killTimer(m_pTimerID);
    reset_station_status();
    key_bar_code.clear();
    m_pResultData.Result_Content.Content_Detail.clear();
    stop_serial_port();
    m_gl_glue_flags = -1;
    m_gl_mes_flags = -1;
    glue_result_slst.clear();
//    stop_serial_port_glue();
}

void TestLineGlueDispenseWgt::read_serial_number()
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

void TestLineGlueDispenseWgt::stop_serial_port()
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

void TestLineGlueDispenseWgt::push_glue_info_to_list()
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
            rd.Key_Bar_Code = new char[keybarcode.length()];
            strcpy_s(rd.Key_Bar_Code,keybarcode.length()+1,keybarcode.data());

            QByteArray beforevalue = queryResult.value(3).toString().toLatin1();
            rd.Before_Value = new char[beforevalue.length()];
            strcpy_s(rd.Before_Value,beforevalue.length()+1,beforevalue.data());

            QByteArray aftervalue = queryResult.value(4).toString().toLatin1();
            rd.After_Value = new char[aftervalue.length()];
            strcpy_s(rd.After_Value,aftervalue.length()+1,aftervalue.data());

            QByteArray itemvalue = queryResult.value(5).toString().toLatin1();
            rd.Item_Value = new char[itemvalue.length()];
            strcpy_s(rd.Item_Value,itemvalue.length()+1,itemvalue.data());

            QByteArray itemqmstatus = queryResult.value(6).toString().toLatin1();
            rd.Item_QM_status = new char[itemqmstatus.length()];
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

QByteArray TestLineGlueDispenseWgt::format_check_json_info()
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

    root.insert("iType",m_pCheckData.Content_Data.Check_Type);
    root.insert("pcbCode",key_bar_code[CODE_ORDER::mainBoardSN]);
    root.insert("workStationSn",GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_WORKSTATION_SN"));

    doc.setObject(root);
    result = doc.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<"check data is:"<<result;
    m_gl_mes_flags = MES_CHECK_FLAGS;
    return result;
}

QByteArray TestLineGlueDispenseWgt::format_result_json_info()
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

    root.insert("iType",m_pCheckData.Content_Data.Check_Type);
    root.insert("pcbCode",key_bar_code[CODE_ORDER::mainBoardSN]);
    root.insert("workStationSn",GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_WORKSTATION_SN"));
    QByteArray qmstatus = "OK";
    m_pResultData.Result_Content.QM_Status = new char[qmstatus.length()];
    strcpy_s(m_pResultData.Result_Content.QM_Status,qmstatus.length()+1,qmstatus.data());
    root.insert("tsRes",m_pResultData.Result_Content.QM_Status);//点胶结果
    QString mach_name = QHostInfo::localHostName();
    root.insert("pcName",mach_name);
    QHostAddress addr;
    //通过遍历获取到本机ip
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
          QLOG_INFO()<<"Get ip adress transform data to MES:"<<address.toString(); //输出IPV4的地址
          addr=address;
          break;
        }
    }

    root.insert("pcIp",addr.toString());
    root.insert("devName",u8"点胶");
    root.insert("devModel","GZTF01");
    root.insert("devNo","GZTF01");
    root.insert("toolNo", key_bar_code[CODE_ORDER::glue_number]);

//    std::list<ResultDetailData>::iterator it;
//    it = m_pResultData.Result_Content.Content_Detail.begin();
//    int i=0;

//    while (it != m_pResultData.Result_Content.Content_Detail.end()) {
//        ResultDetailData amb = *it;
//        QJsonObject value;
//        value.insert("itemId", i+1);
//        value.insert("itemName", "点胶");
//        value.insert("itemL_Limit", amb.Before_Value);
//        value.insert("itemH_Limit", amb.After_Value);
//        value.insert("itemTsValue", amb.Item_Value);
//        value.insert("TestValue", amb.Item_Value);
//        value.insert("itemLenght", "5");
//        value.insert("itemRes", amb.Item_QM_status);

//        ats_value.insert(i,value);

//        ++it;
//        ++i;
//    }

    QJsonObject value;
    value.insert("itemId", 1);
    value.insert("itemName", u8"点胶");
    value.insert("itemL_Limit", glue_result_slst[3]);//点胶后
    value.insert("itemH_Limit", glue_result_slst[2]);//点胶前
    value.insert("itemTsValue", glue_result_slst[4]);//胶重
    value.insert("TestValue", glue_result_slst[4]);
    value.insert("itemLenght", "5");

    if(m_gl_glue_flags == GLUE_RESULT_NG){
        value.insert("itemRes", "NG");//点胶结果
    }else if(m_gl_glue_flags == GLUE_RESULT_OK){
        value.insert("itemRes", "OK");
    }

    ats_value.push_back(value);

    m_gl_glue_flags = -1;

    root.insert("atsTsItemsModels",ats_value);

    doc.setObject(root);
    result = doc.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<"final result:"<<result;
    m_gl_mes_flags = MES_RESULT_FLAGS;
    return result;
}

void TestLineGlueDispenseWgt::update_ui_according_to_mes_info()
{
    m_pLineEditGlueStandard->setText(QString::number(m_pGlueStandardValue)+" g");
    m_pGlueCheckState->set_color(1);
    set_table_data();
}

void TestLineGlueDispenseWgt::set_table_data()
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

void TestLineGlueDispenseWgt::reset_station_status()
{
    m_pGlueCheckState->set_color(0);
    m_pPasteGlueCheckState->set_color(0);
    m_pSNCheckState->set_color(0);
    m_pFinalCheckState->set_color(0);
    m_pLineEditGlueStandard->setText("");
    tableWidget->clearContents();
    m_pLogTextEdit->clear();
}

void TestLineGlueDispenseWgt::check_glue_info_of_db(const QString middleframenumber, bool &midfexistsign, bool &oksign)
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

void TestLineGlueDispenseWgt::push_key_bar_to_list(QString barcode)
{
    //restart to scan another product
    if(key_bar_code.size() == 1)
    {
        m_pLogTextEdit->clear();
        m_pGlueCheckState->set_color(0);
        m_pPasteGlueCheckState->set_color(0);
        m_pSNCheckState->set_color(0);
        m_pFinalCheckState->set_color(0);

        QByteArray stationCode = barcode.toLatin1();
        m_pCheckData.Content_Data.Serial_Number = new char[stationCode.length()];
        strcpy_s(m_pCheckData.Content_Data.Serial_Number,stationCode.length()+1,stationCode.data());

        m_pResultData.Result_Content.Serial_Number = new char[stationCode.length()];
        strcpy_s(m_pResultData.Result_Content.Serial_Number,stationCode.length()+1,stationCode.data());
    }
    //restart to scan another product
}

void TestLineGlueDispenseWgt::insert_glue_result_to_db(QList<QString> info, const QString result)
{
    int maxID = GDataFactory::get_factory()->get_biggest_id(constRedFlagGlueInfoTable);

    QString strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7','%8')").
                arg(constRedFlagGlueInfoTable).
                arg(maxID).
                arg(info[0]).
                arg(info[1]).
                arg(info[2]).
                arg(info[3]).
                arg(info[4]).
                arg(result);
    QString strErrorMsg;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
        QLOG_INFO()<<"insert change product info success!";
    else
        QLOG_WARN()<<"insert change product info failed!";
}

void TestLineGlueDispenseWgt::reset_check_result_data_after_over()
{
    m_pFinalCheckState->set_color(1);
    key_bar_code.clear();
    key_bar_code.insert(CODE_ORDER::glue_number, GDataFactory::get_factory()->get_config_para("GLUE_DISPENSE_GLUE_NUMBER"));
    //truncate_data_in_db();
}

void TestLineGlueDispenseWgt::truncate_data_in_db()
{
    QString strTruncate = QString("truncate table public.%1").arg(constRedFlagGlueInfoTable);
    QString tmpEr;
    if(GDataFactory::get_pgsql()->ExecSql(strTruncate,tmpEr))
    {
        log_add_text_edit(u8"缓存数据清空成功!");
        QLOG_INFO()<<"clear cached data SUCCESS!";
    }
    else {
        log_add_text_edit(u8"缓存数据清空失败!");
        QLOG_WARN()<<"clear cached data FAILED!";
    }
}

void TestLineGlueDispenseWgt::log_add_text_edit(QString text)
{
    QString time_log = QDateTime::currentDateTime().toString(fmtDateTime);
    m_pLogTextEdit->append(time_log + " "+ text);
}

void TestLineGlueDispenseWgt::slot_rev_serial_number(const QByteArray &data)
{
    QString cd = data;
    if(cd.contains('\r'))
        cd.remove('\r');

    push_key_bar_to_list(cd);

    if(ngCheckBox->isChecked())
    {
        key_bar_code.clear();
        key_bar_code.insert(CODE_ORDER::glue_number,cd);
        m_pLineEditGlueInfo->setText(cd);
        GDataFactory::get_factory()->set_config_para_1("GLUE_DISPENSE_GLUE_NUMBER",cd);
        GDataFactory::get_factory()->save_config_file();
        log_add_text_edit(u8"当前胶料条码内容为:"+cd);
    }
    else
    {
        log_add_text_edit(u8"当前关键件条码内容为:"+cd);

        if(m_gl_glue_flags == -1 || (cd != key_bar_code[CODE_ORDER::mainBoardSN] && m_gl_glue_flags == GLUE_RESULT_NG)){
            key_bar_code.insert(CODE_ORDER::mainBoardSN, cd);
            GDataFactory::get_mes_process()->exec_http_common_get(format_check_json_info(),2);
        }else if(m_gl_glue_flags == GLUE_RESULT_OK){
            if(cd == key_bar_code[CODE_ORDER::mainBoardSN]){
                m_pSNCheckState->set_color(1);
                log_add_text_edit(u8"点胶后主板条码校验一致！");

                //upload info to MES
                GDataFactory::get_mes_process()->exec_http_common_get(format_result_json_info(),3);
                //upload info to MES
            }else{
                m_pSNCheckState->set_color(0);
                log_add_text_edit(u8"点胶后主板条码校验不一致，请重新扫码！");
            }
        }
    }
}

void TestLineGlueDispenseWgt::slot_state_changed(int state)
{
    if(state != 0)
    {
        m_pLineEditGlueInfo->setText("");
        //truncate_data_in_db();
    }
}

void TestLineGlueDispenseWgt::slot_rev_mes_info(const QByteArray &data)
{
    if(this->isVisible())
        ;
    else
        return;
    QLOG_WARN()<<"the info of mes reply is:"<<data;
    QByteArray responseByte = data;
    QJsonDocument doucment = QJsonDocument::fromJson(responseByte);
//    if (!doucment.isNull() /*&& json_error.error == QJsonParseError::NoError*/) {
//        if (doucment.isObject()) {
            const QJsonObject obj = doucment.object();
            qDebug()<<obj;
            QJsonValue value = obj.value("code");
            QString g_code_text = value.toString();
            QLOG_INFO() << "mes json code value is" << g_code_text;
            value = obj.value("result");
            QString g_str_result = value.toString();
            QLOG_INFO() << "mes json result is" << g_str_result;

            if(m_gl_mes_flags == MES_CHECK_FLAGS){
                if(g_code_text == "OK"){//校验通过启动点胶
                    log_add_text_edit(u8"Mes校验返回结果描述为:"+g_str_result);
                    log_add_text_edit(u8"主板条码校验通过，请开始点胶");
                    m_pGlueCheckState->set_color(1);
                }else if(g_code_text == "NG"){
                    log_add_text_edit(u8"Mes返回结果描述为:"+g_code_text+":"+g_str_result);
                }
            }else if(m_gl_mes_flags == MES_RESULT_FLAGS){
                if(g_code_text == "OK"){
                    log_add_text_edit(u8"Mes结果上传返回结果描述为:"+g_str_result);
                    reset_check_result_data_after_over();
                    m_pFinalCheckState->set_color(1);

                }else if(g_code_text == "NG"){
                    reset_check_result_data_after_over();
                    m_pFinalCheckState->set_color(0);
                    log_add_text_edit(u8"Mes结果上传返回结果描述为:"+g_str_result);
                }
            }
//         }
//    }else{
//        QLOG_INFO()<<"json error:"<<json_error.errorString();
//    }
}



