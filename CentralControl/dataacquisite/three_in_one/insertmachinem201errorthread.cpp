#include "insertmachinem201errorthread.h"
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "QsLog.h"
#include <QDate>
#include <QDir>
InsertMachineM201ErrorThread* InsertMachineM201ErrorThread::m_pInstance = nullptr;
InsertMachineM201ErrorThread::InsertMachineM201ErrorThread()
{

}

QString InsertMachineM201ErrorThread::traverseFolder(const QString &path,const QString &month,const QString &day) {
    QString tmpDay = day;
    if(tmpDay.length() == 1)
        tmpDay.prepend('0');
    QString strfilenamewithpath = path;
    strfilenamewithpath.append("/");
    QDir dir(path);
    if (!dir.exists()) {
        QLOG_WARN() << "The directory does not exist!";
        return strfilenamewithpath;
    }

    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot); // 设置过滤器，列出所有文件和子目录，但不包括.和..
    dir.setSorting(QDir::Size | QDir::Reversed); // 可选择排序方式

    QFileInfoList list = dir.entryInfoList(); // 获取文件列表
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString tmpstr = month;
        tmpstr.append(" ");
        tmpstr.append(tmpDay);
        if(fileInfo.fileName().contains(tmpstr))
            strfilenamewithpath.append(fileInfo.fileName());
    }
//    return strfilenamewithpath;
    QFile file(strfilenamewithpath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QLOG_ERROR()<<strfilenamewithpath;
        QLOG_ERROR()<< "can not open the log file";
    }

    QTextStream in(&file);
    in.setCodec("Big5");
    QList<QStringList> tmplst;

    QString line;
    while (!in.atEnd())
    {
        line = in.readLine();
    }
    file.close();
    return line;
}

void InsertMachineM201ErrorThread::run()
{
    while (1) {
        QDate currentDate = QDate::currentDate();

        QString strMonth;
        switch(currentDate.month())
        {
        case 1:
            strMonth = "Jan";
            break;
        case 2:
            strMonth = "Feb";
            break;
        case 3:
            strMonth = "Mar";
            break;
        case 4:
            strMonth = "Apr";
            break;
        case 5:
            strMonth = "May";
            break;
        case 6:
            strMonth = "Jun";
            break;
        case 7:
            strMonth = "Jul";
            break;
        case 8:
            strMonth = "Aug";
            break;
        case 9:
            strMonth = "Sep";
            break;
        case 10:
            strMonth = "Oct";
            break;
        case 11:
            strMonth = "Nov";
            break;
        case 12:
            strMonth = "Dec";
            break;
        }

        //insert-machine1
        QString path = "";
        path = "//781E6V16V3103D/2025/";
//        path = "//127.0.0.1/InsertMachine1Logs/";
        path.append(strMonth);
        QString line = traverseFolder(path,strMonth,QString::number(currentDate.day()));

        QString errorcode = "";
        if(line.contains("Code:"))
        {
            QStringList lststr = line.split('-');
            errorcode = lststr[2];
        }
        emit signal_current_error(1,errorcode);

        QEventLoop eventloop;
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        //insert-machine2
        path = "//781E6V16V3102D/2025/";
//        path = "//127.0.0.1/InsertMachine2Logs/";
        path.append(strMonth);
        line = traverseFolder(path,strMonth,QString::number(currentDate.day()));

        errorcode = "";
        if(line.contains("Code:"))
        {
            QStringList lststr = line.split('-');
            errorcode = lststr[2];
        }
        emit signal_current_error(2,errorcode);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        //insert-machine3
        path = "//10.50.130.246/2025/";
//        path = "//127.0.0.1/InsertMachine3Logs/";
        path.append(strMonth);
        line = traverseFolder(path,strMonth,QString::number(currentDate.day()));

        errorcode = "";
        if(line.contains("Code:"))
        {
            QStringList lststr = line.split('-');
            errorcode = lststr[2];
        }
        emit signal_current_error(3,errorcode);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        //insert-machine4
        path = "//10.50.130.247/2025/";
//        path = "//127.0.0.1/InsertMachine4Logs/";
        path.append(strMonth);
        line = traverseFolder(path,strMonth,QString::number(currentDate.day()));

        errorcode = "";
        if(line.contains("Code:"))
        {
            QStringList lststr = line.split('-');
            errorcode = lststr[2];
        }
        emit signal_current_error(4,errorcode);
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
