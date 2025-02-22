﻿#include "centralwidget.h"
#include <QPainter>
#include "gdatafactory.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimerEvent>
#include <QEventLoop>
#include <QtEndian>
#include "QsLog.h"
#include "gdataacquisitionfactory.h"
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    tabWidget = new QTabWidget();
    tabWidget->setTabsClosable(true);
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(slot_close_current_tab(int)));
    QHBoxLayout* hAll = new QHBoxLayout();
    QVBoxLayout* vLeft = new QVBoxLayout();
    QVBoxLayout* vRight = new QVBoxLayout();

    vLeft->addWidget(GDataFactory::get_left_wgt());

    vRight->addWidget(GDataFactory::get_right_head_wgt());
//    vRight->addWidget(GDataFactory::get_right_query_wgt());

    vRight->addWidget(tabWidget);
//    vRight->addWidget(GDataFactory::get_test_wgt());
//    vRight->addWidget(GDataFactory::get_right_main_wgt());
    vRight->setStretch(0,1);
    vRight->setStretch(1,14);
//    vRight->setStretch(2,25);

    hAll->addLayout(vLeft);
    hAll->addLayout(vRight);
    hAll->setStretch(0,4);
    hAll->setStretch(1,15);
    this->setLayout(hAll);

    m_pUdpSocket = nullptr;
    m_pUdpSocket = new QUdpSocket(this);
    m_pUdpSocket->bind(QHostAddress("192.168.8.196"),50001);
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    connect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    connect(this,SIGNAL(signal_proc_final_data(QByteArray)),this,SLOT(slot_proc_final_data(QByteArray)));

    //for the sanner behind the AOI
    if(GDataFactory::get_factory()->get_config_para("SCANNER_TIMER") == "1")
    {
        m_pTimerID = startTimer(300);
        m_pMonitorID = startTimer(200);
    }
    else
        ;
}

void CentralWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(240,242,245)));

}

static int monitorCount = 0;
void CentralWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(event->timerId() == m_pTimerID)
    {
        monitorCount = 0;
        killTimer(m_pTimerID);
        //before change SN
//        send_message_to_plc(READ_PLC,ADDRESS_D000,0x0C,nullptr);
        //before change SN
        send_message_to_plc(READ_PLC,ADDRESS_D000,0x14,nullptr);
//        QLOG_WARN()<<"****************Read Scanner**************";
    }
    else
    {
        if(monitorCount == 20)
        {
            monitorCount = 0;
            m_pTimerID = startTimer(300);
        }
        monitorCount++;
    }
}

void CentralWidget::slot_receive_message()
{
    m_mutex.lock();
    if(m_pUdpSocket == nullptr)
        return;
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    }
    m_mutex.unlock();
    emit signal_proc_plc_msg(datagram);
//    slot_proc_plc_msg(datagram);
}

void CentralWidget::slot_proc_plc_msg(const QByteArray data_array)
{
//    QLOG_WARN()<<"SCANNER REV PLC MSG:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
    if(data_array.length() > 10)//check the header length
    {
        if((data_array[10] == 0x01) && (data_array[11] == 0x01))
        {
            if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
            {
//                QLOG_INFO()<<"Rev Read Back Msg From PLC:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
                QByteArray tmpArray;
                for(int i=14;i<data_array.length();i++)
                    tmpArray.append(data_array[i]);
                emit signal_proc_final_data(tmpArray);
            }
            else
            {
                QLOG_WARN()<<"PLC read back message is ABNORMAL!";
            }
        }
        else if((data_array[10] == 0x01) && (data_array[11] == 0x02))
        {
            if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
            {
                m_pTimerID = startTimer(300);
//                QLOG_INFO()<<"Rev Write Back Msg From PLC:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
            }
            else
            {
                QLOG_WARN()<<"PLC write back message ABNORMAL!";
            }
        }
    }
    else
    {
        QLOG_WARN()<<"PLC message header lenght is WRONG!";
    }
}

