#include "tconmelsecsubboard.h"
#include <gdatafactory.h>
#include <QEventLoop>
#include <QSqlQuery>
#include <QsLog.h>
#include <QDateTime>
#include <QMutex>

tconMelsecSubBoard* tconMelsecSubBoard::m_pObj = nullptr;

tconMelsecSubBoard::tconMelsecSubBoard(int devNo)
{
    qRegisterMetaType<QByteArray>("QByteArray");
    m_nDevNo = devNo;
    if(m_nDevNo == TCON_SUB_DEV_NO_A){
        dev_name = "SubBoardDeviceA";
    }
    else if(m_nDevNo == TCON_SUB_DEV_NO_B){
        dev_name = "SubBoardDeviceB";
    }

    m_pMelsecUdpSubBoard = new QMelsecUdpClient();
    //m_pMelsecUdpSubBoard->moveToThread(this);
    connect(m_pMelsecUdpSubBoard, &QMelsecUdpClient::signal_proc_real_data, this, &tconMelsecSubBoard::slot_melsec_udp_data);

    //m_pMelsecSubBoard = new QMelsec(dev_name);
    m_pTimer = new QTimer();
    m_pTimer->moveToThread(this);
    connect(m_pTimer, &QTimer::timeout, this, &tconMelsecSubBoard::timerExp);
    connect(this,&tconMelsecSubBoard::signal_ui_warnning,GDataFactory::get_divide_board_wgt(),&DivideBoardWgt::slot_warnings);
    connect(this, &tconMelsecSubBoard::signal_ui_equipment_state, GDataFactory::get_divide_board_wgt(), &DivideBoardWgt::slot_equipment_state);
    connect(this, &tconMelsecSubBoard::signal_ui_key_part_times,GDataFactory::get_divide_board_wgt(),&DivideBoardWgt::slot_key_part_action_times);
}

void tconMelsecSubBoard::connectToPLC(const QString ipPath, const quint16 port, const quint16 local_port)
{
    m_pMelsecUdpSubBoard->init_ip_port(ipPath, port, local_port);

}

void tconMelsecSubBoard::isStartSubBoard()
{
    if(!isEnabled){
        //m_pTimer->start(5000);
        this->start();
        isEnabled = true;
        m_pMqSerViceSrc = new MQService;
        m_pMqSerViceSrc->MqStartService(true);

        load_data_from_db();
    }
}

void tconMelsecSubBoard::isStopSubBoard()
{
    if(isEnabled){
        m_pTimer->stop();
//        this->quit();
//        this->wait();
        isEnabled = false;

        m_pMqSerViceSrc->stopService();
    }
}

void tconMelsecSubBoard::timerExp()
{
    dataStruct.Equipment_State = DEV_STATE_FREE;
    dataStruct.Equipment_Warnings.clear();

    m_pMelsecUdpSubBoard->send_message_to_plc(6000,"D",7);

//    QEventLoop eventloop;
//    QTimer::singleShot(100,&eventloop,&QEventLoop::quit);
//    eventloop.exec();

    m_pMelsecUdpSubBoard->send_message_to_plc(130,"TS",4);
}

