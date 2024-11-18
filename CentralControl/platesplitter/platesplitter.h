#ifndef PLATESPLITTER_H
#define PLATESPLITTER_H

#include <QWidget>
#include <QTextEdit>
#include "passwidget.h"
class PlateSplitter : public QWidget
{
    Q_OBJECT
public:
    explicit PlateSplitter(QWidget *parent = nullptr);

private:
    QTextEdit* m_pLogEdit;

    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);
    void timerEvent(QTimerEvent *event);

    int m_pTimerID;
    PassWidget* m_pPassWgt;

signals:

public slots:
    void slot_rev_logs(const QString msg);
};

#endif // PLATESPLITTER_H
