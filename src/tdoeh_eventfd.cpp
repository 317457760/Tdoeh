/*************************************************************************************
 * 文件名: tdoeh_eventfd.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月09日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <sys/eventfd.h>
#include <unistd.h>
#include "tdoeh_eventfd.h"

namespace Tdoeh{

CEventFd::CEventFd() : m_hFd(-1)
{
}

CEventFd::~CEventFd()
{
    if(m_hFd != -1){
        close();
    }
}

int CEventFd::open(int iFlags)
{
    m_hFd = eventfd(0, iFlags);
    if(m_hFd == -1){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}

int CEventFd::fd()
{
    return m_hFd;
}

int CEventFd::read(uint64_t *events)
{
    if(::read(m_hFd, events, sizeof(uint64_t)) != sizeof(uint64_t)){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}

int CEventFd::write(uint64_t events)
{
    if(::write(m_hFd, &events, sizeof(uint64_t)) != sizeof(uint64_t)){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}

int CEventFd::notify()
{
    uint64_t events = 1LL;
    if(::write(m_hFd, &events, sizeof(uint64_t)) != sizeof(uint64_t)){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}

void CEventFd::close()
{
    ::close(m_hFd);
}

}
