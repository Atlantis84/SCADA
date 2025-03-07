#include "tboxwidget.h"
#include <QIcon>
#include <QPainter>
#include <QPen>
#include "gdatafactory.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDirIterator>
#include <QMessageBox>
#include "imessagebox.h"
TBoxWidget::TBoxWidget(QWidget *parent) : QWidget(parent)
{
    tcpClient1 = new TcpClient();
    connect(tcpClient1,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));
    tcpClient2 = new TcpClient();
    connect(tcpClient2,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));
    tcpClient3 = new TcpClient();
    connect(tcpClient3,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));
    tcpClient4 = new TcpClient();
    connect(tcpClient4,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));

    tcpClient5 = new TcpClient();
    connect(tcpClient5,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));
    tcpClient6 = new TcpClient();
    connect(tcpClient6,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));
    tcpClient7 = new TcpClient();
    connect(tcpClient7,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));
    tcpClient8 = new TcpClient();
    connect(tcpClient8,SIGNAL(signal_rev_result(int)),this,SLOT(slot_rev_result(int)));

    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget|Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/icon/24x24.png"));

    tboxResultWgt = new TBoxResultWgt();

    QVBoxLayout* vAll = new QVBoxLayout();

    QGroupBox* grpScanInfo = new QGroupBox(u8"扫码信息");
    grpScanInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelSN = new QLabel(u8"产品SN:");
    labelSN->setStyleSheet("border:none;");
    lineEdit_SN = new QLineEdit();
    lineEdit_SN->setReadOnly(true);
    lineEdit_SN->setAlignment(Qt::AlignCenter);
    lineEdit_SN->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");

    hBox1->addWidget(labelSN);
    hBox1->addWidget(lineEdit_SN);
    grpScanInfo->setLayout(hBox1);

    QGroupBox* grpMesInfo = new QGroupBox(u8"日志信息");
    grpMesInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    textEditMesInfo = new QTextEdit();
    textEditMesInfo->setReadOnly(true);
    textEditMesInfo->setStyleSheet("color:rgb(0,0,0);background-color:rgb(0,0,0);font: 10pt 'LiSu'");
    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addWidget(textEditMesInfo);
    grpMesInfo->setLayout(hBox2);

    QGroupBox* grpFileInfo = new QGroupBox(u8"换产文件名称");
    grpFileInfo->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox3 = new QHBoxLayout();
    QLabel* labelFile = new QLabel(u8"文件名称:");
    labelFile->setStyleSheet("border:none;");
    lineEdit_FileName = new QLineEdit();
    lineEdit_FileName->setReadOnly(true);
    lineEdit_FileName->setAlignment(Qt::AlignCenter);
    lineEdit_FileName->setStyleSheet("background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    hBox3->addWidget(labelFile);
    hBox3->addWidget(lineEdit_FileName);
    grpFileInfo->setLayout(hBox3);

    QGroupBox* grpChangeResult = new QGroupBox(u8"换产结果");
    grpChangeResult->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox4 = new QHBoxLayout();
    tboxResultWgt = new TBoxResultWgt();
    hBox4->addWidget(tboxResultWgt);
    grpChangeResult->setLayout(hBox4);

    QGroupBox* grpStations = new QGroupBox(u8"请勾选换产机台");
    grpStations->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    QHBoxLayout* hBox5 = new QHBoxLayout();
    cbStation1 = new QCheckBox(u8"机台1");
    connect(cbStation1,SIGNAL(stateChanged(int)),this,SLOT(slot_station1(int)));
    cbStation1->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation2 = new QCheckBox(u8"机台2");
    connect(cbStation2,SIGNAL(stateChanged(int)),this,SLOT(slot_station2(int)));
    cbStation2->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation3 = new QCheckBox(u8"机台3");
    connect(cbStation3,SIGNAL(stateChanged(int)),this,SLOT(slot_station3(int)));
    cbStation3->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation4 = new QCheckBox(u8"机台4");
    connect(cbStation4,SIGNAL(stateChanged(int)),this,SLOT(slot_station4(int)));
    cbStation4->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation5 = new QCheckBox(u8"机台5");
    connect(cbStation5,SIGNAL(stateChanged(int)),this,SLOT(slot_station5(int)));
    cbStation5->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation6 = new QCheckBox(u8"机台6");
    connect(cbStation6,SIGNAL(stateChanged(int)),this,SLOT(slot_station6(int)));
    cbStation6->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation7 = new QCheckBox(u8"机台7");
    connect(cbStation7,SIGNAL(stateChanged(int)),this,SLOT(slot_station7(int)));
    cbStation7->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    cbStation8 = new QCheckBox(u8"机台8");
    connect(cbStation8,SIGNAL(stateChanged(int)),this,SLOT(slot_station8(int)));
    cbStation8->setStyleSheet("QCheckBox{border-color:rgba(0,0,0,0);color:rgba(0,0,0,150);font-family:KaiTi;font-size:12pt;}");
    hBox5->addStretch(50);
    hBox5->addWidget(cbStation1);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation2);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation3);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation4);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation5);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation6);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation7);
    hBox5->addStretch(20);
    hBox5->addWidget(cbStation8);
    hBox5->addStretch(50);
    grpStations->setLayout(hBox5);

    vAll->addWidget(grpStations);
    vAll->addWidget(grpScanInfo);
    vAll->addWidget(grpFileInfo);
    vAll->addWidget(grpChangeResult);
    vAll->addWidget(grpMesInfo);
    vAll->setStretch(0,1);
    vAll->setStretch(1,1);
    vAll->setStretch(2,1);
    vAll->setStretch(3,3);
    vAll->setStretch(4,5);
    this->setLayout(vAll);
}

void TBoxWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void TBoxWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    GDataFactory::get_factory()->stop_serial_port_tbox();
    GDataFactory::get_factory()->read_serial_number_tbox();
}

void TBoxWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    GDataFactory::get_factory()->stop_serial_port_tbox();
    tcpClient1->disconnect_from_server();
    tcpClient2->disconnect_from_server();
    tcpClient3->disconnect_from_server();
    tcpClient4->disconnect_from_server();
    tcpClient5->disconnect_from_server();
    tcpClient6->disconnect_from_server();
    tcpClient7->disconnect_from_server();
    tcpClient8->disconnect_from_server();
    this->lineEdit_SN->clear();
    this->lineEdit_FileName->clear();
    this->tboxResultWgt->set_change_result(1);

    cbStation1->setCheckState(Qt::CheckState::Unchecked);
    cbStation2->setCheckState(Qt::CheckState::Unchecked);
    cbStation3->setCheckState(Qt::CheckState::Unchecked);
    cbStation4->setCheckState(Qt::CheckState::Unchecked);
    cbStation5->setCheckState(Qt::CheckState::Unchecked);
    cbStation6->setCheckState(Qt::CheckState::Unchecked);
    cbStation7->setCheckState(Qt::CheckState::Unchecked);
    cbStation8->setCheckState(Qt::CheckState::Unchecked);
}

QString TBoxWidget::list_files_to_find_target(const QString path,QString partOfName,QString& filePath)
{
    QString strFileName="";

    QStringList list;
    QString dirPath = path;
    QDirIterator it(dirPath,QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::AllDirs,QDirIterator::Subdirectories);

    while (it.hasNext()) {
        bool findResult = false;
        it.next();
        if(it.fileInfo().suffix() == "FlcConfig")
        {
            if(it.fileInfo().fileName().contains(partOfName))
            {
                findResult = true;
                strFileName = it.fileInfo().fileName();
                filePath = it.fileInfo().filePath();
            }
        }
        if(findResult)
            break;
        else
            continue;
    }
    return strFileName;
}

void TBoxWidget::slot_rev_product_sn(const QString sn)
{
    if(this->lstStations.size() == 0)
    {
//        QMessageBox msgBox;
//        msgBox.setStyleSheet("QMessageBox{background-color:rgb(255,255,255);}");
//        msgBox.findChild<QLabel*>("qt_msgbox_label")->setStyleSheet("color:rgb(255,255,255);");
//        msgBox.warning(this,"警告信息",u8"请选择换产的机台!");
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"请选择换产机台!");
        return;
    }

    tboxResultWgt->set_change_result(0);
    tboxResultWgt->repaint();
    this->lineEdit_SN->clear();
    this->lineEdit_SN->setText(sn);

    GDataFactory::get_mes_process()->exec_http_get_tbox(QUrl::toPercentEncoding(sn));
}

bool TBoxWidget::remove_folder_content(const QString &folderDir)
{
    QDir dir(folderDir);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists())  {return false;}
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(fileList.size()>0)
    {
        int infoNum=fileList.size();
        for(int i=infoNum-1;i>=0;i--)
        {
            curFile=fileList[i];
            if(curFile.isFile())
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
            if(curFile.isDir())
            {
                QDir dirTemp(curFile.filePath());
                QFileInfoList fileList1=dirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size()==0)
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else
                {
                    for(int j=0;j<fileList1.size();j++)
                    {
                        if(!(fileList.contains(fileList1[j])))
                            fileList.append(fileList1[j]);
                    }
                }
            }
        }
    }
