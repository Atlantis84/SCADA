#include "onebuttonchangem201wgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QSignalMapper>
#include "QsLog.h"
#include "gdataacquisitionfactory.h"
#include "gdatafactory.h"
#include "imessagebox.h"
#include <QApplication>
#include <QSqlQuery>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <QTextCodec>
#include <QDir>

OneButtonChangeM201Wgt* OneButtonChangeM201Wgt::m_pInstance = nullptr;
OneButtonChangeM201Wgt::OneButtonChangeM201Wgt(QWidget *parent) : QWidget(parent)
{
    m_pCurrentStyle = "13633";
    m_pCurrentMaterialNumber = "350238";
    m_pAlreadyStartTimer = false;

    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget|Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/icon/24x24.png"));

    m_pUpBoardModbusObj = new ModbusClientDll();
    m_pUpBoardModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                      DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_BOARD_IP"),
                                      DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_BOARD_PORT").toInt());

    m_pUpUtensilModbusObj = new ModbusClientDll();
    m_pUpUtensilModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                        DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_UTENSIL_IP"),
                                        DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_UTENSIL_PORT").toInt());

//    m_pTcpCommonUseWaveSoldering = new TcpCommonUse();
//    connect(m_pTcpCommonUseWaveSoldering,SIGNAL(signal_proc_server_info(const QByteArray&)),this,
//            SLOT(slot_rev_tcp_server_wave_soldering_info(const QByteArray&)));
//    if(m_pTcpCommonUseWaveSoldering->connect_to_tcp_server(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_WAVE_SOLDERING_1_IP"),
//                                                           DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE1_PORT").toInt()))
//    {
//        QLOG_INFO()<<"one button change production of Wave-Soldering connect to Server SUCCESS";
//        QString loginString = "jtauto;27508459";
//        m_pTcpCommonUseWaveSoldering->send_data_to_server(loginString.toLatin1());
//    }
//    else {
//        QLOG_WARN()<<"one button change production of Wave-Soldering connect to Server FAILED";
//    }

    m_pPickUpCoverModbusObj = new ModbusClientDll();
    m_pPickUpCoverModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                          DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_IP"),
                                          DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_PORT").toInt());

    m_pDownUtensilModbusObj = new ModbusClientDll();
    m_pDownUtensilModbusObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                          DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_IP"),
                                          DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_PORT").toInt());

    tboxResultWgt = new TBoxResultWgt();
    connect(this,SIGNAL(signal_notify_to_change_production(const QString,const QString)),this,SLOT(slot_change_production(const QString,const QString)));
    QVBoxLayout* vAll = new QVBoxLayout();

    QGroupBox* grpScanInfo = new QGroupBox(u8"扫码信息");
    grpScanInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelSN = new QLabel(u8"      产品SN:");
    labelSN->setStyleSheet("border:none;");
    lineEdit_SN = new QLineEdit();
    lineEdit_SN->setReadOnly(true);
    lineEdit_SN->setAlignment(Qt::AlignCenter);
    lineEdit_SN->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox1->addStretch();
    hBox1->addWidget(labelSN);
    hBox1->addWidget(lineEdit_SN);
    QPushButton* btnReadFile = new QPushButton(u8"测试文件读取");
    btnReadFile->setStyleSheet("QPushButton{border:1px solid rgba(24,144,255,100);min-height:40px;color:rgba(0,0,0,150);background-color:rgba(24,144,255,100);}\
                                QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnReadFile,SIGNAL(clicked()),this,SLOT(slot_btn_read_file()));
    QPushButton* btnExportFile = new QPushButton(u8"导出文件");
    btnExportFile->setStyleSheet("QPushButton{border:1px solid rgba(24,144,255,100);min-height:40px;color:rgba(0,0,0,150);background-color:rgba(24,144,255,100);}\
                                QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
//    hBox1->addWidget(btnReadFile);
//    hBox1->addWidget(btnExportFile);
    hBox1->addStretch();
    grpScanInfo->setLayout(hBox1);

    QGroupBox* grpBoardStyle = new QGroupBox(u8"板型-料号信息");
    grpBoardStyle->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox11 = new QHBoxLayout();
    QLabel* labelStyle = new QLabel(u8"请选择换产板型:");
    labelStyle->setStyleSheet("border:none;");
    m_pCmbStationStyle = new QComboBox();

//    //load product info from db
//    load_product_info_from_db();

    QStringList lstTableName;
    lstTableName.clear();
    QMap<QString,QStringList>::const_iterator it;
    for (it=m_pMapProductInfo.constBegin();it!=m_pMapProductInfo.constEnd();++it) {
        lstTableName.push_back(it.key());
    }
    for(int i=0;i<lstTableName.size();i++)
        m_pCmbStationStyle->insertItem(i,lstTableName[i]);
    m_pCmbStationStyle->setCurrentIndex(-1);
    connect(m_pCmbStationStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_index_changed(int)));
    m_pCmbStationStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:KaiTi;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:120px;"
                                            "min-width:300px;min-height:30px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    QLabel* labelMaterialNumber = new QLabel(u8"异型插请选择料号:");
    labelMaterialNumber->setStyleSheet("border:none;");
    m_pCmbMaterialNumber = new QComboBox();
    connect(m_pCmbMaterialNumber,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_material_number_changed(int)));
    m_pCmbMaterialNumber->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:KaiTi;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:120px;"
                                            "min-width:300px;min-height:30px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    hBox11->addStretch();
    hBox11->addWidget(labelStyle);
    hBox11->addWidget(m_pCmbStationStyle);
    hBox11->addWidget(labelMaterialNumber);
    hBox11->addWidget(m_pCmbMaterialNumber);
    hBox11->addStretch();
    QVBoxLayout* vBox11 = new QVBoxLayout();
    vBox11->addLayout(hBox11);

    QPushButton* btnTest = new QPushButton(u8"一   键   换   产");
    btnTest->setStyleSheet("QPushButton{border:1px solid rgba(24,144,255,100);min-height:40px;color:rgba(0,0,0,150);background-color:rgba(24,144,255,100);}\
                                QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnTest,SIGNAL(clicked()),this,SLOT(slot_btn_change_production()));
    vBox11->addWidget(btnTest);
    grpBoardStyle->setLayout(vBox11);

    QGroupBox* grpInsertInfo = new QGroupBox(u8"录入板型-料号信息");
    grpInsertInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QLabel* labelInsertStyle = new QLabel(u8"请输入板型:");
    labelInsertStyle->setStyleSheet("border:none;");
    m_pLineEditBoardStyle = new QLineEdit();
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,m_pLineEditBoardStyle);
    m_pLineEditBoardStyle->setValidator(validator);
    m_pLineEditBoardStyle->setAlignment(Qt::AlignCenter);
    m_pLineEditBoardStyle->setStyleSheet("color:rgb(0,0,0);min-width:80px;border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");

    QLabel* labelInsertMaterial = new QLabel(u8"请输入料号:");
    labelInsertMaterial->setStyleSheet("border:none;");
    m_pLineEditMaterialNumber = new QLineEdit();
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,m_pLineEditMaterialNumber);
    m_pLineEditMaterialNumber->setValidator(validator1);
    m_pLineEditMaterialNumber->setAlignment(Qt::AlignCenter);
    m_pLineEditMaterialNumber->setStyleSheet("color:rgb(0,0,0);min-width:80px;border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");

    QPushButton* btnInsert = new QPushButton(u8"  录 入 产 品 信 息  ");
    btnInsert->setStyleSheet("QPushButton{border:1px solid rgba(24,144,255,100);min-height:30px;color:rgba(0,0,0,150);background-color:rgba(24,144,255,100);}\
                                QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnInsert,SIGNAL(clicked()),this,SLOT(slot_btn_insert_product_info()));
    hBox2->addWidget(labelInsertStyle);
    hBox2->addWidget(m_pLineEditBoardStyle);
    hBox2->addWidget(labelInsertMaterial);
    hBox2->addWidget(m_pLineEditMaterialNumber);
    hBox2->addWidget(btnInsert);
    grpInsertInfo->setLayout(hBox2);

    QGroupBox* grpChangeResult = new QGroupBox(u8"换产结果");
    grpChangeResult->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox4 = new QHBoxLayout();
    tboxResultWgt = new TBoxResultWgt();
    hBox4->addWidget(tboxResultWgt);
    grpChangeResult->setLayout(hBox4);

    QGroupBox* grpStations = new QGroupBox(u8"请勾选换产机台");
    grpStations->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox5 = new QHBoxLayout();
    cbStation1 = new QCheckBox(u8"分板机");
    cbStation2 = new QCheckBox(u8"上夹具");
    cbStation3 = new QCheckBox(u8"点胶机");
    cbStation4 = new QCheckBox(u8"异型插1");

    cbStation5 = new QCheckBox(u8"异型插2");
    cbStation6 = new QCheckBox(u8"异型插3");
    cbStation7 = new QCheckBox(u8"异型插4");
    cbStation8 = new QCheckBox(u8"炉前CCD");
    cbStation9 = new QCheckBox(u8"波峰焊");
    cbStation10 = new QCheckBox(u8"取上盖");
    cbStation11 = new QCheckBox(u8"炉后AOI");
    cbStation12 = new QCheckBox(u8"下夹具");

    lstCheckBox<<cbStation1<<cbStation2<<cbStation3<<cbStation4<<cbStation5<<cbStation6<<cbStation7<<cbStation8<<cbStation9<<cbStation10<<cbStation11<<cbStation12;
    QSignalMapper* mapper = new QSignalMapper();
    for (int i=0;i<12;i++) {
        lstCheckBox[i]->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
        mapper->setMapping(lstCheckBox[i],i);
        connect(lstCheckBox[i],SIGNAL(stateChanged(int)),mapper,SLOT(map()));
    }
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(slot_from_mapper(int)));

