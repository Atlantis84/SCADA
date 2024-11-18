#ifndef THREEINONEINICONFIG_H
#define THREEINONEINICONFIG_H

#include "iniconfigabstract.h"
class ThreeInOneIniConfig : public IniConfigAbstract
{
private:
    ThreeInOneIniConfig();
    static ThreeInOneIniConfig* m_pInstance;

public:
    void load_ini_config();
    static ThreeInOneIniConfig* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new ThreeInOneIniConfig();
        return m_pInstance;
    }

};

#endif // THREEINONEINICONFIG_H
