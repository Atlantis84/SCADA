#include "testdivideboardwgt.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "QsLog.h"
#include <QSqlQuery>
#include "gdataacquisitionfactory.h"
#include <QDateTime>
#include <QTimerEvent>
#include <QtMath>
#include <QTextCodec>
TestDivideBoardWgt* TestDivideBoardWgt::m_pInstance = nullptr;
TestDivideBoardWgt::TestDivideBoardWgt(QWidget *parent) : QWidget(parent)
{
    m_pNullKnifeSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("NULL_KNIFE_SPEED_UPPER").toDouble();
    m_pNullKnifeSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("NULL_KNIFE_SPEED_FLOOR").toDouble();
    m_pDownKnifeSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("DOWN_KNIFE_SPEED_UPPER").toDouble();
    m_pDownKnifeSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("DOWN_KNIFE_SPEED_FLOOR").toDouble();
    m_pMainRollSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("MAIN_ROLL_SPEED_UPPER").toDouble();
    m_pMainRollSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("NULL_KNIFE_SPEED_FLOOR").toDouble();
    m_pHighSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("HIGH_SPEED_UPPER").toDouble();
    m_pHighSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("HIGH_SPEED_FLOOR").toDouble();
    m_pLowSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("LOW_SPEED_UPPER").toDouble();
    m_pLowSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("LOW_SPEED_FLOOR").toDouble();
    m_pProduceSpeedFloor = DataAcquisitionConfig::get_instance()->get_config_para("PRODUCE_SPEED_FLOOR").toDouble();
    m_pProduceSpeedUpper = DataAcquisitionConfig::get_instance()->get_config_para("PRODUCE_SPEED_UPPER").toDouble();

    lstLineEdit<<m_pA_ZAxis<<m_pB_ZAxis<<m_pA_Yxis<<m_pB_Yxis<<m_pA_Xxis<<m_pB_Xxis<<m_pA_XSpeed<<
                 m_pB_XSpeed<<m_pA_YSpeed<<m_pB_YSpeed<<m_pA_ZSpeed<<m_pB_ZSpeed<<m_pA_JGSpeedD<<
                 m_pB_JGSpeed<<m_pA_HighSpeed<<m_pB_HighSpeed<<m_pA_LowSpeed<<m_pB_LowSpeed<<
                 m_pA_BUJU1<<m_pB_BUJU1<<m_pA_BUJU2<<m_pB_BUJU2;

    for (int i=0;i<lstLineEdit.size();i++) {
        lstLineEdit[i] = new QLineEdit();
        lstLineEdit[i]->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                    "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                                    "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
        lstLineEdit[i]->setAlignment(Qt::AlignCenter);
        lstLineEdit[i]->setReadOnly(true);
    }


    m_pEquipStatus = new BallWidget();
    m_pLineEditOKQuantity = new QLineEdit();
    m_pLineEditOKQuantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    m_pLineEditOKQuantity->setAlignment(Qt::AlignCenter);
    m_pLineEditOKQuantity->setReadOnly(true);
//    m_pLineEditNGQuantity = new QLineEdit();
//    m_pLineEditNGQuantity->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
//                            "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
//                            "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");

    m_pListWgt = new QListWidget();
    m_pListWgt->setStyleSheet("QListWidget{color:rgb(255,0,0);background-color:rgba(0,0,0,50);font:10pt 'KaiTi'}");

    QVBoxLayout* vAll = new QVBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"状态信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addStretch();
    hBox1->addWidget(new QLabel(u8" 设备状态:"));
    hBox1->addWidget(m_pEquipStatus);
    hBox1->addStretch();
    grpBox1->setLayout(hBox1);

    QGroupBox* grpBox2 = new QGroupBox(u8"设备产量");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(new QLabel(u8"当前产量:"));
    hBox2->addWidget(m_pLineEditOKQuantity);
//    hBox2->addWidget(new QLabel(u8"当前NG数量:"));
//    hBox2->addWidget(m_pLineEditNGQuantity);
    grpBox2->setLayout(hBox2);

    QGroupBox* grpBox3 = new QGroupBox(u8"报警信息");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QHBoxLayout* hBox3 = new QHBoxLayout();
    hBox3->addWidget(m_pListWgt);
    grpBox3->setLayout(hBox3);

    QGroupBox* grpBox4 = new QGroupBox(u8"参数信息");
    grpBox4->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);}");
    QVBoxLayout* vBox1 = new QVBoxLayout();

    QHBoxLayout* hBox4 = new QHBoxLayout();
    QLabel* labelA_ZAxis = new QLabel(u8"A主轴(Pls/mm):");
    hBox4->addWidget(labelA_ZAxis);hBox4->addWidget(lstLineEdit[0]);
    QLabel* labelB_ZAxis = new QLabel(u8"B主轴(Pls/mm):");
    hBox4->addWidget(labelB_ZAxis);hBox4->addWidget(lstLineEdit[1]);
    QLabel* labelA_Yxis = new QLabel(u8"AY轴(Pls/mm):");
    hBox4->addWidget(labelA_Yxis);hBox4->addWidget(lstLineEdit[2]);
    QLabel* labelB_Yxis = new QLabel(u8"BY轴(Pls/mm):");
    hBox4->addWidget(labelB_Yxis);hBox4->addWidget(lstLineEdit[3]);

    QHBoxLayout* hBox5 = new QHBoxLayout();
    QLabel* labelA_Xxis = new QLabel(u8"AX轴(Pls/mm):");
    hBox5->addWidget(labelA_Xxis);hBox5->addWidget(lstLineEdit[4]);
    QLabel* labelB_Xxis = new QLabel(u8"BX轴(Pls/mm):");
    hBox5->addWidget(labelB_Xxis);hBox5->addWidget(lstLineEdit[5]);
    QLabel* labelA_XSpeed = new QLabel(u8"AX空刀速度(mm/s):");
    hBox5->addWidget(labelA_XSpeed);hBox5->addWidget(lstLineEdit[6]);
    QLabel* labelB_XSpeed = new QLabel(u8"BX空刀速度(mm/s):");
    hBox5->addWidget(labelB_XSpeed);hBox5->addWidget(lstLineEdit[7]);

    QHBoxLayout* hBox6 = new QHBoxLayout();
    QLabel* labelA_YSpeed = new QLabel(u8"AY空刀速度(mm/s):");
    hBox6->addWidget(labelA_YSpeed);hBox6->addWidget(lstLineEdit[8]);
    QLabel* labelB_YSpeed = new QLabel(u8"BY空刀速度(mm/s):");
    hBox6->addWidget(labelB_YSpeed);hBox6->addWidget(lstLineEdit[9]);
    QLabel* labelA_ZSpeed = new QLabel(u8"AZ下刀速度(mm/s):");
    hBox6->addWidget(labelA_ZSpeed);hBox6->addWidget(lstLineEdit[10]);
    QLabel* labelB_ZSpeed = new QLabel(u8"BZ下刀速度(mm/s):");
    hBox6->addWidget(labelB_ZSpeed);hBox6->addWidget(lstLineEdit[11]);

    QHBoxLayout* hBox7 = new QHBoxLayout();
    QLabel* labelA_JGSpeed = new QLabel(u8"A加工速度(mm/s):");
    hBox7->addWidget(labelA_JGSpeed);hBox7->addWidget(lstLineEdit[12]);
    QLabel* labelB_JGSpeed = new QLabel(u8"B加工速度(mm/s):");
    hBox7->addWidget(labelB_JGSpeed);hBox7->addWidget(lstLineEdit[13]);
    QLabel* labelA_HighSpeed = new QLabel(u8"A高速(mm/s):");
    hBox7->addWidget(labelA_HighSpeed);hBox7->addWidget(lstLineEdit[14]);
    QLabel* labelB_HighSpeed = new QLabel(u8"B高速(mm/s):");
    hBox7->addWidget(labelB_HighSpeed);hBox7->addWidget(lstLineEdit[15]);

    QHBoxLayout* hBox8 = new QHBoxLayout();
    QLabel* labelA_LowSpeed = new QLabel(u8"A低速(mm/s):");
    hBox8->addWidget(labelA_LowSpeed);hBox8->addWidget(lstLineEdit[16]);
    QLabel* labelB_LowSpeed = new QLabel(u8"B低速(mm/s):");
    hBox8->addWidget(labelB_LowSpeed);hBox8->addWidget(lstLineEdit[17]);
    QLabel* labelA_BUJU1 = new QLabel(u8"A步距1(mm):");
    hBox8->addWidget(labelA_BUJU1);hBox8->addWidget(lstLineEdit[18]);
    QLabel* labelB_BUJU1 = new QLabel(u8"B步距1(mm):");
    hBox8->addWidget(labelB_BUJU1);hBox8->addWidget(lstLineEdit[19]);

    QHBoxLayout* hBox9 = new QHBoxLayout();
    QLabel* labelA_BUJU2 = new QLabel(u8"A步距2(mm):");
    hBox9->addWidget(labelA_BUJU2);hBox9->addWidget(lstLineEdit[20]);
    QLabel* labelB_BUJU2 = new QLabel(u8"B步距2(mm):");
    hBox9->addWidget(labelB_BUJU2);hBox9->addWidget(lstLineEdit[21]);

    vBox1->addLayout(hBox4);
    vBox1->addLayout(hBox5);
    vBox1->addLayout(hBox6);
    vBox1->addLayout(hBox7);
    vBox1->addLayout(hBox8);
    vBox1->addLayout(hBox9);
    grpBox4->setLayout(vBox1);

    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox2);
    vAll->addWidget(grpBox4);
    vAll->addWidget(grpBox3);

    this->setLayout(vAll);

    load_data_from_db();

    m_pMqttClient = new QMqttClient();
    QString strHostname = DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_IP");
    m_pMqttClient->setHostname(strHostname);
    quint16 pt = static_cast<quint16>(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_MQTT_PORT").toInt());
    m_pMqttClient->setPort(pt);
    connect(m_pMqttClient, &QMqttClient::stateChanged, this, &TestDivideBoardWgt::updateLogStateChange);
    connect(m_pMqttClient, &QMqttClient::disconnected, this, &TestDivideBoardWgt::brokerDisconnected);

    m_pMqttClient->setUsername("admin");
    m_pMqttClient->setPassword("Admin12345");
    if(m_pMqttClient->state() == QMqttClient::Disconnected)
        m_pMqttClient->connectToHost();

    if(DataAcquisitionConfig::get_instance()->get_config_para("THREE_IN_ONE_DATA_ACQUISITION") == "1")
    {
        m_pTimerID = -1;
        m_pTimerID = startTimer(5000);
    }
}

