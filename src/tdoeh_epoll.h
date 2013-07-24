/*************************************************************************************
 * 文件名: tdoeh_epoll.h
 * 作  者: 思凡
 * 日  期: 2013年06月13日
 * 描  述: 对于epoll的封装
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_EPOLL_H__
#define __TDOEH_EPOLL_H__

#include "tdoeh_base.h"

namespace Tdoeh{

class CEpoll : public CBase{
    enum{
        EPOLL_MAX_EVENT = 100000 //一次最多只处理这些事件
    };

    enum{
        DEFAULT_EVENT_SIZE = 1000
    };

    bool m_bEt;
    struct epoll_event *m_pEvents;
    int m_hEpollFd;
    int m_iEventCount;
    int m_iReadyEventCount;

public:
    CEpoll();
    ~CEpoll();

private:
    int ctrl(int hFd, uint32_t nData, uint32_t nEvent, int iOption);

public:
    int create(bool bEt, int iDefaultEventSize);
    int add(int hFd, uint32_t nData, uint32_t nEvent);
    int mod(int hFd, uint32_t nData, uint32_t nEvent);
    int del(int hFd, uint32_t nData, uint32_t nEvent);
    int wait(int millsecond);
    int get(int iIndex, int *pFd, uint32_t *pData, uint32_t *pEvent);
};

}

#endif
