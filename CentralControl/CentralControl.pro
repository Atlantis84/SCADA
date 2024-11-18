#-------------------------------------------------
#
# Project created by QtCreator 2021-10-25T18:26:45
#
#-------------------------------------------------

QT += webenginewidgets core gui sql network serialport axcontainer xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CentralControl
TEMPLATE = app

MOC_DIR = .\tmp
RCC_DIR = .\tmp
OBJECTS_DIR = .\tmp
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8

SOURCES += \
    T109/screwmachinereadthread.cpp \
    T109/screwmachinewgt.cpp \
        boardstylewidget.cpp \
#        centralservice/control.cpp \
#        centralservice/md5.cpp \
#    centralservice/mysqlxwapper.cpp \
#        centralservice/processor.cpp \
#        centralservice/processor.modify.cpp \
#        centralservice/processor.query.cpp \
#        centralservice/testdbthread.cpp \
#        centralservice/testplanthread.cpp \
        centralwidget.cpp \
        circlewidget.cpp \
        commonwaiting/commonwaitingwgt.cpp \
    dataacquisite/dataacquisitionconfig.cpp \
    dataacquisite/gdataacquisitionfactory.cpp \
    dataacquisite/tcon/finsudpprocessbase.cpp \
    dataacquisite/tcon/tcondataacquisitionaoithread.cpp \
    dataacquisite/tcon/tcondataacquisitionpackagethread.cpp \
    dataacquisite/tcon/tcondataacquisitonthread.cpp \
    dataacquisite/tcon/tconfinsudpprocessaoi.cpp \
    dataacquisite/tcon/tconfinsudpprocesspackage.cpp \
    dataacquisite/tcon/tconfinsudpprocessteststation.cpp \
    dataacquisite/tcon/tconmelsecsubboard.cpp \
    dataacquisite/tcon/tconmelsecupperboard.cpp \
    dataacquisite/three_in_one/aoibusinessm201obj.cpp \
    dataacquisite/three_in_one/collectinsertmachineerrorsthread.cpp \
    dataacquisite/three_in_one/httpthreeinonemsg.cpp \
    dataacquisite/three_in_one/insertmachinebusinessm201obj.cpp \
    dataacquisite/three_in_one/insertmachinem201errorthread.cpp \
    dataacquisite/three_in_one/insertmachinem201thread.cpp \
    dataacquisite/three_in_one/m201udpbusinessobj.cpp \
    dataacquisite/three_in_one/plcbusinessm201obj.cpp \
    dataacquisite/three_in_one/plcbusinessm201thread.cpp \
    dataacquisite/three_in_one/tvtestbusinessm201obj.cpp \
    dataacquisite/three_in_one/ui/insertmachineerrorswgt.cpp \
    dataacquisite/three_in_one/ui/onebuttonchangem201wgt.cpp \
    dataacquisite/three_in_one/ui/testdivideboardwgt.cpp \
    dataacquisite/three_in_one/ui/threeinonegluewgt.cpp \
    dataacquisite/three_in_one/ui/wavesolderingm201wgt.cpp \
    dataacquisite/three_in_one/ui/wavesolderingm202wgt.cpp \
    dataacquisite/three_in_one/ui/wavesolderingm203wgt.cpp \
    dataacquisite/three_in_one/ui/wavesolderingwgt.cpp \
    dataacquisite/three_in_one/wavesolderingm201thread.cpp \
    dataacquisite/three_in_one/wavesolderingm202thread.cpp \
    dataacquisite/ui/aoistationwgt.cpp \
    dataacquisite/ui/ballwidget.cpp \
    dataacquisite/ui/dataacquisitonerrorconfigwgt.cpp \
    dataacquisite/ui/divideboardwgt.cpp \
    dataacquisite/ui/leftdivideboardwgt.cpp \
    dataacquisite/ui/leftupboardwgt.cpp \
    dataacquisite/ui/lineoverviewwgt.cpp \
    dataacquisite/ui/linestatewidget.cpp \
    dataacquisite/ui/packagestationwgt.cpp \
    dataacquisite/ui/rightdivideboardwgt.cpp \
    dataacquisite/ui/rightupboardwgt.cpp \
    dataacquisite/ui/teststationwgt.cpp \
    dataacquisite/ui/upboardconfigwgt.cpp \
    dataacquisite/ui/upboardwgt.cpp \
    dataacquisite/ui/workstationinfowgt.cpp \
        datacommserial.cpp \
        dragtreewidget.cpp \
        elecequiptestplanwgt_pd.cpp \
        gdatafactory.cpp \
    gluedispense/ccdkeypartwidget.cpp \
    gluedispense/clearoldsigncommservice.cpp \
    gluedispense/fpcpressfitwgt.cpp \
        gluedispense/gluedispensewgt.cpp \
    gluedispense/glueweighcommservice.cpp \
    gluedispense/mainboardbindwgt.cpp \
    gluedispense/pressstationserialcom.cpp \
    gluedispense/redflagtestcommandwgt.cpp \
    gluedispense/repairstationwgt.cpp \
    gluedispense/stationconfigwgt.cpp \
    gluedispense/testlinegluedispensewgt.cpp \
    gluedispense/zebraprinterservice.cpp \
        historydisplaywgt.cpp \
        httpbusiness/httpaoimsg.cpp \
        httpbusiness/httpmsgprocess.cpp \
    httpbusiness/httppackagemsg.cpp \
        httpbusiness/httpregistermsg.cpp \
        httpbusiness/httptconmsg.cpp \
        httpbusiness/httptvmsg.cpp \
        httpbusiness/httpvisionmsg.cpp \
        httpcpps/httpconnectionhandler.cpp \
        httpcpps/httpconnectionhandlerpool.cpp \
        httpcpps/httpcookie.cpp \
        httpcpps/httpglobal.cpp \
        httpcpps/httplistener.cpp \
        httpcpps/httprequest.cpp \
        httpcpps/httprequesthandler.cpp \
        httpcpps/httpresponse.cpp \
        httpcpps/httpsession.cpp \
        httpcpps/httpsessionstore.cpp \
        httpcpps/staticfilecontroller.cpp \
        httpcpps/template.cpp \
        httpcpps/templatecache.cpp \
        httpcpps/templateloader.cpp \
        httptestserver/httptestsever.cpp \
        imessagebox.cpp \
        leftwidget.cpp \
        lightbar/lightbarcleanwgt.cpp \
        lightbar/lightbarnewproductwgt.cpp \
        lightbar/lightbarwashingresultwgt.cpp \
        lightbar/logindialog.cpp \
        lightbar/resultwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        mesprocess.cpp \
        platesplitter/platesplitter.cpp \
        plcwakeupdlg.cpp \
        qslogwidget.cpp \
        result/passwidget.cpp \
        rightheadwidget.cpp \
        rightlabel.cpp \
        rightmainwidget.cpp \
        rightmainwindow.cpp \
        rightquerywidget.cpp \
        stationmonitorwgt_pd.cpp \
        stationtabwgt_pd.cpp \
        tbox/tboxresultwgt.cpp \
        tbox/tboxwidget.cpp \
        tcpclient/tcpclient.cpp \
        testplanwgt.cpp \
        testwidget.cpp \
        title/TitleBar.cpp \
        title/titlewidget.cpp \
        tvplanwgt.cpp \
        userwgt/usertabwgt.cpp \
    waitcountwgt.cpp \
        welcomewidget.cpp

