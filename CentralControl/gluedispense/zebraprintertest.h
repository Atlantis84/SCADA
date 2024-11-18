#ifndef ZEBRAPRINTERTEST_H
#define ZEBRAPRINTERTEST_H

#include <QObject>

class zebraPrinterTest : public QObject
{
    Q_OBJECT
public:
    zebraPrinterTest();
    int printrjtlabel(const char * zebraPrintername, const char * invdate,
        const char * listno, const char * sellerlistno, const char * supno, const char * supname);
};

#endif // ZEBRAPRINTERTEST_H
