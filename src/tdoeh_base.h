/*************************************************************************************
 * 文件名: tdoeh_base.h
 * 作  者: 思凡
 * 日  期: 2013年06月07日
 * 描  述: 所有类的基类
 * 修  改: 
 * 说  明: 因为会设置最后错误信息，如果在多线程中使用可能会得到错误的错误信息
 *************************************************************************************/
#ifndef __TDOEH_BASE_H__
#define __TDOEH_BASE_H__

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <new>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <linux/types.h>
#include "tdoeh_noncopyable.h"

namespace Tdoeh{

//当类调用自己的成员函数时使用
#define TDOEH_RETURN_IF_ERROR(X) \
    do{ \
        if((X) < 0){ \
            return m_iErrorNumber; \
        } \
    }while(0)

//设置错误信息和错误码并返回
#define TDOEH_SET_ERROR_RETURN(X, Y, ...) \
    do{ \
        m_iErrorNumber = X; \
        ::snprintf(m_szErrorMessage, sizeof(m_szErrorMessage), Y, ##__VA_ARGS__); \
        return m_iErrorNumber; \
    }while(0)

//设置错误码并返回,用于系统调用错误或者通用错误
#define TDOEH_SET_ERROR_NUMBER_RETURN(X) \
    do{ \
        m_iErrorNumber = X; \
        return m_iErrorNumber; \
    }while(0)

#define TDOEH_NEW \
    new(std::nothrow)


class CBase : public CNoncopyable{
protected:
    char m_szErrorMessage[1024];    //最长错误信息为1024
    int m_iErrorNumber;

public:
    //基本错误
    enum{
        E_SUCCESS = 0,
        
        E_UNKNOWN = -5000,      //E_UNKNOWN为系统中错误的最大值
        E_ARGUMENTS = E_UNKNOWN - 1,    //非法参数
        E_NOMEMORY = E_UNKNOWN - 2,     //内存不足

        //类自定义错误最大值
        E_MAX_ERROR_NUMBER = -10000
    };

    enum{
        INVALID_HANDLE = -1
    };

public:
    CBase();
    virtual ~CBase();

public:
    int getLastErrorNumber();
    const char *getLastErrorMessage(int iErrorNumber = 0);
};

}
#endif
