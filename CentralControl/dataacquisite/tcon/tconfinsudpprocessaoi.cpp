#include "tconfinsudpprocessaoi.h"
#include "QsLog.h"
#include "gdatafactory.h"
#include <QSqlQuery>
TconFinsUDPProcessAOI* TconFinsUDPProcessAOI::m_pObj = nullptr;
void TconFinsUDPProcessAOI::insert_data_to_db()
{
    QString strSql = QString("select max(id) from public.%1").
            arg(constAOIDataAcquisition);

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
            arg(constAOIDataAcquisition).arg(id+1).        
            arg(m_pAOIDataTable[DATA_TYPE::Equipment_State]).
            arg(m_pAOIDataTable[DATA_TYPE::Key_Part_Action_Times]).
            arg(m_pAOIDataTable[DATA_TYPE::Ok_Ng_Quantity]).
            arg(m_pAOIDataTable[DATA_TYPE::Equipment_Warnings]).
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

void TconFinsUDPProcessAOI::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constAOIWarningsTable);
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

void TconFinsUDPProcessAOI::process_warnings_data(QByteArray data)
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
    for (int i=errorStr.length()-1;i>=11;i--) {//there are some addresses free,so subtract 11
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

    if(m_pAOIDataTable.contains(DATA_TYPE::Equipment_Warnings))
        m_pAOIDataTable[DATA_TYPE::Equipment_Warnings] = strInfo;
    else
        m_pAOIDataTable.insert(DATA_TYPE::Equipment_Warnings,strInfo);

    if(m_pAOIDataTable.contains(DATA_TYPE::Key_Part_Action_Times))
        m_pAOIDataTable[DATA_TYPE::Key_Part_Action_Times] = "";
    else
        m_pAOIDataTable.insert(DATA_TYPE::Key_Part_Action_Times,"");

    emit signal_warnings(lstInfo);
}

void TconFinsUDPProcessAOI::start_mq_service()
{
    if(m_pMqSerViceSrc == nullptr)
    {
        m_pMqSerViceSrc = new MQService;
        m_pMqSerViceSrc->MqStartService(true);
        m_pTimerID = startTimer(5000);
    }
}

void TconFinsUDPProcessAOI::stop_mq_service()
{
    if(m_pMqSerViceSrc != nullptr)
    {
        killTimer(m_pTimerID);
        m_pMqSerViceSrc->stopService();
    }
}

TconFinsUDPProcessAOI::TconFinsUDPProcessAOI()
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    m_pCurrentPassRate = "";
    load_data_from_db();
    m_pMqSerViceSrc = nullptr;
    connect(this,SIGNAL(signal_warnings(QList<QString>)),GDataFactory::get_aoi_station_wgt_tcon(),SLOT(slot_warnings(QList<QString>)));
    connect(this,SIGNAL(signal_ok_ng(QList<QString>,int)),GDataFactory::get_aoi_station_wgt_tcon(),SLOT(slot_ok_ng(QList<QString>,int)));
    connect(this,SIGNAL(signal_equipment_state(QByteArray)),GDataFactory::get_aoi_station_wgt_tcon(),SLOT(slot_equipment_state(QByteArray)));
}

void TconFinsUDPProcessAOI::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        if(m_pAOIDataTable.size() >= 4)
        {
            //insert to DB and upload to EAM
            QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_B");
            QString BoardType = GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE");
            QString productType = "";
            for(int i=0;i<5;i++)
                productType.append(BoardType[i]);
            productType.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B"));

            if(m_pAOIDataTable.contains(DATA_TYPE::Product_ProjectID))
                m_pAOIDataTable[DATA_TYPE::Product_ProjectID] = projID;
            else
                m_pAOIDataTable.insert(DATA_TYPE::Product_ProjectID,projID);

            if(m_pAOIDataTable.contains(DATA_TYPE::Product_Type))
                m_pAOIDataTable[DATA_TYPE::Product_Type] = productType;
            else
                m_pAOIDataTable.insert(DATA_TYPE::Product_Type,productType);

            m_pMqSerViceSrc->push(DEV_TYPE::TconAOIdev,m_pAOIDataTable);
            insert_data_to_db();
        }
    }
}