//    hBox5->addStretch(50);
    hBox5->addWidget(cbStation1);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation2);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation3);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation4);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation5);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation6);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation7);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation8);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation9);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation10);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation11);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation12);
//    hBox5->addStretch(50);
    grpStations->setLayout(hBox5);

    vAll->addWidget(grpStations);
    vAll->addWidget(grpBoardStyle);
    vAll->addWidget(grpInsertInfo);
    vAll->addWidget(grpScanInfo);
//    vAll->addWidget(grpChangeResult);
    vAll->setStretch(0,1);
    vAll->setStretch(1,1);
    vAll->setStretch(2,1);
    vAll->setStretch(3,1);
    vAll->setStretch(4,5);
//    vAll->setStretch(5,1);
    this->setLayout(vAll);
}

void OneButtonChangeM201Wgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void OneButtonChangeM201Wgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    //load product info from db
    load_product_info_from_db();

    for (int i=0;i<lstCheckBox.length();i++)
        lstCheckBox[i]->setCheckState(Qt::CheckState::Unchecked);

    tboxResultWgt->set_change_result(1);
    tboxResultWgt->repaint();

    lstStations.clear();

    GDataAcquisitionFactory::get_instance()->stop_serial_port_m201();
    GDataAcquisitionFactory::get_instance()->read_serial_number_m201();
}

