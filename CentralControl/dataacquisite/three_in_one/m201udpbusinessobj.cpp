#include "m201udpbusinessobj.h"
#include "QsLog.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "gdataacquisitionfactory.h"
#include "gdatafactory.h"
#include <QtEndian>
#include <QTextCodec>
M201UdpBusinessObj* M201UdpBusinessObj::m_pInstance = nullptr;

M201UdpBusinessObj::M201UdpBusinessObj(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    connect(this,SIGNAL(signal_equipment_error_glue(const int)),GDataFactory::get_three_in_one_glue_wgt(),SLOT(slot_equipment_error_glue(const int)));
    connect(this,SIGNAL(signal_equipment_output_glue(const int)),GDataFactory::get_three_in_one_glue_wgt(),SLOT(slot_equipment_output_glue(const int)));
    connect(this,SIGNAL(signal_equipment_state_glue(const int)),GDataFactory::get_three_in_one_glue_wgt(),SLOT(slot_equipment_state_glue(const int)));

    connect(this,SIGNAL(signal_equipment_error_devide(const int)),GDataFactory::get_test_divide_board_wgt(),SLOT(slot_equipment_error_devide(const int)));
    connect(this,SIGNAL(signal_equipment_output_devide(const int)),GDataFactory::get_test_divide_board_wgt(),SLOT(slot_equipment_output_devide(const int)));
    connect(this,SIGNAL(signal_equipment_state_devide(const int)),GDataFactory::get_test_divide_board_wgt(),SLOT(slot_equipment_state_devide(const int)));
    connect(this,SIGNAL(signal_equipment_paras_devide(const QList<QString>)),GDataFactory::get_test_divide_board_wgt(),SLOT(slot_equipment_paras_devide(const QList<QString>)));

    connect(this,SIGNAL(signal_notify_result_of_change_production_for_divide_glue(const int)),GDataFactory::get_one_button_change_m201_wgt(),SLOT(slot_rev_result_of_change_production_for_divide_glue(const int)));
}

void M201UdpBusinessObj::slot_receive_udp_message_from_client()
{
    m_mutex.lock();
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    }
    m_mutex.unlock();
    emit signal_proc_client_info(datagram);
}

