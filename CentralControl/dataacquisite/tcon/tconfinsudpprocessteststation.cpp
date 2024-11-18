#include "tconfinsudpprocessteststation.h"
#include "QsLog.h"
#include "gdatafactory.h"
#include <QSqlQuery>
#include <QtEndian>
#include <QEventLoop>
TconFinsUDPProcessTestStation* TconFinsUDPProcessTestStation::m_pObj = nullptr;

TconFinsUDPProcessTestStation::TconFinsUDPProcessTestStation()
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    load_data_from_db();
    m_pMqSerViceSrcA = nullptr;
    m_pMqSerViceSrcB = nullptr;
    m_pTimerID = startTimer(5000);//start timer in 5s to upload data to DB and EAM
    connect(this,SIGNAL(signal_warnings(QList<QString>)),GDataFactory::get_test_station_wgt(),SLOT(slot_warnings(QList<QString>)));
    connect(this,SIGNAL(signal_ok_ng(QList<QString>,int)),GDataFactory::get_test_station_wgt(),SLOT(slot_ok_ng(QList<QString>,int)));
    connect(this,SIGNAL(signal_equipment_state(QByteArray)),GDataFactory::get_test_station_wgt(),SLOT(slot_equipment_state(QByteArray)));
}

void TconFinsUDPProcessTestStation::start_mq_service()
{
    if((m_pMqSerViceSrcA == nullptr)&&(m_pMqSerViceSrcB == nullptr))
    {
        m_pMqSerViceSrcA = new MQService;
        m_pMqSerViceSrcA->MqStartService(true);
        m_pMqSerViceSrcB = new MQService;
        m_pMqSerViceSrcB->MqStartService(true);
        m_pTimerID = startTimer(5000);
    }
}

void TconFinsUDPProcessTestStation::stop_mq_service()
{
    if((m_pMqSerViceSrcA != nullptr) && (m_pMqSerViceSrcB != nullptr))
    {
        killTimer(m_pTimerID);
        m_pMqSerViceSrcA->stopService();
        m_pMqSerViceSrcB->stopService();
    }
}

void TconFinsUDPProcessTestStation::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        //insert to DB and upload to EAM
        insert_data_to_db();
    }
}