void OneButtonChangeM201Wgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    GDataAcquisitionFactory::get_instance()->stop_serial_port_m201();
    if(m_pAlreadyStartTimer)
    {
        killTimer(m_pTimerID);
        m_pAlreadyStartTimer = false;
    }
}

void OneButtonChangeM201Wgt::slot_rev_logs(const QString str)
{
    this->textEditMesInfo->append(str);
}

bool OneButtonChangeM201Wgt::copy_file_from_src_to_dest(const QString fileNameWithPath, const QString dstPath)
{
    bool r = false;
    if(!QFile::copy(fileNameWithPath,dstPath))
    {
        QLOG_WARN()<<u8"换产文件拷贝失败!";
        r = false;
    }
    else
    {
        QLOG_WARN()<<u8"换产文件拷贝成功!";
        r = true;
    }
    return r;
}

static bool aaaa = false;
static bool bbbb = false;
static bool cccc = false;
static bool dddd = false;
static bool eeee = false;
static bool ffff = false;
static bool gggg = false;
void OneButtonChangeM201Wgt::slot_btn_change_production()
{
//    GDataAcquisitionFactory::get_aoi_business_m201_obj()->change_production_of_aoi("13633-350238");
    slot_change_production(m_pCurrentStyle,m_pCurrentMaterialNumber);
}

void OneButtonChangeM201Wgt::slot_from_mapper(int id)
{
    if(lstCheckBox[id]->checkState() == Qt::CheckState::Checked)
    {
        if(lstStations.contains(id))
            ;
        else
            lstStations.push_back(id);
    }
    else
    {
        if(lstStations.contains(id))
            lstStations.removeOne(id);
        else
            ;
    }
}

