#include "screwmachinewgt.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include <QGroupBox>
#include "QsLog.h"
#include "gdatafactory.h"
#include <QEventLoop>
#include <QTextCodec>
#include <QPushButton>
#include <QElapsedTimer>
#include <QCoreApplication>
ScrewMachineWgt* ScrewMachineWgt::m_pObj = nullptr;
ScrewMachineWgt::ScrewMachineWgt(QWidget *parent) :
    QWidget(parent),
    m_pTimerID(-1)
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    m_pPassWgtPCBAMes = new PassWidget();
    m_pPassWgtPrint = new PassWidget();
    m_pPassWgtScrewMes = new PassWidget();
    m_pPassWgtTcpState = new PassWidget();

    m_pMesAccessManager = new QNetworkAccessManager();
    m_pMesAccessManagerSave = new QNetworkAccessManager();
    m_pTcpCommonUse = new TcpCommonUse();

    QHBoxLayout* hAll = new QHBoxLayout();

    QVBoxLayout* vBoxLeft = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QGroupBox* grpBoxScanner = new QGroupBox(u8"扫码器");
    grpBoxScanner->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QLabel* label = new QLabel(u8"扫码值:");
    label->setStyleSheet("border:none;");
    m_pEditScanner = new QLineEdit();
    m_pEditScanner->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    m_pEditScanner->setReadOnly(true);
    m_pEditScanner->setAlignment(Qt::AlignCenter);
    hBox1->addWidget(label);
    hBox1->addWidget(m_pEditScanner);
//    QPushButton* btnTest = new QPushButton(u8"测试");
//    connect(btnTest,SIGNAL(clicked()),this,SLOT(slot_test()));
//    hBox1->addWidget(btnTest);
    grpBoxScanner->setLayout(hBox1);

    QGroupBox* grpMesPCBA = new QGroupBox(u8"Mes校验结果(PCBA)");
    grpMesPCBA->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(m_pPassWgtPCBAMes);
    grpMesPCBA->setLayout(vBox1);

    QGroupBox* grpPrint = new QGroupBox(u8"打印结果");
    grpPrint->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addWidget(m_pPassWgtPrint);
    grpPrint->setLayout(vBox2);

    vBoxLeft->addWidget(grpBoxScanner);
    vBoxLeft->addWidget(grpMesPCBA);
    vBoxLeft->addWidget(grpPrint);
    vBoxLeft->setStretch(0,1);
    vBoxLeft->setStretch(1,4);
    vBoxLeft->setStretch(2,4);

    QVBoxLayout* vBoxRight = new QVBoxLayout();
    QHBoxLayout* hBox2 = new QHBoxLayout();
    QLabel* labelCamera = new QLabel(u8"扫码值:");
    labelCamera->setStyleSheet("border:none;");
    m_pEditCamera = new QLineEdit();
    m_pEditCamera->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    m_pEditCamera->setReadOnly(true);
    m_pEditCamera->setAlignment(Qt::AlignCenter);
    hBox2->addWidget(labelCamera);
    hBox2->addWidget(m_pEditCamera);
    QGroupBox* grpCamera = new QGroupBox(u8"相机");
    grpCamera->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    grpCamera->setLayout(hBox2);

    QVBoxLayout* vBox3 = new QVBoxLayout();
    vBox3->addWidget(m_pPassWgtTcpState);
    QGroupBox* grpMesPrintState = new QGroupBox(u8"Tcp连接状态");
    grpMesPrintState->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    grpMesPrintState->setLayout(vBox3);

    QVBoxLayout* vBox4 = new QVBoxLayout();
    vBox4->addWidget(m_pPassWgtScrewMes);
    QGroupBox* grpCameraMes = new QGroupBox(u8"压合锁螺丝Mes校验结果");
    grpCameraMes->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    grpCameraMes->setLayout(vBox4);

    vBoxRight->addWidget(grpCamera);
    vBoxRight->addWidget(grpMesPrintState);
    vBoxRight->addWidget(grpCameraMes);
    vBoxRight->setStretch(0,1);
    vBoxRight->setStretch(1,4);
    vBoxRight->setStretch(2,4);

    QGroupBox* grpPCBA = new QGroupBox(u8"固定PCBA");
    grpPCBA->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,255);}");
    QGroupBox* grpLockScrew = new QGroupBox(u8"压合锁螺丝");
    grpLockScrew->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,255);}");

    grpPCBA->setLayout(vBoxLeft);
    grpLockScrew->setLayout(vBoxRight);

    hAll->addWidget(grpPCBA);
    hAll->addWidget(grpLockScrew);

    this->setLayout(hAll);
    GDataFactory::get_fins_udp_service_tbox_screw_machine()->init_ip_port(GDataFactory::get_factory()->get_config_para("PLC_IP"),
                                                                          GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt());

    GDataFactory::get_fins_udp_service_tbox_screw_machine()->moveToThread(&smReadThread);

    connect(m_pTcpCommonUse,SIGNAL(signal_proc_server_info(const QByteArray&)),this,SLOT(process_info_from_tcp_server(const QByteArray&)));
}

