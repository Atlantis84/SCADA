#ifndef LIGHTBARWASHINGRESULTWGT_H
#define LIGHTBARWASHINGRESULTWGT_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
class LightBarWashingResultWgt : public QWidget
{
    Q_OBJECT
public:
    explicit LightBarWashingResultWgt(QWidget *parent = nullptr);

private:
    QLineEdit* m_pLineEditSN;
    QTableWidget* tableWidget;
    QWidget* createTableWidget();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

signals:

public slots:
    void slot_btn_query();
};

#endif // LIGHTBARWASHINGRESULTWGT_H