void OneButtonChangeM201Wgt::slot_rev_tcp_server_wave_soldering_info(const QByteArray &data)
{
    QString tmpStr = data;
    if(data.contains("OK"))
    {
        if(lstStations.contains(8))
        {
            lstCheckBox[8]->setCheckState(Qt::CheckState::Unchecked);
            lstStations.removeOne(8);
        }
    }

    if(data.contains("NG"))
    {
        QLOG_ERROR()<<"Wave-Soldering change production FAILED";
    }
    return;
}

void OneButtonChangeM201Wgt::slot_rev_serial_number(const QByteArray &data)
{
    QString strFullSN = data;
    QStringList lstSN = strFullSN.split('/');
    if(lstSN.size()>0)
        ;
    else
        return;

    tboxResultWgt->set_change_result(0);
    tboxResultWgt->repaint();

    QString strSN = lstSN[lstSN.size()-1];
    this->lineEdit_SN->setText(strSN);
    GDataFactory::get_mes_process()->exec_http_get_m201(strSN);
}

void OneButtonChangeM201Wgt::slot_rev_info_from_mes(const QByteArray &data)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    QJsonObject jsonObject = jsonDocument.object();
    QString statusString = "";
    if(jsonObject.contains("status"))
    {
        QJsonValue instructionValue = jsonObject.value(QStringLiteral("status"));
        if(instructionValue.isString())
            statusString = instructionValue.toString();
        else
        {
            QLOG_WARN()<<"MES status para data format is ABNORMAL!";
            return;
        }
    }

    QString boardStyle,materialNumber;

    if(statusString == "success")
    {
        if(jsonObject.contains("dataInfo"))
        {
            QJsonValue arrayValue = jsonObject.value(QStringLiteral("dataInfo"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();

                    QString tmpBoardStyle = icon["tpS_TEMPLATE"].toString();
                    for(int i=tmpBoardStyle.length()-3;i>=0;i--)
                        boardStyle.prepend(tmpBoardStyle[i]);

                    materialNumber = icon["modeL_CODE"].toString();
                }
            }
        }
        emit signal_notify_to_change_production(boardStyle,materialNumber);
    }
    else {
        QLOG_WARN()<<"MES status para is error!";
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"MES status error!");
        return;
    }
}

bool OneButtonChangeM201Wgt::writeToFile(const QString &fileName, const QString &text) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"换产文件打开失败!");
        return false;
    }

    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}

bool OneButtonChangeM201Wgt::clearFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;;
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    file.resize(0);
    file.close();
    return true;
}

bool OneButtonChangeM201Wgt::deleteSpecificFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.exists()) {
        QLOG_WARN()<<"File does not exist";
        return false;
    }

    if (file.remove()) {
        QLOG_WARN()<<"File deleted successfully";
        return true;
    } else {
        QLOG_WARN()<<"Failed to delete file";
        return false;
    }
}