void ScrewMachineWgt::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(m_pTcpCommonUse->get_client_state() == QAbstractSocket::SocketState::ConnectedState)
        m_pPassWgtTcpState->set_color(1);
    else
    {
        m_pPassWgtTcpState->set_color(2);
        m_pTcpCommonUse->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TCP_SERVER_IP"),
                                               GDataFactory::get_factory()->get_config_para("TCP_SERVER_PORT").toInt());
    }
}

void ScrewMachineWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if(m_pTimerID == -1)
    {
        smReadThread.start();
        m_pTimerID = startTimer(1000);
        m_pTcpCommonUse->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TCP_SERVER_IP"),
                                               GDataFactory::get_factory()->get_config_para("TCP_SERVER_PORT").toInt());
    }
}

void ScrewMachineWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    killTimer(m_pTimerID);
    m_pTimerID = -1;
    m_pTcpCommonUse->disconnect_from_server();
}

float Byte2Float(QByteArray bt)
{
    float result = 0;
    int size = bt.size();
    char* data_char = bt.data();
    char* p = (char*)&result;
    for (int index = 0;index<size;index++) {
        *(p+index) = *(data_char+size-1-index);
    }
    return result;
}

void ScrewMachineWgt::process_plc_data(QByteArray data)
{
    if(data.length() != 204)
    {
        QLOG_ERROR()<<"the data length is not correct,end the procedure";
        smReadThread.set_sign(false);
        return;
    }

    //the sign of pcb scanner
    if((data[0] & 0x01) || (data[1] & 0x01))
    {
        //refresh the Mes save status
        m_pPassWgtScrewMes->set_color(3);
        //refresh the Mes save status

        //clear the pcb sign
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10100,0x02,clearData);

        QByteArray lengthOfSN;
        lengthOfSN.append(data[3]);
        lengthOfSN.append(data[2]);
        int tmpLength = lengthOfSN[1]*16 + lengthOfSN[0];

        QByteArray tmpArraySN_PCB;
        for(int i=4;i<tmpLength+4;i++)
            tmpArraySN_PCB.append(data[i]);

        tmpArraySN_PCB = GDataFactory::get_factory()->alter_order_of_byte_array(tmpArraySN_PCB);
        m_pEditScanner->setText(QString(tmpArraySN_PCB));

        if(get_mes_info_by_sn(tmpArraySN_PCB))//the result of pcb code from mes is OK,notify to print
        {
            QString tmpSN = tmpArraySN_PCB;
            QByteArray mesSign;
            mesSign.append(static_cast<char>(0x00));
            mesSign.append(static_cast<char>(0x01));
            GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10202,0x02,mesSign);
            m_pPassWgtPCBAMes->set_color(1);
            QJsonObject  root;
            root.insert("PRODUCT_SN",tmpSN);
            QJsonDocument doc_data(root);
            QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
            m_pTcpCommonUse->send_data_to_server(request_data);
        }
        else//notify plc when the mes result from is NG
        {
            m_pPassWgtPCBAMes->set_color(2);
            QByteArray mesSign;
            mesSign.append(static_cast<char>(0x00));
            mesSign.append(static_cast<char>(0x02));
            GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10202,0x02,mesSign);
            smReadThread.set_sign(false);
            return;
        }
    }

    //the sign of outer shell label from PLC
    if((data[91] & 0x01) || (data[90] & 0x01))
    {
        //clear the outer shell sign
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10145,0x02,clearData);

        //collect the info of screw

        QByteArray first_screw,second_screw,third_screw,forth_screw,fifth_screw;
        first_screw.append(data[82]);
        first_screw.append(data[83]);
        first_screw.append(data[84]);
        first_screw.append(data[85]);
        first_screw.append(data[86]);
        first_screw.append(data[87]);
        first_screw.append(data[88]);
        first_screw.append(data[89]);

        second_screw.append(data[172]);
        second_screw.append(data[173]);
        second_screw.append(data[174]);
        second_screw.append(data[175]);
        second_screw.append(data[176]);
        second_screw.append(data[177]);
        second_screw.append(data[178]);
        second_screw.append(data[179]);

        third_screw.append(data[180]);
        third_screw.append(data[181]);
        third_screw.append(data[182]);
        third_screw.append(data[183]);
        third_screw.append(data[184]);
        third_screw.append(data[185]);
        third_screw.append(data[186]);
        third_screw.append(data[187]);

        forth_screw.append(data[188]);
        forth_screw.append(data[189]);
        forth_screw.append(data[190]);
        forth_screw.append(data[191]);
        forth_screw.append(data[192]);
        forth_screw.append(data[193]);
        forth_screw.append(data[194]);
        forth_screw.append(data[195]);

        fifth_screw.append(data[196]);
        fifth_screw.append(data[197]);
        fifth_screw.append(data[198]);
        fifth_screw.append(data[199]);
        fifth_screw.append(data[200]);
        fifth_screw.append(data[201]);
        fifth_screw.append(data[202]);
        fifth_screw.append(data[203]);

        float aa;
        first_screw = GDataFactory::get_factory()->alter_order_of_byte_array(first_screw);
        memcpy(&aa,first_screw.mid(0,4),4);
        QLOG_WARN()<<aa;

        float bb;
        second_screw = GDataFactory::get_factory()->alter_order_of_byte_array(second_screw);
        memcpy(&bb,second_screw.mid(0,4),4);
        QLOG_WARN()<<bb;

        float cc;
        third_screw = GDataFactory::get_factory()->alter_order_of_byte_array(third_screw);
        memcpy(&cc,third_screw.mid(0,4),4);
        QLOG_WARN()<<cc;

        float dd;
        forth_screw = GDataFactory::get_factory()->alter_order_of_byte_array(forth_screw);
        memcpy(&dd,forth_screw.mid(0,4),4);
        QLOG_WARN()<<dd;

        float ee;
        fifth_screw = GDataFactory::get_factory()->alter_order_of_byte_array(fifth_screw);
        memcpy(&ee,fifth_screw.mid(0,4),4);
        QLOG_WARN()<<ee;

        QList<QString> lstScrewInfo;
        lstScrewInfo<<QString::number(aa)<<QString::number(bb)<<QString::number(cc)<<
                      QString::number(dd)<<QString::number(ee);
        //collect the info of screw

        //the second SN
        QByteArray tmpLengthOfSN;
        tmpLengthOfSN.append(data[92]);
        tmpLengthOfSN.append(data[93]);
        int tmpLength = data[92]*16 + data[93];
        QLOG_ERROR()<<"the length of sn is:"<<tmpLength;
        QByteArray tmpArraySN_Lock;
        QString tmpSNtest = "123456str";
        if(tmpLength > 16)
        {
            for(int i=94+tmpLength-16;i<tmpLength+94;i++)
                tmpArraySN_Lock.append(data[i]);
        }
        else {
            for(int i=94;i<tmpLength+94;i++)
                tmpArraySN_Lock.append(data[i]);
        }

        tmpArraySN_Lock = GDataFactory::get_factory()->alter_order_of_byte_array(tmpArraySN_Lock);
        m_pEditCamera->setText(tmpArraySN_Lock);

        //lilei-20240614
        if(tmpArraySN_Lock.trimmed() == "")
        {
            //refresh the status
            m_pPassWgtPCBAMes->set_color(3);
            m_pPassWgtPrint->set_color(3);
            //refresh the status

            m_pPassWgtScrewMes->set_color(2);
            QByteArray mesSign;
            mesSign.append(static_cast<char>(0x00));
            mesSign.append(static_cast<char>(0x02));
            GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10204,0x02,mesSign);
            return;
        }
        //lilei-20240614

        if(save_product_info_to_mes(tmpArraySN_Lock,lstScrewInfo))
        {
            QLOG_ERROR()<<"first step";
            //refresh the status
            m_pPassWgtPCBAMes->set_color(3);
            m_pPassWgtPrint->set_color(3);
            //refresh the status

            m_pPassWgtScrewMes->set_color(1);
            QByteArray mesSign;
            mesSign.append(static_cast<char>(0x00));
            mesSign.append(static_cast<char>(0x01));
            GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10204,0x02,mesSign);
        }
        else
        {
            QLOG_ERROR()<<"second step";
            //refresh the status
            m_pPassWgtPCBAMes->set_color(3);
            m_pPassWgtPrint->set_color(3);
            //refresh the status

            m_pPassWgtScrewMes->set_color(2);
            QByteArray mesSign;
            mesSign.append(static_cast<char>(0x00));
            mesSign.append(static_cast<char>(0x02));
            GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10204,0x02,mesSign);
        }
    }
    else {
        //clear the outer shell sign,because scann failed
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10145,0x02,clearData);
    }

    smReadThread.set_sign(false);
}

