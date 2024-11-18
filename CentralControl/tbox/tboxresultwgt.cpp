#include "tboxresultwgt.h"
#include <QPainter>
TBoxResultWgt::TBoxResultWgt(QWidget *parent) : QWidget(parent)
{
    changeResult = 1;
}

void TBoxResultWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setFont(QFont("KaiTi",50,QFont::Bold,false));
    if(changeResult == 1)
    {
        painter.fillRect(this->rect(),QBrush(QColor(0,255,0,100)));
        painter.setPen(QPen(QColor(0,255,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"换  产  完  成");
    }
    else if(changeResult == 0){
        painter.fillRect(this->rect(),QBrush(QColor(255,0,0,100)));
        painter.setPen(QPen(QColor(255,0,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"换  产  中...");
    }
    else if(changeResult == 2)
    {
        painter.fillRect(this->rect(),QBrush(QColor(255,0,0,100)));
        painter.setPen(QPen(QColor(255,0,0)));
        painter.drawText(this->rect(),Qt::AlignHCenter | Qt::AlignVCenter,u8"换  产  失  败");
    }
}