static QString m_pFileNameWithPath="";
void OneButtonChangeM201Wgt::test_change_production()
{
    QString tmpBoardStyle = "";
    if(ffff == false)
    {
        tmpBoardStyle = "13633";
        ffff = true;
    }
    else {
        ffff = false;
        tmpBoardStyle = "13634";
    }
    for (int i=0;i<lstStations.length();i++) {
        switch (lstStations[i]) {
        case 0:
        {
            QLOG_INFO()<<u8"分板机换产";
            if(aaaa == false)
            {
                QString boardType = "13633";
                QString machineType = "123";
                aaaa = true;
                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_divide_machine("10.50.130.241",8081,boardType,machineType);
            }
            else
            {
                aaaa = false;
                QString boardType = "13634";
                QString machineType = "350237";
                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_divide_machine("10.50.130.241",8081,boardType,machineType);
            }
        }
            break;
        case 1:
        {

            ModbusClientDll* testObj = new ModbusClientDll();
            testObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_UTENSIL_IP"),
                                                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_UTENSIL_PORT").toInt());


            uint16_t tt;
            QLOG_INFO()<<u8"上夹具换产";
            if(bbbb == false)
            {
                tt = 99;
                testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);
                bbbb = true;
            }
            else
            {
                tt = 98;
                testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);
                bbbb = false;
            }
        }
            break;
        case 2:
        {
            QLOG_INFO()<<u8"点胶机换产";
            if(cccc == false)
            {
                QString boardType = "13633hfg";
                QString machineType = "350238";
                cccc = true;
                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine("10.50.130.243",8081,boardType,machineType);
            }
            else {
                cccc = false;
                QString boardType = "13634k";
                QString machineType = "350237";
                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine("10.50.130.243",8081,boardType,machineType);
            }
        }
            break;
        case 3:
        {
            QLOG_INFO()<<u8"异型插换产";
            QString binDir = QApplication::applicationDirPath();
            binDir = binDir+"/"+"ActiveFile.txt";
            m_pFileNameWithPath = binDir;

            if(clearFile(binDir))
                ;
            else
            {
                QLOG_WARN()<<u8"clear file failed";
                return;
            }

            if(writeToFile(binDir,tmpBoardStyle))
                ;
            else {
                QLOG_WARN()<<u8"write to file failed!";
                return;
            }

            deleteSpecificFile("//10.50.130.244/ll/ActiveFile.txt");
        //    deleteSpecificFile("//10.50.130.245/ll/ActiveFile.txt");
        //    deleteSpecificFile("//10.50.130.246/ll/ActiveFile.txt");
            deleteSpecificFile("//10.50.130.247/ll/ActiveFile.txt");
            copy_file_from_src_to_dest(m_pFileNameWithPath,u8"//10.50.130.247/p_p_autoload/ActiveFile.txt");
            copy_file_from_src_to_dest(m_pFileNameWithPath,u8"//10.50.130.244/p_p_autoload/ActiveFile.txt");
//            m_pTimerID = startTimer(1000);
            break;
        }
        case 4:
            {
            ModbusClientDll* testObj = new ModbusClientDll();
            testObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_IP"),
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_PORT").toInt());


            uint16_t tt;
            QLOG_INFO()<<u8"取上盖换产";
            if(dddd == false)
            {
                tt = 100;
                testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);
                dddd = true;
            }
            else
            {
                tt = 99;
                testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);
                dddd = false;
            }
            }
            break;
        case 5:
        {
            QLOG_INFO()<<u8"炉后AOI换产";
            if(gggg == false)
            {
                gggg = true;
                GDataAcquisitionFactory::get_aoi_business_m201_obj()->change_production_of_aoi("13633-350238");
            }
            else {
                gggg = false;
                GDataAcquisitionFactory::get_aoi_business_m201_obj()->change_production_of_aoi("13634.350237");
            }
        }
            break;
        case 6:
        {
            QLOG_INFO()<<u8"下夹具换产";
            ModbusClientDll* testObj = new ModbusClientDll();
            testObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_IP"),
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_PORT").toInt());


            uint16_t tt;
            if(eeee == false)
            {
                tt = 100;
                testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);
                eeee = true;
            }
            else
            {
                tt = 98;
                testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);
                eeee = false;
            }
        }
            break;
        default:
            break;
        }
    }
}