void CentralWidget::slot_proc_final_data(const QByteArray data_array)
{
    if(data_array.length() == 20)
    {
        if(data_array.contains('/'))//new SN
        {
            if(data_array[19] == 0x01)
            {
                QByteArray tmpArray;
                for(int i=0;i<19;i++)
                    tmpArray.append(data_array[i]);
                QString productsn = tmpArray;
                if(productsn == "")
                {
                    m_pTimerID = startTimer(300);
                    return;
                }
                QString bdType = "";
                QString halfMaterialNumber = "";
                int countToExit = 0;
                while (1)
                {
                    if(get_board_type_and_half_number(bdType,halfMaterialNumber,productsn))
                        break;
                    else {
                        if(countToExit == 5)
                        {
                            m_pTimerID = startTimer(300);
                            return;
                        }
                        countToExit++;
                        QEventLoop eventloop;
                        QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
                        eventloop.exec();
                        continue;
                    }
                }

                QByteArray arrayToPLC;
                tmpArray.clear();
                for (int i=0;i<10;i++)
                    tmpArray.append(static_cast<char>(0x00));
                arrayToPLC.append(tmpArray);
                arrayToPLC.append(",");
                arrayToPLC.append(bdType.toLatin1());
                arrayToPLC.append(halfMaterialNumber.toLatin1());
                send_message_to_plc(WRITE_PLC,ADDRESS_D050,static_cast<short>(arrayToPLC.length()),arrayToPLC);
            }
            else
            {
                QLOG_INFO()<<"scanner request sign is zero";
                m_pTimerID = startTimer(300);
            }
        }
        else
        {//old SN
            //the sign of plc notify central control to request bdtype and half number from mes by the sn
//            QLOG_ERROR()<<"***********************old SN is:"<<data_array;
            if(data_array[10] == 0x01)
            {
                QByteArray tmpArray;
                for(int i=0;i<10;i++)
                    tmpArray.append(data_array[i]);
                QString productsn = tmpArray;
                if(productsn == "")
                {
                    m_pTimerID = startTimer(300);
                    return;
                }
                QString bdType = "";
                QString halfMaterialNumber = "";
                int countToExit = 0;
                while (1)
                {
                    if(get_board_type_and_half_number(bdType,halfMaterialNumber,productsn))
                        break;
                    else {
                        if(countToExit == 5)
                        {
                            m_pTimerID = startTimer(300);
                            return;
                        }
                        countToExit++;
                        QEventLoop eventloop;
                        QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
                        eventloop.exec();
                        continue;
                    }
                }

                QByteArray arrayToPLC;
                arrayToPLC.append(tmpArray);
                arrayToPLC.append(",");
                arrayToPLC.append(bdType.toLatin1());
                arrayToPLC.append(halfMaterialNumber.toLatin1());
                send_message_to_plc(WRITE_PLC,ADDRESS_D050,static_cast<short>(arrayToPLC.length()),arrayToPLC);
            }
            else
            {
                QLOG_INFO()<<"scanner request sign is zero";
                m_pTimerID = startTimer(300);
            }
        }
    }
    else
    {
        QLOG_WARN()<<"the msg of sanner reading from PLC is ABNROMAL";
    }
}

void CentralWidget::slot_create_tab_item(const int itemID,const QString barName)
{
    bool findSign = false;
    for(int i=0;i<tabWidget->tabBar()->count();i++)
    {
        if(tabWidget->tabText(i) != barName)
            continue;
        else
        {
            tabWidget->setCurrentIndex(i);
            findSign = true;
            return;
        }
    }

    if(itemID == 0)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_test_wgt(),barName);
    else if(itemID == 1)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_light_bar_wgt(),barName);
    else if(itemID == 2)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_plate_splitter(),barName);
    else if(itemID == 3)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_logs_widget(),barName);
    else if(itemID == 4)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_tbox_wgt(),barName);
    else if(itemID == 5)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_glue_dispense_wgt(),barName);
    else if(itemID == 6)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_line_overview_wgt(),barName);
    else if(itemID == 7)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_up_board_wgt(),barName);
    else if(itemID == 8)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_divide_board_wgt(),barName);
    else if(itemID == 9)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_test_station_wgt(),barName);
    else if(itemID == 10)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_aoi_station_wgt_tcon(),barName);
    else if(itemID == 11)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_package_station_wgt(),barName);
    else if(itemID == 12)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_main_board_bind_wgt(),barName);
    else if(itemID == 13)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_fpc_press_fit_wgt(),barName);
    else if(itemID == 14)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_data_acq_error_wgt(),barName);
    else if(itemID == 15)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_test_glue_dispense_wgt(),barName);
    else if(itemID == 16)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_screw_machine_wgt(),barName);
    else if(itemID == 17)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_test_divide_board_wgt(),barName);
    else if(itemID == 18)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_repair_station_wgt(),barName);
    else if(itemID == 19)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_three_in_one_glue_wgt(),barName);
    else if(itemID == 20)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_wave_soldering_wgt_M201(),barName);
    else if(itemID == 21)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_wave_soldering_wgt_M202(),barName);
    else if(itemID == 22)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_wave_soldering_wgt_M203(),barName);
    else if(itemID == 23)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_ccd_key_part_wgt(),barName);
    else if(itemID == 24)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_one_button_change_m201_wgt(),barName);
    else if(itemID == 25)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataFactory::get_red_flag_test_command_wgt(),barName);
    else if(itemID == 26)
        tabWidget->insertTab(tabWidget->currentIndex()+1,GDataAcquisitionFactory::get_insert_machine_errors_wgt(),barName);
}

