/*************************************************************************************
 * 文件名: tdoeh_master.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月22日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_master.h"
namespace Tdoeh{

int CMaster::initialize()
{
    m_iServerCount = 0;
    if(m_epoll.create(true, 100) != CBase::E_SUCCESS){
        CLog::printf(CLog::RED, "create epoll error:%s\n", m_epoll.getLastErrorMessage());
        return -1;
    }
    return 0;
}

CMaster::setManagerServer(string strIp, int iPort)
{
    m_sockManager.create(strIp, iPort);
    if(m_epoll.add(m_sockManager.fd(), IO_MANAGER_LISTEN, EPOLLIN | EPOLLRDHUP) != CBase::E_SUCCESS){
        CLog::printf(CLog::RED, "add to epoll error:%s\n", m_epoll.getLastErrorMessage());
        return -1;
    }
    return 0;
}

int CMaster::setServer(string strIp, int iPort, int iType)
{
    CSocket sock;
    m_sockServer.push_back(sock);
    m_sockServer[m_sockServer.size() - 1].create(strIp, iPort, iType);
    if(m_epoll.add(m_sockServer[m_sockServer.size() - 1].fd(), IO_SERVER_LISTEN, EPOLLIN | EPOLLRDHUP) != CBase::E_SUCCESS){
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
    timeEvent.llTimeLine = pThis->getTimeMs() + 1000;
    timeEvent.funEvent = funStatEvent;
    pThis->m_timeEvent.insert(timeEvent);

    while(true){
        long long timeNow = getTimeMs();
        long long waitTime = 1000;

        //处理时间事件,因为在处理函数里可能会插入新的事件,所以要每次取begin
        while(1){
            multiset<STimeEvent>::iterator it = m_timeEvent.begin();
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
        for(int i = 0; i < iIoCount; i++){
            struct CEpoll::epoll_event *event = m_epoll.get(i);
            switch(){
                case IO_MANAGER_LISTEN:
                    break;
                case IO_MANAGER_DATA:
                    break;
                case IO_SERVER_LISTEN:
                    break;
                case IO_MESSAGE_QUEUE:
                    break;
            }
        }
    }

    return 0;
}

static int CMaster::funStatEvent(CMasger *pThis)
{
    CLog::printf("stat event:count %d\n", pThis->m_iCount);
    struct STimeEvent timeEvent;
    timeEvent.llTimeLine = pThis->getTimeMs() + 1000;
    pThis->m_timeEvent.insert(timeEvent);
    return 0;
}

}
