#include "dataacquisitonerrorconfigwgt.h"
#include "gdatafactory.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QFileDialog>
#include "imessagebox.h"
#include <QTextCodec>
#include "gdataacquisitionfactory.h"
static QString str_excel_path;
static int id_to_insert = -1;
DataAcquisitonErrorConfigWgt* DataAcquisitonErrorConfigWgt::m_pErrorWgt = nullptr;
DataAcquisitonErrorConfigWgt::DataAcquisitonErrorConfigWgt(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
    m_pTableView = new QTableView();
    m_pItemModel = new QStandardItemModel();
    QStringList headerlist;
    headerlist.clear();
    headerlist<<u8"序号" << u8"故障名称" << u8"故障描述" << u8"故障代码";
    m_pItemModel->setHorizontalHeaderLabels(headerlist);
    m_pItemModel->setColumnCount(headerlist.size());
    m_pTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->setStyleSheet("color:rgb(0,0,0);gridline-color:rgb(0,0,0);");
    m_pTableView->setModel(m_pItemModel);

    QComboBox* m_pCmbStationStyle = new QComboBox();
    QStringList lstTableName;
    lstTableName.clear();
    lstTableName<<u8"上板机表"<<u8"分板机表"<<u8"测试机台表"<<u8"AOI表"<<u8"摆盘机表"<<u8"M201上板机表"<<
                  u8"M201分板机表"<<u8"M201上夹具表"<<u8"M201点胶机表"<<u8"M201异型插表"<<u8"M201波峰焊表"<<
                  u8"M201取上盖机表"<<u8"M201炉后AOI表"<<u8"M201下夹具表";
    for(int i=0;i<lstTableName.size();i++)
        m_pCmbStationStyle->insertItem(i,lstTableName[i]);
    m_pCmbStationStyle->setCurrentIndex(-1);
    connect(m_pCmbStationStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_index_changed(int)));
    m_pCmbStationStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:KaiTi;font-size:20px;"
                                           "color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);min-width:120px;"
                                            "min-width:300px;min-height:30px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");

    QPushButton* btnImport = new QPushButton(u8"    导 入 数 据    ");
    btnImport->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:200px;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnImport,SIGNAL(clicked()),this,SLOT(slot_btn_import_click()));

    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(m_pTableView);
    QGroupBox* grpDataInfo = new QGroupBox(u8"数据信息");
    grpDataInfo->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpDataInfo->setLayout(vBox1);

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelImport = new QLabel(u8"请选择需要导入表:");
    hBox1->addStretch();
    hBox1->addWidget(labelImport);
    hBox1->addWidget(m_pCmbStationStyle);
    hBox1->addWidget(btnImport);
    hBox1->addStretch();
    QGroupBox* grpOperation = new QGroupBox(u8"操作");
    grpOperation->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpOperation->setLayout(hBox1);


    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addWidget(grpDataInfo);
    vAll->addWidget(grpOperation);
    this->setLayout(vAll);

    m_pImportThread = new ImportThread();
    connect(m_pImportThread,SIGNAL(signal_import_data(QList<QString>)),this,SLOT(slot_import_data(QList<QString>)));
    connect(m_pImportThread,SIGNAL(signal_import_over()),this,SLOT(slot_import_over()));
}

