#include "repairstationwgt.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include "zebraprinterservice.h"

RepairStationWgt* RepairStationWgt::m_pObj = nullptr;

RepairStationWgt::RepairStationWgt(QWidget *parent) : QWidget(parent)
{
    m_pLogTextEdit = new QTextEdit();
    m_pLogTextEdit->setStyleSheet("QTextEdit{background-color:rgba(255,255,255,100);color:rgb(0,0,0);font-family:KaiTi;font-size:20px;}");

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

    QGroupBox* grpFinalResult = new QGroupBox(u8"校验及结果显示");
    grpFinalResult->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox21 = new QHBoxLayout();
    QLabel* labelScanBoard = new QLabel(u8"扫码信息");
    QLabel* labelClearSign = new QLabel(u8"清除老化标志");
    QLabel* labelMesResult = new QLabel(u8"MES上传");

    m_pScanBoard = new BallWidget();
    m_pClearSign = new BallWidget();
    m_pMesResult = new BallWidget();

    hBox21->addStretch();
    hBox21->addWidget(m_pScanBoard);
    hBox21->addWidget(labelScanBoard);
    hBox21->addStretch();
    hBox21->addWidget(m_pClearSign);
    hBox21->addWidget(labelClearSign);
    hBox21->addStretch();
    hBox21->addWidget(m_pMesResult);
    hBox21->addWidget(labelMesResult);
    hBox21->addStretch();
    grpFinalResult->setLayout(hBox21);

    QHBoxLayout* vBoxScan = new QHBoxLayout();
    QGroupBox* grpScanInfo = new QGroupBox(u8"校验及结果显示");
    grpScanInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QLabel* m_pLabelScanInfo  = new QLabel("扫码信息：");
    m_pScanSNEdit = new QLineEdit;
    m_pScanSNEdit->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    vBoxScan->addWidget(m_pLabelScanInfo);
    vBoxScan->addWidget(m_pScanSNEdit);
    vBoxScan->addSpacing(100);
    grpScanInfo->setLayout(vBoxScan);
    connect(m_pScanSNEdit,&QLineEdit::returnPressed,this,&RepairStationWgt::slot_read_serial_number);

    QVBoxLayout* vBox21 = new QVBoxLayout();
    QGroupBox* grpLogInfo = new QGroupBox(u8"log信息");
    grpLogInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    vBox21->addWidget(m_pLogTextEdit);
    grpLogInfo->setLayout(vBox21);

    vAll->addWidget(grpEquipInfo);
    vAll->addWidget(grpFinalResult);
    vAll->addWidget(grpScanInfo);
    vAll->addWidget(grpLogInfo);
    this->setLayout(vAll);
}


void RepairStationWgt::slot_station_config()
{
    GDataFactory::get_station_config_wgt()->set_station_characters(2);
    GDataFactory::get_station_config_wgt()->show();
}

QString extractTextBetween(const QString& input, const QString& start, const QString& end)
{
    int startPos = input.indexOf(start);
    if (startPos == -1) {
        return "";
    }

    startPos += start.length();
    int endPos = input.indexOf(end, startPos);
    if (endPos == -1) {
        return "";
    }

    return input.mid(startPos, endPos - startPos);
}

void RepairStationWgt::slot_read_serial_number()
{
    QString textScanNumer = m_pScanSNEdit->text();
    QLOG_INFO()<<"read serial number is:"<<textScanNumer;
    log_add_text_edit(u8"读取条码内容："+textScanNumer);
    QString resetAgingStart = "Reset aging:";
    QString resetAgingEnd = ";";
    QString snStart = "SN:";
    QString snEnd = ";";

//    zebraPrinterService dd;
//    QString msgInfo;
//    dd.printExcuteFunc(textScanNumer,GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_MODEL_PATH"),msgInfo);

    if(textScanNumer.contains("reset aging:")){//PAD
        QString resetAgingText = extractTextBetween(textScanNumer, resetAgingStart, resetAgingEnd);
        QString snText = extractTextBetween(textScanNumer, snStart, snEnd);

        if(!snText.isEmpty())
        {
            m_pScanBoard->set_color(1);
            m_pLogTextEdit->append(snText);
        }
        else
        {
            QLOG_WARN()<<"snText read fail";
            log_add_text_edit(u8"Seral_number读取失败");
            return;
        }

        if(!resetAgingText.isEmpty())
        {
            if(resetAgingText == "OK")
                m_pClearSign->set_color(1);
            else if(resetAgingText == "NG")
                m_pClearSign->set_color(1);
        }
        else
        {
            QLOG_WARN()<<"resetAgingText read fail";
        }

        //上传MES数据

    }
    else {//底座逻辑
        // 监听串口端口变化
        m_nTimeId = startTimer(500);
        m_previousPorts =  QSerialPortInfo::availablePorts();
    }
}

void RepairStationWgt::log_add_text_edit(QString text)
{
    QString time_log = QDateTime::currentDateTime().toString(fmtDateTime);
    m_pLogTextEdit->append(time_log + " "+ text);
}

void RepairStationWgt::format_result_json_info()
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

    content.insert("SERIAL_NUMBER",m_clearAgingResult.m_serial_number);
    content.insert("AGING_FLAG",m_clearAgingResult.m_result);
}

bool containsSerialPort(const QList<QSerialPortInfo> &ports, const QSerialPortInfo &port)
{
    for (const QSerialPortInfo &portInfo : ports) {
        if (portInfo.portName() == port.portName()) {
            return true;
        }
    }
    return false;
}

void RepairStationWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimeId){
        // 获取当前的串口列表
        QList<QSerialPortInfo> currentPorts = QSerialPortInfo::availablePorts();

        // 检查新出现的串口
        for (const QSerialPortInfo &portInfo : currentPorts) {
            if (!containsSerialPort(m_previousPorts, portInfo)) {
                emit serialPortInserted(portInfo.portName());
            }
        }

        // 检查被移除的串口
        for (const QSerialPortInfo &portInfo : m_previousPorts) {
            if (!containsSerialPort(currentPorts, portInfo)) {
                emit serialPortRemoved(portInfo.portName());
            }
        }

        // 更新上一次的串口列表
        m_previousPorts = currentPorts;
    }
}

void RepairStationWgt::closeEvent(QCloseEvent *event)
{
    m_pScanSNEdit->clear();
    killTimer(m_nTimeId);
    m_pScanBoard->set_color(0);
    m_pClearSign->set_color(0);
    m_pMesResult->set_color(0);
}

void RepairStationWgt::showEvent(QShowEvent *event)
{
    m_pScanSNEdit->clear();
    m_pScanSNEdit->setFocus();

    m_pResultData.Source_System = (char*)("EQUIPMENT");
    m_pResultData.Message_Type = (char*)("CLEAR_AGING");

    m_pLineEditStationCode->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_STATION_CODE"));
    m_pLineEditEquipCode->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_EQUIPMENT_CODE"));
    m_pLineEditEquipName->setText(GDataFactory::get_factory()->get_config_para("PRESS_FIT_EQUIPMENT_NAME"));

}
