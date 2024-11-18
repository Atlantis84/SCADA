#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <QtMsgHandler>
#include <QMessageLogContext>
#include <QString>
#include <QObject>

namespace Service {
class LogService;
}

class LogService:public QObject
{
    Q_OBJECT

public:
    explicit LogService(QObject *parent=0);
    ~LogService();
public:
    static void traceLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static LogService* getInstance();
    static bool isEnableShowLogMessage();
    static void setEnableShowLogMessage(bool enable);
signals:
    void showLogMessage(QString msg);
private:
    static LogService* bean;
    static bool enableShowLogMessage;
};

#endif // LOGSERVICE_H