void TconFinsUDPProcessTestStation::process_ok_ng_data(QByteArray data)
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
                emit signal_ok_ng(lstInfo,values);

                if(values == 31)//station1
                {
                    QString str_ok_ng_ratio = "";
                    float work_device_1,work_device_2,work_device_3,work_device_4,work_device_5;
                    work_device_1=work_device_2=work_device_3=work_device_4=work_device_5=0.0;
                    work_device_1 = lstInfo[0].toFloat()/(lstInfo[0].toFloat()+lstInfo[1].toFloat());
                    work_device_2 = lstInfo[4].toFloat()/(lstInfo[4].toFloat()+lstInfo[5].toFloat());
                    work_device_3 = lstInfo[8].toFloat()/(lstInfo[8].toFloat()+lstInfo[9].toFloat());
                    work_device_4 = lstInfo[12].toFloat()/(lstInfo[12].toFloat()+lstInfo[13].toFloat());
                    work_device_5 = lstInfo[16].toFloat()/(lstInfo[16].toFloat()+lstInfo[17].toFloat());

                    str_ok_ng_ratio.append(lstInfo[0]).append(',').append(lstInfo[1]).append(',').append(QString::number(work_device_1)).append(',').
                            append(lstInfo[4]).append(',').append(lstInfo[5]).append(',').append(QString::number(work_device_2)).append(',').
                            append(lstInfo[8]).append(',').append(lstInfo[9]).append(',').append(QString::number(work_device_3)).append(',').
                            append(lstInfo[12]).append(',').append(lstInfo[13]).append(',').append(QString::number(work_device_4)).append(',').
                            append(lstInfo[16]).append(',').append(lstInfo[17]).append(',').append(QString::number(work_device_5));
                    if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Work_Device_Ok_Ng_Ratio))
                        m_pTconTestStationDataTableA[DATA_TYPE::Work_Device_Ok_Ng_Ratio] = str_ok_ng_ratio;
                    else
                        m_pTconTestStationDataTableA.insert(DATA_TYPE::Work_Device_Ok_Ng_Ratio,str_ok_ng_ratio);

                    //the total OKs of station1
                    int oks1 = lstInfo[0].toInt() + lstInfo[4].toInt() + lstInfo[8].toInt() +
                            lstInfo[12].toInt() + lstInfo[16].toInt();                    

                    //the total NGs of station1
                    int ngs1 = lstInfo[1].toInt() + lstInfo[5].toInt() + lstInfo[9].toInt() +
                            lstInfo[13].toInt() + lstInfo[17].toInt();

                    //the total quatity of station1
                    int total = lstInfo[20].toInt();

                    QString str_ok_ng_quantity="";
                    str_ok_ng_quantity.append(QString::number(oks1)).append(',').append(QString::number(ngs1)).append(',').append(QString::number(total));

                    if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Ok_Ng_Quantity))
                        m_pTconTestStationDataTableA[DATA_TYPE::Ok_Ng_Quantity] = str_ok_ng_quantity;
                    else
                        m_pTconTestStationDataTableA.insert(DATA_TYPE::Ok_Ng_Quantity,str_ok_ng_quantity);

                    QString key_part_action_times="";
                    key_part_action_times.append(lstInfo[3]).append(',').
                            append(lstInfo[7]).append(',').
                            append(lstInfo[11]).append(',').
                            append(lstInfo[15]).append(',').
                            append(lstInfo[19]);
                    if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Key_Part_Action_Times))
                        m_pTconTestStationDataTableA[DATA_TYPE::Key_Part_Action_Times] = key_part_action_times;
                    else
                        m_pTconTestStationDataTableA.insert(DATA_TYPE::Key_Part_Action_Times,key_part_action_times);
                }
                else if(values == 32)
                {
                    QString str_ok_ng_ratio = "";
                    float work_device_1,work_device_2,work_device_3,work_device_4,work_device_5;
                    work_device_1=work_device_2=work_device_3=work_device_4=work_device_5=0.0;
                    work_device_1 = lstInfo[0].toFloat()/(lstInfo[0].toFloat()+lstInfo[1].toFloat());
                    work_device_2 = lstInfo[4].toFloat()/(lstInfo[4].toFloat()+lstInfo[5].toFloat());
                    work_device_3 = lstInfo[8].toFloat()/(lstInfo[8].toFloat()+lstInfo[9].toFloat());
                    work_device_4 = lstInfo[12].toFloat()/(lstInfo[12].toFloat()+lstInfo[13].toFloat());
                    work_device_5 = lstInfo[16].toFloat()/(lstInfo[16].toFloat()+lstInfo[17].toFloat());

                    str_ok_ng_ratio.append(lstInfo[0]).append(',').append(lstInfo[1]).append(',').append(QString::number(work_device_1)).append(',').
                            append(lstInfo[4]).append(',').append(lstInfo[5]).append(',').append(QString::number(work_device_2)).append(',').
                            append(lstInfo[8]).append(',').append(lstInfo[9]).append(',').append(QString::number(work_device_3)).append(',').
                            append(lstInfo[12]).append(',').append(lstInfo[13]).append(',').append(QString::number(work_device_4)).append(',').
                            append(lstInfo[16]).append(',').append(lstInfo[17]).append(',').append(QString::number(work_device_5));
                    if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Work_Device_Ok_Ng_Ratio))
                        m_pTconTestStationDataTableB[DATA_TYPE::Work_Device_Ok_Ng_Ratio] = str_ok_ng_ratio;
                    else
                        m_pTconTestStationDataTableB.insert(DATA_TYPE::Work_Device_Ok_Ng_Ratio,str_ok_ng_ratio);

                    //the total OKs of station2
                    int oks2 = lstInfo[0].toInt() + lstInfo[4].toInt() + lstInfo[8].toInt() +
                            lstInfo[12].toInt() + lstInfo[16].toInt();

                    //the total NGs of station2
                    int ngs2 = lstInfo[1].toInt() + lstInfo[5].toInt() + lstInfo[9].toInt() +
                            lstInfo[13].toInt() + lstInfo[17].toInt();

                    //the total quatity of station2
                    int total = lstInfo[20].toInt();

                    QString str_ok_ng_quantity;
                    str_ok_ng_quantity.append(QString::number(oks2)).append(',').append(QString::number(ngs2)).append(',').append(QString::number(total));
                    if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Ok_Ng_Quantity))
                        m_pTconTestStationDataTableB[DATA_TYPE::Ok_Ng_Quantity] = str_ok_ng_quantity;
                    else
                        m_pTconTestStationDataTableB.insert(DATA_TYPE::Ok_Ng_Quantity,str_ok_ng_quantity);

                    QString key_part_action_times="";
                    key_part_action_times.append(lstInfo[3]).append(',').
                            append(lstInfo[7]).append(',').
                            append(lstInfo[11]).append(',').
                            append(lstInfo[15]).append(',').
                            append(lstInfo[19]);
                    if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Key_Part_Action_Times))
                        m_pTconTestStationDataTableB[DATA_TYPE::Key_Part_Action_Times] = key_part_action_times;
                    else
                        m_pTconTestStationDataTableB.insert(DATA_TYPE::Key_Part_Action_Times,key_part_action_times);
                }
            }
        }
    }
}