//    dir.removeRecursively();
    return true;
}

void TBoxWidget::slot_rev_product_info_from_mes(const QString mesInfo, const QString orderNumber,const QString partOfInfo)
{
    QLOG_INFO()<<u8"当前产品Mes信息为:"+mesInfo;

    QStringList lstStr;
    lstStr = partOfInfo.split("-");
    QString fileName;

    for (int i=0;i<lstStr.size();i++) {
        if(i == lstStr.size()-1)
        {
            fileName.append(lstStr[i].left(2));
            fileName.append("-");
        }
        else {
            fileName.append(lstStr[i]);
            fileName.append("-");
        }
    }

    fileName.append(orderNumber);
    QString filePath = "";
    QLOG_INFO()<<u8"换产文件匹配名字:"<<fileName;
    QLOG_INFO()<<u8"换产文件源目录为:"<<GDataFactory::get_factory()->get_config_para("TBOX_SRC_FILE_PATH");
    QString str = list_files_to_find_target(GDataFactory::get_factory()->get_config_para("TBOX_SRC_FILE_PATH"),fileName,filePath);
    QString FileName = str;

    this->lineEdit_FileName->clear();
    this->lineEdit_FileName->setText(str);

    QLOG_INFO()<<filePath;

    if(str != "")
    {
        QString dstPath = GDataFactory::get_factory()->get_config_para("TBOX_DST_FILE_PATH");
        remove_folder_content(dstPath);
        if(copy_file_from_src_to_dest(filePath,GDataFactory::get_factory()->get_config_para("TBOX_DST_FILE_PATH")+str.prepend('/')))
        {
            for(int i=1;i<9;i++)
            {
                if(lstStations.contains(i))
                    send_to_test_software(FileName,dstPath,i);
            }
//            if(lstStations.contains(1))
//                send_to_test_software(FileName,dstPath,1);
//            if(lstStations.contains(2))
//                send_to_test_software(FileName,dstPath,2);
//            if(lstStations.contains(3))
//                send_to_test_software(FileName,dstPath,3);
//            if(lstStations.contains(4))
//                send_to_test_software(FileName,dstPath,4);
        }
    }
    else
    {
        QLOG_INFO()<<u8"未找到换产文件!";
    }
}

void TBoxWidget::slot_rev_logs(const QString str)
{
    this->textEditMesInfo->append(str);
}

void TBoxWidget::slot_rev_result(int id)
{
    if(this->lstStations.size() == 0)
    {
        this->tboxResultWgt->set_change_result(1);
        this->tboxResultWgt->repaint();
    }
    else {
        if(lstStations.contains(id))
        {
            lstStations.removeOne(id);
            if(id == 1)
                cbStation1->setCheckState(Qt::CheckState::Unchecked);
            if(id == 2)
                cbStation2->setCheckState(Qt::CheckState::Unchecked);
            if(id == 3)
                cbStation3->setCheckState(Qt::CheckState::Unchecked);
            if(id == 4)
                cbStation4->setCheckState(Qt::CheckState::Unchecked);

            if(id == 5)
                cbStation5->setCheckState(Qt::CheckState::Unchecked);
            if(id == 6)
                cbStation6->setCheckState(Qt::CheckState::Unchecked);
            if(id == 7)
                cbStation7->setCheckState(Qt::CheckState::Unchecked);
            if(id == 8)
                cbStation8->setCheckState(Qt::CheckState::Unchecked);

            if(lstStations.size() == 0)
            {
                this->tboxResultWgt->set_change_result(1);
                this->tboxResultWgt->repaint();
            }
        }
    }
}

void TBoxWidget::slot_station1(int arg)
{
    if(arg == 0)
    {
//        tcpClient1->disconnect_from_server();
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(1);
        }
    }
    else
    {
//        tcpClient1->disconnect_from_server();
//        if(tcpClient1->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION1"),
//                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
//        {
            lstStations.push_back(1);
//        }
//        else
//        {
//            IMessageBox* msgBox = new IMessageBox(3);
//            msgBox->warning(u8"请确保机台1网络连接成功与测试软件是否开启!");
//        }
    }
}

void TBoxWidget::slot_station2(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(2);
        }
    }
    else
        lstStations.push_back(2);
}

void TBoxWidget::slot_station3(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(3);
        }
    }
    else
        lstStations.push_back(3);
}

void TBoxWidget::slot_station4(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(4);
        }
    }
    else
        lstStations.push_back(4);
}