void DataAcquisitonErrorConfigWgt::set_table_data(int id)
{
    m_pItemModel->removeRows(0,m_pItemModel->rowCount());
    QString strSql;
    if(id == 0)
        strSql = QString("select * from public.%1").arg(constUpBoardWarningsTable);
    else if(id == 1)
        strSql = QString("select * from public.%1").arg(constSubBoardWarningsTable);
    else if(id == 2)
        strSql = QString("select * from public.%1").arg(constTestStationWarningsTable);
    else if(id == 3)
        strSql = QString("select * from public.%1").arg(constAOIWarningsTable);
    else if(id == 4)
        strSql = QString("select * from public.%1").arg(constPackageWarningsTable);
    else if(id == 5)
        strSql = QString("select * from public.%1").arg(constM201UpBoardWarningsTable);
    else if(id == 6)
        strSql = QString("select * from public.%1").arg(constM201DivideMachineWarningsTable);
    else if(id == 7)
        strSql = QString("select * from public.%1").arg(constM201UpUtensilWarningsTable);
    else if(id == 8)
        strSql = QString("select * from public.%1").arg(constM201GlueMachineWarningsTable);
    else if(id == 10)
        strSql = QString("select * from public.%1").arg(constM201WaveSolderingWarningsTable);
    else if(id == 11)
        strSql = QString("select * from public.%1").arg(constM201PickUpCoverWarningsTable);
    else if(id == 13)
        strSql = QString("select * from public.%1").arg(constM201DownUtensilWarningsTable);
    else
        return;

    QSqlQuery queryResult;
    if(id <5)
    {
        if(GDataFactory::get_pgsql_tcon_data_acquisition()->GetQueryResultDataAcquisition(strSql,queryResult))
        {
            QLOG_TRACE()<<u8"get product info, query database success!";
            int itemCount = 0;
            while(queryResult.next())
            {
                m_pItemModel->setItem(itemCount,0,new QStandardItem(queryResult.value(0).toString()));
                m_pItemModel->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);
                m_pItemModel->setItem(itemCount,1,new QStandardItem(queryResult.value(1).toString()));
                m_pItemModel->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,1)->setFlags(Qt::ItemIsSelectable);
                m_pItemModel->setItem(itemCount,2,new QStandardItem(queryResult.value(2).toString()));
                m_pItemModel->item(itemCount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,2)->setFlags(Qt::ItemIsSelectable);
                m_pItemModel->setItem(itemCount,3,new QStandardItem(queryResult.value(3).toString()));
                m_pItemModel->item(itemCount,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,3)->setFlags(Qt::ItemIsSelectable);
                itemCount++;
            }
            m_pTableView->setModel(m_pItemModel);
        }
        else
        {
            QLOG_WARN()<<u8"get product info, query database failed!";
        }
    }
    else
    {
        if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->GetQueryResultWithConnectionName(strSql,queryResult))
        {
            QLOG_TRACE()<<u8"get product info, query database success!";
            int itemCount = 0;
            while(queryResult.next())
            {
                m_pItemModel->setItem(itemCount,0,new QStandardItem(queryResult.value(0).toString()));
                m_pItemModel->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);
                m_pItemModel->setItem(itemCount,1,new QStandardItem(queryResult.value(1).toString()));
                m_pItemModel->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,1)->setFlags(Qt::ItemIsSelectable);
                m_pItemModel->setItem(itemCount,2,new QStandardItem(queryResult.value(2).toString()));
                m_pItemModel->item(itemCount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,2)->setFlags(Qt::ItemIsSelectable);
                m_pItemModel->setItem(itemCount,3,new QStandardItem(queryResult.value(3).toString()));
                m_pItemModel->item(itemCount,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_pItemModel->item(itemCount,3)->setFlags(Qt::ItemIsSelectable);
                itemCount++;
            }
            m_pTableView->setModel(m_pItemModel);
        }
        else
        {
            QLOG_WARN()<<u8"get product info, query database failed!";
        }
    }
}

void DataAcquisitonErrorConfigWgt::slot_import_data()
{

}

void DataAcquisitonErrorConfigWgt::slot_import_data(QList<QString> data)
{
    insert_info_to_db(data);
}

void DataAcquisitonErrorConfigWgt::slot_import_over()
{
    m_pWakeUp.close();
    m_pImportThread->terminate();
}

QString DataAcquisitonErrorConfigWgt::open_excel_file()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(u8"产品信息文件");//
    fileDialog->setNameFilter("Conf(*.csv)");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog->exec()){
       fileNames = fileDialog->selectedFiles();
    }

    if(fileNames.size()==0){
        return "";
    }else{
        return fileNames.first();
    }
}

void DataAcquisitonErrorConfigWgt::slot_comb_index_changed(int id)
{
    id_to_insert = id;
    set_table_data(id);
}

void DataAcquisitonErrorConfigWgt::insert_info_to_db(QList<QString> info)
{
    QString strSql = "";
    QSqlQuery queryResult;
    QString strError;

    if(info.size() != 4)
    {
        QLOG_WARN()<<u8"the format of data imported is not correct!";
        return;
    }

    if(id_to_insert < 5)
    {
        if(id_to_insert == 0)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constUpBoardWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 1)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constSubBoardWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 2)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constTestStationWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 3)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constAOIWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 4)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constPackageWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else
        {
            QLOG_WARN()<<"the table id is not correct!";
            return;
        }

        if(GDataFactory::get_pgsql_tcon_data_acquisition()->ExecSqlDataAcquisition(strSql,strError))
        {
            ;
        }
        else
        {
            QLOG_WARN()<<"insert into DB FAILED!";
        }
    }
    else
    {
        if(id_to_insert == 10)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201WaveSolderingWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 8)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201GlueMachineWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 6)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201DivideMachineWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 5)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201UpBoardWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 7)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201UpUtensilWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 11)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201PickUpCoverWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);
        else if(id_to_insert == 13)
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").arg(constM201DownUtensilWarningsTable).arg(info[0].toInt()).
                                                            arg(info[1]).arg(info[2]).arg(info[3]);


        if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->ExecSqlWithConnectionName(strSql,strError))
        {
            ;
        }
        else
        {
            QLOG_WARN()<<"insert into DB FAILED!";
        }
    }
}