static int m_pRequestBoard = 0;
static int m_pRequestBoardB = 0;
void TconFinsUDPProcessTestStation::process_equipment_state(QByteArray data)
{
    data = GDataFactory::get_factory()->alter_order_of_byte_array(data);
    emit signal_equipment_state(data);
    if(data[0] & 0x01)
    {
        QLOG_INFO()<<u8"the station1 is running";
        if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Equipment_State))
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pTconTestStationDataTableA[DATA_TYPE::Equipment_State] = strState;
        }
        else
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pTconTestStationDataTableA.insert(DATA_TYPE::Equipment_State,strState);
        }

        if(data[0] & 0x40)
        {
            m_pRequestBoard++;
            if(m_pRequestBoard >=3)
            {
                QString strState = "";
                strState.append(",");
                strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
                strState.append(",");
                strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
                strState.prepend("3");
                if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Equipment_State))
                    m_pTconTestStationDataTableA[DATA_TYPE::Equipment_State] = strState;
                else
                    m_pTconTestStationDataTableA.insert(DATA_TYPE::Equipment_State,strState);
            }
        }
        else
            m_pRequestBoard = 0;
    }
    if(data[0] & 0x02)
    {
        QLOG_INFO()<<"the station1 is pausing";
        QString strState = "";
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("3");
        if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Equipment_State))
            m_pTconTestStationDataTableA[DATA_TYPE::Equipment_State] = strState;
        else
            m_pTconTestStationDataTableA.insert(DATA_TYPE::Equipment_State,strState);
    }
    if(data[0] & 0x04)
    {
        QLOG_INFO()<<"the station1 is stopping";
        QString strState = "";
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("1");
        if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Equipment_State))
            m_pTconTestStationDataTableA[DATA_TYPE::Equipment_State] = strState;
        else
            m_pTconTestStationDataTableA.insert(DATA_TYPE::Equipment_State,strState);
    }

    if(data[0] & 0x08)
    {
        QLOG_INFO()<<"the station2 is running";
        if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Equipment_State))
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pTconTestStationDataTableB[DATA_TYPE::Equipment_State] = strState;
        }
        else
        {
            QString strState = "";
            strState.append(",");
            strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
            strState.append(",");
            strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
            strState.prepend("2");
            m_pTconTestStationDataTableB.insert(DATA_TYPE::Equipment_State,strState);
        }

        if(data[0] & 0x80)
        {
            m_pRequestBoardB++;
            if(m_pRequestBoardB >=3)
            {
                QString strState = "";
                strState.append(",");
                strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
                strState.append(",");
                strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
                strState.prepend("3");
                if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Equipment_State))
                    m_pTconTestStationDataTableB[DATA_TYPE::Equipment_State] = strState;
                else
                    m_pTconTestStationDataTableB.insert(DATA_TYPE::Equipment_State,strState);
            }
        }
        else
            m_pRequestBoardB = 0;
    }
    if(data[0] & 0x10)
    {
        QLOG_INFO()<<"the station2 is pausing";
        QString strState = "";
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("3");
        if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Equipment_State))
            m_pTconTestStationDataTableB[DATA_TYPE::Equipment_State] = strState;
        else
            m_pTconTestStationDataTableB.insert(DATA_TYPE::Equipment_State,strState);
    }
    if(data[0] & 0x20)
    {
        QLOG_INFO()<<"the station2 is stopping";
        QString strState = "";
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("1");
        if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Equipment_State))
            m_pTconTestStationDataTableB[DATA_TYPE::Equipment_State] = strState;
        else
            m_pTconTestStationDataTableB.insert(DATA_TYPE::Equipment_State,strState);
    }

}

void TconFinsUDPProcessTestStation::process_warnings_data(QByteArray data)
{
    if(data.length() == 0x10)
    {
        QString errorStr = "";
        int j = 0;
        bool ok;
        for (int i=0;i<8;i++) {
            QByteArray ab;
            ab.append(data[j]);
            ab.append(data[j+1]);
            ushort values = ab.toHex().toUShort(&ok, 16);
            errorStr.prepend(QString::number(values,2).rightJustified(16, '0'));
            j+=2;
        }

        int warningID = 0;
        QList<QString> lstInfo;
        for (int i=errorStr.length()-1;i>=10;i--) {//there are some addresses free,so subtract 11
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

        if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Equipment_Warnings))
            m_pTconTestStationDataTableA[DATA_TYPE::Equipment_Warnings] = strInfo;
        else
            m_pTconTestStationDataTableA.insert(DATA_TYPE::Equipment_Warnings,strInfo);

        emit signal_warnings(lstInfo);
    }
    else
    {
        QString errorStr = "";
        int j = 0;
        bool ok;
        for (int i=0;i<9;i++) {
            QByteArray ab;
            ab.append(data[j]);
            ab.append(data[j+1]);
            ushort values = ab.toHex().toUShort(&ok, 16);
            errorStr.prepend(QString::number(values,2).rightJustified(16, '0'));
            j+=2;
        }

        int warningID = 118;
        QList<QString> lstInfo;
        for (int i=errorStr.length()-1;i>=10;i--) {//there are some addresses free,so subtract 11
            warningID++;
            if(errorStr[i] == '1')
            {
                lstInfo.push_back(m_pMapWarningsInfoOfDB[warningID]);
            }
        }
        QString strInfo = "";
        for (int i=0;i<lstInfo.length();i++)
        {
            if(i == lstInfo.length()-1)
                strInfo.append(lstInfo[i]);
            else
                strInfo.append(lstInfo[i]+",");
        }

        if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Equipment_Warnings))
            m_pTconTestStationDataTableB[DATA_TYPE::Equipment_Warnings] = strInfo;
        else
            m_pTconTestStationDataTableB.insert(DATA_TYPE::Equipment_Warnings,strInfo);

        emit signal_warnings(lstInfo);
    }
}

