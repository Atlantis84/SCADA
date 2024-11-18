#ifndef BallWidget_H
#define BallWidget_H

#include <QWidget>

class BallWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BallWidget(QWidget *parent = nullptr);
    void set_color(int colorsign);//0:green 1:red 2:blue
private:
    void paintEvent(QPaintEvent *event);
    int m_pColorSign;
public:

signals:

public slots:
};

#endif // BallWidget_H