void TestDivideBoardWgt::check_para_state(const QList<QString> lstinfo)
{
    QJsonArray content;
    for (int i=0;i<lstinfo.size();i++)
    {
        //主轴转速
        if(i==0)
        {
            double a = lstinfo[i].toDouble();
//            QLOG_ERROR()<<u8"主轴转速实际值:"<<QString::number(a);
//            QLOG_ERROR()<<u8"主轴转速标准值:"<<QString::number(m_pMainRollSpeedFloor);
//            if(qFuzzyCompare(a,m_pMainRollSpeedFloor))
//            if((a>m_pMainRollSpeedFloor) || (a<m_pMainRollSpeedUpper))
            if(GDataAcquisitionFactory::get_instance()->isGreater(m_pMainRollSpeedFloor,a)||
                    GDataAcquisitionFactory::get_instance()->isLess(m_pMainRollSpeedUpper,a))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMainRollSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMainRollSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_31");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机主轴转速");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pMainRollSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pMainRollSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_31");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机主轴转速");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //空刀速度
        if(i == 6)
        {
            double a = lstinfo[i].toDouble();
            QLOG_WARN()<<u8"空刀速度:"<<QString::number(a);
//            if((a>m_pNullKnifeSpeedFloor) && (a<m_pNullKnifeSpeedUpper))
            if(GDataAcquisitionFactory::get_instance()->isGreater(a,m_pNullKnifeSpeedFloor) &&
                    GDataAcquisitionFactory::get_instance()->isLess(a,m_pNullKnifeSpeedUpper))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pNullKnifeSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pNullKnifeSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_28");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机空刀速度");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pNullKnifeSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pNullKnifeSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_28");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机空刀速度");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
        }

        //下刀速度
        if(i == 10)
        {
            double a = lstinfo[i].toDouble();
            QLOG_INFO()<<u8"下刀速度:"<<QString::number(a);
            QLOG_INFO()<<u8"下刀速度上限:"<<m_pDownKnifeSpeedUpper;
            QLOG_INFO()<<u8"下刀速度下限:"<<m_pDownKnifeSpeedFloor;
//            if((a>m_pDownKnifeSpeedFloor) && (a<m_pDownKnifeSpeedUpper))
            if(GDataAcquisitionFactory::get_instance()->isGreater(m_pDownKnifeSpeedFloor,a) ||
                    GDataAcquisitionFactory::get_instance()->isLess(m_pDownKnifeSpeedUpper,a))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pDownKnifeSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pDownKnifeSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_29");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机下刀速度");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