void TconFinsUDPProcessTestStation::insert_data_to_db()
{

    if(m_pTconTestStationDataTableA.size() >= 4)
    {
        QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_A");
        QString BoardType = GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE");
        QString productType = "";
        for(int i=0;i<5;i++)
            productType.append(BoardType[i]);
        productType.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_A"));

        if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Product_ProjectID))
            m_pTconTestStationDataTableA[DATA_TYPE::Product_ProjectID] = projID;
        else
            m_pTconTestStationDataTableA.insert(DATA_TYPE::Product_ProjectID,projID);

        if(m_pTconTestStationDataTableA.contains(DATA_TYPE::Product_Type))
            m_pTconTestStationDataTableA[DATA_TYPE::Product_Type] = productType;
        else
            m_pTconTestStationDataTableA.insert(DATA_TYPE::Product_Type,productType);

//        QLOG_WARN()<<"Push A Info to MQ";
        m_pMqSerViceSrcA->push(DEV_TYPE::TconTestDevA, m_pTconTestStationDataTableA);
        QString strSql = QString("select max(id) from public.%1").
                arg(constTestAStationDataAcquisition);

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
                arg(constTestAStationDataAcquisition).arg(id+1).
                arg(m_pTconTestStationDataTableA[DATA_TYPE::Equipment_State]).
                arg(m_pTconTestStationDataTableA[DATA_TYPE::Key_Part_Action_Times]).
                arg(m_pTconTestStationDataTableA[DATA_TYPE::Ok_Ng_Quantity]).
                arg(m_pTconTestStationDataTableA[DATA_TYPE::Equipment_Warnings]).
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

//    QEventLoop eventloop;
//    QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
//    eventloop.exec();

    if(m_pTconTestStationDataTableB.size() >= 4)
    {
        QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_B");
        QString BoardType = GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE");
        QString productType = "";
        for(int i=0;i<5;i++)
            productType.append(BoardType[i]);
        productType.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B"));

        if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Product_ProjectID))
            m_pTconTestStationDataTableB[DATA_TYPE::Product_ProjectID] = projID;
        else
            m_pTconTestStationDataTableB.insert(DATA_TYPE::Product_ProjectID,projID);

        if(m_pTconTestStationDataTableB.contains(DATA_TYPE::Product_Type))
            m_pTconTestStationDataTableB[DATA_TYPE::Product_Type] = productType;
        else
            m_pTconTestStationDataTableB.insert(DATA_TYPE::Product_Type,productType);

//        QLOG_WARN()<<"Push B Info to MQ";
        m_pMqSerViceSrcB->push(DEV_TYPE::TconTestDevB, m_pTconTestStationDataTableB);
        QString strSql = QString("select max(id) from public.%1").
                arg(constTestBStationDataAcquisition);

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
                arg(constTestBStationDataAcquisition).arg(id+1).
                arg(m_pTconTestStationDataTableB[DATA_TYPE::Equipment_State]).
                arg(m_pTconTestStationDataTableB[DATA_TYPE::Key_Part_Action_Times]).
                arg(m_pTconTestStationDataTableB[DATA_TYPE::Ok_Ng_Quantity]).
                arg(m_pTconTestStationDataTableB[DATA_TYPE::Equipment_Warnings]).
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
}

void TconFinsUDPProcessTestStation::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constTestStationWarningsTable);
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

void TconFinsUDPProcessTestStation::process_plc_data(QByteArray data)
{
    QLOG_INFO()<<"rev tcon test station data aquisition:"<<GDataFactory::get_factory()->bytes_to_str(data);
    if((data.length() == 0x10) || (data.length() == 0x12))
    {
        process_warnings_data(data);
    }
    else if(data.length() == 0x5C)//OKs_NGs
    {
        process_ok_ng_data(data);
    }
    else if(data.length() == 0x02)//the state of equipment
    {
        process_equipment_state(data);
    }
}
