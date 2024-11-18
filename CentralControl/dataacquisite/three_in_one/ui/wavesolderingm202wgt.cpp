#include "wavesolderingm202wgt.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "gdataacquisitionfactory.h"
#include <QSqlQuery>
#include "QsLog.h"
#include <QTextCodec>
#include <QDateTime>
WaveSolderingM202Wgt* WaveSolderingM202Wgt::m_pInstance;
WaveSolderingM202Wgt::WaveSolderingM202Wgt()
{
    load_data_from_db();
}

void WaveSolderingM202Wgt::process_warnings_data(const QString data)
{
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    int warningID = -1;
    QString warningInfo = "";
    for (int i=0;i<lst1.size();i++) {
        QMap<int,QString>::const_iterator it;
        for (it=m_pMapWarningsInfoOfDB.constBegin();it!=m_pMapWarningsInfoOfDB.constEnd();++it)
        {
            if(it.value().trimmed().contains(lst1[i].trimmed()))
            {
                warningID = it.key();
                warningInfo = it.value();
            }
            else
                continue;
        }
    }

    if(warningID != -1)
    {
        m_pListWgt->addItem(warningInfo);
//        if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Equipment_Warnings))
//            m_pWaveSolderingDataTable[DATA_TYPE::Equipment_Warnings] = warningInfo;
//        else
//            m_pWaveSolderingDataTable.insert(DATA_TYPE::Equipment_Warnings,warningInfo);
    }
}

void WaveSolderingM202Wgt::process_ok_ng_data(const QString data)
{
    m_pLineEditOKQuantity->setText(data);

//    if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Ok_Ng_Quantity))
//        m_pWaveSolderingDataTable[DATA_TYPE::Ok_Ng_Quantity] = data;
//    else
//        m_pWaveSolderingDataTable.insert(DATA_TYPE::Ok_Ng_Quantity,data);
}

void WaveSolderingM202Wgt::process_equipment_state(const QString data)
{
    QStringList lst1 = data.split(";");

    if(lst1.size()>0)
        ;
    else
        return;

    QStringList lst2 = lst1[0].split(',');
    if(lst2.size()>=2)
        ;
    else
        return;

    QString strState = "";
    if(lst2[1] == "1")
    {
        m_pEquipStatus->set_color(1);
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("2");
    }
    else if(lst2[1] == "0")
    {
        m_pEquipStatus->set_color(2);

        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("3");
    }
    else
    {
        m_pEquipStatus->set_color(0);

        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("1");
    }

//    if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Equipment_State))
//        m_pWaveSolderingDataTable[DATA_TYPE::Equipment_State] = strState;
//    else
//        m_pWaveSolderingDataTable.insert(DATA_TYPE::Equipment_State,strState);
}

void WaveSolderingM202Wgt::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constM201WaveSolderingWarningsTable);
    QSqlQuery queryResult;
    if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
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

void WaveSolderingM202Wgt::process_set_value(const QString data)
{
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    if(lst1.size() == 11)
        ;
    else
        return;

    m_pPlolderSpd->set_OKs(lst1[0].split(',')[1]);
    m_pB1PreHeat->set_OKs(lst1[1].split(',')[1]);
    m_pB2PreHeat->set_OKs(lst1[2].split(',')[1]);
    m_pB3PreHeat->set_OKs(lst1[3].split(',')[1]);
    m_pSolderPot->set_OKs(lst1[4].split(',')[1]);
    m_pWave1SV->setText(lst1[5].split(',')[1]);
    m_pWave2SV->setText(lst1[6].split(',')[1]);

    QString strSetInfo = "";
    for(int i=0;i<7;i++)
    {
        if(i<6)
        {
            strSetInfo.append(lst1[i]);
            strSetInfo.append(";");
        }
        else
            strSetInfo.append(lst1[i]);
    }

//    if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Param_Set_Value))
//        m_pWaveSolderingDataTable[DATA_TYPE::Param_Set_Value] = strSetInfo;
//    else
//        m_pWaveSolderingDataTable.insert(DATA_TYPE::Param_Set_Value,strSetInfo);
}

void WaveSolderingM202Wgt::process_practical_value(const QString data)
{
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    if(lst1.size() == 5)
        ;
    else
        return;

    m_pPlolderSpd->set_NGs(lst1[0].split(',')[1]);
    m_pB1PreHeat->set_NGs(lst1[1].split(',')[1]);
    m_pB2PreHeat->set_NGs(lst1[2].split(',')[1]);
    m_pB3PreHeat->set_NGs(lst1[3].split(',')[1]);
    m_pSolderPot->set_NGs(lst1[4].split(',')[1]);

    QString strPracticalInfo = "";
    for(int i=0;i<5;i++)
    {
        if(i<4)
        {
            strPracticalInfo.append(lst1[i]);
            strPracticalInfo.append(";");
        }
        else
            strPracticalInfo.append(lst1[i]);
    }

//    if(m_pWaveSolderingDataTable.contains(DATA_TYPE::Param_Practical_Value))
//        m_pWaveSolderingDataTable[DATA_TYPE::Param_Practical_Value] = strPracticalInfo;
//    else
//        m_pWaveSolderingDataTable.insert(DATA_TYPE::Param_Practical_Value,strPracticalInfo);
}

bool WaveSolderingM202Wgt::find_warnings(const QString data)
{
    bool result = false;
    QStringList lst1 = data.split(";");
    lst1.pop_back();

    for (int i=0;i<lst1.size();i++) {
        QMap<int,QString>::const_iterator it;
        for (it=m_pMapWarningsInfoOfDB.constBegin();it!=m_pMapWarningsInfoOfDB.constEnd();++it) {
            if(it.value().trimmed().contains(lst1[i].trimmed()))
                result = true;
            else
                continue;
        }
    }

    return result;
}

void WaveSolderingM202Wgt::process_info_from_tcp_server(const QByteArray &data)
{
    if(data == "you can input a command now")
    {
        emit signal_to_start_acquisition_after_confirm(DEV_TYPE_M201::M202WaveSoldering);
        return;
    }

    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QString tmpData = codec->toUnicode(data);
    QLOG_INFO()<<tmpData;

    if(find_warnings(tmpData))
    {
        process_warnings_data(tmpData);
        return;
    }

    if(tmpData.contains(u8"三色灯状态"))
    {
        process_equipment_state(tmpData);
        return;
    }

    if(tmpData.contains(u8"运输速度,"))
    {
        process_set_value(tmpData);
        return;
    }

    if(tmpData.contains(u8"运输速度实际值,"))
    {
        process_practical_value(tmpData);
        return;
    }

    if(!(tmpData.contains(";")) && !(tmpData.contains("No alarm")))
        process_ok_ng_data(tmpData);
}

void WaveSolderingM202Wgt::slot_connect_state(int connectState)
{
    m_pTcpConnectStatus->set_color(connectState);
}
