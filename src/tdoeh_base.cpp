/*************************************************************************************
 * 文件名: tdoeh_base.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月07日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "tdoeh_base.h"

namespace Tdoeh{

CBase::CBase()
{
    m_szErrorMessage[0] = '\0';
    m_iErrorNumber = E_SUCCESS;
}

CBase::~CBase()
{
}

int CBase::getLastErrorNumber()
{
    return m_iErrorNumber;
}

const char *CBase::getLastErrorMessage(int iErrorNumber)
{
    if(iErrorNumber == 0){
        iErrorNumber = m_iErrorNumber;
    }

    if(iErrorNumber == E_SUCCESS){
        snprintf(m_szErrorMessage, sizeof(m_szErrorMessage), "success");
        return m_szErrorMessage;
    }

    //linux系统api错误
    if(iErrorNumber > E_UNKNOWN){
        snprintf(m_szErrorMessage, sizeof(m_szErrorMessage), "%s", 
                    strerror(iErrorNumber));
        return m_szErrorMessage;
    }

    //类通用错误
    switch(iErrorNumber){
        case E_ARGUMENTS:
            snprintf(m_szErrorMessage, sizeof(m_szErrorMessage), 
                    "invalid argument");
            break;
        case E_NOMEMORY:
            snprintf(m_szErrorMessage, sizeof(m_szErrorMessage), 
                    "no enough memory");
            break;

    }

    //类自己定义错误
    return m_szErrorMessage;
}

}
