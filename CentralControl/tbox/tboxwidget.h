#ifndef TBOXWIDGET_H
#define TBOXWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include "tboxresultwgt.h"
#include "tcpclient.h"
#include <QCheckBox>
class TBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TBoxWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

    bool copy_file_from_src_to_dest(const QString fileNameWithPath,const QString dstPath);
    void send_to_test_software(const QString fileNameWithPath,const QString dstPath,const int stationID);

    QLineEdit* lineEdit_SN;
    QLineEdit* lineEdit_FileName;
    QTextEdit* textEditMesInfo;
    TBoxResultWgt* tboxResultWgt;

    QString list_files_to_find_target(const QString path,QString partOfName,QString& filePath);
    bool remove_folder_content(const QString &folderDir);

    TcpClient* tcpClient1;
    TcpClient* tcpClient2;
    TcpClient* tcpClient3;
    TcpClient* tcpClient4;
    TcpClient* tcpClient5;
    TcpClient* tcpClient6;
    TcpClient* tcpClient7;
    TcpClient* tcpClient8;

    QCheckBox* cbStation1;
    QCheckBox* cbStation2;
    QCheckBox* cbStation3;
    QCheckBox* cbStation4;
    QCheckBox* cbStation5;
    QCheckBox* cbStation6;
    QCheckBox* cbStation7;
    QCheckBox* cbStation8;

    QList<int> lstStations;

signals:

public slots:
    void slot_rev_product_sn(const QString sn);
    void slot_rev_product_info_from_mes(const QString mesInfo,const QString orderNumber,const QString partOfInfo);
    void slot_rev_logs(const QString str);
    void slot_rev_result(int id);
    void slot_station1(int arg);
    void slot_station2(int arg);
    void slot_station3(int arg);
    void slot_station4(int arg);

    void slot_station5(int arg);
    void slot_station6(int arg);
    void slot_station7(int arg);
    void slot_station8(int arg);
};

#endif // TBOXWIDGET_H