void M201UdpBusinessObj::slot_proc_client_info(const QByteArray &data_array)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QString tmpData = codec->toUnicode(data_array);
    QString tmpStr = data_array;
    QLOG_WARN()<<"rev info from glue or divide,the info is:"<<tmpData;
    int index = tmpStr.indexOf(QChar::Null);
    if(-1 != index)tmpStr.truncate(index);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(tmpStr.toUtf8(),&error);
    QJsonObject jsonObject = jsonDocument.object();

    if(jsonObject.contains("equipmentType"))
    {
        QJsonValue instructionValue = jsonObject.value("equipmentType");
        if(instructionValue.toString() == "TV_DEVIDE")
        {
            QLOG_INFO()<<"rev info from Divide Machine";

            if(jsonObject.contains("responseCode"))//change production of DIVIDE-MACHINE
            {
                QLOG_INFO()<<"DIVIDE change production SUCCESS";
                emit signal_notify_result_of_change_production_for_divide_glue(1);
            }
            else
            {
                //machine error
                if(jsonObject.contains("errorCode"))
                {
                    QJsonValue arrayValue = jsonObject.value(QStringLiteral("errorCode"));
                    if(arrayValue.isArray())
                    {
                        QJsonArray array = arrayValue.toArray();
                        for (int i=0;i<array.size();i++)
                        {
                            QString tmpError = array.at(i).toString();
                            emit signal_equipment_error_devide(tmpError.toInt());
                        }
                    }
                }
//                emit signal_equipment_error_devide(1);
                //product quantity
                if(jsonObject.contains("outputQuantity"))
                {
                    QJsonValue instructionValue = jsonObject.value("outputQuantity");
                    QString tmpquantity = instructionValue.toString();
                    int q = tmpquantity.toInt();
                    emit signal_equipment_output_devide(q);
                }

                //keyPartActionTimes
                QJsonValue arrayValue = jsonObject.value(QStringLiteral("keyPartActionTimes"));
                if(arrayValue.isArray())
                {
                    QJsonArray array = arrayValue.toArray();
                    QList<QString> lstInfo;
                    for (int i=0;i<array.size();i++)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();
                        for(const QString &key : icon.keys())
                            lstInfo.push_back(icon[key].toString());
                    }
                    emit signal_equipment_paras_devide(lstInfo);
                }

                //status
                if(jsonObject.contains("equimentStatus"))
                {
                    QJsonValue instructionValue = jsonObject.value("equimentStatus");
                    QString tmpStatus = instructionValue.toString();
                    emit signal_equipment_state_devide(tmpStatus.toInt());
                }
            }
        }
        else if(instructionValue.toString() == "TV_GLUE")
        {
            QLOG_INFO()<<"rev info from Glue Machine";

            if(jsonObject.contains("responseCode"))//change production of GLUE-MACHINE
            {
                QLOG_INFO()<<"GLUE change production SUCCESS";
                emit signal_notify_result_of_change_production_for_divide_glue(2);
            }
            else
            {
                //response-info to client
                QJsonObject  root;
                root.insert("responseCode","OK");
                root.insert("responseInfo",u8"成功");
                QJsonDocument doc_data(root);
                QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
                QHostAddress hAddr(GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_GLUE_IP"));
                int pt = GDataAcquisitionFactory::get_data_acquisition_config()->get_config_para("THREE_IN_ONE_GLUE_PORT").toInt();
                qint64 size = 0;
                size = m_pUdpSocketToClient->writeDatagram(request_data,request_data.length(),hAddr,static_cast<quint16>(pt));
                //response-info to client

                //machine error
                if(jsonObject.contains("errorCode"))
                {
                    QJsonValue arrayValue = jsonObject.value(QStringLiteral("errorCode"));
                    if(arrayValue.isArray())
                    {
                        QJsonArray array = arrayValue.toArray();
                        for (int i=0;i<array.size();i++)
                        {
                            QString tmpError = array.at(i).toString();
                            emit signal_equipment_error_glue(tmpError.toInt());
                        }
                    }
                }

//                emit signal_equipment_error_glue(1);

                if(jsonObject.contains("keyPartActionTimes"))
                {
                    QJsonValue objvalue = jsonObject.value(QStringLiteral("keyPartActionTimes"));
                    if(objvalue.isObject())
                    {
                        QJsonObject obj = objvalue.toObject();
                        QString stroutp = obj.value("Key_Part_Name_A").toString();
                        int outp = stroutp.toInt();
                        emit signal_equipment_output_glue(outp);

                    }
                }

                //status
                if(jsonObject.contains("equipmentStatus"))
                {
                    QJsonValue instructionValue = jsonObject.value("equipmentStatus");
                    QString tmpStatus = instructionValue.toString();
                    emit signal_equipment_state_glue(tmpStatus.toInt());
                }
            }
        }
    }
}

void M201UdpBusinessObj::init_udp_server(const quint16 port)
{
    m_pUdpSocket = new QUdpSocket(this);
    m_pUdpSocketToClient = new QUdpSocket(this);
    if(m_pUdpSocket->bind(QHostAddress::Any,port))
        QLOG_INFO()<<"init udp server SUCCESS!";
    else
    {
        QLOG_WARN()<<"init udp server FAILED!";
        return;
    }

    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_udp_message_from_client()));
    connect(this,SIGNAL(signal_proc_client_info(const QByteArray&)),this,SLOT(slot_proc_client_info(const QByteArray&)));
}

void M201UdpBusinessObj::change_production_of_divide_machine(const QString ip,const quint16 port,const QString boardType,const QString machineType)
{
    QJsonObject  root;
    root.insert("equipmentType","TV_DIVIDE");
    root.insert("boardType",boardType);
    root.insert("machineType",machineType);
    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    QHostAddress hAddr(ip);
    qint64 size = 0;
    size = m_pUdpSocketToClient->writeDatagram(request_data,request_data.length(),hAddr,port);
}

void M201UdpBusinessObj::change_production_of_glue_machine(const QString ip, const quint16 port, const QString boardType, const QString machineType)
{
    QJsonObject  root;
    root.insert("equipmentType","TV_GLUE");
    root.insert("boardType",boardType);
    root.insert("machineType",machineType);
    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    QHostAddress hAddr(ip);
    qint64 size = 0;
    size = m_pUdpSocketToClient->writeDatagram(request_data,request_data.length(),hAddr,port);
}

void M201UdpBusinessObj::change_production_of_vision(const QString ip, const quint16 port,const int taskid)
{
    QString strid = "";

    strid = QString::number(taskid);
    std::string stdstr = strid.toStdString();

    qint64 size = 0;
    QHostAddress hAddr(ip);
    size = m_pUdpSocketToClient->writeDatagram(stdstr.c_str(),1,hAddr,port);
}
