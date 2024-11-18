#include "lightbarwashingresultwgt.h"
#include "TitleBar.h"
#include "gdatafactory.h"
#include <QIcon>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
LightBarWashingResultWgt::LightBarWashingResultWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1500,700);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"灯条擦拭结果查询");
    pTitleBar->setFixedWidth(1500);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(createTableWidget());
    QGroupBox* grpAllData = new QGroupBox(u8"擦拭结果信息");
    grpAllData->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpAllData->setLayout(vBox1);

    QHBoxLayout* hBox1 = new QHBoxLayout();
    m_pLineEditSN = new QLineEdit();
    m_pLineEditSN->setAlignment(Qt::AlignCenter);
    m_pLineEditSN->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    QLabel* labelSN = new QLabel(u8"产品SN:");
    labelSN->setStyleSheet("border:none;font-family:KaiTi");
    QPushButton* btnQuery = new QPushButton(u8" 查  询 ");
    btnQuery->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);min-width:100;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");

    connect(btnQuery,SIGNAL(clicked()),this,SLOT(slot_btn_query()));
    hBox1->addWidget(labelSN);
    hBox1->addWidget(m_pLineEditSN);
    hBox1->addWidget(btnQuery);
    QGroupBox* grpOperation = new QGroupBox(u8"查询操作");
    grpOperation->setStyleSheet("QGroupBox{font-family:KaiTi;border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpOperation->setLayout(hBox1);

    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpAllData);
    vAll->addWidget(grpOperation);
    vAll->setStretch(0,9);
    vAll->setStretch(1,2);
    this->setLayout(vAll);
}

QWidget *LightBarWashingResultWgt::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(10000,4);

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"产品SN"<<u8"擦拭次数"<<u8"擦拭时间"<<u8"擦拭人员";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    return tableWidget;
}

void LightBarWashingResultWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void LightBarWashingResultWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->tableWidget->clearContents();
}

void LightBarWashingResultWgt::slot_btn_query()
{
    this->tableWidget->clearContents();
    QString strSql;
    if(m_pLineEditSN->text().trimmed() != "")
        strSql = QString("select * from public.%1 where \"Product_SN\" like '\%%2\%'").arg(constLightBarWashingResultTable).
                arg(this->m_pLineEditSN->text().trimmed());
    else
        strSql = QString("select * from public.%1").arg(constLightBarWashingResultTable);

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
            tableWidget->setItem(itemCount,2,new QTableWidgetItem(queryResult.value(2).toString()));
            tableWidget->item(itemCount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,2)->setFlags(Qt::ItemIsSelectable);
            tableWidget->setItem(itemCount,3,new QTableWidgetItem(queryResult.value(3).toString()));
            tableWidget->item(itemCount,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->item(itemCount,3)->setFlags(Qt::ItemIsSelectable);
            itemCount++;
        }
    }
    else
    {
        QLOG_WARN()<<u8"get product info, query database failed!";
    }
}
