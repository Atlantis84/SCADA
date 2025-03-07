#ifndef SCREWMACHINEWGT_H
#define SCREWMACHINEWGT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include "passwidget.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "tcpcommonuse.h"
#include "screwmachinereadthread.h"
#include "httpbasedoncurl.h"
class ScrewMachineWgt : public QWidget
{
    Q_OBJECT

public:
    static ScrewMachineWgt* get_instance()
    {
        if(m_pObj == nullptr)
            m_pObj = new ScrewMachineWgt();
        return m_pObj;
    }

private:
    explicit ScrewMachineWgt(QWidget *parent = nullptr);

    static ScrewMachineWgt* m_pObj;

    ScrewMachineReadThread smReadThread;
    HttpBasedOnCurl* m_pHttpBasedOnCurl;

    QLineEdit* m_pEditScanner;
    QLineEdit* m_pEditCamera;

    PassWidget* m_pPassWgtPCBAMes;
    PassWidget* m_pPassWgtPrint;
    PassWidget* m_pPassWgtScrewMes;
    PassWidget* m_pPassWgtTcpState;

    QNetworkAccessManager* m_pMesAccessManager;
    QNetworkAccessManager* m_pMesAccessManagerSave;

    bool get_mes_info_by_sn(const QByteArray data);
    bool save_product_info_to_mes(const QByteArray data,const QList<QString> lstinfo);

    TcpCommonUse* m_pTcpCommonUse;

protected:
    void timerEvent(QTimerEvent *event);
    int m_pTimerID;

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

signals:

    void signal_mes_info_by_sn(const QByteArray& sn);
    void signal_product_info_to_mes(const QByteArray data,const QList<QString> lstinfo);
public slots:
    void process_plc_data(QByteArray data);
    void process_info_from_tcp_server(const QByteArray& data);
    void slot_test();
    void slot_mes_info_by_sn(const QByteArray& sn);
    void slot_product_info_to_mes(const QByteArray data,const QList<QString> lstinfo);
};

#endif // SCREWMACHINEWGT_H