void OneButtonChangeM201Wgt::slot_change_production(const QString boardStyle, const QString materialNumber)
{
    if(lstStations.length() == 0)
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"未选择换产机台，请勾选需要换产的机台!");
        return;
    }

    m_pTimerID = startTimer(1000);
    m_pAlreadyStartTimer = true;
    for (int i=0;i<lstStations.length();i++) {
        switch (lstStations[i]) {
        case 0:
        {
            QString ip = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DEVIDE_IP");
            int port = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DEVIDE_PORT").toInt();
            GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_divide_machine(ip,static_cast<quint16>(port),boardStyle,materialNumber);
            slot_rev_result_of_change_production_for_divide_glue(1);
        }
            break;
        case 1:
        {
            ModbusClientDll* testObj = new ModbusClientDll();
            testObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_UTENSIL_IP"),
                                                DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_UP_UTENSIL_PORT").toInt());


            uint16_t tt;
            QLOG_INFO()<<u8"up-utensil change production";
            MODBUS_STATUS status;
            if(boardStyle == "13633")
                tt = 99;
            else if(boardStyle == "13662")
                tt = 92;
            else if(boardStyle == "51210")
                tt = 93;
            else if(boardStyle == "12903")
                tt = 95;
            else if(boardStyle == "51283")
                tt = 96;
            else if(boardStyle == "13671")
                tt = 97;
            else if(boardStyle == "51603")
                tt = 100;
            else
                tt = 98;

            status = testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);

            if(status == MODBUS_STATUS::RES_OK)
            {
                QLOG_INFO()<<"the up-utensil change production success";
                lstCheckBox[1]->setCheckState(Qt::CheckState::Unchecked);
                lstStations.removeOne(1);
            }
            else
                QLOG_WARN()<<"the up-utensil change production failed";
        }
            break;
        case 2:
        {
            QLOG_INFO()<<u8"glue change production";
            QString ip = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_GLUE_IP");
            int port = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_GLUE_PORT").toInt();
//            if(boardStyle == "13633")
//            {
//                QString tmpBoardStyle = boardStyle;
//                tmpBoardStyle.append("hfg");
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine(ip,static_cast<quint16>(port),tmpBoardStyle,"123");
//            }
//            else if(boardStyle == "13634")
//            {
//                QString tmpBoardStyle = boardStyle;
//                tmpBoardStyle.append("k");
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine(ip,static_cast<quint16>(port),tmpBoardStyle,"123");
//            }
//            else if(boardStyle == "13671")
//            {
//                QString tmpBoardStyle = boardStyle;
//                tmpBoardStyle.append("hHX");
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine(ip,static_cast<quint16>(port),tmpBoardStyle,"123");
//            }
//            else if(boardStyle == "13662")
//            {
//                QString tmpBoardStyle = boardStyle;
//                tmpBoardStyle.append("-0716");
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine(ip,static_cast<quint16>(port),tmpBoardStyle,"123");
//            }
//            else
            GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_glue_machine(ip,static_cast<quint16>(port),boardStyle,"123");
            slot_rev_result_of_change_production_for_divide_glue(2);
        }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        {
            QString binDir = QApplication::applicationDirPath();
            binDir = binDir+"/"+"ActiveFile.txt";

            if(clearFile(binDir))
                ;
            else
            {
                QLOG_WARN()<<u8"clear file failed";
                return;
            }

            QString strfilename = boardStyle;
//            strfilename.append("-");
//            strfilename.append(materialNumber);
            if(writeToFile(binDir,strfilename))
                ;
            else {
                QLOG_WARN()<<u8"write to file failed!";
                return;
            }
            QString insertIP = "";
            if(lstStations[i] == 3)
                insertIP = "781E6V16V3103D"/*DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE1_IP")*/;
            else if(lstStations[i] == 4)
                insertIP = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE2_IP");
            else if(lstStations[i] == 5)
                insertIP = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE3_IP");
            else if(lstStations[i] == 6)
                insertIP = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_INSERT_MACHINE4_IP");
            insertIP.prepend("//");
            insertIP.append("/p_p_autoload/ActiveFile.txt");
            deleteSpecificFile(insertIP);
            bool cResult = copy_file_from_src_to_dest(binDir,insertIP);
            if(cResult)
            {
                lstStations.removeOne(lstStations[i]);
                if(lstStations[i] == 3)
                    QLOG_INFO()<<"the insert-machine1 change production success";
                else if(lstStations[i] == 4)
                    QLOG_INFO()<<"the insert-machine2 change production success";
                else if(lstStations[i] == 5)
                    QLOG_INFO()<<"the insert-machine3 change production success";
                else if(lstStations[i] == 6)
                    QLOG_INFO()<<"the insert-machine4 change production success";
                else
                    ;
                lstCheckBox[i]->setCheckState(Qt::CheckState::Unchecked);
            }
            else
            {
                if(lstStations[i] == 3)
                    QLOG_WARN()<<"the insert-machine1 change production failed";
                else if(lstStations[i] == 4)
                    QLOG_WARN()<<"the insert-machine2 change production failed";
                else if(lstStations[i] == 5)
                    QLOG_WARN()<<"the insert-machine3 change production failed";
                else if(lstStations[i] == 6)
                    QLOG_WARN()<<"the insert-machine4 change production failed";
                else
                    ;
            }
        }
            break;
        case 7:
        {
            //front CCD
        }
            break;
        case 8:
        {
            //wave-soldering
            QString stationname = "Wave_Soldering";
            QString testfilename = "";
            testfilename.append(boardStyle);
//            testfilename.append('_');
//            testfilename.append(materialNumber);
            emit signal_notify_wave_soldering_to_change_production(true,stationname,testfilename);
        }
            break;
        case 9:
        {
            ModbusClientDll* testObj = new ModbusClientDll();
            testObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_IP"),
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_PICK_UPPER_COVER_PORT").toInt());


            uint16_t tt;
            QLOG_INFO()<<u8"pick-up-cover change production";
            MODBUS_STATUS status;
            if(boardStyle == "13633")
                tt = 100;
            else if(boardStyle == "51283")
                tt = 96;
            else if(boardStyle == "51603")
                tt = 97;
            else if(boardStyle == "13671")
                tt = 98;
            else
                tt = 99;

            status = testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);

            if(status == MODBUS_STATUS::RES_OK)
            {
                QLOG_INFO()<<"the pick-up-cover change production success";
                lstCheckBox[9]->setCheckState(Qt::CheckState::Unchecked);
                lstStations.removeOne(9);
            }
            else
            {
                QLOG_WARN()<<"the pick-up-cover change production failed";
            }
        }
            break;
        case 10:
        {
            QLOG_INFO()<<u8"back AOI change production";
            if(boardStyle == "13633")
                GDataAcquisitionFactory::get_aoi_business_m201_obj()->change_production_of_aoi("78Q-1156132");
            else
                GDataAcquisitionFactory::get_aoi_business_m201_obj()->change_production_of_aoi("PI2322Y-1202524");

//            GDataAcquisitionFactory::get_aoi_business_m201_obj()->change_production_of_aoi();

//            QString tmpip = "192.168.0.100";
//            quint16 tmpport = 7930;
//            if(boardStyle == "12903")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,1);
//            else if(boardStyle == "13633")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,2);
//            else if(boardStyle == "13634")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,3);
//            else if(boardStyle == "13662")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,4);
//            else if(boardStyle == "13671")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,5);
//            else if(boardStyle == "51210")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,6);
//            else if(boardStyle == "51283")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,7);
//            else if(boardStyle == "51603")
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,8);
//            else
//                GDataAcquisitionFactory::get_M201_udp_business_obj()->change_production_of_vision(tmpip,tmpport,1);
        }
            break;
        case 11:
        {
            QLOG_INFO()<<u8"down-utensil change production";
            ModbusClientDll* testObj = new ModbusClientDll();
            testObj->init_ip_port(MODBUS_TYPE::MODBUS_TCP,
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_IP"),
                                  DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DOWN_UTENSIL_PORT").toInt());


            uint16_t tt;
            MODBUS_STATUS status;
            if(boardStyle == "13633")
                tt = 100;
            else if(boardStyle == "51283")
                tt = 93;
            else if(boardStyle == "51603")
                tt = 94;
            else if(boardStyle == "51210")
                tt = 95;
            else if(boardStyle == "13662")
                tt = 96;
            else if(boardStyle == "12903")
                tt = 97;
            else if(boardStyle == "13671")
                tt = 99;
            else
                tt = 98;

            status = testObj->sendMessageToPLC(MODBUS_WRITE_SIGN,0x05EA,1,&tt);

            if(status == MODBUS_STATUS::RES_OK)
            {
                QLOG_INFO()<<"the down-utensil change production success";
                lstCheckBox[i]->setCheckState(Qt::CheckState::Unchecked);
                lstStations.removeOne(lstStations[i]);
            }
            else {
                QLOG_WARN()<<"the down-utensil change production failed";
            }
        }
            break;
        default:
            break;
        }
    }
}

