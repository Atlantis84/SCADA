#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>

class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

    void set_product_state(int productState);

private:
    int productStateSign;

signals:

public slots:
};

#endif // RESULTWIDGET_H
