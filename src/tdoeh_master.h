/*************************************************************************************
 * 文件名: tdoeh_master.h
 * 作  者: 思凡
 * 日  期: 2013年06月22日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_singleton.h"
#include "tdoeh_netthread.h"
#include "tdoeh_workthread.h"
#include "tdoeh_epoll.h"
#include "tdoeh_socket.h"
#include <set>

namespace Tdoeh{

class CMaster : public CSingleton<CMaster>
{
    //网络线程
    CNetThread *m_netThreads;
    int m_iNetThreadCount;

    //工作线程
    CWorkThread *m_workThreads;
    int m_iWorkThreadCount;

    //master与netthread的消息队列
    CMessageQueue *m_pMQMasterNet;
    //netthread与workthread的消息队列
    CMessageQueue *m_pMQNetWork

    CEpoll m_epoll;

    //管理端口
    CSocket m_sockManager;

    //服务端口
    CSocket m_sockServer;

    int m_iCount;   //测试

    //时间事件
    typedef int (*FUN_TIMEEVENT)(CMaster *pThis);
    struct STimeEvent{
        long long llTimeLine;
        FUN_TIMEEVENT funEvent;
        bool operator < (const STimeEvent &t)const{
            return llTimeLine < t.llTimeLine;
        }
    };
    //时间队列
    std::multiset<STimeEvent> m_timeEvent;

    //IO事件列表
    enum{
        IO_MANAGER_LISTEN = 1,  //管理端口监听
        IO_MANAGER_DATA = 2,    //管理端口数据
        IO_SERVER_LISTEN = 3,   //服务端口监听
        IO_MESSAGE_QUEUE = 4    //消息队列信息
    };

private:
    long long getTimeMs();
    void ManagerListen(int iFd, uint32_t nEvent);
    void ManagerData(int iFd, uint32_t nEvent);
    void ServerListen(int iFd, uint32_t nEvent);
    void MessageQueue(int iFd, uint32_t nEvent);

public:
    int initialize();
    int setManagerServer(const char *szIp, int iPort);
    int setServer(const char *szIp, int iPort);
    int setNetThreadCount(int iCount);
    int setWorkThreadCount(int iCount);
    int start();

public:
    static int funStatEvent(CMaster *pThis);
};

}
