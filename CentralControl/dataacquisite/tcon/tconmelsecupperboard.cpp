#include "tconmelsecupperboard.h"
#include <QEventLoop>
#include <QSqlQuery>
#include <QsLog.h>
#include <gdatafactory.h>

tconMelsecUpperBoard* tconMelsecUpperBoard::m_pObj = nullptr;

tconMelsecUpperBoard::tconMelsecUpperBoard(int devNo)
{
    m_nDevNo = devNo;
    qRegisterMetaType<QList<QString>>("QList<QString>");
    qRegisterMetaType<QByteArray>("QByteArray");
    if(devNo == TCON_UP_DEV_NO_A){
        dev_name = "UpperBoardDeviceA";
    }
    else if(devNo == TCON_UP_DEV_NO_B){
        dev_name = "UpperBoardDeviceB";
    }

    m_melsec_udp_client = new QMelsecUdpClient();
    //m_melsec_udp_client->moveToThread(this);
    connect(m_melsec_udp_client, &QMelsecUdpClient::signal_proc_real_data, this, &tconMelsecUpperBoard::slot_recv_melsec_udp_data);

    m_pTimer = new QTimer();
    m_pTimer->moveToThread(this);
    connect(m_pTimer, &QTimer::timeout, this, &tconMelsecUpperBoard::timerExp);
    //m_pMelsecUpperBoard = new QMelsec("UpperBoardDevice");
    //    m_pMelsecSubBoard->moveToThread(this);

    connect(this,&tconMelsecUpperBoard::signal_ui_warnning,GDataFactory::get_up_board_wgt(),&UpBoardWgt::slot_warnings);
    connect(this, &tconMelsecUpperBoard::signal_ui_equipment_state, GDataFactory::get_up_board_wgt(), &UpBoardWgt::slot_equipment_state);
}

void tconMelsecUpperBoard::connectToPLC(const QString ipPath, const quint16 port, const quint16 local_port)
{
    m_melsec_udp_client->init_ip_port(ipPath, port,local_port);
}

void tconMelsecUpperBoard::isStartUpperBoard()
{
    if(!isEnabled){
        this->start();
        //m_pTimer->start(5000);
        isEnabled = true;

        m_pMqSerViceSrc = new MQService;
        m_pMqSerViceSrc->MqStartService(true);

        load_data_from_db();
    }
}

void tconMelsecUpperBoard::isStopUpperBoard()
{
    if(isEnabled){
//        this->quit();
//        this->wait();
        m_pTimer->stop();
        isEnabled = false;
        m_pMqSerViceSrc->stopService();
    }
}

void tconMelsecUpperBoard::timerExp()
{
    m_melsec_udp_client->send_message_to_plc(TCON_UP_MC_CODE,
                                             TCON_UP_MC_ADDR,
                                             TCON_UP_MC_LENGTH);
}

