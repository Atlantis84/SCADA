#ifndef FTPSERVICE_H
#define FTPSERVICE_H

#include "Models/boardmodel.h"

#include <QObject>
#include <QRunnable>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QUrl>
#include <QTextCodec>
#include <QMutex>
#include <QList>
#include <QTimer>

class FtpService : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit FtpService();
public:
    void run();
    void push(BoardModel* boardModelLA,BoardModel* boardModelLB);
    void upload(BoardModel* boardModel,QString workcenterId);
    void taskSleep(int sleepTimeMs);
    void startService();
    void stopService();
    bool isStarted();
    bool isStoped();
private slots:
    void when_upload_timeout();
private:
    bool started=false;
    bool stoped=true;
    bool connected=false;
    bool disconnected=true;
    QString boardModelIdLA="";
    QString boardModelIdLB="";
    BoardModel* boardModelLA=0;
    BoardModel* boardModelLB=0;
    bool doneLAPushAction=true;
    bool doneLBPushAction=true;
    bool donePushAction=true;
    long lastPushBoardTestTimeMs=0;
    QTextCodec* textCodec=0;
    QNetworkAccessManager* manager=0;
    QEventLoop* eventLoop=0;
    QTimer* uploadTimer=0;
    QMutex uploadMutex;
    QList<QString> uploadFilePathes;
    QList<QString> uploadFtpFileNames;
};

#endif // FTPSERVICE_H
