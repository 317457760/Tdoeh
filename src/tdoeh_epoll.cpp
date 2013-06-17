/*************************************************************************************
 * 文件名: tdoeh_epoll.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月15日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_epoll.h"

CEpoll::CEpoll() : 
    m_bEt(true), 
    m_pEvents(NULL),
    m_hEpollFd(-1),
    m_iEventCount(0)
{
}

CEpoll::~CEpoll()
{
    if(m_pEvents != NULL){
        delete []m_pEvents;
    }
    if(m_hEpollFd != -1){
        close(m_hEpollFd);
    }
}

int CEpoll::create(bool bEt, int iDefaultEventSize)
{
    if(m_hEpollFd != -1){
        return E_SUCCESS;
    }
    if((m_hEpollFd = epoll_create1(EPOLL_CLOEXEC)) == -1){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    if(iDefaultEventSize > EPOLL_MAX_EVENT){
        m_iEventCount = EPOLL_MAX_EVENT;
    }
    if(iDefaultEventSize <= 0){
        m_iEventCount = DEFAULT_EVENT_SIZE;
    }
    m_iEventCount = iDefaultEventSize;

    m_bEt = bEt;
    m_pEvents = TDOEH_NEW epoll_event[m_iEventCount]; 
    if(m_pEvents == NULL){
        m_iEventCount = 0; 
        TDOEH_SET_ERROR_NUMBER_RETURN(CBase::E_NOMEMORY);
    }

    return E_SUCCESS;
}

int CEpoll::add(int hFd, long long llData, uint32_t nEvent)
{
    return ctrl(fd, data, event, EPOLL_CTL_ADD);
}

int CEpoll::mod(int hFd, long long llData, uint32_t nEvent)
{
    return ctrl(fd, data, event, EPOLL_CTL_ADD);
}

int CEpoll::del(int hFd, long long llData, uint32_t nEvent)
{
    return ctrl(fd, data, event, EPOLL_CTL_ADD);
}


int CEpoll::ctrl(int hFd, long long llData, uint32_t nEvent, int iOption)
{
    struct epoll_event ev;
    ev.data.u64 = llData;

    if(m_bEt){
        ev.events = nEvent | EPOLLET;
    }else{
        ev.events = nEvent;
    }

    if(epoll_ctl(m_hEpollFd, iOption, hFd, &ev) == -1){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}

int CEpoll::wait(int millsecond)
{
    int iReadyCount = epoll_wait(m_hEpollFd, m_pEvents, m_iEventCount, millsecond);
    if(iReadyCount == -1){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return iReadyCount;
}
