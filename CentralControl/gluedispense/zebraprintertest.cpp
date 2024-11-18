#include "zebraprintertest.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

zebraPrinterTest::zebraPrinterTest()
{
    const char * zebraPrintername = "ZDesigner ZT610-600dpi ZPL";
    const char * invdate = "2020-06-01";
    const char * listno = "DEP2020601ZE003";
    const char * sellerlistno = "XSCK202005042038";
    const char * supno = "G1006";
    const char * supname = "测试有限公司";

    int result = printrjtlabel(zebraPrintername, invdate,
        listno, sellerlistno, supno, supname);
}


int zebraPrinterTest::printrjtlabel(const char * zebraPrintername, const char * invdate,
    const char * listno, const char * sellerlistno, const char * supno, const char * supname)
{
    LPTSTR szPrinterName;
    LPBYTE lpData;
    DWORD dwCount;
    HANDLE     hPrinter;
    DOC_INFO_1 DocInfo;
    DWORD      dwJob;
    DWORD      dwBytesWritten;


    wchar_t * zebraPrinternamew = new wchar_t[strlen(zebraPrintername) + 1];
    mbstowcs_s(NULL, zebraPrinternamew, strlen(zebraPrintername) + 1, zebraPrintername, strlen(zebraPrintername));

    //    lpData = (LPBYTE)("^XA \n^LH0,0 \n^FO300,203\n^BY3,2.4,50\n^B3N,Y,,Y,N\n^FDABC123^FS\n^XZ   ");
    //    dwCount = strlen("^XA \n^LH0,0 \n^FO300,203\n^BY3,2.4,50\n^B3N,Y,,Y,N\n^FDABC123^FS\n^XZ   ");

    /*lpData = (LPBYTE)("^XA^LH0,0^XZ\n "
        "^XA^FO35,35^A0,50,50^FD\n "
        "^FS\n "
        "^CI26\n "
        "^SEE:GB18030.DAT\n "
        "^CW1,E:SIMSUN.FNT\n "
        "^FO250,10^A0,25,25^FDZebra Technologies^FS\n "
        "^FO250,40^A0,25,25^FDVernon Hills, IL^FS\n "
        "^FO250,70^A0,25,25^FD847-913-2259^FS\n "
        "^FO250,100^A1N,25,25^FD中文^FS \n"
        "^XZ");
    dwCount = strlen("^XA^LH0,0^XZ\n "
        "^XA^FO35,35^A0,50,50^FD\n "
        "^FS\n "
        "^CI26\n "
        "^SEE:GB18030.DAT\n "
        "^CW1,E:SIMSUN.FNT\n "
        "^FO250,10^A0,25,25^FDZebra Technologies^FS\n "
        "^FO250,40^A0,25,25^FDVernon Hills, IL^FS\n "
        "^FO250,70^A0,25,25^FD847-913-2259^FS\n "
        "^FO250,100^A1N,25,25^FD中文^FS \n"
        "^XZ");*/

    std::string printcontents = "^XA^LH0,0^XZ\n "
        "^XA^FO35,35^A0,50,50^FD\n "
        "^FS\n "
        "^CI26\n "
        "^SEE:GB18030.DAT\n "
        "^CW1,E:SIMSUN.FNT\n ";
    //    "^FO250,10^A0,25,25^FDZebra Technologies^FS\n "
    //    "^FO250,40^A0,25,25^FDVernon Hills, IL^FS\n "
    //    "^FO250,70^A0,25,25^FD847-913-2259^FS\n "
    std::string invdatestr = "^FO250,10^A1N,25,25^FD日期：" + std::string(invdate);
    invdatestr = invdatestr + "^FS\n ";

    std::string listnostr = "^FO250,40^A1N,25,25^FD单号：" + std::string(listno);
    listnostr = listnostr + "^FS\n ";

    std::string sellerlistnostr = "^FO250,70^A1N,25,25^FD对单：" + std::string(sellerlistno);
    sellerlistnostr = sellerlistnostr + "^FS\n ";

    std::string supnostr = "^FO250,100^A1N,25,25^FD编码：" + std::string(supno);
    supnostr = supnostr + "^FS\n ";

    std::string supnamestr = "^FO250,130^A1N,25,25^FD供应商：^FS\n ";

    int j = 130;
    std::string tmpsupname;
    std::string accumlatesupname;

    for (int i = 0; i < strlen(supname); i++)
    {
        accumlatesupname = accumlatesupname + std::string(supname, i, 1);

        if (i % 19 == 0 && i > 0)
        {

            tmpsupname = tmpsupname + "^FO350," + std::to_string(j);
            tmpsupname = tmpsupname + "^A1N,25,25^FD";
            tmpsupname = tmpsupname + accumlatesupname;
            tmpsupname = tmpsupname + "^FS\n ";
            accumlatesupname = "";

            j = j + 30;


        }
    }

    if (strlen(supname) % 19 != 0)
    {
        tmpsupname = tmpsupname + "^FO350," + std::to_string(j);
        tmpsupname = tmpsupname + "^A1N,25,25^FD";
        tmpsupname = tmpsupname + accumlatesupname;
        tmpsupname = tmpsupname + "^FS\n ";
        accumlatesupname = "";
        j = j + 30;
    }


    std::string endstr = "^XZ";

    printcontents = printcontents + invdatestr;
    printcontents = printcontents + listnostr;
    printcontents = printcontents + sellerlistnostr;
    printcontents = printcontents + supnostr;
    printcontents = printcontents + supnamestr;
    printcontents = printcontents + tmpsupname;
    printcontents = printcontents + endstr;


    lpData = (LPBYTE)printcontents.c_str();
    dwCount = strlen(printcontents.c_str());

    // Need a handle to the printer.
    //szPrinterName = _T("ZDesigner GK888t (EPL)");
    szPrinterName = (LPTSTR)zebraPrinternamew;
    if (!OpenPrinter(szPrinterName, &hPrinter, NULL))
    {
        return -1;
    }

    // Fill in the structure with info about this "document."
    QString pstr = "My Document";
    DocInfo.pDocName = (LPWSTR)pstr.toStdString().c_str();
    DocInfo.pOutputFile = NULL;
    QString pDatastr = "RAW";
    DocInfo.pDatatype = (LPWSTR)pDatastr.toStdString().c_str();
    // Inform the spooler the document is beginning.
    if ((dwJob = StartDocPrinter(hPrinter, 1, (LPBYTE)&DocInfo)) == 0)
    {

        ClosePrinter(hPrinter);
        return -2;
    }
    // Start a page.
    if (!StartPagePrinter(hPrinter))
    {

        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        return -3;
    }
    // Send the data to the printer.
    if (!WritePrinter(hPrinter, lpData, dwCount, &dwBytesWritten))
    {

        EndPagePrinter(hPrinter);
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        return -4;
    }
    // End the page.
    if (!EndPagePrinter(hPrinter))
    {

        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        return -5;
    }
    // Inform the spooler that the document is ending.
    if (!EndDocPrinter(hPrinter))
    {

        ClosePrinter(hPrinter);
        return -6;
    }
    // Tidy up the printer handle.
    ClosePrinter(hPrinter);
    // Check to see if correct number of bytes were written.
    if (dwBytesWritten != dwCount)
    {

        return -7;
    }
    return 0;
}
