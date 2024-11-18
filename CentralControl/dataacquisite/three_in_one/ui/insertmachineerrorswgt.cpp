#include "insertmachineerrorswgt.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "gdataacquisitionfactory.h"
#include "QsLog.h"
#include <QTimerEvent>
InsertMachineErrorsWgt* InsertMachineErrorsWgt::m_pInstance = nullptr;
InsertMachineErrorsWgt::InsertMachineErrorsWgt(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    qRegisterMetaType<QList<QStringList>>("QList<QStringList>");
    m_pTableView = new QTableView();
    m_pItemModel = new QStandardItemModel();
    QStringList headerlist;
    headerlist.clear();
    headerlist<<u8"工站名称" << u8"日期" << u8"时间" << u8"故障代码"<<u8"故障描述";
    m_pItemModel->setHorizontalHeaderLabels(headerlist);
    m_pItemModel->setColumnCount(headerlist.size());
    m_pTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->setStyleSheet("color:rgb(0,0,0);gridline-color:rgb(0,0,0);");
    m_pTableView->setModel(m_pItemModel);

    m_pCmbStationStyle = new QComboBox();
    QStringList lstTableName;
    lstTableName.clear();
    lstTableName<<u8"异型插1"<<u8"异型插2"<<u8"异型插3"<<u8"异型插4";
    for(int i=0;i<lstTableName.size();i++)
        m_pCmbStationStyle->insertItem(i,lstTableName[i]);
    m_pCmbStationStyle->setCurrentIndex(-1);
    connect(m_pCmbStationStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_station_index_changed(int)));
    m_pCmbStationStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:KaiTi;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:120px;"
                                            "min-width:250px;min-height:30px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    QPushButton* btnImport = new QPushButton(u8"    查 询 数 据    ");
    btnImport->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200px;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnImport,SIGNAL(clicked()),this,SLOT(slot_btn_import_click()));

    QPushButton* btnExport = new QPushButton(u8"    导 出 数 据    ");
    btnExport->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200px;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                             QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnExport,SIGNAL(clicked()),this,SLOT(slot_btn_export_click()));

    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(m_pTableView);
    QGroupBox* grpDataInfo = new QGroupBox(u8"数据信息");
    grpDataInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpDataInfo->setLayout(vBox1);

    m_pDateEdit = new QDateEdit();
    m_pDateEdit->setDateTime(QDateTime::currentDateTime());
    m_pDateEdit->setDisplayFormat("yyyy/MM/dd");
    m_pDateEdit->setStyleSheet("QDateEdit{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);\
                            background-color:rgba(235,236,240,255);font-family:\"Microsoft YaHei\";\
                            font-size: 20px;font-weight:normal;\
                            border-radius:0px;min-width:250px;\
                            selection-background-color:rgba(0,0,0,100);\
                            selection-color:#5D5C6C;}\
                            ");

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelImport = new QLabel(u8"请选择机台:");
    QLabel* labelDateTime = new QLabel(u8"请选择时间:");
    hBox1->addStretch();
    hBox1->addWidget(labelImport);
    hBox1->addWidget(m_pCmbStationStyle);
    hBox1->addWidget(labelDateTime);
    hBox1->addWidget(m_pDateEdit);
    hBox1->addWidget(btnImport);
    hBox1->addWidget(btnExport);
    hBox1->addStretch();
    QGroupBox* grpOperation = new QGroupBox(u8"操作");
    grpOperation->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpOperation->setLayout(hBox1);


    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addWidget(grpDataInfo);
    vAll->addWidget(grpOperation);
    this->setLayout(vAll);

    connect(GDataAcquisitionFactory::get_collect_machine_error_thread(),SIGNAL(signal_send_errors_to_ui(QList<QStringList>)),this,SLOT(slot_set_table_data(QList<QStringList>)));
    GDataAcquisitionFactory::get_collect_machine_error_thread()->start();
}

void InsertMachineErrorsWgt::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

void InsertMachineErrorsWgt::slot_comb_station_index_changed(int id)
{
    QLOG_INFO()<<m_pCmbStationStyle->currentIndex();
}

void InsertMachineErrorsWgt::slot_set_table_data(QList<QStringList> info)
{
    m_pItemModel->removeRows(0,m_pItemModel->rowCount());
    int itemCount = 0;
    for (int i=0;i<info.size();i++) {
        QStringList tmplst = info[i];

        m_pItemModel->setItem(itemCount,0,new QStandardItem(m_pCmbStationStyle->currentText()));
        m_pItemModel->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_pItemModel->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);

        m_pItemModel->setItem(itemCount,1,new QStandardItem(m_pDateEdit->text()));
        m_pItemModel->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_pItemModel->item(itemCount,1)->setFlags(Qt::ItemIsSelectable);

        m_pItemModel->setItem(itemCount,2,new QStandardItem(tmplst[0]));
        m_pItemModel->item(itemCount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_pItemModel->item(itemCount,2)->setFlags(Qt::ItemIsSelectable);

        m_pItemModel->setItem(itemCount,3,new QStandardItem(tmplst[2]));
        m_pItemModel->item(itemCount,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_pItemModel->item(itemCount,3)->setFlags(Qt::ItemIsSelectable);

        m_pItemModel->setItem(itemCount,4,new QStandardItem(tmplst[4]));
        m_pItemModel->item(itemCount,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_pItemModel->item(itemCount,4)->setFlags(Qt::ItemIsSelectable);
        itemCount++;
    }

    m_pTableView->setModel(m_pItemModel);
}

void InsertMachineErrorsWgt::slot_btn_import_click()
{
    if(m_pCmbStationStyle->currentIndex() == -1)
    {
        QLOG_WARN()<<"please select the station";
        return;
    }

    QDate speDate = m_pDateEdit->date();
    QDate currentDate = QDate::currentDate();
    qint64 days = currentDate.daysTo(speDate);
    GDataAcquisitionFactory::get_collect_machine_error_thread()->set_para_value(true,m_pCmbStationStyle->currentIndex()+1,days);
}

void InsertMachineErrorsWgt::slot_btn_export_click()
{
    QLOG_INFO()<<m_pItemModel->rowCount();
    QString dtStr="e:/";
    dtStr.append(m_pCmbStationStyle->currentText());
    QString tmpStr = QDateTime::currentDateTime().toString("yyyyMMddHHmmss")+".csv";
    dtStr = dtStr.append(tmpStr);
    QFile file;
    file.setFileName(dtStr);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QLOG_WARN()<<"Open file Failed!";
        return;
    }

    QTextStream out(&file);
    out.setCodec("GBK");
    QString s1 = u8"工站名称";
    QString s2 = u8"日期";
    QString s3 = u8"时间";
    QString s4 = u8"故障代码";
    QString s5 = u8"故障描述";
    out<<s1<<",";
    out<<s2<<",";
    out<<s3<<",";
    out<<s4<<",";
    out<<s5<<",";
    out<<"\n";

    for(int i=0;i<m_pItemModel->rowCount();i++)
    {
        out<<m_pItemModel->item(i,0)->text()<<",";
        out<<m_pItemModel->item(i,1)->text()<<",";
        out<<m_pItemModel->item(i,2)->text()<<",";
        out<<m_pItemModel->item(i,3)->text()<<",";
        out<<m_pItemModel->item(i,4)->text()<<",";
        out<<"\n";
    }

    file.close();
}
