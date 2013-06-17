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

class CEpoll : public CBase{
    enum{
        EPOLL_MAX_EVENT = 100000; //一次只处理这些事件
    };

    enum{
        DEFAULT_EVENT_SIZE = 1000
    };

    int m_hEpollFd;
    int m_iEventCount;
    struct epoll_event *m_pEvents;
    bool m_bEt;

public:
    CEpoll();
    ~CEpoll();

private:
    int ctrl(int hFd, long long llData, uint32_t nEvent, int iOption);

public:
    int create(bool bEt, int iDefaultEventSize);
    int add(int hFd, long long llData, uint32_t nEvent);
    int mod(int hFd, long long llData, uint32_t nEvent);
    int del(int hFd, long long llData, uint32_t nEvent);
    int wait(int millsecond);
    struct epoll_event *get(int iIndex);
};

#endif
