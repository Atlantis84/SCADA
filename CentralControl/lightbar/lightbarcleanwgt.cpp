#include "lightbarcleanwgt.h"
#include "TitleBar.h"
#include <QIcon>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include "gdatafactory.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include "logindialog.h"
#include <QEventLoop>
#include "imessagebox.h"
LightBarCleanWgt::LightBarCleanWgt(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(800,700);
//    TitleBar *pTitleBar = new TitleBar(this);
//    pTitleBar->setTitle(u8"灯条擦拭");
//    pTitleBar->setFixedWidth(800);
//    installEventFilter(pTitleBar);

    resultWgt = new ResultWidget();
    resultWgt->set_product_state(1);
    resultProcedureWgt = new ResultWidget();
    resultProcedureWgt->set_product_state(4);
    QGroupBox* grpScanInfo = new QGroupBox(u8"扫码信息");
    grpScanInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QHBoxLayout* hBox11 = new QHBoxLayout();
    QVBoxLayout* vBox11 = new QVBoxLayout();
    QLabel* label_SN = new QLabel(u8"第一次扫码SN:");
    label_SN->setStyleSheet("border:none;");
    lineEdit_SN = new QLineEdit();
    lineEdit_SN->setReadOnly(true);
    lineEdit_SN->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox1->addWidget(label_SN);
    hBox1->addWidget(lineEdit_SN);

    QLabel* label_SN2 = new QLabel(u8"第二次扫码SN:");
    label_SN2->setStyleSheet("border:none;");
    lineEdit_SN2 = new QLineEdit();
    lineEdit_SN2->setReadOnly(true);
    lineEdit_SN2->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox11->addWidget(label_SN2);
    hBox11->addWidget(lineEdit_SN2);
    vBox11->addLayout(hBox1);
    vBox11->addLayout(hBox11);
    grpScanInfo->setLayout(vBox11);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QHBoxLayout* hBox3 = new QHBoxLayout();
    QHBoxLayout* hBox4 = new QHBoxLayout();
    QHBoxLayout* hBox5 = new QHBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    QGroupBox* grpMesInfo = new QGroupBox(u8"MES信息");
    grpMesInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QLabel* label_PcbCode = new QLabel(u8" PCB喷码组号:");
    label_PcbCode->setStyleSheet("border:none;");
    lineEdit_PcbCode = new QLineEdit();
    lineEdit_PcbCode->setReadOnly(true);
    lineEdit_PcbCode->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox2->addWidget(label_PcbCode);
    hBox2->addWidget(lineEdit_PcbCode);
    QLabel* label_ModelCode = new QLabel(u8"    半品料号:");
    label_ModelCode->setStyleSheet("border:none;");
    lineEdit_ModelCode = new QLineEdit();
    lineEdit_ModelCode->setReadOnly(true);
    lineEdit_ModelCode->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox3->addWidget(label_ModelCode);
    hBox3->addWidget(lineEdit_ModelCode);
    QLabel* label_ResultCode = new QLabel(u8"    结果代码:");
    label_ResultCode->setStyleSheet("border:none;");
    lineEdit_ResultCode = new QLineEdit();
    lineEdit_ResultCode->setReadOnly(true);
    lineEdit_ResultCode->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox4->addWidget(label_ResultCode);
    hBox4->addWidget(lineEdit_ResultCode);
    QLabel* label_Result = new QLabel(u8"    返回信息:");
    label_Result->setStyleSheet("border:none;");
    lineEdit_Result = new QLineEdit();
    lineEdit_Result->setReadOnly(true);
    lineEdit_Result->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox5->addWidget(label_Result);
    hBox5->addWidget(lineEdit_Result);
    vBox1->addLayout(hBox2);
    vBox1->addLayout(hBox3);
    vBox1->addLayout(hBox4);
    vBox1->addLayout(hBox5);
    grpMesInfo->setLayout(vBox1);

    QGroupBox* grpResultWgt = new QGroupBox(u8"当前产品状态");
    grpResultWgt->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox6 = new QHBoxLayout();
    hBox6->addWidget(resultProcedureWgt);
    hBox6->addWidget(resultWgt);
    grpResultWgt->setLayout(hBox6);

    QGroupBox* grpPermitWgt = new QGroupBox(u8"操作");
    grpPermitWgt->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox7 = new QHBoxLayout();
    btnManualPass = new QPushButton(u8"人工清洗完毕");
    btnManualPass->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnManualPass,SIGNAL(clicked()),this,SLOT(slot_btn_click()));

    QPushButton* btnManualInsert = new QPushButton(u8"录入新物料号");
    btnManualInsert->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                                    QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnManualInsert,SIGNAL(clicked()),this,SLOT(slot_btn_new_product_number()));
    QPushButton* btnCheckResult = new QPushButton(u8"查看擦拭记录");
    btnCheckResult->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnCheckResult,SIGNAL(clicked()),this,SLOT(slot_btn_check_washing_records()));
    hBox7->addWidget(btnCheckResult);
    hBox7->addWidget(btnManualInsert);
    hBox7->addWidget(btnManualPass);
    grpPermitWgt->setLayout(hBox7);

    QVBoxLayout* vAll = new QVBoxLayout();