HEADERS += \
    T109/screwmachinereadthread.h \
    T109/screwmachinewgt.h \
        boardstylewidget.h \
#        centralservice/GlobDef.h \
#        centralservice/md5.h \
#    centralservice/mysqlxwapper.h \
#        centralservice/procact.h \
#        centralservice/processor.h \
#        centralservice/testdbthread.h \
#        centralservice/testplanthread.h \
#    centralservice/threadpool.h \
        centralwidget.h \
        circlewidget.h \
        commonwaiting/commonwaitingwgt.h \
    dataacquisite/dataacquisiteheaders.h \
    dataacquisite/dataacquisitionconfig.h \
    dataacquisite/gdataacquisitionfactory.h \
    dataacquisite/tcon/finsudpprocessbase.h \
    dataacquisite/tcon/lineoverviewwgt.h \
    dataacquisite/tcon/tconacquisiton_global.h \
    dataacquisite/tcon/tcondataacquisitionaoithread.h \
    dataacquisite/tcon/tcondataacquisitionpackagethread.h \
    dataacquisite/tcon/tcondataacquisitonthread.h \
    dataacquisite/tcon/tconfinsudpprocessaoi.h \
    dataacquisite/tcon/tconfinsudpprocesspackage.h \
    dataacquisite/tcon/tconfinsudpprocessteststation.h \
    dataacquisite/tcon/tconmelsecsubboard.h \
    dataacquisite/tcon/tconmelsecupperboard.h \
    dataacquisite/three_in_one/aoibusinessm201obj.h \
    dataacquisite/three_in_one/collectinsertmachineerrorsthread.h \
    dataacquisite/three_in_one/httpthreeinonemsg.h \
    dataacquisite/three_in_one/insertmachinebusinessm201obj.h \
    dataacquisite/three_in_one/insertmachinem201errorthread.h \
    dataacquisite/three_in_one/insertmachinem201thread.h \
    dataacquisite/three_in_one/m201udpbusinessobj.h \
    dataacquisite/three_in_one/plcbusinessm201obj.h \
    dataacquisite/three_in_one/plcbusinessm201thread.h \
    dataacquisite/three_in_one/tvtestbusinessm201obj.h \
    dataacquisite/three_in_one/ui/insertmachineerrorswgt.h \
    dataacquisite/three_in_one/ui/onebuttonchangem201wgt.h \
    dataacquisite/three_in_one/ui/testdivideboardwgt.h \
    dataacquisite/three_in_one/ui/threeinonegluewgt.h \
    dataacquisite/three_in_one/ui/wavesolderingm201wgt.h \
    dataacquisite/three_in_one/ui/wavesolderingm202wgt.h \
    dataacquisite/three_in_one/ui/wavesolderingm203wgt.h \
    dataacquisite/three_in_one/ui/wavesolderingwgt.h \
    dataacquisite/three_in_one/wavesolderingm201thread.h \
    dataacquisite/three_in_one/wavesolderingm202thread.h \
    dataacquisite/ui/aoistationwgt.h \
    dataacquisite/ui/ballwidget.h \
    dataacquisite/ui/dataacquisitonerrorconfigwgt.h \
    dataacquisite/ui/divideboardwgt.h \
    dataacquisite/ui/leftdivideboardwgt.h \
    dataacquisite/ui/leftupboardwgt.h \
    dataacquisite/ui/lineoverviewwgt.h \
    dataacquisite/ui/linestatewidget.h \
    dataacquisite/ui/packagestationwgt.h \
    dataacquisite/ui/rightdivideboardwgt.h \
    dataacquisite/ui/rightupboardwgt.h \
    dataacquisite/ui/teststationwgt.h \
    dataacquisite/ui/upboardconfigwgt.h \
    dataacquisite/ui/upboardwgt.h \
    dataacquisite/ui/workstationinfowgt.h \
        datacommserial.h \
        dragtreewidget.h \
        elecequiptestplanwgt_pd.h \
        gdatafactory.h \
    global/commonheaders.h \
    global/dataacquisitiondefines.h \
    global/plcaddress.h \
    global/uiheaders.h \
    gluedispense/ccdkeypartwidget.h \
    gluedispense/clearoldsigncommservice.h \
    gluedispense/fpcpressfitwgt.h \
        gluedispense/gluedispensewgt.h \
    gluedispense/glueweighcommservice.h \
    gluedispense/mainboardbindwgt.h \
    gluedispense/paddatastructure.h \
    gluedispense/pressstationserialcom.h \
    gluedispense/redflagtestcommandwgt.h \
    gluedispense/repairstationwgt.h \
    gluedispense/stationconfigwgt.h \
    gluedispense/testlinegluedispensewgt.h \
    gluedispense/zebraprinterservice.h \
        historydisplaywgt.h \
        httpbusiness/httpaoimsg.h \
        httpbusiness/httpmsgprocess.h \
    httpbusiness/httppackagemsg.h \
        httpbusiness/httpregistermsg.h \
        httpbusiness/httptconmsg.h \
        httpbusiness/httptvmsg.h \
        httpbusiness/httpvisionmsg.h \
        httpheaders/httpconnectionhandler.h \
        httpheaders/httpconnectionhandlerpool.h \
        httpheaders/httpcookie.h \
        httpheaders/httpglobal.h \
        httpheaders/httplistener.h \
        httpheaders/httprequest.h \
        httpheaders/httprequesthandler.h \
        httpheaders/httpresponse.h \
        httpheaders/httpsession.h \
        httpheaders/httpsessionstore.h \
        httpheaders/staticfilecontroller.h \
        httpheaders/template.h \
        httpheaders/templatecache.h \
        httpheaders/templateglobal.h \
        httpheaders/templateloader.h \
        httptestserver/httptestsever.h \
        imessagebox.h \
        leftwidget.h \
        lightbar/lightbarcleanwgt.h \
        lightbar/lightbarnewproductwgt.h \
        lightbar/lightbarwashingresultwgt.h \
        lightbar/logindialog.h \
        lightbar/resultwidget.h \
        mainwindow.h \
        mesprocess.h \
        platesplitter/platesplitter.h \
        plcwakeupdlg.h \
        qslogwidget.h \
        result/passwidget.h \
        rightheadwidget.h \
        rightlabel.h \
        rightmainwidget.h \
        rightmainwindow.h \
        rightquerywidget.h \
        stationmonitorwgt_pd.h \
        stationtabwgt_pd.h \
        tbox/tboxresultwgt.h \
        tbox/tboxwidget.h \
        tcpclient/tcpclient.h \
        testplanwgt.h \
        testwidget.h \
        title/TitleBar.h \
        title/titlewidget.h \
        tvplanwgt.h \
        userwgt/usertabwgt.h \
    waitcountwgt.h \
        welcomewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += .\title
