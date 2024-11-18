#include "lightbarnewproductwgt.h"
#include <QIcon>
#include "TitleBar.h"
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include "gdatafactory.h"
#include <QSqlQuery>
#include "imessagebox.h"
LightBarNewProductWgt::LightBarNewProductWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(750,700);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"录入新物料号");
    pTitleBar->setFixedWidth(750);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(createTableWidget());
    QGroupBox* grpAllData = new QGroupBox(u8"已录入物料信息");
    grpAllData->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpAllData->setLayout(vBox1);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QHBoxLayout* hBox2 = new QHBoxLayout();
    QLabel* labelID = new QLabel(u8"物料号ID:");
    QLabel* labelSN = new QLabel(u8"半品料号:");
    m_pLineEditID = new QLineEdit();
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,m_pLineEditID);
    m_pLineEditID->setValidator(validator);
    m_pLineEditID->setAlignment(Qt::AlignCenter);
    m_pLineEditID->setStyleSheet("color:rgb(0,0,0);max-width:80px;border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    m_pLineEditSN = new QLineEdit();
    QRegExp regx1("[A-Z0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,m_pLineEditSN);
    m_pLineEditSN->setValidator(validator1);
    m_pLineEditSN->setAlignment(Qt::AlignCenter);
    m_pLineEditSN->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox1->addWidget(labelID);
    hBox1->addWidget(m_pLineEditID);
    hBox1->addWidget(labelSN);
    hBox1->addWidget(m_pLineEditSN);
    vBox2->addLayout(hBox1);

    QPushButton* btnQuery = new QPushButton(u8"  查  询  ");
    btnQuery->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnQuery,SIGNAL(clicked()),this,SLOT(slot_btn_query()));
    QPushButton* btnDelete = new QPushButton(u8"  删  除  ");
    btnDelete->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(slot_btn_delete()));
    QPushButton* btnInsert = new QPushButton(u8"  插  入  ");
    btnInsert->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnInsert,SIGNAL(clicked()),this,SLOT(slot_btn_insert()));
    hBox2->addWidget(btnQuery);
    hBox2->addWidget(btnInsert);
    hBox2->addWidget(btnDelete);
    vBox2->addLayout(hBox2);

    QGroupBox* grpOperation = new QGroupBox(u8"操作");
    grpOperation->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpOperation->setLayout(vBox2);

    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpAllData);
    vAll->addWidget(grpOperation);
    vAll->setStretch(0,4);
    vAll->setStretch(1,1);
    this->setLayout(vAll);
}

QString LightBarNewProductWgt::select_id_from_db(const QString halfmaterialnumber)
{
    QString strSql = QString("select * from public.%1 where \"Product_Material_Number\" like '\%%2\%'").
            arg(constLightBarProductTable).arg(halfmaterialnumber);

    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        if(queryResult.size() == 0)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"当前物料未录入，请录入物料信息!");
            return "";
        }
        while(queryResult.next())
        {
            return queryResult.value(0).toString();
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}

QWidget *LightBarNewProductWgt::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(10000,2);
    connect(tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slot_select_item(QTableWidgetItem*)));

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"物料号ID"<<u8"半品料号";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    return tableWidget;
}

void LightBarNewProductWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void LightBarNewProductWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    set_table_data();
}

void LightBarNewProductWgt::slot_btn_query()
{
    this->tableWidget->clearContents();

    QString strSql;
    if((this->m_pLineEditID->text().trimmed() != "") && (this->m_pLineEditSN->text().trimmed() != ""))
        strSql = QString("select * from public.%1 where \"Product_ID\" like '\%%2\%' and \"Product_Material_Number\" like '\%%3\%'").
                arg(constLightBarProductTable).arg(this->m_pLineEditID->text().trimmed()).arg(this->m_pLineEditSN->text().trimmed());
    else
    {
        if(this->m_pLineEditID->text().trimmed() != "")
            strSql = QString("select * from public.%1 where \"Product_ID\" like '\%%2\%'").
                    arg(constLightBarProductTable).arg(this->m_pLineEditID->text().trimmed());
        else if(this->m_pLineEditSN->text().trimmed() != "")
            strSql = QString("select * from public.%1 where \"Product_Material_Number\" like '\%%2\%'").
                    arg(constLightBarProductTable).arg(this->m_pLineEditSN->text().trimmed());
        else
            strSql = QString("select * from public.%1").
                    arg(constLightBarProductTable);
    }

    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        int itemCount = 0;
        while(queryResult.next())
        {
            tableWidget->setItem(itemCount,0,new QTableWidgetItem(queryResult.value(0).toString()));
            tableWidget->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable);
            tableWidget->setItem(itemCount,1,new QTableWidgetItem(queryResult.value(1).toString()));
            tableWidget->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable);
            itemCount++;
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}

void LightBarNewProductWgt::slot_btn_insert()
{
    if((this->m_pLineEditID->text().trimmed() == "") || (this->m_pLineEditSN->text().trimmed() == ""))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"物料号ID或半品料号为空，请输入相关信息!");
        return;
    }

    QString strSql = QString("select * from public.%1 where \"Product_ID\"='%2'").
            arg(constLightBarProductTable).arg(this->m_pLineEditID->text().trimmed());
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() != 0)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"物料号ID已存在，请重新输入!");
            this->m_pLineEditID->setText("");
            return;
        }
    }

    strSql = QString("select * from public.%1 where \"Product_Material_Number\"='%2'").
                arg(constLightBarProductTable).arg(this->m_pLineEditSN->text().trimmed());
    queryResult.clear();
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() != 0)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"半品料号已存在，请重新输入!");
            this->m_pLineEditSN->setText("");
            return;
        }
    }

    insert_new_product_to_db();
    set_table_data();

}

void LightBarNewProductWgt::slot_btn_delete()
{
    if(this->m_pLineEditID->text().trimmed() == "")
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"物料号ID或半品料号为空，请输入相关信息!");
        return;
    }

    QString strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("delete from %1 where \"Product_ID\"='%2'").
            arg(constLightBarProductTable).arg(this->m_pLineEditID->text().trimmed());
    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"delete into DB FAILED!";
    }

    set_table_data();
}

void LightBarNewProductWgt::slot_select_item(QTableWidgetItem *item)
{
    tableWidget->setCurrentCell(item->row(), QItemSelectionModel::Select);
}

void LightBarNewProductWgt::insert_new_product_to_db()
{
    QString strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values('%2','%3')").
            arg(constLightBarProductTable).arg(this->m_pLineEditID->text().trimmed()).
            arg(this->m_pLineEditSN->text().trimmed());
    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"insert into DB FAILED!";
    }
}

void LightBarNewProductWgt::set_table_data()
{
    this->tableWidget->clearContents();
    QString strSql = QString("select * from public.%1").
            arg(constLightBarProductTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        int itemCount = 0;
        while(queryResult.next())
        {
            tableWidget->setItem(itemCount,0,new QTableWidgetItem(queryResult.value(0).toString()));
            tableWidget->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);
            tableWidget->setItem(itemCount,1,new QTableWidgetItem(queryResult.value(1).toString()));
            tableWidget->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,1)->setFlags(Qt::ItemIsSelectable);
            itemCount++;
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}