void TBoxWidget::slot_station5(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(5);
        }
    }
    else
        lstStations.push_back(5);
}

void TBoxWidget::slot_station6(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(6);
        }
    }
    else
        lstStations.push_back(6);
}

void TBoxWidget::slot_station7(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(7);
        }
    }
    else
        lstStations.push_back(7);
}

void TBoxWidget::slot_station8(int arg)
{
    if(arg == 0)
    {
        if(lstStations.length() == 0)
            ;
        else {
            lstStations.removeOne(8);
        }
    }
    else
        lstStations.push_back(8);
}

bool TBoxWidget::copy_file_from_src_to_dest(const QString fileNameWithPath, const QString dstPath)
{
    bool r = false;
    if(!QFile::copy(fileNameWithPath,dstPath))
    {
        QLOG_WARN()<<u8"换产文件拷贝失败!";
        r = false;
    }
    else
    {
        QLOG_WARN()<<u8"换产文件拷贝成功!";
        r = true;
    }
    return r;
}

void TBoxWidget::send_to_test_software(const QString fileNameWithPath,const QString dstPath,const int stationID)
{
    if(stationID == 1)
    {
        tcpClient1->disconnect_from_server();
        QLOG_INFO()<<GDataFactory::get_factory()->get_config_para("TBOX_STATION1");
        QLOG_INFO()<<GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT");
        if(tcpClient1->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION1"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient1->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station1 SUCCESS!";
            QString strData;
//            strData.append("\"reType\":\"TBOX_TEST\",");天线测试
            strData.append("\"reType\":\"TBOX_PCBA\",");//PCBA测试
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");

            QLOG_INFO()<<strData;
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;

            tcpClient1->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station1 FAILED!";
    }
    else if(stationID == 2)
    {
        tcpClient2->disconnect_from_server();
        if(tcpClient2->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION2"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient2->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station2 SUCCESS!";
//            QJsonObject  root;
//            root.insert("reType","TBOX_TEST");
//            root.insert("reProjectID",fileNameWithPath);
//            root.insert("reProjectPath",dstPath);

//            QJsonDocument doc_data(root);
//            QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);

            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;

            tcpClient2->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station2 FAILED!";
    }
    else if(stationID == 3)
    {
        tcpClient3->disconnect_from_server();
        if(tcpClient3->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION3"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient3->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station3 SUCCESS!";
//            QJsonObject  root;
//            root.insert("reType","TBOX_TEST");
//            root.insert("reProjectID",fileNameWithPath);
//            root.insert("reProjectPath",dstPath);

//            QJsonDocument doc_data(root);
//            QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;
            tcpClient3->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station3 FAILED!";
    }
    else if(stationID == 4)
    {
        tcpClient4->disconnect_from_server();
        if(tcpClient4->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION4"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient4->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station4 SUCCESS!";
            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;
            tcpClient4->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station4 FAILED!";
    }
    else if(stationID == 5)
    {
        tcpClient5->disconnect_from_server();
        if(tcpClient5->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION5"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient5->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station5 SUCCESS!";
            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;
            tcpClient5->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station5 FAILED!";
    }
    else if(stationID == 6)
    {
        tcpClient6->disconnect_from_server();
        if(tcpClient6->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION6"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient6->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station6 SUCCESS!";
            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;
            tcpClient6->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station6 FAILED!";
    }
    else if(stationID == 7)
    {
        tcpClient7->disconnect_from_server();
        if(tcpClient7->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION7"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient7->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station7 SUCCESS!";
            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;
            tcpClient7->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station7 FAILED!";
    }
    else if(stationID == 8)
    {
        tcpClient8->disconnect_from_server();
        if(tcpClient8->connect_to_tcp_server(GDataFactory::get_factory()->get_config_para("TBOX_STATION8"),
                                             GDataFactory::get_factory()->get_config_para("TBOX_STATION_PORT").toInt()))
        {
            tcpClient8->set_identity_id(stationID);
            QLOG_INFO()<<"connect to test software Station8 SUCCESS!";
            QString strData;
            strData.append("\"reType\":\"TBOX_PCBA\",");
            strData.append("\"reProjectID\":\"");
            strData.append(fileNameWithPath);
            strData.append("\",");
            strData.append("\"reProjectPath\":\"");
            strData.append(dstPath);
            strData.append("\"");
            QByteArray request_data = strData.toLatin1();
            QLOG_INFO()<<request_data;
            tcpClient8->send_to_tcp_server(request_data);
        }
        else
            QLOG_WARN()<<"connect to test software Station8 FAILED!";
    }
}
