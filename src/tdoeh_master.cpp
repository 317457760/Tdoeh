/*************************************************************************************
 * 文件名: tdoeh_master.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月22日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <sys/epoll.h>
#include <sys/time.h>
#include "tdoeh_master.h"
#include "tdoeh_log.h"
using namespace std;

namespace Tdoeh{

int CMaster::initialize()
{
    m_iCount = 0;
    if(m_epoll.create(false, 100) != CBase::E_SUCCESS){
        CLog::printf(CLog::RED, "create epoll error:%s\n", m_epoll.getLastErrorMessage());
        return -1;
    }
    return 0;
}

int CMaster::setManagerServer(const char *szIp, int iPort)
{
    if(m_sockManager.create() < 0){
        CLog::printf(CLog::RED, "manager sock create error:%s\n", m_sockManager.getLastErrorMessage());
        return -1;
    }
    if(m_sockManager.bind(szIp, iPort) < 0){
        CLog::printf(CLog::RED, "manager sock bind error:%s\n", m_sockManager.getLastErrorMessage());
        return -1;
    }
    if(m_sockManager.listen(1) < 0){
        CLog::printf(CLog::RED, "manager sock bind error:%s\n", m_sockManager.getLastErrorMessage());
        return -1;
    }
    if(m_epoll.add(m_sockManager.fd(), IO_MANAGER_LISTEN, EPOLLIN | EPOLLRDHUP) != CBase::E_SUCCESS){
        CLog::printf(CLog::RED, "add to epoll error:%s\n", m_epoll.getLastErrorMessage());
        return -1;
    }
    return 0;
}

int CMaster::setServer(const char *szIp, int iPort)
{
    if(m_sockServer.create() < 0){
        CLog::printf(CLog::RED, "server sock create error:%s\n", m_sockServer.getLastErrorMessage());
        return -1;
    }
    if(m_sockServer.bind(szIp, iPort) < 0){
        CLog::printf(CLog::RED, "server sock bind error:%s\n", m_sockServer.getLastErrorMessage());
        return -1;
    }
    if(m_sockServer.listen(1000)){
        CLog::printf(CLog::RED, "server sock listen error:%s\n", m_sockServer.getLastErrorMessage());
        return -1;
    }
    if(m_epoll.add(m_sockServer.fd(), IO_SERVER_LISTEN, EPOLLIN | EPOLLRDHUP) != CBase::E_SUCCESS){
        CLog::printf(CLog::RED, "add to epoll error:%s\n", m_epoll.getLastErrorMessage());
        return -1;
    }
    return 0;
}


int CMaster::setNetThreadCount(int iCount)
{
    m_iNetThreadCount = iCount;
    return 0;
}

int CMaster::setWorkThreadCount(int iCount)
{
    m_iWorkThreadCount = iCount;
    return 0;
}

long long CMaster::getTimeMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + tv.tv_usec/1000);
}

int CMaster::start()
{
    //启动网络线程
    m_netThreads = new CNetThread[m_iNetThreadCount];
    for(int i = 0; i < m_iNetThreadCount; i++){
        m_netThreads[i].setMaster(this);
        m_netThreads[i].start();
    }

    //启动工作线程
    m_workThreads = new CWorkThread[m_iWorkThreadCount];
    for(int i = 0; i < m_iWorkThreadCount; i++){
        m_workThreads[i].setMaster(this);
        m_workThreads[i].start();
    }

    //添加时间事件
    struct STimeEvent timeEvent;
    timeEvent.llTimeLine = getTimeMs() + 1000;
    timeEvent.funEvent = funStatEvent;
    m_timeEvent.insert(timeEvent);

    while(true){
        long long timeNow = getTimeMs();
        long long waitTime = 1000;

        //处理时间事件,因为在处理函数里可能会插入新的事件,所以要每次取begin
        while(1){
            multiset<STimeEvent>::iterator it = m_timeEvent.begin();
            if(it == m_timeEvent.end()){
                break;
            }
            if(it->llTimeLine > timeNow){
                waitTime = it->llTimeLine - timeNow;
                break;
            }

            FUN_TIMEEVENT funEvent = it->funEvent;
            m_timeEvent.erase(it);

            funEvent(this);
        }

        if(waitTime > 1000){
            waitTime = 1000;
        }

        int iIoCount = m_epoll.wait(waitTime);
        //CLog::printf(CLog::GREEN, "get epoll count %d\n", iIoCount);
        for(int i = 0; i < iIoCount; i++){
            int iFd;
            uint32_t nFlag;
            uint32_t nEvent;
            int iReturn = m_epoll.get(i, &iFd, &nFlag, &nEvent);
            if(iReturn < 0){
                CLog::printf(CLog::RED, "get epoll event error %d\n", i);
                continue;
            }
            if(nEvent & EPOLLRDHUP){
                m_epoll.del(iFd, 0, nEvent);
                CLog::printf(CLog::RED, "EPOOLRDHUP\n");
                continue;
            }
            if(nEvent & EPOLLERR){
                CLog::printf(CLog::RED, "EPOOLRERR\n");
            }
            switch(nFlag){
                case IO_MANAGER_LISTEN:
                    ManagerListen(iFd, nEvent);
                    break;
                case IO_MANAGER_DATA:
                    ManagerData(iFd, nEvent);
                    break;
                case IO_SERVER_LISTEN:
                    ServerListen(iFd, nEvent);
                    break;
                case IO_MESSAGE_QUEUE:
                    MessageQueue(iFd, nEvent);
                    break;
            }
        }
    }

    return 0;
}

int CMaster::funStatEvent(CMaster *pThis)
{
    CLog::printf(CLog::RED, "stat event:count %d\n", pThis->m_iCount);
    pThis->m_iCount++;
    struct STimeEvent timeEvent;
    timeEvent.llTimeLine = pThis->getTimeMs() + 1000;
    timeEvent.funEvent = funStatEvent;
    pThis->m_timeEvent.insert(timeEvent);
    return 0;
}

void CMaster::ManagerListen(int iFd, uint32_t nEvent)
{
    if(nEvent & EPOLLIN){
        if(m_sockManager.accept(&iFd) < 0){
            CLog::printf(CLog::RED, "accept error:%s\n", m_sockManager.getLastErrorMessage());
            return;
        }
        if(m_epoll.add(iFd, IO_MANAGER_DATA, EPOLLIN | EPOLLRDHUP) != CBase::E_SUCCESS){
            CLog::printf(CLog::RED, "add to epoll error:%s\n", m_epoll.getLastErrorMessage());
            return;
        }
    }
}

void CMaster::ManagerData(int iFd, uint32_t nEvent)
{
    CSocket sock;
    if(sock.initialize(iFd, false) < 0){
        CLog::printf(CLog::RED, "initialize error:%s\n", sock.getLastErrorMessage());
        return;
    }

    char szBuffer[1024];
    int iLen = sock.recv(szBuffer, sizeof(szBuffer));
    if(iLen < 0){
        CLog::printf(CLog::RED, "recv error:%s\n", sock.getLastErrorMessage());
        return;
    }
    if(iLen == 0){
        if(m_epoll.del(iFd, 0, nEvent) < 0){
            CLog::printf(CLog::RED, "del error:%s\n", m_epoll.getLastErrorMessage());
            return;
        }
    }
    
    szBuffer[iLen] = '\0';
    CLog::printf(CLog::GREEN, "len:%d message:%s\n", iLen, szBuffer);
}

void CMaster::ServerListen(int iFd, uint32_t nEvent)
{
    CLog::printf(CLog::RED, "server listen\n");
}

void CMaster::MessageQueue(int iFd, uint32_t nEvent)
{
    CLog::printf(CLog::RED, "message queue\n");
}

}
