#ifndef LINESTATEWIDGET_H
#define LINESTATEWIDGET_H

#include <QObject>
#include <QWidget>

class LineStateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineStateWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // LINESTATEWIDGET_H