void ScrewMachineWgt::process_info_from_tcp_server(const QByteArray& data)
{
    QLOG_ERROR()<<"Rev from TCP Server,the data is:"<<data;
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QString tmpStr = tc->toUnicode(data);


    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(tmpStr.toUtf8(),&error);
    QLOG_INFO()<<error.errorString();

    QJsonObject jsonObject = jsonDocument.object();
    int resultCode = -1;
    if(jsonObject.contains("ResultCode"))
    {
        QJsonValue instructionValue = jsonObject.value(QStringLiteral("ResultCode"));
        resultCode = instructionValue.toInt();
    }
    else//if the reply of mes is ABNORMAL,print NG and return
    {
        QByteArray printSign;
        printSign.append(static_cast<char>(0x00));
        printSign.append(static_cast<char>(0x02));
        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10203,0x02,printSign);
        return;
    }

    if(resultCode == 200)
    {
        m_pPassWgtPrint->set_color(1);
        QByteArray printSign;
        printSign.append(static_cast<char>(0x00));
        printSign.append(static_cast<char>(0x01));
        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10203,0x02,printSign);
    }
    else {
        m_pPassWgtPrint->set_color(2);
        QByteArray printSign;
        printSign.append(static_cast<char>(0x00));
        printSign.append(static_cast<char>(0x02));
        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10203,0x02,printSign);
    }
}

