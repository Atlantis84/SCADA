#include "leftwidget.h"
#include <QPainter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QList>
#include <QDebug>
#include "gdatafactory.h"
#include <QStyleFactory>
LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBottom = new QVBoxLayout();

    QTreeWidget *treeWidget = new QTreeWidget();
    connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_getPath(QTreeWidgetItem*,int)));
    treeWidget->setColumnCount(1);
    treeWidget->setFont(QFont(u8"KaiTi",13,QFont::Normal,false));
    treeWidget->setStyleSheet("QTreeWidget{color:rgb(255,255,255);}QTreeView::item{ padding:5px; } ");
    QList<QTreeWidgetItem*> items;
    for (int i=0;i<5;i++)
    {
        if(i==0)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"换产管理")));
            item->setIcon(0,QIcon(":/icon/root2.png"));
            items.append(item);

            QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"一键换产(TCON)")));
            childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
            items.at(i)->addChild(childitem);

            QTreeWidgetItem* childitem1 = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"一键换产(TBOX)")));
            childitem1->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
            items.at(i)->addChild(childitem1);

            QTreeWidgetItem* childitem2 = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"一键换产(M201)")));
            childitem2->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
            items.at(i)->addChild(childitem2);
        }
        else if(i == 1)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"单工站管理")));
            item->setIcon(0,QIcon(":/icon/root4.png"));
            items.append(item);

            QList<QString> sList;
            sList<<u8"灯条擦拭"<<u8"分板上下料"<<u8"T109线体"<<u8"物联自动包装";
            for(int j=0;j<4;j++)
            {
                QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(sList[j]));
                childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                items.at(i)->addChild(childitem);

                if(j==2)
                {
                    QList<QString> tmpList;
                    tmpList<<u8"螺钉机";
                    QTreeWidgetItem* childitem1 = new QTreeWidgetItem(childitem,QStringList(tmpList[0]));
                    childitem1->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    childitem->addChild(childitem1);
                }
             }
        }
        else if(i==2)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"数据采集")));
            item->setIcon(0,QIcon(":/icon/root5.png"));
            items.append(item);

            QTreeWidgetItem* itemTCon = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"TCON自动测")));
            item->addChild(itemTCon);

            QList<QString> sList;
            sList<<u8"TCON线体概览"<<u8"上板机"<<u8"分板机"<<u8"测试机台"<<u8"AOI"<<u8"摆盘机"<<u8"数采配置";
            for(int j=0;j<7;j++)
            {
                if(j!=0)
                {
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(itemTCon,QStringList(sList[j]));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    itemTCon->addChild(childitem);
                }
            }

            QTreeWidgetItem* itemThreeInOne = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"三合一(M201)")));
            item->addChild(itemThreeInOne);

            QList<QString> sList1;
            sList1<<u8"锯刀分板(M201)"<<u8"点胶机"<<u8"波峰焊(M201)"<<u8"异型插数据统计";
            for(int j=0;j<4;j++)
            {
                QTreeWidgetItem* childitem = new QTreeWidgetItem(itemThreeInOne,QStringList(sList1[j]));
                childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                itemThreeInOne->addChild(childitem);
            }

            QTreeWidgetItem* itemThreeInOne2 = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"三合一(M202)")));
            item->addChild(itemThreeInOne2);

            QList<QString> sList2;
            sList2<<u8"波峰焊(M202)";
            for(int j=0;j<1;j++)
            {
                QTreeWidgetItem* childitem = new QTreeWidgetItem(itemThreeInOne2,QStringList(sList2[j]));
                childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                itemThreeInOne2->addChild(childitem);
            }

            QTreeWidgetItem* itemThreeInOne3 = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"三合一(M203)")));
            item->addChild(itemThreeInOne3);

            QList<QString> sList3;
            sList3<<u8"波峰焊(M203)";
            for(int j=0;j<1;j++)
            {
                QTreeWidgetItem* childitem = new QTreeWidgetItem(itemThreeInOne3,QStringList(sList3[j]));
                childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                itemThreeInOne3->addChild(childitem);
            }
        }
        else if(i == 3)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"多工站管理")));
            item->setIcon(0,QIcon(":/icon/multi.png"));
            items.append(item);

            QTreeWidgetItem* itemHongQi = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"红旗PAD")));
            itemHongQi->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
            items.at(i)->addChild(itemHongQi);

            QList<QString> sList;
            sList<<u8"中框条码绑定"<<u8"主板绑定"<<u8"FPC压合"<<u8"测试点胶"<<u8"维修工位"<<u8"Pogopin压合"<<u8"命令测试";
            for(int j=0;j<7;j++)
            {
                QTreeWidgetItem* childitem = new QTreeWidgetItem(itemHongQi,QStringList(sList[j]));
                childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                itemHongQi->addChild(childitem);
            }
        }
        else
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"其他操作")));
            item->setIcon(0,QIcon(":/icon/root3.png"));
            items.append(item);

            QList<QString> sList;
            sList<<u8"实时日志"<<u8"数据管理"<<u8"站点监视"<<u8"用户管理"<<u8"操作";
            for (int j=0;j<5;j++)
            {
                QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(sList[j]));
                childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                items.at(i)->addChild(childitem);
            }
        }
    }
    treeWidget->insertTopLevelItems(0,items);
    treeWidget->setHeaderHidden(true);
    treeWidget->setStyle(QStyleFactory::create("windows"));

    treeWidget->expandAll();

    vBottom->addWidget(treeWidget);
    vAll->addSpacing(70);
    vAll->addLayout(vBottom);
    vAll->setMargin(0);
    this->setLayout(vAll);
}

void LeftWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(22,194,194)));
    int headHeight = this->height()/13;
    QRect rtHead(0,0,this->width(),headHeight);
    painter.fillRect(rtHead,QBrush(QColor(0,166,170)));
    QRect rtHeadLeft(10,15,this->width()/2-20,headHeight-30);
    QRect rtHeadRight(this->width()/2,0,this->width()/2,headHeight);
    painter.fillRect(rtHeadLeft,QBrush(QColor(255,255,255,255)));

    painter.setFont(QFont(u8"KaiTi",20,QFont::Normal,false));

    painter.save();
    painter.setPen(QColor(27,162,160));
//    painter.drawText(rtHeadLeft,Qt::AlignHCenter | Qt::AlignVCenter,u8"Hisense");
    painter.drawPixmap(rtHeadLeft,QPixmap(":/icon/2.png"));
    painter.restore();

    painter.setPen(Qt::white);
    painter.drawText(rtHeadRight,Qt::AlignHCenter | Qt::AlignVCenter,u8"中控系统");

}

void LeftWidget::slot_getPath(QTreeWidgetItem *wItem, int count)
{
    Q_UNUSED(count);
    QStringList filePath;
    QTreeWidgetItem *itemFile = wItem;
    while (itemFile != nullptr)
    {
        filePath<<itemFile->text(0);
        itemFile = itemFile->parent();
    }

    QString strPath;
    for (int i=filePath.size()-1;i>=0;i--)
    {
        strPath +=filePath.at(i);
        if(i!=0)
            strPath+="/";
    }

    if(strPath.contains(u8"一键换产(TCON)"))
        if((GDataFactory::get_factory()->get_current_usr_name() == "tcon")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(0,u8"一键换产");

    if(strPath.contains(u8"灯条擦拭"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "myl")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(1,u8"灯条擦拭");
    }

    if(strPath.contains(u8"分板上下料"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "fbj")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(2,u8"分板上下料");
    }

    if(strPath.contains(u8"实时日志"))
        emit signal_create_tab_item(3,u8"实时日志");

    if(strPath.contains(u8"一键换产(TBOX)"))
        if((GDataFactory::get_factory()->get_current_usr_name() == "tbox")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(4,u8"TBOX");

    if(strPath.contains(u8"中框条码绑定"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "zkbd")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(5,u8"中框条码绑定");
    }

//    if(strPath.contains(u8"测试点胶"))
//    {
//        if((GDataFactory::get_factory()->get_current_usr_name() == "csdj")||
//                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
//        emit signal_create_tab_item(15,u8"测试点胶");
//    }
//    if(strPath.contains(u8"维修工位"))
//    {
//        if((GDataFactory::get_factory()->get_current_usr_name() == "wxgw")||
//                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
//        emit signal_create_tab_item(16,u8"维修工位");
//    }

    if(strPath.contains(u8"TCON线体概览"))
        emit signal_create_tab_item(6,u8"TCON线体概览");
    if(strPath.contains(u8"上板机"))
        emit signal_create_tab_item(7,u8"上板机");
    if(strPath.contains(u8"分板机"))
        emit signal_create_tab_item(8,u8"分板机");
    if(strPath.contains(u8"测试机台"))
        emit signal_create_tab_item(9,u8"测试机台");
    if(strPath.contains(u8"AOI"))
        emit signal_create_tab_item(10,u8"AOI");
    if(strPath.contains(u8"摆盘机"))
        emit signal_create_tab_item(11,u8"摆盘机");

    if(strPath.contains(u8"主板绑定"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "zbbd")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(12,u8"主板绑定");
    }
    if(strPath.contains(u8"FPC压合"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "fpcyh")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(13,u8"FPC压合");
    }

    if(strPath.contains(u8"数采配置"))
        emit signal_create_tab_item(14,u8"数采配置");

    if(strPath.contains(u8"测试点胶"))
    {
        emit signal_create_tab_item(15,u8"测试点胶");
    }

    if(strPath.contains(u8"螺钉机"))
        emit signal_create_tab_item(16,u8"螺钉机");

    if(strPath.contains(u8"锯刀分板(M201)"))
        emit signal_create_tab_item(17,u8"锯刀分板(M201)");

    if(strPath.contains(u8"维修工位"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "wxgw")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(18,u8"维修工位");
    }

    if(strPath.contains(u8"点胶机"))
        emit signal_create_tab_item(19,u8"点胶机");

    if(strPath.contains(u8"波峰焊(M201)"))
        emit signal_create_tab_item(20,u8"波峰焊(M201)");

    if(strPath.contains(u8"波峰焊(M202)"))
        emit signal_create_tab_item(21,u8"波峰焊(M202)");

    if(strPath.contains(u8"波峰焊(M203)"))
        emit signal_create_tab_item(22,u8"波峰焊(M203)");

    if(strPath.contains(u8"Pogopin压合"))
        emit signal_create_tab_item(23,u8"Pogopin压合");

    if(strPath.contains(u8"一键换产(M201)"))
        emit signal_create_tab_item(24,u8"一键换产(M201)");

    if(strPath.contains(u8"命令测试"))
        emit signal_create_tab_item(25,u8"命令测试");

    if(strPath.contains(u8"异型插数据统计"))
        emit signal_create_tab_item(26,u8"异型插数据统计");

    if(strPath.contains(u8"操作4"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "tcon")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            GDataFactory::get_factory()->truncate_change_info_table_pure();
    }

    if(strPath.contains(u8"用户管理"))
    {
        if(GDataFactory::get_factory()->get_current_usr_name() == "admin")
            GDataFactory::get_user_tab_wgt()->show();
    }

    emit signal_send_path(strPath);
}
