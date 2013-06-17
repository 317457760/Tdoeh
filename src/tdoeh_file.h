/*************************************************************************************
 * 文件名: tdoeh_file.h
 * 作  者: 思凡
 * 日  期: 2013年06月08日
 * 描  述: 文件操作
 * 修  改: 
 * 注  意: 只适合简单操作，每次都会打开关闭文件,
 *         都为static函数，所以不会有lasterror
 *************************************************************************************/
#ifndef __TDOEH_FILE_H__
#define __TDOEH_FILE_H__

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

namespace Tdoeh{

class CFile
{
    enum{
        SUCCESS = 0,
        FILE_EXIST = 1,
        FILE_NO_EXIST = 0,
        SYSTEM_ERROR = -1
    };

public: 
    static ssize_t size(char *szFileName);

    static ssize_t write(char *szFileName, 
            const char *pBuffer, int nSize, ssize_t nStartPos = 0);

    static ssize_t read(char *szFileName, 
            char *pBuffer, int nSize, ssize_t nStartPos = 0);

    static int exist(char *szFileName);

    static int remove(char *szFileName);
};

}

#endif
