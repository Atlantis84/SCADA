#include "logservice.h"
#include "iniservice.h"

#include <QDir>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QCoreApplication>

LogService::LogService(QObject *parent):QObject(parent)
{

}

LogService::~LogService()
{
}

LogService* LogService::bean=0;
bool LogService::enableShowLogMessage=false;

void LogService::traceLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();
    QString typeStr;
    QString positionStr;
    switch(type)
    {
        case QtDebugMsg:
            typeStr = QString("[DEBUG]");
            positionStr = QString("");
            break;
        case QtWarningMsg:
            typeStr = QString("[WARNING]");
            positionStr = QString("\r\n    @ %1 : %2").arg(QString(context.file)).arg(context.line);
            break;
        case QtCriticalMsg:
            typeStr = QString("[CRITICAL]");
            positionStr = QString("\r\n    @ %1 : %2").arg(QString(context.file)).arg(context.line);
            break;
        case QtFatalMsg:
            typeStr = QString("[FATAL]");
            positionStr = QString("\r\n    @ %1 : %2").arg(QString(context.file)).arg(context.line);
            break;
       default:break;
    }
    //日志文件夹
    QDir dir;
    QString logsDirName=QCoreApplication::applicationDirPath()+QDir::separator()+"logs";
    if(!dir.exists(logsDirName))
    {
        dir.mkdir(logsDirName);
    }
    // 设置输出信息格式
    QString logFilePath=logsDirName+QDir::separator()+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".log";
    QString dateTimeStr=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString message = QString("%1 %2 %3 %4").arg(dateTimeStr).arg(typeStr).arg(msg).arg(positionStr);
    // 显示
    if(LogService::enableShowLogMessage)
    {
        emit LogService::getInstance()->showLogMessage(message);
    }
    // 输出信息至文件中（读写、追加形式）
    if(IniService::App_EnableTraceLogToFile)
    {
        QFile file(logFilePath);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << "\r\n";
        file.flush();
        file.close();
    }
    // 解锁
    mutex.unlock();
}

LogService* LogService::getInstance()
{
    if(bean==0)
    {
        bean=new LogService();
    }
    return bean;
}

bool LogService::isEnableShowLogMessage()
{
    return LogService::enableShowLogMessage;
}

void LogService::setEnableShowLogMessage(bool enable)
{
    LogService::enableShowLogMessage=enable;
}