void DataAcquisitonErrorConfigWgt::slot_btn_import_click()
{
//    m_pItemModel->removeRows(0,m_pItemModel->rowCount());
    if(id_to_insert == -1)
    {
        IMessageBox* box = new IMessageBox(3);
        box->warning(u8"请选择需要插入的数据表");
        return;
    }

    str_excel_path = open_excel_file();
    QFile m_file_Confirm(str_excel_path);
    if(!m_file_Confirm.exists())
    {
        IMessageBox* msg = new IMessageBox(3);
        msg->warning(u8"所选文件不存在!");
        return;
    }

    QString strTruncate = "";
    if(id_to_insert < 5)
    {
        if(id_to_insert == 0)
            strTruncate = QString("truncate table public.%1").arg(constUpBoardWarningsTable);
        else if(id_to_insert == 1)
            strTruncate = QString("truncate table public.%1").arg(constSubBoardWarningsTable);
        else if(id_to_insert == 2)
            strTruncate = QString("truncate table public.%1").arg(constTestStationWarningsTable);
        else if(id_to_insert == 3)
            strTruncate = QString("truncate table public.%1").arg(constAOIWarningsTable);
        else if(id_to_insert == 4)
            strTruncate = QString("truncate table public.%1").arg(constPackageWarningsTable);

        QString tmpEr;
        if(GDataFactory::get_pgsql_tcon_data_acquisition()->ExecSqlDataAcquisition(strTruncate,tmpEr))
            ;
        else
        {
            QLOG_WARN()<<"truncate table failed";
            return;
        }
    }
    else
    {
        if(id_to_insert == 10)
            strTruncate = QString("truncate table public.%1").arg(constM201WaveSolderingWarningsTable);
        else if(id_to_insert == 8)
            strTruncate = QString("truncate table public.%1").arg(constM201GlueMachineWarningsTable);
        else if(id_to_insert == 6)
            strTruncate = QString("truncate table public.%1").arg(constM201DivideMachineWarningsTable);
        else if(id_to_insert == 5)
            strTruncate = QString("truncate table public.%1").arg(constM201UpBoardWarningsTable);
        else if(id_to_insert == 7)
            strTruncate = QString("truncate table public.%1").arg(constM201UpUtensilWarningsTable);
        else if(id_to_insert == 11)
            strTruncate = QString("truncate table public.%1").arg(constM201PickUpCoverWarningsTable);
        else if(id_to_insert == 13)
            strTruncate = QString("truncate table public.%1").arg(constM201DownUtensilWarningsTable);


        QString tmpEr;
        if(GDataAcquisitionFactory::get_pgsql_three_in_one_data_acquisition()->ExecSqlWithConnectionName(strTruncate,tmpEr))
            ;
        else
        {
            QLOG_WARN()<<"truncate table failed";
            return;
        }
    }


    m_pWakeUp.show();
    m_pImportThread->start();
}

ImportThread::ImportThread(QWidget *parent)
{
    Q_UNUSED(parent);
}

void ImportThread::run()
{
    while(1)
    {
        QFile inFile(str_excel_path);
        QStringList lines;/*行数据*/
        QTextCodec *codec=QTextCodec::codecForName("GBK");

        if (inFile.open(QIODevice::ReadOnly))
        {
            QTextStream stream_text(&inFile);
            stream_text.setCodec(codec);
            while (!stream_text.atEnd())
            {
                lines.push_back(stream_text.readLine());
            }
            for (int i = 1; i < lines.size(); i++)
            {
                QString line = lines.at(i);
                QStringList split = line.split(",");/*列数据*/
                QList<QString> lstInfo;

                for (int col = 0; col < split.size(); col++)
                {
                    lstInfo.push_back(split[col]);
                }
                emit signal_import_data(lstInfo);
            }

            inFile.close();
        }

        emit signal_import_over();
        break;

    }
}