//                QLOG_ERROR()<<u8"下刀速度异常!";
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pDownKnifeSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pDownKnifeSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_29");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机下刀速度");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //加工速度
        if(i == 12)
        {
            double a = lstinfo[i].toDouble();
            QLOG_WARN()<<u8"加工速度:"<<QString::number(a);
            if(GDataAcquisitionFactory::get_instance()->isGreater(m_pProduceSpeedFloor,a) ||
                    GDataAcquisitionFactory::get_instance()->isLess(m_pProduceSpeedUpper,a))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pProduceSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pProduceSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_30");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机加工速度");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pProduceSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pProduceSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_30");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机加工速度");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
        }

        //高速
        if(i == 14)
        {
            double a = lstinfo[i].toDouble();
            QLOG_WARN()<<u8"高速:"<<QString::number(a);
//            if((a>m_pHighSpeedFloor) && (a<m_pHighSpeedUpper))
            if(GDataAcquisitionFactory::get_instance()->isGreater(a,m_pHighSpeedFloor) &&
                    (GDataAcquisitionFactory::get_instance()->isLess(a,m_pHighSpeedUpper)))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pHighSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pHighSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_32");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机高速");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pHighSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pHighSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_32");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机高速");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
        }

        //低速
        if(i == 16)
        {
            double a = lstinfo[i].toDouble();
            QLOG_WARN()<<u8"低速:"<<QString::number(a);
//            if((a>m_pLowSpeedFloor) && (a<m_pLowSpeedUpper))
            if(GDataAcquisitionFactory::get_instance()->isGreater(a,m_pLowSpeedFloor) &&
                    GDataAcquisitionFactory::get_instance()->isLess(a,m_pLowSpeedUpper))
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pLowSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pLowSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_33");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机低速");
                contentJsonObject.insert("COMPARE_RESULT","OK");
                content.append(contentJsonObject);
            }
            else
            {
                QJsonObject contentJsonObject;
                contentJsonObject.insert("SET_VALUE_UPPER",QString::number(m_pLowSpeedUpper));
                contentJsonObject.insert("SET_VALUE_FLOOR",QString::number(m_pLowSpeedFloor));
                contentJsonObject.insert("ACTUAL_VALUE",QString::number(a));
                contentJsonObject.insert("MEASURED_TYPE","HD_M201_DIVIDE_BOARD_33");
                contentJsonObject.insert("MEASURED_TYPE_NAME",u8"分板机低速");
                contentJsonObject.insert("COMPARE_RESULT","NG");
                content.append(contentJsonObject);
            }
        }
    }

    if(m_pDivideMachineDataTable.contains(DATA_TYPE_M201::Param_Content_M201))
        m_pDivideMachineDataTable[DATA_TYPE_M201::Param_Content_M201] = content;
    else
        m_pDivideMachineDataTable.insert(DATA_TYPE_M201::Param_Content_M201,content);
}

