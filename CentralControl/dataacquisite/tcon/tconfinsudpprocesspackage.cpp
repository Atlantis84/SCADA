#include "tconfinsudpprocesspackage.h"
#include "QsLog.h"
#include "gdatafactory.h"
#include <QSqlQuery>
TconFinsUDPProcessPackage* TconFinsUDPProcessPackage::m_pObj = nullptr;
void TconFinsUDPProcessPackage::insert_data_to_db()
{
    QString strSql = QString("select max(id) from public.%1").
            arg(constPacketDataAcquisition);

    int id = 0;
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql_tcon_data_acquisition()->GetQueryResultDataAcquisition(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }

    strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7')").
            arg(constPacketDataAcquisition).arg(id+1).
            arg(m_pTconPackageDataTable[DATA_TYPE::Equipment_State]).
            arg(m_pTconPackageDataTable[DATA_TYPE::Key_Part_Action_Times]).
            arg(m_pTconPackageDataTable[DATA_TYPE::Ok_Ng_Quantity]).
            arg(m_pTconPackageDataTable[DATA_TYPE::Equipment_Warnings]).
            arg(QDateTime::currentDateTime().toString(fmtDateTime));
    QString strError;
    if(GDataFactory::get_pgsql_tcon_data_acquisition()->ExecSqlDataAcquisition(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!";
    }
}

void TconFinsUDPProcessPackage::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constPackageWarningsTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql_tcon_data_acquisition()->GetQueryResultDataAcquisition(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get warning info, query database success!";
        while(queryResult.next())
        {
            QString strInfo="";
            strInfo.append(queryResult.value(1).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(2).toString());
            strInfo.append(',');
            strInfo.append(queryResult.value(3).toString());
            m_pMapWarningsInfoOfDB.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}

void TconFinsUDPProcessPackage::process_warnings_data(QByteArray data)
{
    QString errorStr = "";
    int j = 0;
    bool ok;
    for (int i=0;i<3;i++) {
        QByteArray ab;
        ab.append(data[j]);
        ab.append(data[j+1]);
        ushort values = ab.toHex().toUShort(&ok, 16);
        errorStr.prepend(QString::number(values,2).rightJustified(16, '0'));
        j+=2;
    }

    int warningID = 0;
    QList<QString> lstInfo;
    for (int i=errorStr.length()-1;i>=8;i--) {//there are some addresses free,so subtract 8
        warningID++;
        if(errorStr[i] == '1')
            lstInfo.push_back(m_pMapWarningsInfoOfDB[warningID]);
    }
    QString strInfo = "";
    for (int i=0;i<lstInfo.length();i++)
    {
        if(i == lstInfo.length()-1)
            strInfo.append(lstInfo[i]);
        else
            strInfo.append(lstInfo[i]+",");
    }

    if(m_pTconPackageDataTable.contains(DATA_TYPE::Equipment_Warnings))
        m_pTconPackageDataTable[DATA_TYPE::Equipment_Warnings] = strInfo;
    else
        m_pTconPackageDataTable.insert(DATA_TYPE::Equipment_Warnings,strInfo);

    if(m_pTconPackageDataTable.contains(DATA_TYPE::Ok_Ng_Quantity))
        m_pTconPackageDataTable[DATA_TYPE::Ok_Ng_Quantity] = "";
    else
        m_pTconPackageDataTable.insert(DATA_TYPE::Ok_Ng_Quantity,"");

    if(m_pTconPackageDataTable.contains(DATA_TYPE::Key_Part_Action_Times))
        m_pTconPackageDataTable[DATA_TYPE::Key_Part_Action_Times] = "";
    else
        m_pTconPackageDataTable.insert(DATA_TYPE::Key_Part_Action_Times,"");

    emit signal_warnings(lstInfo);
}

static int m_pRequestBoard = 0;
void TconFinsUDPProcessPackage::process_equipment_state(QByteArray data)
{
    data = GDataFactory::get_factory()->alter_order_of_byte_array(data);
    emit signal_equipment_state(data);
    if(data[0] & 0x01)
    {
        QLOG_INFO()<<"the equipment is running";
        if(m_pTconPackageDataTable.contains(DATA_TYPE::Equipment_State))
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pTconPackageDataTable[DATA_TYPE::Equipment_State] = strState;
        }
        else
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pTconPackageDataTable.insert(DATA_TYPE::Equipment_State,strState);
        }

        if(data[0] & 0x04)
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("3");
            m_pRequestBoard++;
            if(m_pRequestBoard >= 3)
            {
                if(m_pTconPackageDataTable.contains(DATA_TYPE::Equipment_State))
                    m_pTconPackageDataTable[DATA_TYPE::Equipment_State] = strState;
                else
                    m_pTconPackageDataTable.insert(DATA_TYPE::Equipment_State,strState);
            }
        }
        else
            m_pRequestBoard = 0;
    }
    else
    {
        QString strState = "";
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("1");
        if(m_pTconPackageDataTable.contains(DATA_TYPE::Equipment_State))
            m_pTconPackageDataTable[DATA_TYPE::Equipment_State] = strState;
        else
            m_pTconPackageDataTable.insert(DATA_TYPE::Equipment_State,strState);
    }
}

TconFinsUDPProcessPackage::TconFinsUDPProcessPackage()
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    load_data_from_db();
    m_pMqSerViceSrc = nullptr;
    connect(this,SIGNAL(signal_warnings(QList<QString>)),GDataFactory::get_package_station_wgt(),SLOT(slot_warnings(QList<QString>)));
    connect(this,SIGNAL(signal_equipment_state(QByteArray)),GDataFactory::get_package_station_wgt(),SLOT(slot_equipment_state(QByteArray)));
}

void TconFinsUDPProcessPackage::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        if(m_pTconPackageDataTable.size() == 4)
        {
            //insert to DB and upload to EAM
            m_pMqSerViceSrc->push(DEV_TYPE::TconPackeageDev,m_pTconPackageDataTable);
            insert_data_to_db();
        }
    }
}

void TconFinsUDPProcessPackage::stop_mq_service()
{
    if(m_pMqSerViceSrc != nullptr)
    {
        killTimer(m_pTimerID);
        m_pMqSerViceSrc->stopService();
    }
}

void TconFinsUDPProcessPackage::start_mq_service()
{
    if(m_pMqSerViceSrc == nullptr)
    {
        m_pMqSerViceSrc = new MQService;
        m_pMqSerViceSrc->MqStartService(true);
        m_pTimerID = startTimer(5000);
    }
}

void TconFinsUDPProcessPackage::process_plc_data(QByteArray data)
{
    QLOG_INFO()<<"rev tcon package data aquisiton:"<<data;
    if(data.length() == 0x0A)
    {
        process_warnings_data(data);
    }
    else if(data.length() == 0x02)
    {
        process_equipment_state(data);
    }
}