void OneButtonChangeM201Wgt::slot_rev_result_of_change_production_for_divide_glue(const int id)
{
    if(id == 1)
    {
        QLOG_INFO()<<"the divide change production success";
        if(lstStations.contains(0))
        {
            lstCheckBox[0]->setCheckState(Qt::CheckState::Unchecked);
            lstStations.removeOne(0);
        }
    }
    else if(id ==2)
    {
        QLOG_INFO()<<"the glue change production success";
        if(lstStations.contains(2))
        {
            lstCheckBox[2]->setCheckState(Qt::CheckState::Unchecked);
            lstStations.removeOne(2);
        }
    }
}

void OneButtonChangeM201Wgt::slot_rev_result_of_change_production_for_aoi(const int r)
{
    if(r==1)
    {
        QLOG_INFO()<<"the aoi change production success";
        if(lstStations.contains(10))
        {
            lstCheckBox[10]->setCheckState(Qt::CheckState::Unchecked);
            lstStations.removeOne(10);
        }
    }
    else {
        QLOG_WARN()<<"the aoi change production failed";
    }
}

void OneButtonChangeM201Wgt::slot_comb_index_changed(int index)
{
    Q_UNUSED(index);
    m_pCurrentStyle = m_pCmbStationStyle->currentText();
    m_pCmbMaterialNumber->clear();
    for(int i=0;i<m_pMapProductInfo[m_pCurrentStyle].size();i++)
        m_pCmbMaterialNumber->insertItem(i,m_pMapProductInfo[m_pCurrentStyle][i]);
    m_pCmbMaterialNumber->setCurrentIndex(-1);
}

