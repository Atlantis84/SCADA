#ifndef ZEBRAPRINTERSERVICE_H
#define ZEBRAPRINTERSERVICE_H

#include <QObject>

class zebraPrinterService : public QObject
{
    Q_OBJECT
public:
    explicit zebraPrinterService(QObject *parent = nullptr);

    bool printExcuteFunc(QString printStrSN, QString printFilePath, QString &msg);
};

#endif // ZEBRAPRINTER_H
