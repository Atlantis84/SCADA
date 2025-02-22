#include "ballwidget.h"
#include <QPainter>
#include <QIcon>
#include <QPainterPath>
BallWidget::BallWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    this->setMinimumSize(40,40);
    this->setMaximumSize(40,40);
    resize(40,40);
    m_pColorSign = 0;
}

void BallWidget::set_color(int colorsign)
{
    m_pColorSign = colorsign;
    this->repaint();
}

void BallWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    switch (m_pColorSign)
    {
    case 0:
    {
        QPainterPath path;
        path.addEllipse(this->rect());
        painter.fillPath(path,QBrush(QColor(255,0,0)));
    }
        break;
    case 1:
    {
        QPainterPath path;
        path.addEllipse(this->rect());
        painter.fillPath(path,QBrush(QColor(0,255,0)));
    }
        break;
    case 2:
        QPainterPath path;
        path.addEllipse(this->rect());
        painter.fillPath(path,QBrush(QColor(255,255,0)));
        break;
    }
}
