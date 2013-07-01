/*************************************************************************************
 * 文件名: tdoeh_master.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月22日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_master.h"

int CMaster::initialize()
{
    m_iServerCount = 0;
    m_epoll.create(true, 100);
    return 0;
}

CMaster::setManagerServer(string strIp, int iPort)
{
    m_sockManager.create(strIp, iPort);
    m_epoll.add(m_sockManager.fd(), IO_MANAGER_LISTEN, EPOLLIN|EPOLLRDHUP);
    return 0;
}

int CMaster::setServer(string strIp, int iPort, int iType)
{

    CSocket sock;
    m_sockServer.push_back(sock);
    m_sockServer[m_sockServer.size() - 1].create(strIp, iPort, iType);
    m_epoll.add(m_sockServer[m_sockServer.size() - 1].fd(), IO_SERVER_LISTEN, EPOLLIN|EPOLLRDHUP);
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
    pThis->m_timeEvent.insert(timeEvent);

    while(true){
        long long timeNow = getTimeMs();

        while(1){
            multiset<STimeEvent>::iterator it = m_timeEvent.begin();
            if(it->llTimeLine < timeNow){
                it->funEvent(this);
                continue;
            }

            break;
        }


        long long waitTime = 1000;
        if(m_timeEvent.size() > 0){
            waitTime = timeNow - m_timeEvent.begin()->llTimeLine;
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