void TestDivideBoardWgt::slot_test()
{

}

//static int divide_board_warning_id = 0;
void TestDivideBoardWgt::slot_equipment_error_devide(const int error)
{
//    if(divide_board_warning_id == 27)
//        divide_board_warning_id = 0;
//    divide_board_warning_id++;
//    if(error == 27)
//        return;

    QMap<int,QString>::const_iterator it;
    QString warningInfo = "";
    bool findSign = false;
    for (it=m_pMapWarningsInfoOfDB.constBegin();it!=m_pMapWarningsInfoOfDB.constEnd();++it)
    {
//        if(it.key() == divide_board_warning_id)
        if(it.key() == error)
        {
            warningInfo = it.value();
            warningInfo.prepend(u8"分板机:");
            findSign = true;
            break;
        }
    }

    if(findSign)
    {
        m_pListWgt->addItem(warningInfo);
        QStringList lstInfo = warningInfo.split(',');

        QJsonArray fault;
        QJsonObject faultJsonObject;
        QString strFaultCode = "HD_M201_DIVIDE_BOARD_";
        strFaultCode = strFaultCode.append(QString::number(error));
        faultJsonObject.insert("FAULT_CODE",strFaultCode);
//        lstInfo[0].prepend(u8"分板机");
        faultJsonObject.insert("FAULT_NAME",lstInfo[0]);
        QLOG_WARN()<<u8"分板机停机原因为:"<<lstInfo[0];
        faultJsonObject.insert("STATE","1");
        fault.append(faultJsonObject);

        if(m_pDivideMachineDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pDivideMachineDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pDivideMachineDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
    else
    {
        QJsonArray fault;
        QJsonObject faultJsonObject;
        fault.append(faultJsonObject);

        if(m_pDivideMachineDataTable.contains(DATA_TYPE_M201::Fault_Content_M201))
            m_pDivideMachineDataTable[DATA_TYPE_M201::Fault_Content_M201] = fault;
        else
            m_pDivideMachineDataTable.insert(DATA_TYPE_M201::Fault_Content_M201,fault);
    }
}

static int divide_board_quantity = -1;
void TestDivideBoardWgt::slot_equipment_output_devide(const int output)
{
    QTime time = QTime::currentTime();
    if(time.minute() == 0)
    {
        if(divide_board_quantity == -1)
            divide_board_quantity = output;
        else
        {
            if((output-divide_board_quantity)<10)
                ;
            else {
                QString strhour = QString(u8"%1时产量为:").arg(time.hour()-1);
                QLOG_ERROR()<<u8"分板机"<<strhour<<QString::number(output-divide_board_quantity);
                divide_board_quantity = output;
            }

        }
    }

//    QLOG_ERROR()<<u8"分板机产量为:"<<output;
    this->m_pLineEditOKQuantity->setText(QString::number(output));
//    if(m_pDivideMachineDataTable.contains(DATA_TYPE::Ok_Ng_Quantity))
//        m_pDivideMachineDataTable[DATA_TYPE::Ok_Ng_Quantity] = QString::number(output);
//    else
//        m_pDivideMachineDataTable.insert(DATA_TYPE::Ok_Ng_Quantity,QString::number(output));
}

void TestDivideBoardWgt::slot_equipment_state_devide(const int status)
{
    QString strState = "";
    if(status == 1)
    {
        this->m_pEquipStatus->set_color(0);

        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("1");

        if(m_pDivideMachineDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDivideMachineDataTable[DATA_TYPE_M201::Equipment_State_M201] = "1";
        else
            m_pDivideMachineDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"1");
    }
    else if(status == 2)
    {
        this->m_pEquipStatus->set_color(1);
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("2");

        if(m_pDivideMachineDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDivideMachineDataTable[DATA_TYPE_M201::Equipment_State_M201] = "2";
        else
            m_pDivideMachineDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"2");
    }
    else
    {
        this->m_pEquipStatus->set_color(2);
        strState.append(",");
        strState.append(QString::number(QDateTime::currentDateTime().toTime_t()));
        strState.append(",");
        strState.append(QDateTime::currentDateTime().toString(fmtDateTime));
        strState.prepend("3");

        if(m_pDivideMachineDataTable.contains(DATA_TYPE_M201::Equipment_State_M201))
            m_pDivideMachineDataTable[DATA_TYPE_M201::Equipment_State_M201] = "3";
        else
            m_pDivideMachineDataTable.insert(DATA_TYPE_M201::Equipment_State_M201,"3");
    }

//    if(m_pDivideMachineDataTable.contains(DATA_TYPE::Equipment_State))
//        m_pDivideMachineDataTable[DATA_TYPE::Equipment_State] = strState;
//    else
//        m_pDivideMachineDataTable.insert(DATA_TYPE::Equipment_State,strState);
}

void TestDivideBoardWgt::slot_equipment_paras_devide(const QList<QString> lstinfo)
{
    QString strParaList = "";
    for (int i=0;i<lstinfo.size();i++)
    {
        lstLineEdit[i]->setText(lstinfo[i]);
        if(i != (lstinfo.size()-1))
        {
            strParaList.append(lstinfo[i]);
            strParaList.append(",");
        }
        else
            strParaList.append(lstinfo[i]);
    }

    check_para_state(lstinfo);
}

void TestDivideBoardWgt::updateLogStateChange()
{
    QLOG_WARN()<<"the MQTT state of divideboard-m201 is changed";
}

void TestDivideBoardWgt::brokerDisconnected()
{
    QLOG_WARN()<<"the MQTT of divideboard-m201 disconnect now";
}

void TestDivideBoardWgt::load_data_from_db()
{
    QString strSql = QString("select * from public.%1").arg(constM201DivideMachineWarningsTable);
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

void TestDivideBoardWgt::set_divide_params(QVector<double> vpara)
{
    if(vpara.size() == 12)
    {
        m_pNullKnifeSpeedUpper = vpara[0];
        m_pNullKnifeSpeedFloor = vpara[1];

        m_pDownKnifeSpeedUpper = vpara[2];
        m_pDownKnifeSpeedFloor = vpara[3];

        m_pMainRollSpeedUpper = vpara[4];
        m_pMainRollSpeedFloor = vpara[5];

        m_pHighSpeedUpper = vpara[6];
        m_pHighSpeedFloor = vpara[7];

        m_pLowSpeedUpper = vpara[8];
        m_pLowSpeedFloor = vpara[9];

        m_pProduceSpeedUpper = vpara[10];
        m_pProduceSpeedFloor = vpara[11];
    }
}

void TestDivideBoardWgt::timerEvent(QTimerEvent *event)
{
    if(GDataAcquisitionFactory::get_instance()->isTimeBetween730And830() ||
            GDataAcquisitionFactory::get_instance()->isTimeBetween1830And2030())
        return;

    if(event->timerId() == m_pTimerID)
    {
        if(m_pDivideMachineDataTable.size() < 3)
        {
            QLOG_WARN()<<"the size of Data in MQ from business-Divide is WRONG";
            return;
        }

        QString strTopic = "HD_M201_DIVIDE_BOARD_TOPIC";
        auto subscription = m_pMqttClient->subscribe(strTopic);
        if (!subscription)
            QLOG_WARN()<<"Could not subscribe. Is there a valid connection?";
        else
            QLOG_INFO()<<"the MQTT subscribe of divideboard-m201 is success";

        QJsonObject data;
        QJsonArray content,fault,rejectrate;
        QString systemstatus;
        systemstatus = m_pDivideMachineDataTable[DATA_TYPE_M201::Equipment_State_M201].toString();
        fault = m_pDivideMachineDataTable[DATA_TYPE_M201::Fault_Content_M201].toJsonArray();
        content = m_pDivideMachineDataTable[DATA_TYPE_M201::Param_Content_M201].toJsonArray();

        data.insert("SystemState",systemstatus);
        data.insert("ORG","4330");
        data.insert("TimeStamp",QString::number(QDateTime::currentMSecsSinceEpoch()));
        data.insert("DeviceName","DivideBoardM201");
        data.insert("SystemNo","HS0543302024FZSB01039");
        data.insert("CONTENT",content);
        data.insert("FAULT",fault);
        data.insert("REJECTRATE",rejectrate);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(data);
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QString tmpData = codec->toUnicode(jsonDoc.toJson());
        QLOG_WARN()<<"divide-board submit msg to andon:"<<tmpData;
        QByteArray byteArray=jsonDoc.toJson(QJsonDocument::Compact);
        m_pMqttClient->publish(strTopic,byteArray);
    }
}