void ScrewMachineWgt::slot_test()
{
    QString testSN = "ZT1E2R311N0286";
//    QJsonObject  root;
//    root.insert("PRODUCT_SN",testSN);
//    QJsonDocument doc_data(root);
//    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
//    m_pTcpCommonUse->send_data_to_server(request_data);

    get_mes_info_by_sn(testSN.toUtf8());
}

void ScrewMachineWgt::slot_mes_info_by_sn(const QByteArray &sn)
{
//    if(get_mes_info_by_sn(sn))//the result of pcb code from mes is OK,notify to print
//    {
//        QString tmpSN = sn;
//        QByteArray mesSign;
//        mesSign.append(static_cast<char>(0x00));
//        mesSign.append(static_cast<char>(0x01));
//        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10192,0x02,mesSign);
//        m_pPassWgtPCBAMes->set_color(1);
//        QJsonObject  root;
//        root.insert("PRODUCT_SN",tmpSN);
//        QJsonDocument doc_data(root);
//        QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
//        m_pTcpCommonUse->send_data_to_server(request_data);
//    }
//    else//notify plc when the mes result from is NG
//    {
//        m_pPassWgtPCBAMes->set_color(2);
//        QByteArray mesSign;
//        mesSign.append(static_cast<char>(0x00));
//        mesSign.append(static_cast<char>(0x02));
//        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10192,0x02,mesSign);
//    }
}

