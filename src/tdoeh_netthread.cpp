/*************************************************************************************
 * 文件名: tdoeh_netthread.cpp
 * 作  者: 思凡
 * 日  期: 2013年07月23日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <unistd.h>
#include "tdoeh_netthread.h"
#include "tdoeh_log.h"

namespace Tdoeh{

CNetThread::CNetThread() : m_pMaster(NULL)
{
}

CNetThread::~CNetThread()
{
}

void CNetThread::setMaster(CMaster *pMaster)
{
    m_pMaster = pMaster;
}

int CNetThread::work()
{
    sleep(1);
    CLog::printf(CLog::RED, "net thread work\n");  
    return 0;
}

}
