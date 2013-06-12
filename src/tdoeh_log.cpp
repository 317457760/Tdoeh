/*************************************************************************************
 * 文件名: tdoeh_log.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月09日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <stdio.h>
#include "tdoeh_log.h"

namespace Tdoeh{

void CLog::printf(int nColor, const char *szFormat, ...)
{
    char szNewFormat[MAX_FORMAT_SIZE];

    switch(nColor){
        case GREEN:
            snprintf(szNewFormat, sizeof(szNewFormat), "\e[32m%s\e[0m", szFormat);
            break;
        case RED:
            snprintf(szNewFormat, sizeof(szNewFormat), "\e[31m%s\e[0m", szFormat);
            break;
        case YELLOW:
            snprintf(szNewFormat, sizeof(szNewFormat), "\e[33m%s\e[0m", szFormat);
            break;
        case BLUE:
            snprintf(szNewFormat, sizeof(szNewFormat), "\e[34m%s\e[0m", szFormat);
            break;
        default:
            snprintf(szNewFormat, sizeof(szNewFormat), "%s", szFormat);
            break;
    }

    va_list ap;
    va_start(ap, szFormat);
    vfprintf(stderr, szNewFormat, ap);
    va_end(ap);
}

}