void ScrewMachineWgt::slot_product_info_to_mes(const QByteArray data, const QList<QString> lstinfo)
{
//    if(save_product_info_to_mes(data,lstinfo))
//    {
//        m_pPassWgtScrewMes->set_color(1);
//        QByteArray mesSign;
//        mesSign.append(static_cast<char>(0x00));
//        mesSign.append(static_cast<char>(0x01));
//        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10194,0x02,mesSign);
//    }
//    else
//    {
//        m_pPassWgtScrewMes->set_color(2);
//        QByteArray mesSign;
//        mesSign.append(static_cast<char>(0x00));
//        mesSign.append(static_cast<char>(0x02));
//        GDataFactory::get_fins_udp_service_tbox_screw_machine()->send_message_to_plc(WRITE_PLC,ADDRESS_SCREW_MACHINE_D10194,0x02,mesSign);
//    }
}

bool ScrewMachineWgt::get_mes_info_by_sn(const QByteArray data)
{
    bool result = false;
    QString productsn = data;
    if(productsn.contains('/'))
    {
        productsn.remove('\r');
        QStringList lst = productsn.split('/');
        if(lst.size() == 2)
            productsn = lst[1];
        else
        {
            QLOG_WARN()<<"the SN format is ABNORMAL!";
            return false;
        }
    }
    else
        productsn.remove('\r');

    QLOG_ERROR()<<"the sn of pcba is:"<<productsn;
//    QString urlDes = "http://127.0.0.1:8888/post";
    QString urlDes = "";
    urlDes.append("http://");
    urlDes.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    urlDes.append(":");
    urlDes.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    urlDes.append("/Api/Ats/AtsCkRun");
    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root, data_value,ats_models;
    QJsonArray   ats_value;

    root.insert("iType", "2");
//    root.insert("reType", "AOI_ZD");
    root.insert("empNo","");
    root.insert("pcbCode", productsn);
    root.insert("workStationSn", GDataFactory::get_factory()->get_config_para("TBOX_SCREW_MACHINE_WORK_STATION_PCB"));

    QNetworkReply* reply;
    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    reply = m_pMesAccessManager->post(request, request_data);

    QEventLoop eventloop;
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    if(reply->isFinished())
    {
        QByteArray responseByte = reply->readAll();
        QTextCodec *tc = QTextCodec::codecForName("UTF8");
        QString tmpStr = tc->toUnicode(responseByte);
        QLOG_ERROR()<<"the pcba Mes reply data is:"<<tmpStr;

        QJsonParseError jError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte,&jError);
        QLOG_ERROR()<<jError.errorString();
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("code"))
        {
            QJsonValue instructionValue = jsonObject.value(QStringLiteral("code"));
            QString rMes = instructionValue.toString();
            if(rMes == "OK")
                result = true;
            else
                result = false;
        }
        else
        {
            QLOG_WARN()<<"the info from Mes exist ERROR!";
            result = false;
        }
    }
    else
    {
        QLOG_WARN()<<"http post request reply is TIMEOUT!";
        result = false;
    }

    return result;
}

