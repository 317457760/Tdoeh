/*************************************************************************************
 * 文件名: tdoeh_workthread.cpp
 * 作  者: 思凡
 * 日  期: 2013年07月23日
 * 描  述:
 * 修  改: 
 *************************************************************************************/
#include <unistd.h>
#include "tdoeh_workthread.h"
#include "tdoeh_log.h"

namespace Tdoeh{

CWorkThread::CWorkThread() : m_pMaster(NULL)
{
}

CWorkThread::~CWorkThread()
{
}

void CWorkThread::setMaster(CMaster *pMaster)
{
    m_pMaster = pMaster;
}

int CWorkThread::work()
{
    sleep(1);
    CLog::printf(CLog::RED, "net thread work\n");  
    return 0;
}

}
