#ifndef PADDATASTRUCTURE_H
#define PADDATASTRUCTURE_H

#include <map>
#include <list>
#include <string>
#pragma pack(1)
using namespace std;
struct CheckContentData
{
    char* Serial_Number;
    char* Station_Code;
    char* Equipment_Code;
    char* Equipment_Name;
    char* Check_Type;
    char* Field1;
    char* Field2;
    char* Field3;
};

struct PadCheckFullData{
    char* Message_ID;
    char* Source_System;
    char* Message_Type;
    char* Message_Time;
    std::list<string> Key_Bar_Code[1];
    CheckContentData Content_Data;
};

typedef struct ResultDetailData
{
    char* Key_Bar_Code;
    char* Before_Value;
    char* After_Value;
    char* Item_Value;
    char* Item_QM_status;
    char* Field1;
    char* Field2;
    char* Field3;
}ResultDetailDataApp;

struct ResultContentData
{
    char* Serial_Number;
    char* Station_Code;
    char* Equipment_Code;
    char* Equipment_Name;
    char* QM_Status;
    std::list<ResultDetailData> Content_Detail;
};

struct ResultContentData1
{
    char* Serial_Number;
    char* Station_Code;
    char* Equipment_Code;
    char* Equipment_Name;
    char* QM_Status;
    std::list<ResultDetailData> Content_Detail[1];
};

struct PadResultFullData{
    char* Message_ID;
    char* Source_System;
    char* Message_Type;
    char* Message_Time;
    ResultContentData Result_Content;
};

struct PadResultFullData1{
    char* Message_ID;
    char* Source_System;
    char* Message_Type;
    char* Message_Time;
    ResultContentData1 Result_Content;
};

struct TestStructData
{
    int test1;
    unsigned short test2;
    short test3;
};
#pragma pack()
#endif // PADDATASTRUCTURE_H
