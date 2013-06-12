/*************************************************************************************
 * 文件名: tdoeh_file.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月08日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "tdoeh_file.h"
namespace Tdoeh{

ssize_t CFile::size(char *szFileName)
{
    struct stat st;
    if(stat(szFileName, &st) < 0){
        return SYSTEM_ERROR; 
    }

    return st.st_size;
}

ssize_t CFile::write(char *szFileName, 
            const char *pBuffer, int nSize, ssize_t nStartPos)
{
    int flags = O_CREAT|O_WRONLY|O_LARGEFILE;
    if(nStartPos == 0){
        flags |= O_TRUNC;
    }
    if(nStartPos == -1){
        flags |= O_APPEND;
    }
    
    int hHandle = ::open(szFileName, flags, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if(hHandle < 0){
        return SYSTEM_ERROR; 
    }

    if((nStartPos > 0) && (::lseek(hHandle, SEEK_SET, nStartPos) < 0)){
        return SYSTEM_ERROR;
    }

    ssize_t iReturn = ::write(hHandle, pBuffer, nSize);

    ::close(hHandle);
    return iReturn;
}

ssize_t CFile::read(char *szFileName, 
            char *pBuffer, int nSize, ssize_t nStartPos)
{
    int hHandle = ::open(szFileName, O_RDONLY|O_LARGEFILE);
    if(hHandle < 0){
        return SYSTEM_ERROR; 
    }

    if((nStartPos > 0) && (::lseek(hHandle, SEEK_SET, nStartPos) < 0)){
        return SYSTEM_ERROR;
    }

    ssize_t iReturn = ::read(hHandle, pBuffer, nSize);

    ::close(hHandle);
    return iReturn;
}

int CFile::exist(char *szFileName)
{
    struct stat st;
    if(stat(szFileName, &st) < 0){
        if(errno == ENOENT){
            return FILE_NO_EXIST;
        }
        return SYSTEM_ERROR; 
    }

    return FILE_EXIST;
}

int CFile::remove(char *szFileName)
{
    if(unlink(szFileName) < 0 && errno != ENOENT){
        return SYSTEM_ERROR;
    }

    return SUCCESS;
}

}
