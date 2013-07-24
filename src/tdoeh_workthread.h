/*************************************************************************************
 * 文件名: tdoeh_workthread.h
 * 作  者: 思凡
 * 日  期: 2013年07月23日
 * 描  述: 工作线程
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_WORK_THREAD_H__
#define __TDOEH_WORK_THREAD_H__

#include "tdoeh_thread.h"

namespace Tdoeh{

class CMaster;

class CWorkThread : public CThread
{
    CMaster *m_pMaster;

public:
    CWorkThread();
    ~CWorkThread();

public:
    void setMaster(CMaster *pMaster);
    int work();
};

};

#endif
