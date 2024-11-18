#include "usertabwgt.h"
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
#include <QLabel>

UserTabWgt::UserTabWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(750,700);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"用户管理");
    pTitleBar->setFixedWidth(750);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(createTableWidget());
    QGroupBox* grpAllData = new QGroupBox(u8"人员信息");
    grpAllData->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpAllData->setLayout(vBox1);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelName = new QLabel(u8"用户名:");
    m_pLineEditName = new QLineEdit();
    QRegExp regx("[a-z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,m_pLineEditName);
    m_pLineEditName->setValidator(validator);
    m_pLineEditName->setAlignment(Qt::AlignCenter);
    m_pLineEditName->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox1->addWidget(labelName);
    hBox1->addWidget(m_pLineEditName);

    QLabel* labelPassword = new QLabel(u8"密码:");
    m_pLineEditPassword = new QLineEdit();
    QRegExp regx1("[a-z0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,m_pLineEditPassword);
    m_pLineEditPassword->setValidator(validator1);
    m_pLineEditPassword->setAlignment(Qt::AlignCenter);
    m_pLineEditPassword->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox1->addWidget(labelPassword);
    hBox1->addWidget(m_pLineEditPassword);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QPushButton* btnQuery = new QPushButton(u8"  查  询  ");
    btnQuery->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnQuery,SIGNAL(clicked()),this,SLOT(slot_btn_query()));

    QPushButton* btnInsert = new QPushButton(u8"  插  入  ");
    btnInsert->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnInsert,SIGNAL(clicked()),this,SLOT(slot_btn_insert()));

    QPushButton* btnDelete = new QPushButton(u8"  删  除  ");
    btnDelete->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(slot_btn_delete()));
    hBox2->addWidget(btnQuery);
    hBox2->addWidget(btnInsert);
    hBox2->addWidget(btnDelete);

    vBox2->addLayout(hBox1);
    vBox2->addLayout(hBox2);

    QGroupBox* grpOperations = new QGroupBox(u8"操作");
    grpOperations->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpOperations->setLayout(vBox2);

    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpAllData);
    vAll->addWidget(grpOperations);
    vAll->setStretch(0,4);
    vAll->setStretch(1,1);
    this->setLayout(vAll);
}

QWidget *UserTabWgt::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(10000,2);
    connect(tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slot_select_item(QTableWidgetItem*)));

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"用户名"<<u8"密码";
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

void UserTabWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void UserTabWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    set_table_data();
}

void UserTabWgt::set_table_data()
{
    this->tableWidget->clearContents();
    QString strSql = QString("select * from public.%1").
            arg(constUserInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get product info, query database success!";
        int itemCount = 0;
        while(queryResult.next())
        {
            tableWidget->setItem(itemCount,0,new QTableWidgetItem(queryResult.value(1).toString()));
            tableWidget->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,0)->setFlags(Qt::ItemIsSelectable);
            tableWidget->setItem(itemCount,1,new QTableWidgetItem(GDataFactory::get_factory()->DecodeStr(queryResult.value(2).toString())));
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

void UserTabWgt::slot_btn_query()
{
    this->tableWidget->clearContents();

    QString strSql;
    if((this->m_pLineEditName->text().trimmed() != "") && (this->m_pLineEditPassword->text().trimmed() != ""))
        strSql = QString("select * from public.%1 where \"usrname\" like '\%%2\%' and \"password\" like '\%%3\%'").
                arg(constUserInfoTable).arg(this->m_pLineEditName->text().trimmed()).arg(GDataFactory::get_factory()->EncodeStr(this->m_pLineEditPassword->text().trimmed()));
    else
    {
        if(this->m_pLineEditName->text().trimmed() != "")
            strSql = QString("select * from public.%1 where \"usrname\" like '\%%2\%'").
                    arg(constUserInfoTable).arg(this->m_pLineEditName->text().trimmed());
        else if(this->m_pLineEditPassword->text().trimmed() != "")
            strSql = QString("select * from public.%1 where \"password\" like '\%%2\%'").
                    arg(constUserInfoTable).arg(GDataFactory::get_factory()->EncodeStr(this->m_pLineEditPassword->text().trimmed()));
        else
            strSql = QString("select * from public.%1").
                    arg(constUserInfoTable);
    }

    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"get user info, query database success!";
        int itemCount = 0;
        while(queryResult.next())
        {
            tableWidget->setItem(itemCount,0,new QTableWidgetItem(queryResult.value(1).toString()));
            tableWidget->item(itemCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable);
            tableWidget->setItem(itemCount,1,new QTableWidgetItem(GDataFactory::get_factory()->DecodeStr(queryResult.value(2).toString())));
            tableWidget->item(itemCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable);
            itemCount++;
        }
    }
    else
    {
        QLOG_WARN()<<u8"get user info, query database failed!";
    }
}

void UserTabWgt::slot_btn_insert()
{
    if((this->m_pLineEditName->text().trimmed() == "") || (this->m_pLineEditPassword->text().trimmed() == ""))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"用户名或密码为空，请输入相关信息!");
        return;
    }

    QString strSql = QString("select * from public.%1 where \"usrname\"='%2'").
            arg(constUserInfoTable).arg(this->m_pLineEditName->text().trimmed());
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() != 0)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"用户名已存在，请重新输入!");
            this->m_pLineEditName->setText("");
            return;
        }
    }

    strSql = QString("select * from public.%1 where \"password\"='%2'").
                arg(constUserInfoTable).arg(GDataFactory::get_factory()->EncodeStr(this->m_pLineEditPassword->text().trimmed()));
    queryResult.clear();
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() != 0)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"密码已使用，请重新输入!");
            this->m_pLineEditPassword->setText("");
            return;
        }
    }

    insert_new_usr_to_db();
    set_table_data();
}

void UserTabWgt::insert_new_usr_to_db()
{
//    QString strSql = QString("select max(id) from public.%1").
//            arg(constUserInfoTable);

//    int id = -1;
//    QSqlQuery queryResult;
//    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
//    {
//        while(queryResult.next())
//        {
//            id = queryResult.value(0).toInt();
//        }
//    }

    QString strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("insert into %1 values(%2,'%3','%4')").
            arg(constUserInfoTable).arg(1).arg(this->m_pLineEditName->text().trimmed()).
            arg(GDataFactory::get_factory()->EncodeStr(this->m_pLineEditPassword->text().trimmed()));
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

void UserTabWgt::slot_btn_delete()
{
    if(this->m_pLineEditName->text().trimmed() == "")
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"用户名为空，请输入相关信息!");
        return;
    }

    QString strSql = "";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    strSql = QString("delete from %1 where \"usrname\"='%2'").
            arg(constUserInfoTable).arg(this->m_pLineEditName->text().trimmed());
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