void tconMelsecSubBoard::slot_melsec_udp_data(const QByteArray data_array)
{
    static QString pressStopStr;
    int tmpState = -1;
    QString tmpWarn;
    QLOG_INFO()<<dev_name<<"tconMelsecSubBoard: "<<data_array<<data_array.length();
    if(data_array.length() == 14){
        for(int i=0;i<data_array.length();i++){
            if(i%2==0){
                if(i/2 == ORDER_RECV_ID::X0){//前急停已按下
                    QByteArray tmpByteArray(data_array.mid(i,2));
                    QString resData = m_pMelsecUdpSubBoard->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    if(resData == "1"){
                        pressStopStr = m_getDBWarnAddrValues[1];
                    }
                }
                else if(i/2==ORDER_RECV_ID::X1){//后急停已按下
                    QByteArray tmpByteArray(data_array.mid(i,2));
                    QString resData = m_pMelsecUdpSubBoard->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    if(resData == "1"){
                        pressStopStr = m_getDBWarnAddrValues[2];
                    }
                }
                else if(i/2==ORDER_RECV_ID::stop_state){//停止


                }
                else if(i/2==ORDER_RECV_ID::run_state){
                    QByteArray tmpByteArray(data_array.mid(i,2));
                    QString resData = m_pMelsecUdpSubBoard->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    if(resData == "1"){
                        dataStruct.Equipment_State = DEV_STATE_NOMAL;
                    }else{
                        dataStruct.Equipment_State = DEV_STATE_STOP;
                    }

                }
                else if(i/2==ORDER_RECV_ID::free_state){
                    QByteArray tmpByteArray(data_array.mid(i,2));
                    QString resData = m_pMelsecUdpSubBoard->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    currentFreeState = resData.toInt();
                    if(dataStruct.Equipment_State == DEV_STATE_NOMAL){
                        if(currentFreeState == 0){
                           m_nFreeStateCount = 0;
                        }else{
                            m_nFreeStateCount++;
                            if(m_nFreeStateCount >= 3){
                                dataStruct.Equipment_State = DEV_STATE_FREE;
                            }
                        }
                    }
                }
                else if(i/2==ORDER_RECV_ID::key_move_count){//走刀次数

                    QByteArray tmpByteArray(data_array.mid(i,4));

                    QString resData = m_pMelsecUdpSubBoard->QByteArrayToValue(tmpByteArray,RESULT_TRANS_DEC,0);
                    QLOG_INFO()<<u8"走刀次数"<<tmpByteArray<<resData;
                    dataStruct.Key_Part_Action_Times=resData;
                }
            }
        }
    }else if(data_array.length() == 8){
        QString strResData = m_pMelsecUdpSubBoard->QByteArrayToValue(data_array,RESULT_TRANS_BITS,4);
        QLOG_INFO() <<dev_name<<"strResData"<<strResData;

        int faultBitsID = 0;
        for(int i=0; i<strResData.length(); i+=16){
            QString tmpResData = strResData.mid(i,16);
            for(int j=tmpResData.length()-1;j>=0;j--){
                if(i==0){
                    if(j>=0&&j<11){
                        continue;
                    }
                }else if(i==16){
                    if(j>=0&&j<11){
                        continue;
                    }
                }else if(i==16*3){
                    if(j>=0&&j<14){
                        continue;
                    }
                }

                faultBitsID++;
                if(tmpResData[j] == '1'){
                    if(!dataStruct.Equipment_Warnings.isEmpty())
                        dataStruct.Equipment_Warnings.append(",");

                    dataStruct.Equipment_Warnings.append(m_getDBWarnAddrValues[faultBitsID+2]);
                }
            }
        }

        if(!pressStopStr.isEmpty()){
            if(!dataStruct.Equipment_Warnings.isEmpty())
                dataStruct.Equipment_Warnings.append(",");
            dataStruct.Equipment_Warnings.append(pressStopStr);
            pressStopStr.clear();
        }

        tmpState = dataStruct.Equipment_State.toInt();

        dataStruct.Equipment_State.append(",");
        dataStruct.Equipment_State.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        dataStruct.Equipment_State.append(",");
        dataStruct.Equipment_State.append(QDateTime::currentDateTime().toString(fmtDateTime));

        m_pTconSubStationDataTable.insert(DATA_TYPE::Key_Part_Action_Times, dataStruct.Key_Part_Action_Times);
        m_pTconSubStationDataTable.insert(DATA_TYPE::Equipment_State, dataStruct.Equipment_State);
        m_pTconSubStationDataTable.insert(DATA_TYPE::Equipment_Warnings, dataStruct.Equipment_Warnings);

        insert_data_to_db();

        //EAM上传数据
        if(m_nDevNo == TCON_SUB_DEV_NO_A){
            QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_A");
            m_pTconSubStationDataTable[DATA_TYPE::Product_ProjectID] = projID;
            m_pMqSerViceSrc->push(DEV_TYPE::TconSubBoardDevA, m_pTconSubStationDataTable);

        }
        else if(m_nDevNo == TCON_SUB_DEV_NO_B){
            QString projID = GDataFactory::get_factory()->get_config_para("PROJECT_ID_B");
            m_pTconSubStationDataTable[DATA_TYPE::Product_ProjectID] = projID;
            m_pMqSerViceSrc->push(DEV_TYPE::TconSubBoardDevB, m_pTconSubStationDataTable);

        }
        if(!dataStruct.Equipment_Warnings.isEmpty()){
            QList<QString>lstWarn;
            lstWarn.push_back(dataStruct.Equipment_Warnings);
            emit signal_ui_warnning(lstWarn,m_nDevNo);
        }

        emit signal_ui_equipment_state(tmpState ,m_nDevNo);

        emit signal_ui_key_part_times(dataStruct.Key_Part_Action_Times.toInt(),m_nDevNo);
    }
}

void tconMelsecSubBoard::run()
{
    while (1) {

        dataStruct.Equipment_State = DEV_STATE_FREE;
        dataStruct.Equipment_Warnings.clear();
        m_pMelsecUdpSubBoard->send_message_to_plc(6000,"D",7);

        QEventLoop eventloop;
        QTimer::singleShot(100,&eventloop,&QEventLoop::quit);
        eventloop.exec();
        m_pMelsecUdpSubBoard->send_message_to_plc(130,"TS",4);
        QTimer::singleShot(4900,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}

void tconMelsecSubBoard::insert_data_to_db()
{
    QString strSql, strStationTable;
    if(m_nDevNo == TCON_SUB_DEV_NO_A){
        strStationTable = constSubABoardDataAcquisition;
    }else{
        strStationTable = constSubBBoardDataAcquisition;
    }
    strSql = QString("select max(id) from public.%1").
            arg(strStationTable);

    int id = 0;
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql_tcon_data_acquisition()->GetQueryResultDataAcquisition(strSql,queryResult))
    {
        while(queryResult.next())
        {
            id = queryResult.value(0).toInt();
        }
    }

    QLOG_INFO()<<dev_name<<m_pTconSubStationDataTable[DATA_TYPE::Key_Part_Action_Times]<<strStationTable;

    strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7')").
            arg(strStationTable).arg(id+1).
            arg(m_pTconSubStationDataTable[DATA_TYPE::Equipment_State]).
            arg(m_pTconSubStationDataTable[DATA_TYPE::Key_Part_Action_Times]).
            arg(m_pTconSubStationDataTable[DATA_TYPE::Ok_Ng_Quantity]).
            arg(m_pTconSubStationDataTable[DATA_TYPE::Equipment_Warnings]).
            arg(QDateTime::currentDateTime().toString(fmtDateTime).append("_"+dev_name));
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

void tconMelsecSubBoard::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constSubBoardWarningsTable);

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
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}