void tconMelsecUpperBoard::slot_recv_melsec_udp_data(const QByteArray data_array)
{
    DATA_VALUE dataStruct;
    int tmpState = -1;
    QString tmpWarn;
    if(data_array.length() != TCON_UP_MC_LENGTH*2){
        QLOG_WARN() << dev_name << "TCON upper Melsec data length is error!";
    }else{
        for(int i=0;i<data_array.length();i++){
            if(i%2==0){
                if(i/2 <= 0xA){
                    QByteArray tmpByteArray(data_array.mid(i,2));

                    QString resData = m_melsec_udp_client->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    if(resData == "1"){
                        if(!dataStruct.Equipment_Warnings.isEmpty())
                            dataStruct.Equipment_Warnings.append(",");
                        dataStruct.Equipment_Warnings.append(m_getDBWarnAddrValues[i/2+1]);
                        tmpWarn = m_getDBWarnAddrValues[i/2+1];
                    }
                }else if(i/2 == 0xB){//设备状态
                    QByteArray tmpByteArray(data_array.mid(i,2));

                    QString resData = m_melsec_udp_client->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    QLOG_INFO()<<u8"设备状态"<<tmpByteArray<<resData;
                    if(resData == "0"){//device state free
                        dataStruct.Equipment_State.append(DEV_STATE_FREE);
                    }else if(resData == "1"){//device state run
                        dataStruct.Equipment_State.append(DEV_STATE_NOMAL);
                    }else if(resData == "2"){//device state stop
                        dataStruct.Equipment_State.append(DEV_STATE_STOP);
                    }
                    tmpState = dataStruct.Equipment_State.toInt();
                    dataStruct.Equipment_State.append(",");
                    dataStruct.Equipment_State.append(QString::number(QDateTime::currentDateTime().toTime_t()));
                    dataStruct.Equipment_State.append(",");
                    dataStruct.Equipment_State.append(QDateTime::currentDateTime().toString(fmtDateTime));
                }else if(i/2 == 0xC){//当前宽度
                    QByteArray tmpByteArray(data_array.mid(i,2));

                    QString resData = m_melsec_udp_client->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    dataStruct.Key_Part_Action_Times = resData;
                }else if(i/2 == 0xD){//上板数量
                    QByteArray tmpByteArray(data_array.mid(i,4));

                    QString resData = m_melsec_udp_client->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    dataStruct.Ok_Ng_Quantity.append(resData);//产量数据
                }
            }
        }
        m_pTconUpStationDataTable.insert(DATA_TYPE::Ok_Ng_Quantity, dataStruct.Ok_Ng_Quantity);
        m_pTconUpStationDataTable.insert(DATA_TYPE::Equipment_State, dataStruct.Equipment_State);
        m_pTconUpStationDataTable.insert(DATA_TYPE::Key_Part_Action_Times, dataStruct.Key_Part_Action_Times);
        m_pTconUpStationDataTable.insert(DATA_TYPE::Equipment_Warnings, dataStruct.Equipment_Warnings);

        insert_data_to_db();

        if(m_nDevNo == TCON_UP_DEV_NO_A){
            QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_A");
            m_pTconUpStationDataTable[DATA_TYPE::Product_ProjectID] = projID;
            m_pMqSerViceSrc->push(DEV_TYPE::TconUpperBoardDevA, m_pTconUpStationDataTable);

        }
        else if(m_nDevNo == TCON_UP_DEV_NO_B){
            QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_B");
            m_pTconUpStationDataTable[DATA_TYPE::Product_ProjectID] = projID;
            m_pMqSerViceSrc->push(DEV_TYPE::TconUpperBoardDevB, m_pTconUpStationDataTable);
        }
        emit signal_ui_equipment_state(tmpState, m_nDevNo);
        if(!tmpWarn.isEmpty()){
            QList<QString>lstWarn;
            lstWarn.push_back(tmpWarn);
            emit signal_ui_warnning(lstWarn,m_nDevNo);
        }
    }
}

void tconMelsecUpperBoard::run()
{
    while(1){
        m_melsec_udp_client->send_message_to_plc(TCON_UP_MC_CODE,
                                                 TCON_UP_MC_ADDR,
                                                 TCON_UP_MC_LENGTH);
        QEventLoop eventloop;
        QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }

}

void tconMelsecUpperBoard::insert_data_to_db()
{
    QString strStationTable;
    if(m_nDevNo == TCON_UP_DEV_NO_A){
        strStationTable = constUpABoardDataAcquisition;
    }else{
        strStationTable = constUpBBoardDataAcquisition;
    }

    QString strSql = QString("select max(id) from public.%1").
            arg(strStationTable);

    int id = 0;
    QSqlQuery queryResult;
    if(GDataFactory::get_factory()->get_pgsql_tcon_data_acquisition()->GetQueryResultDataAcquisition(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }

    strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7')").
            arg(strStationTable).arg(id+1).
            arg(m_pTconUpStationDataTable[DATA_TYPE::Equipment_State]).
            arg(m_pTconUpStationDataTable[DATA_TYPE::Key_Part_Action_Times]).
            arg(m_pTconUpStationDataTable[DATA_TYPE::Ok_Ng_Quantity]).
            arg(m_pTconUpStationDataTable[DATA_TYPE::Equipment_Warnings]).
            arg(QDateTime::currentDateTime().toString(fmtDateTime).append("_Upper_STATION"));
    QString strError;
    if(GDataFactory::get_factory()->get_pgsql_tcon_data_acquisition()->ExecSqlDataAcquisition(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<dev_name<<"upper board insert into DB FAILED!";
    }
}

void tconMelsecUpperBoard::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constUpBoardWarningsTable);

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
            m_getDBWarnAddrValues.insert(queryResult.value(0).toInt(),strInfo);
        }
    }
    else
    {
        QLOG_WARN()<<dev_name<<u8"get product info, query database failed!";
    }
}