void OneButtonChangeM201Wgt::slot_comb_material_number_changed(int index)
{
    Q_UNUSED(index);
    m_pCurrentMaterialNumber = m_pCmbMaterialNumber->currentText();
}

void OneButtonChangeM201Wgt::slot_btn_insert_product_info()
{
    if((m_pLineEditBoardStyle->text().trimmed().length()!=5)||(m_pLineEditMaterialNumber->text().trimmed().length()!=6))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"请确认板型及料号是否输入正确!");
        return;
    }

    QString strSql = QString("select * from public.%1 where \"boardStyle\"='%2' and \"materialNumber\"='%3'" ).
            arg(constM201BoardStyleAndMaterialNumber).
            arg(m_pLineEditBoardStyle->text().trimmed()).
            arg(m_pLineEditMaterialNumber->text().trimmed());
    QSqlQuery queryResult;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get board-style and material-number success!";
        if(queryResult.size() != 0)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"当前板型及料号已存在，请重新输入!");
            m_pLineEditBoardStyle->setText("");
            m_pLineEditMaterialNumber->setText("");
            return;
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }

    insert_new_product_to_db();
}

void OneButtonChangeM201Wgt::slot_btn_read_file()
{
//    QString path = "//127.0.0.1/InsertMachine4Logs";
//    traverseFolder(path,"Sep");

//    GDataAcquisitionFactory::get_collect_machine_error_thread()->set_para_value(true);


//    QFile file("F:\\Sep 02 , 2024.log");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QLOG_ERROR()<< "无法打开文件";
//        return;
//    }

//    QTextStream in(&file);
//    in.setCodec("Big5");
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        if(line.contains("Code:"))
//        {
//            QStringList lststr = line.split('-');
//            QLOG_INFO()<<lststr[4];
//        }
//    }

//    file.close();
}

void OneButtonChangeM201Wgt::slot_btn_export_file()
{

}

void OneButtonChangeM201Wgt::insert_new_product_to_db()
{
    QString strSql = QString("select max(id) from public.%1").
            arg(constM201BoardStyleAndMaterialNumber);

    int id = 0;
    QSqlQuery queryResult;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }
    strSql = QString("insert into %1 values(%2,'%3','%4')").
            arg(constM201BoardStyleAndMaterialNumber).arg(id+1).
            arg(m_pLineEditBoardStyle->text().trimmed()).
            arg(m_pLineEditMaterialNumber->text().trimmed());
    QString strError;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->ExecSqlWithConnectionName(strSql,strError))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"数据录入成功!");
        m_pLineEditBoardStyle->setText("");
        m_pLineEditMaterialNumber->setText("");
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!"<<strError;
    }
}

void OneButtonChangeM201Wgt::load_product_info_from_db()
{
    m_pMapProductInfo.clear();
    QString strSql = QString("select distinct \"boardStyle\" from public.%1").
            arg(constM201BoardStyleAndMaterialNumber);

    QList<QString> lstStr;

    QSqlQuery queryResult;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
    {
        while(queryResult.next())
        {
            lstStr.push_back(queryResult.value(0).toString());
        }
    }

    for (int i=0;i<lstStr.length();i++) {
        strSql = QString("select * from public.%1 where \"boardStyle\" = '%2'").
                arg(constM201BoardStyleAndMaterialNumber).arg(lstStr[i]);
        if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
        {
            QStringList lst;
            while(queryResult.next())
            {
                lst.push_back(queryResult.value(2).toString());
            }
            m_pMapProductInfo.insert(lstStr[i],lst);
        }
    }
}

static int countm201=0;
void OneButtonChangeM201Wgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        tboxResultWgt->set_change_result(0);
        tboxResultWgt->repaint();
        countm201++;

        if(lstStations.size() == 0)
        {
            tboxResultWgt->set_change_result(1);
            tboxResultWgt->repaint();
            killTimer(m_pTimerID);
            m_pAlreadyStartTimer = false;
        }

        if(countm201 == 10)
        {
            countm201 = 0;
            killTimer(m_pTimerID);
            tboxResultWgt->set_change_result(2);
            tboxResultWgt->repaint();
            m_pAlreadyStartTimer = false;
        }
    }
}
