#include "zebraprinterservice.h"
#include "imessagebox.h"
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QDateTime>
#include <gdatafactory.h>
#include <QsLog.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <QsLog.h>

zebraPrinterService::zebraPrinterService(QObject *parent) : QObject(parent)
{

}

//---------------------------------------------------------------------------------------------
//控制打印指令
bool zebraPrinterService::printExcuteFunc(QString printStrSN, QString printFilePath, QString &msg)
{
    bool bSuccessful = false;
    //QString strLine;
    bool isFindReplace = false;
    IMessageBox* msgBox=new IMessageBox(3);
    LPTSTR szPrinterName;
    LPBYTE lpData;
    DWORD dwCount;
    HANDLE     hPrinter;
    DOC_INFO_1 DocInfo;
    DWORD      dwJob;
    DWORD      dwBytesWritten;

    QLOG_WARN()<<"printStrSN: " << printStrSN;

    if (printStrSN.length() > 0){

        QFile fileFind;
        if (false == fileFind.exists(printFilePath)){
            //提示警告
            //msgBox->error(u8"打印模板文件不存在");
            msg = u8"打印模板文件不存在";
            return false;
        }
        QString csTempFile = QCoreApplication::applicationDirPath() + "/TempFile.prn";
        QFile fSourceFile(printFilePath);
        bSuccessful = fSourceFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QFile fTempFile(csTempFile);
        bSuccessful = fTempFile.open(QIODevice::WriteOnly | QIODevice::Text);

        QDateTime time = QDateTime::currentDateTime();
        QString Datestr = time.toString("yyyy-MM-dd");
        //msgBox->error(csTempFile);
        std::string printcontents;
        while (!fSourceFile.atEnd())
        {
            QByteArray line = fSourceFile.readLine();
            QString strLine(line);
            if (strLine.contains(GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_SN_INFO")) == true)
            {
                //printStrSN.insert(printStrSN.length()-2, ">6");
                QLOG_WARN()<<"RED_FLAG_PRINT_SN_INFO: " << GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_SN_INFO");
                strLine.replace(GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_SN_INFO"), printStrSN);
                isFindReplace = true;
            }

            strLine += "\n";
            QTextStream txtOutput(&fTempFile);
            txtOutput << strLine << endl;

            printcontents = printcontents + strLine.toStdString();
        }

        fSourceFile.close();
        fTempFile.close();

        if (!isFindReplace){
            //msgBox->error(u8"模板文件匹配字符错误，请重新设置模板匹配字符！");
            msg = u8"模板文件匹配字符错误，请重新设置模板匹配字符！";
            //AfxMessageBox(_T("模板文件匹配字符错误，请重新设置模板匹配字符！"), MB_ICONSTOP);
            return false;
        }

        lpData = (LPBYTE)printcontents.c_str();
        dwCount = strlen(printcontents.c_str());

        // Need a handle to the printer.
        //szPrinterName = _T("ZDesigner GK888t (EPL)");
        ;
        szPrinterName = (LPTSTR)GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_PORT").utf16();
        if (!OpenPrinter(szPrinterName, &hPrinter, NULL))
        {
            return -1;
        }

        // Fill in the structure with info about this "document."
        QString pstr = "My Document";
        DocInfo.pDocName = TEXT("My Document");
        DocInfo.pOutputFile = NULL;
        DocInfo.pDatatype = TEXT("RAW");
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
        return bSuccessful;

//        QString strPortName("////.//" + GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_PORT"));//cmdExec
//        //bSuccessful = copyFileToPath(csTempFile, strPortName,true);
//        //bSuccessful = QFile::copy(csTempFile, strPortName);
//        //SetDlgItemText(IDC_EDIT_PCBSN, _T(""));

//        //调用打印机打印
//        //m_pEditPCBSN.SetFocus();
//        QProcess process;
//        QString command="copy "+csTempFile+ " " + GDataFactory::get_factory()->get_config_para("RED_FLAG_PRINT_PORT");
//        command=command.replace("/","\\");
//        //msgBox->error(command);
//        qDebug()<<u8"命令是:"<<command<<endl;
//        process.start("cmd.exe",QStringList()<<"/c"<<command);

//        process.waitForFinished();
//        QString strTemp=QString::fromLocal8Bit(process.readAllStandardOutput());
//            qDebug()<<strTemp;
//        if (strTemp != u8"已复制         1 个文件。\r\n"){
//            //msgBox->error(u8"打印失败，请检查打印机端口！");
//            //AfxMessageBox(_T("打印失败，请检查打印机端口！"), MB_ICONSTOP);
//            //return bSuccessful;
//            msg = u8"打印失败，请检查打印机端口！";
//            bSuccessful = false;
//        }

//        return bSuccessful;
    }

    return false;
}
