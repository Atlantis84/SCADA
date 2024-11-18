#include "iniconfigabstract.h"

QString IniConfigAbstract::INI_FILE_PATH = "";
QString IniConfigAbstract::INI_TOTAL_FILE_PATH = "";
QString IniConfigAbstract::App_LineCode = "";
QString IniConfigAbstract::App_LineName = "";
QString IniConfigAbstract::Push_HNMQ_Host = "";
int IniConfigAbstract::Push_HNMQ_Port = -1;
QString IniConfigAbstract::Push_HNMQ_ExchangeName = "";
QString IniConfigAbstract::Push_HNMQ_ExchangeType = "";
QString IniConfigAbstract::Push_HNMQ_VirtualHost = "";
bool IniConfigAbstract::Push_HNMQ_EnableAutoDelete = false;
bool IniConfigAbstract::Push_HNMQ_EnablePersistance = false;
QString IniConfigAbstract::Push_HNMQ_RouteKey = "";

QString IniConfigAbstract::Push_HNMQ_UName = "";
QString IniConfigAbstract::Push_HNMQ_UPwd = "";

IniConfigAbstract::IniConfigAbstract()
{

}

IniConfigAbstract::~IniConfigAbstract()
{

}