//    vAll->addSpacing(40);
    vAll->addWidget(grpScanInfo);
    vAll->addWidget(grpMesInfo);
    vAll->addWidget(grpResultWgt);
    vAll->addWidget(grpPermitWgt);
    vAll->setStretch(1,1);
    vAll->setStretch(2,3);
    vAll->setStretch(3,2);
    vAll->setStretch(4,1);
    this->setLayout(vAll);
}

void LightBarCleanWgt::set_product_sn(const int sign,const QString sn)
{
    if(sign == 0)
    {
        this->lineEdit_SN->setText("");
        this->lineEdit_SN2->setText("");
    }
    else if(sign == 1)
    {
        this->lineEdit_SN->setText("");
        this->lineEdit_SN->setText(sn);
    }
    else if(sign == 2)
    {
        this->lineEdit_SN2->setText("");
        this->lineEdit_SN2->setText(sn);
    }

}

void LightBarCleanWgt::trigger_timer_to_query()
{
    m_pTimerID = startTimer(2000);
}

void LightBarCleanWgt::set_ok_ng_state(bool state)
{
    if(state)
        this->resultWgt->set_product_state(1);
    else
        this->resultWgt->set_product_state(2);
}

void LightBarCleanWgt::set_procedure_state(bool state)
{
    if(state)
        this->resultProcedureWgt->set_product_state(4);
    else
        this->resultProcedureWgt->set_product_state(3);
}

void LightBarCleanWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void LightBarCleanWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pCurrentSN = "";
    m_pWashingSign = true;
    GDataFactory::get_factory()->stop_serial_port_light_bar();
    GDataFactory::get_factory()->read_serial_number_light_bar();
}

void LightBarCleanWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->lineEdit_SN->setText("");
    this->lineEdit_SN2->setText("");
    this->lineEdit_Result->setText("");
    this->lineEdit_PcbCode->setText("");
    this->lineEdit_ModelCode->setText("");
    this->lineEdit_ResultCode->setText("");
    m_pWashTimes = 0;
    m_pWashingSign = true;

    this->resultWgt->set_product_state(1);
    this->resultProcedureWgt->set_product_state(4);

    killTimer(m_pTimerID);
    killTimer(m_pTimerIDMonitor);
    GDataFactory::get_factory()->clear_1_2_light_bar_sn();
    GDataFactory::get_factory()->stop_serial_port_light_bar();
}

void LightBarCleanWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        GDataFactory::get_fins_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D10040,0x04,nullptr);
    }
    else if(event->timerId() == m_pTimerIDMonitor)
    {
        if(GDataFactory::get_factory()->get_second_sn_light_bar() == "")
            QLOG_WARN()<<"please scan the product code for the second time";
        else {
            killTimer(m_pTimerIDMonitor);
            m_pTimerIDMonitor = -1;
            insert_wash_record_to_db();

            this->lineEdit_SN->setText("");
            this->lineEdit_SN2->setText("");
            GDataFactory::get_factory()->clear_1_2_light_bar_sn();
            m_pWashTimes = 0;
        }
    }
}

void LightBarCleanWgt::insert_wash_record_to_db()
{
    QString strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values('%2','%3','%4','%5')").
            arg(constLightBarWashingResultTable).arg(this->lineEdit_SN->text().trimmed()).
            arg(QString::number(m_pWashTimes)).arg(currentDateTime.toString("yyyy-MM-dd hh:mm:ss ddd")).
            arg(GDataFactory::get_factory()->get_current_usr_name().append(u8" 正常清洗"));
    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!";
    }
}

void LightBarCleanWgt::insert_wash_record_to_db_manual()
{
    QString strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values('%2','%3','%4','%5')").
            arg(constLightBarWashingResultTable).arg(this->lineEdit_SN->text().trimmed()).
            arg(QString::number(m_pWashTimes)).arg(currentDateTime.toString("yyyy-MM-dd hh:mm:ss ddd")).
            arg(GDataFactory::get_factory()->get_current_usr_name().append(u8" 异常中断人工清洗"));
    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!";
    }
}

void LightBarCleanWgt::send_cmd_to_plc_accroding_to_mes_info()
{
    QString strSendNumber = "";
    if(m_pCurrentSN.length() > 7)
    {
        if(m_pCurrentSN.contains("S00"))
        {
            for (int i=0;i<10;i++)
                strSendNumber.append(m_pCurrentSN[i]);
        }
        else {
            for (int i=0;i<7;i++)
                strSendNumber.append(m_pCurrentSN[i]);
        }
        strSendNumber = strSendNumber.trimmed();
    }

    short numID = GDataFactory::get_light_bar_new_product_wgt()->select_id_from_db(strSendNumber).toShort();

    if(numID == 0)
    {
        GDataFactory::get_factory()->clear_1_2_light_bar_sn();
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"数据库中不存在该产品，请录入!");
        return;
    }
    this->set_product_sn(1,m_pCurrentSN);
    QByteArray dataSign;
    dataSign.append(static_cast<char>(0x00));
    dataSign.append(static_cast<char>(numID));
    GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10010,static_cast<short>(dataSign.length()),dataSign);
    QEventLoop eventloop;
    QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    QByteArray data;
    data.append(strSendNumber.toUtf8());
    for (int i=data.length();i<20;i++)
        data.append(static_cast<char>(0x00));
    GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10011,static_cast<short>(data.length()),data);
    QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    QByteArray finishSign;
    finishSign.append(static_cast<char>(0x00));
    finishSign.append(static_cast<char>(0x01));
    GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10030,0x02,finishSign);
    QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    this->trigger_timer_to_query();
    this->m_pWashingSign = false;
    this->set_procedure_state(false);
}

void LightBarCleanWgt::slot_btn_click()
{
    LoginDialog lgDialog;
    if(lgDialog.exec() == QDialog::Accepted)
    {
        killTimer(m_pTimerIDMonitor);
        killTimer(m_pTimerID);

        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        GDataFactory::get_fins_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D10040,0x04,clearData);

        insert_wash_record_to_db_manual();

        resultWgt->set_product_state(1);
        resultProcedureWgt->set_product_state(4);
        this->lineEdit_SN->setText("");
        this->lineEdit_SN2->setText("");
        GDataFactory::get_factory()->clear_1_2_light_bar_sn();
        m_pWashTimes = 0;
    }
    else
        ;
}

void LightBarCleanWgt::slot_finish_washing(const int washtimes)
{
    //the following operations must be according to mes result
    this->set_procedure_state(true);
    m_pWashingSign = true;
    m_pWashTimes = 0;
    m_pWashTimes = washtimes;

    killTimer(m_pTimerID);
    m_pTimerID = -1;
    m_pTimerIDMonitor = startTimer(1000);
}

