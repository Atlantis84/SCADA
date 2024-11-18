#ifndef MESSERVICE_H
#define MESSERVICE_H

#include "Models/boardmodel.h"
#include "Services/sqlserverservice.h"

#include <QObject>
#include <QRunnable>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTextCodec>
#include <QList>
#include <QMediaPlayer>

class MesService : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit MesService();
signals:
    void showMesMessage(QString message,QString message_code,bool warning);
    void showMesDialog(QString planCode,int planCount,int productCount);
    void showNgCodeDialog(QString title,QString sn,bool isLA,QString ngPointsDesc);
private slots:
    void playerStateChanged(QMediaPlayer::State);
    void when_scanner_getNewCode(QString code,bool isLA);
public:
    void run();
    void push(BoardModel* ngBoardModelLA,BoardModel* ngBoardModelLB);
    QString buildObjectJson(BoardModel *boardModel,QString boardId, QString workcenterId,QString code,bool isNG);
    QString getDefectTypeCode(int type);
    bool pushObjectJson(QString jsonMsg);
    QString buildNgPointsDesc(BoardModel *boardModel,QString boardId);
    bool pushBytes(QByteArray bytes);
    void taskSleep(int sleepTimeMs);
    void startService();
    void stopService();
    bool isStarted();
    bool isStoped();
    void exitEventLoop(int resultCode);
    void playWarnMedia(int volume);
    void setScannerNgCodeLA(QString code);
    void setScannerNgCodeLB(QString code);
private:
    bool started=false;
    bool stoped=true;
    bool connected=false;
    bool disconnected=true;
    QString lastNgBoardModelFirstIdLA="";
    QString lastNgBoardModelIdLA="";
    QString lastNgBoardModelFirstIdLB="";
    QString lastNgBoardModelIdLB="";
    QString lastBoardIdLA="";
    QString lastBoardIdLB="";
    QList<BoardModel*> ngBoardModelsLA;
    QList<BoardModel*> ngBoardModelsLB;
    QString scannerCodeLA="";
    QString scannerCodeLB="";
    QString scannerNgCodeLA="";
    QString scannerNgCodeLB="";
    bool doneNgPushActionLA=true;
    bool doneNgPushActionLB=true;
    bool waitingNgCodeLA=false;
    bool waitingNgCodeLB=false;
    QString jsonLANgPointsDesc="";
    QString jsonLBNgPointsDesc="";
    QTextCodec* textCodec=0;
    QEventLoop* eventLoop=0;
    QMediaPlayer* mediaPlayer=0;
    const QString ngCodeReplaceString="${{{{}}}}$";
    QString jsonLAOKPointsDesc="";
      QString jsonLBOKPointsDesc="";
  public:
      SqlserverService* sqlService=0;
      BoardModel *boardModeltmp;
};

#endif // MESSERVICE_H
