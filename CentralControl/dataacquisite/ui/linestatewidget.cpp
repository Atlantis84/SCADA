#include "linestatewidget.h"
#include <QPainter>
LineStateWidget::LineStateWidget(QWidget *parent) : QWidget(parent)
{

}

void LineStateWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap pixmap(":/icon/LineInfo.TIF");
    painter.drawPixmap(this->rect(),pixmap);
}
