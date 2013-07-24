/*************************************************************************************
 * 文件名: tdoeh_netthread.h
 * 作  者: 思凡
 * 日  期: 2013年07月23日
 * 描  述: 网络线程
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_NET_THREAD_H__
#define __TDOEH_NET_THREAD_H__

#include "tdoeh_thread.h"

namespace Tdoeh{

class CMaster;

class CNetThread : public CThread
{
    CMaster *m_pMaster;

public:
    CNetThread();
    ~CNetThread();

public:
    void setMaster(CMaster *pMaster);
    int work();
};

};

#endif
