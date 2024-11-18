#include "collectinsertmachineerrorsthread.h"
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "QsLog.h"
#include "gdataacquisitionfactory.h"
#include <QSqlQuery>
#include <QDir>
CollectInsertMachineErrorsThread* CollectInsertMachineErrorsThread::m_pInstance = nullptr;
CollectInsertMachineErrorsThread::CollectInsertMachineErrorsThread()
{
    m_pInsertMachineSign = false;
    m_pStationID = -1;
    m_pSpecifyDay = 0;
}

QString traverseFolder(const QString &path,const QString &month,const QString &day) {
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
    return strfilenamewithpath;
}

void CollectInsertMachineErrorsThread::run()
{
    while(1)
    {
        if(m_pInsertMachineSign)
        {
            m_pInsertMachineSign = !m_pInsertMachineSign;

            QDate currentDate = QDate::currentDate();
            QDate previousDate = currentDate.addDays(m_pSpecifyDay);
            QString strMonth;
            switch(previousDate.month())
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
            QString path = "";
            if(m_pStationID == 1)
                path = "//781E6V16V3103D/2024/";
//                path = "//127.0.0.1/InsertMachine1Logs/";
            else if(m_pStationID == 2)
                path = "//781E6V16V3102D/2024/";
//                path = "//127.0.0.1/InsertMachine2Logs/";
            else if(m_pStationID == 3)
                path = "//10.50.130.246/2024/";
//                path = "//127.0.0.1/InsertMachine3Logs/";
            else
                path = "//10.50.130.247/2024/";
//                path = "//127.0.0.1/InsertMachine4Logs/";

            path.append(strMonth);
            QString strOpenFileName = traverseFolder(path,strMonth,QString::number(previousDate.day()));
            QLOG_WARN()<<strOpenFileName;
            QFile file(strOpenFileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QLOG_ERROR()<< "can not open the log file";
            }

            QTextStream in(&file);
            in.setCodec("Big5");

            QList<QStringList> tmplst;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if(line.contains("Code:"))
                {
                    QStringList lststr = line.split('-');
                    tmplst.push_back(lststr);
//                    emit signal_insert_errors_to_db(lststr);
                }
            }
            emit signal_send_errors_to_ui(tmplst);
            file.close();
        }
        QEventLoop eventloop;
        QTimer::singleShot(1000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
