/*************************************************************************************
 * 文件名: tdoeh_master.h
 * 作  者: 思凡
 * 日  期: 2013年06月22日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_singleton.h"

namespace Tdoeh{

class CMaster : public CSingleton<CMaster>
{
    CNetThread *m_netThreads;
    int m_iNetThreadCount;

    CWorkThread *m_workThreads;
    int m_iWorkThreadCount;

    CEpoll m_epoll;
    CSocket m_sockManager;

    vector<CSocket> m_sockServer;

    int m_iCount;

    typedef int (*FUN_TIMEEVENT)(CMaster *pThis);
    struct STimeEvent{
        long long llTimeLine;
        FUN_TIMEEVENT funEvent;
        bool operator < (const STimeEvent &t)const{
            return llTimeLine < t.llTimeLine;
        }
    };
    multiset<STimeEvent> m_timeEvent;

    enum{
        IO_MANAGER_LISTEN = 1,
        IO_MANAGER_DATA = 2,
        IO_SERVER_LISTEN = 3,
        IO_MESSAGE_QUEUE = 4
    };

public:
    enum{
        TCP = 1,
        UDP = 2
    }

private:
    long long getTimeMs();

public:
    int initialize();
    int setManagerServer(string strIp, int iPort);
    int setServer(string strIp, int iPort, int iType);
    int setNetThreadCount(int iCount = 2);
    int setWorkThreadCount(int iCount = 10);
    int start();

public:
    static int funStatEvent(CMaster *pThis);
};

}
