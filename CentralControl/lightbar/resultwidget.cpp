#include "resultwidget.h"
#include <QPainter>
ResultWidget::ResultWidget(QWidget *parent) : QWidget(parent)
{
    productStateSign = 1;
}

void ResultWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setFont(QFont("KaiTi",50,QFont::Bold,false));
    if(productStateSign == 1)
    {
        painter.fillRect(this->rect(),QBrush(QColor(0,255,0,100)));
        painter.setPen(QPen(QColor(0,255,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"良  品");
    }
    else if(productStateSign == 2){
        painter.fillRect(this->rect(),QBrush(QColor(255,0,0,100)));
        painter.setPen(QPen(QColor(255,0,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"不 良 品");
    }
    else if (productStateSign == 3) {
        painter.fillRect(this->rect(),QBrush(QColor(255,0,0,100)));
        painter.setPen(QPen(QColor(255,0,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"擦 拭 中");
    }
    else if(productStateSign == 4)
    {
        painter.fillRect(this->rect(),QBrush(QColor(0,255,0,100)));
        painter.setPen(QPen(QColor(0,255,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"擦拭完成");
    }

}

void ResultWidget::set_product_state(int productState)
{
    productStateSign = productState;
    this->repaint();
}
