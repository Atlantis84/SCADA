#ifndef MAINSERVICE_H
#define MAINSERVICE_H

#include "boardmodel.h"
#include "sqlserverservice.h"
#include "mqservice.h"
#include "scannerservice.h"
#include "boardmodel.h"
#include "componentmodel.h"
#include "statusmodel.h"
#include "usermodel.h"
#include "errortypemodel.h"
#include "messervice.h"
#include "ftpservice.h"

#include <QRunnable>
#include <QList>
#include <QObject>

namespace Service {
class MainService;
}

class MainService : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit MainService();
signals:
    void showUiData(BoardModel* lastBoardModel,//
                    StatusModel* statusModel);
public:
    void run();
    void taskSleep(int sleepTimeMs);
    void startService();
    void stopService();
    bool isStarted();
    bool isStoped();
    void setLoginUserModel(UserModel* userModel);
public:
    SqlserverService* sqlService=0;
    MQService* mqService=0;
    ScannerService* scannerService=0;
    MesService* mesService=0;
    FtpService* ftpService=0;
private:
    UserModel* loginUserModel=0;
    bool started=false;
    bool stoped=true;
    BoardModel* lastBoardModel;
    StatusModel* statusModel;
    QList<ErrorTypeModel*> errorTypeModels;
    BoardModel* lastBoardModelLA=0;
    StatusModel* statusModelLA=0;
    BoardModel* lastBoardModelLB=0;
    StatusModel* statusModelLB=0;
};

#endif // MAINSERVICE_H
