#ifndef TBOXRESULTWGT_H
#define TBOXRESULTWGT_H

#include <QWidget>

class TBoxResultWgt : public QWidget
{
    Q_OBJECT
public:
    explicit TBoxResultWgt(QWidget *parent = nullptr);

    void set_change_result(int r){changeResult = r;}
private:
    void paintEvent(QPaintEvent *event);
    int changeResult;

signals:

public slots:
};

#endif // TBOXRESULTWGT_H
