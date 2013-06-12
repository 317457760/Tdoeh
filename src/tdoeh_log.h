/*************************************************************************************
 * 文件名: tdoeh_log.h
 * 作  者: 思凡
 * 日  期: 2013年06月09日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_LOG_H__
#define __TDOEH_LOG_H__

#include "tdoeh_base.h"

namespace Tdoeh{

class CLog : public CBase{
public:
    enum{
        MAX_LOG_SIZE = 1024,
        MAX_FORMAT_SIZE = 100,
    };

    enum{
        GREEN = 1,
        RED = 2,
        YELLOW = 3,
        BLUE = 4
    };

    static void printf(int nColor, const char *szFormat, ...);
};

}

#endif