void LightBarCleanWgt::slot_btn_check_washing_records()
{
    GDataFactory::get_light_bar_washing_result_wgt()->show();
}

void LightBarCleanWgt::slot_btn_new_product_number()
{
    GDataFactory::get_light_bar_new_product_wgt()->show();
}

void LightBarCleanWgt::slot_rev_sn_info(int codeSign,const QByteArray& data)
{
    m_pCurrentSN = data;
    if(codeSign == 1)
    {
        this->lineEdit_SN->setText("");
        this->lineEdit_SN->setText(m_pCurrentSN);

        QJsonObject  root;
        root.insert("SN",m_pCurrentSN);
        root.insert("CleanEmp","DTCS");
        QJsonDocument doc_data(root);
        QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
        GDataFactory::get_mes_process()->exec_http_post_light_bar(1,request_data);
    }
    else
    {
        if(this->m_pWashingSign == false)
        {
            GDataFactory::get_factory()->set_second_sn_light_bar("");
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"产品擦拭中，请勿扫码!");
            return;
        }

        GDataFactory::get_common_waiting_wgt()->show();
        this->lineEdit_SN2->setText("");
        this->lineEdit_SN2->setText(m_pCurrentSN);

        QJsonObject  root;
        root.insert("SN",m_pCurrentSN);
        root.insert("PCBCode",this->lineEdit_PcbCode->text());
//        root.insert("CleanCount",QString::number(m_pWashTimes));
        root.insert("CleanCount",1);
        root.insert("Type",0);
        root.insert("CleanEmp","DTCS");
        QJsonDocument doc_data(root);
        QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
        GDataFactory::get_mes_process()->exec_http_post_light_bar(2,request_data);
    }
}

void LightBarCleanWgt::slot_rev_mes_result(const QByteArray &data)
{
    this->lineEdit_PcbCode->setText("");
    this->lineEdit_ModelCode->setText("");
    this->lineEdit_ResultCode->setText("");
    this->lineEdit_Result->setText("");

    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = jsonDocument.object();
    QList<QString> lstInfo;
    if(jsonObject.contains("code"))
    {
        QJsonValue code = jsonObject.value(QStringLiteral("code"));
        if(code.toString() == "OK")
        {
            QJsonValue msg = jsonObject.value(QStringLiteral("msg"));
            if(msg.toString().contains(u8"不良维护成功"))
            {
                QJsonObject objData = jsonObject.value(QStringLiteral("data")).toObject();
                QJsonValue PcbCodeValue = objData.value(QStringLiteral("pcbCode"));
                this->lineEdit_PcbCode->setText(PcbCodeValue.toString());
                QJsonValue ModelCodeValue = objData.value(QStringLiteral("modelCode"));
                this->lineEdit_ModelCode->setText(ModelCodeValue.toString());

                this->lineEdit_Result->setText(u8"OK-Mes不良维护成功!");
                this->set_ok_ng_state(false);
                send_cmd_to_plc_accroding_to_mes_info();
            }
            else if(msg.toString().contains(u8"清洗成功"))
            {
                GDataFactory::get_common_waiting_wgt()->slot_close();
                this->lineEdit_Result->setText(u8"OK-Mes清洗成功!");
                this->set_ok_ng_state(true);
            }
        }
        else//NG
        {
            QJsonValue msg = jsonObject.value(QStringLiteral("msg"));
            if(msg.toString().contains(u8"不良维护失败"))
            {
                this->lineEdit_Result->setText(msg.toString());
            }

            if(msg.toString().contains(u8"清洗失败"))
            {
                GDataFactory::get_common_waiting_wgt()->slot_close();
                this->lineEdit_Result->setText(msg.toString());
            }
        }
    }
    else
        QLOG_WARN()<<u8"Mes返回信息内容错误!";
}