bool ScrewMachineWgt::save_product_info_to_mes(const QByteArray data,const QList<QString> lstinfo)
{
    bool result = false;
    QString productsn = data;
    if(productsn.contains('/'))
    {
        productsn.remove('\r');
        QStringList lst = productsn.split('/');
        if(lst.size() == 2)
            productsn = lst[1];
        else
        {
            QLOG_WARN()<<"the SN format is ABNORMAL!";
            return false;
        }
    }
    else
        productsn.remove('\r');

    QLOG_ERROR()<<"the out sn is:"<<productsn;

    QString urlDes/* = "http://127.0.0.1:8888/post"*/;
    urlDes.append("http://");
    urlDes.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    urlDes.append(":");
    urlDes.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    urlDes.append("/Api/Ats/AtsTsRun");
    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root, data_value,ats_models[5];
    QJsonArray   ats_value;

    for (int i=0;i<lstinfo.size();i++) {
        ats_models[i].insert("itemId",QString::number(i));
        ats_models[i].insert("itemName",QString("screw").append(QString::number(i)));
        ats_models[i].insert("itemL_Limit","");
        ats_models[i].insert("itemH_Limit","");
        ats_models[i].insert("itemTsValue","");
        ats_models[i].insert("TestValue",lstinfo[i]);
        ats_models[i].insert("itemLength","123");
        ats_models[i].insert("itemRes","OK");
        ats_models[i].insert("errCodes","");

        ats_value.append(ats_models[i]);
    }

    root.insert("atsTsItemsModels", ats_value);

    root.insert("iType", "2");
    root.insert("empNo","");
    root.insert("pcbCode", productsn);
    root.insert("workStationSn", GDataFactory::get_factory()->get_config_para("TBOX_SCREW_MACHINE_WORK_STATION_LOCK_SCREW"));
    root.insert("tsRes","OK");
    root.insert("tsLength", "");
    root.insert("pcName", "sys001");
    root.insert("pcIp", "127.0.0.1");
    root.insert("devName",u8"压合锁螺丝");//压合锁螺丝
    root.insert("devModel", "9527");
    root.insert("devNo", "9527");
    root.insert("toolNo", "");
    root.insert("toolBitSn", "");
    root.insert("logData", "");
    root.insert("softVer","");

    data_value.insert("macInfo","");
    data_value.insert("imeiInfo","");
    data_value.insert("meidInfo","");
    data_value.insert("keyInfo","");
    data_value.insert("ProgramName","");
    data_value.insert("topEltNumber","");
    data_value.insert("botEltNumber","");
    root.insert("data",data_value);

    QNetworkReply* reply;
    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    QTextCodec *tc = QTextCodec::codecForName("UTF8");
    QString tmpStr = tc->toUnicode(request_data);
    QLOG_INFO()<<"the MSG send to MES is:"<<request_data;
    reply = m_pMesAccessManagerSave->post(request, request_data);

    QEventLoop eventloop;
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    if(reply->isFinished())
    {
        QByteArray responseByte = reply->readAll();
        QTextCodec *tc = QTextCodec::codecForName("UTF8");
        QString tmpStr = tc->toUnicode(responseByte);
        QLOG_ERROR()<<"the Mes reply SAVE data is:"<<tmpStr;

        QJsonParseError jError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte,&jError);
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("code"))
        {
            QJsonValue instructionValue = jsonObject.value(QStringLiteral("code"));
            QString rMes = instructionValue.toString();
            if(rMes == "OK")
                result = true;
            else
                result = false;
        }
        else
        {
            QLOG_WARN()<<"the info from Mes exist ERROR!";
            result = false;
        }
    }
    else
    {
        QLOG_WARN()<<"http post request reply is TIMEOUT!";
        result = false;
    }
    return result;
}