void TconFinsUDPProcessAOI::process_ok_ng_data(QByteArray data)
{
    data = GDataFactory::get_factory()->alter_order_of_byte_array(data);
    QList<QString> lstInfo;
    for (int i=0;i<data.length();i++) {
        if(((i+1)%4) == 0)
        {
            QByteArray ab;
            ab.push_back(data[i]);
            ab.push_back(data[i-1]);
            ab.push_back(data[i-2]);
            ab.push_back(data[i-3]);
            int values = ab.toHex().toInt(nullptr,16);
            lstInfo.push_back(QString::number(values));
            if(i == (data.length()-1))
            {
                if(lstInfo.length() != 6)
                {
                    QLOG_WARN()<<"the list length is not correct!";
                    return;
                }
                emit signal_ok_ng(lstInfo,values);
                //the total OKs
                int oks = lstInfo[0].toInt() + lstInfo[3].toInt();

                //the total NGs
                int ngs = lstInfo[1].toInt() + lstInfo[4].toInt();

                //the total quatity of left
                int total = lstInfo[2].toInt() + lstInfo[5].toInt();

                QString str_ok_ng_quantity="";
//                str_ok_ng_quantity.append(QString::number(total).append(',').append(QString::number(oks)).append(',').append(QString::number(ngs)));
                str_ok_ng_quantity.append(QString::number(total).append(',').append(QString::number(oks)).append(',').append(m_pCurrentPassRate));
                if(m_pAOIDataTable.contains(DATA_TYPE::Ok_Ng_Quantity))
                    m_pAOIDataTable[DATA_TYPE::Ok_Ng_Quantity] = str_ok_ng_quantity;
                else
                    m_pAOIDataTable.insert(DATA_TYPE::Ok_Ng_Quantity,str_ok_ng_quantity);
            }
        }
    }
}

static int m_pRequestBoard = 0;
void TconFinsUDPProcessAOI::process_equipment_state(QByteArray data)
{
    data = GDataFactory::get_factory()->alter_order_of_byte_array(data);
    emit signal_equipment_state(data);
    if(data[0] & 0x01)
    {
        QLOG_INFO()<<"the equipment is running";
        if(m_pAOIDataTable.contains(DATA_TYPE::Equipment_State))
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pAOIDataTable[DATA_TYPE::Equipment_State] = strState;
        }
        else
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pAOIDataTable.insert(DATA_TYPE::Equipment_State,strState);
        }

        if(data[0] & 0x04)
        {
            m_pRequestBoard++;
            if(m_pRequestBoard >= 3)
            {
                QString strState = "";
                strState.append(",");
                strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
                strState.append(",");
                strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
                strState.prepend("3");
                if(m_pAOIDataTable.contains(DATA_TYPE::Equipment_State))
                    m_pAOIDataTable[DATA_TYPE::Equipment_State] = strState;
                else
                    m_pAOIDataTable.insert(DATA_TYPE::Equipment_State,strState);
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
        if(m_pAOIDataTable.contains(DATA_TYPE::Equipment_State))
            m_pAOIDataTable[DATA_TYPE::Equipment_State] = strState;
        else
            m_pAOIDataTable.insert(DATA_TYPE::Equipment_State,strState);
    }
}

void TconFinsUDPProcessAOI::process_plc_data(QByteArray data)
{
    QLOG_INFO()<<"rev tcon aoi data aquisition:"<<data;
    if(data.length() == 0x06)//warnings
    {
        process_warnings_data(data);
    }
    else if(data.length() == 0x18)//Ok_Ng_Quantity
    {
        process_ok_ng_data(data);
    }
    else if(data.length() == 0x02)//equipment state
    {
        process_equipment_state(data);
    }
}