INCLUDEPATH += .\httpheaders
#INCLUDEPATH += .\include\curl-7.80.0
INCLUDEPATH += .\include\jsoncpp\json
INCLUDEPATH += .\include\mysql
INCLUDEPATH += .\include
INCLUDEPATH += .\include\QtMqtt
INCLUDEPATH += .\result
INCLUDEPATH += .\httptestserver
INCLUDEPATH += .\udpservice
INCLUDEPATH += .\lightbar
INCLUDEPATH += .\userwgt
INCLUDEPATH += .\centralservice
INCLUDEPATH += .\platesplitter
INCLUDEPATH += .\tbox
INCLUDEPATH += .\tcpclient
INCLUDEPATH += .\commonwaiting
INCLUDEPATH += ..\FinsUDPService
INCLUDEPATH += .\gluedispense
INCLUDEPATH += .\httpbusiness
INCLUDEPATH += .\dataacquisite\tcon
INCLUDEPATH += .\dataacquisite\ui
INCLUDEPATH += .\dataacquisite\three_in_one
INCLUDEPATH += .\dataacquisite
INCLUDEPATH += ..\QsLog
INCLUDEPATH += ..\DMDBAccess
INCLUDEPATH += .\global
INCLUDEPATH += ..\QMqServiceCommonUse
INCLUDEPATH += ..\QMqServiceSrc
INCLUDEPATH += ..\QMqServiceSrc\rabbitmq-c\include
INCLUDEPATH += .\libmodbus
INCLUDEPATH += ..\QMelsecUdpClient
INCLUDEPATH += ..\TcpCommonUse
INCLUDEPATH += ..\..\ModbusClientDll
INCLUDEPATH += .\T109
INCLUDEPATH += .\dataacquisite\three_in_one\ui
INCLUDEPATH += ..\HttpBasedOnCurl

DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
DEFINES += HTTP_ONLY
DEFINES += CURL_STATICLIB
LIBS += -lws2_32 -lwldap32 -lCrypt32 -ladvapi32 -lWininet

LIBS += -Ldll -lws2_32
LIBS += -lWinspool

LIBS += -L$$PWD\lib\x64\ -lQsLog2
LIBS += -L$$PWD\lib\x64\ -lDMDBAccess
LIBS += -L$$PWD\lib\x64\ -lFinsUDPService
#LIBS += -L$$PWD\lib\x64\ -lMelsecLinkPro
LIBS += -L$$PWD\lib\x64\ -lQMqServiceSrc
LIBS += -L$$PWD\lib\x64\ -lQMelsecUdpClient
LIBS += -L$$PWD\lib\x64\ -lQt5Mqtt
LIBS += -L$$PWD\lib\rabbitmq-c\lib\ -lrabbitmq.4
LIBS += -L$$PWD\lib\rabbitmq-c\lib\ -llibrabbitmq.4
LIBS += -L$$PWD\lib\xml-rpc\x64 -llibxmlrpc -llibxmlrpc_abyss -llibxmlrpc_abyss++ \
                                -llibxmlrpc_client -llibxmlrpc_server -llibxmlrpc_server_abyss \
                                -llibxmlrpc_server_cgi -llibxmlrpc_server_w32httpsys \
                                -llibxmlrpc_util -llibxmlrpc_util++ -llibxmlrpc_xmlparse \
                                -llibxmlrpc_xmltok -llibxmlrpc++
LIBS += -L$$PWD\lib\jsoncpp -ljsoncpp_static
LIBS += -L$$PWD\lib\mysql\lib64 -lmysqlcppconn8
LIBS += -L$$PWD\lib\x64\ -lTcpCommonUse
LIBS += -L$$PWD\lib\x64\ -lModbusClientDll
LIBS += -L$$PWD\lib\x64\ -lQMqServiceCommonUse

LIBS += -L$$PWD\lib\x64\ -lHttpBasedOnCurl

RESOURCES += \
    rc.qrc

DISTFILES +=

DEFINES += UNICODE
DEFINES += UMBCS
QMAKE_CXXFLAGS -= -Zc:strictStrings