void CentralWidget::slot_close_current_tab(int index)
{
    if(tabWidget->tabText(index) == u8"实时日志")
        GDataFactory::get_logs_widget()->close();
    else if(tabWidget->tabText(index) == u8"灯条擦拭")
        GDataFactory::get_light_bar_wgt()->close();
    else if(tabWidget->tabText(index) == u8"分板上下料")
        GDataFactory::get_plate_splitter()->close();
    else if(tabWidget->tabText(index) == u8"一键换产")
        GDataFactory::get_test_wgt()->close();
    else if(tabWidget->tabText(index) == u8"TBOX")
        GDataFactory::get_tbox_wgt()->close();
    else if(tabWidget->tabText(index) == u8"中框条码绑定")
        GDataFactory::get_glue_dispense_wgt()->close();
    else if(tabWidget->tabText(index) == u8"主板绑定")
        GDataFactory::get_main_board_bind_wgt()->close();
    else if(tabWidget->tabText(index) == u8"FPC压合")
        GDataFactory::get_fpc_press_fit_wgt()->close();
    else if(tabWidget->tabText(index) == u8"测试点胶")
        GDataFactory::get_test_glue_dispense_wgt()->close();
    else if(tabWidget->tabText(index) == u8"螺钉机")
        GDataFactory::get_screw_machine_wgt()->close();
    else if(tabWidget->tabText(index) == u8"点胶机")
        GDataFactory::get_three_in_one_glue_wgt()->close();
    else if(tabWidget->tabText(index) == u8"波峰焊(M201)")
        GDataFactory::get_wave_soldering_wgt_M201()->close();
    else if(tabWidget->tabText(index) == u8"波峰焊(M202)")
        GDataFactory::get_wave_soldering_wgt_M202()->close();
    else if(tabWidget->tabText(index) == u8"波峰焊(M203)")
        GDataFactory::get_wave_soldering_wgt_M203()->close();
    else if(tabWidget->tabText(index) == u8"Pogopin压合")
        GDataFactory::get_ccd_key_part_wgt()->close();
    else if(tabWidget->tabText(index) == u8"一键换产(M201)")
        GDataFactory::get_one_button_change_m201_wgt()->close();
    else if(tabWidget->tabText(index) == u8"命令测试")
        GDataFactory::get_red_flag_test_command_wgt()->close();

    tabWidget->removeTab(index);
}

void CentralWidget::send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    m_mutex.lock();
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_mutex.unlock();
}

QByteArray CentralWidget::make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //��ʼ��
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x0D));//PLC IP Address
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xC4));//self IP Address
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(0x82));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

bool CentralWidget::get_board_type_and_half_number(QString &bdType, QString &halfNum,QString productsn)
{
    QLOG_INFO()<<"the SN of SCANER request is:"<<productsn;
    //the SN is changed by shixiangkeji,process the SN
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
        ;
    //the SN is changed by shixiangkeji,process the SN
    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
    tmpurl.append(productsn);

    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest request;
    request.setUrl(QUrl(tmpurl));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("appId","CenterController");
    request.setRawHeader("publicKey",PUBLIC_KEY);

//    QNetworkAccessManager* tmpaccessManager = new QNetworkAccessManager();
    reply = GDataFactory::get_access_manager_plc()->get(request);
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    if(reply->isFinished())
    {
        QByteArray responseByte = reply->readAll();
        QLOG_INFO()<<"the response of MES is:" << QString(responseByte);
        if (reply->error() == QNetworkReply::NoError){

            QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);

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
                }
            }

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
                            QString tmpStr = icon["tpS_TEMPLATE"].toString();
                            for(int i=0;i<5;i++)
                                bdType.append(tmpStr[i]);
                            halfNum = icon["modeL_CODE"].toString();
                        }

                        disconnect(reply,&QNetworkReply::finished,&eventloop,&QEventLoop::quit);
                        reply->abort();
                        reply->deleteLater();
                        return true;
                    }
                    else {
                        QLOG_WARN()<<"the mes info dataInfo is not array";
                    }
                }
                else
                {
                    QLOG_WARN()<<"the data of MES exist ERROR!";
                }
            }
            else
            {
                QLOG_WARN()<<"MES status para is error!";
            }
        }
        else
        {
            QLOG_WARN()<<"MES reply exist ERROR! the error is:"<<reply->errorString();
        }
    }
    else
    {
        QLOG_WARN()<<"MES reply TIMEOUT!";
    }
    disconnect(reply,&QNetworkReply::finished,&eventloop,&QEventLoop::quit);
    reply->abort();
    reply->deleteLater();
    return false;
}
