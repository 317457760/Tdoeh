/*************************************************************************************
 * 文件名: tdoeh_thread.h
 * 作  者: 思凡
 * 日  期: 2013年06月09日
 * 描  述: 
 * 修  改: 
 * 说  明: 在start时没有加锁,所以由用户控制只能调用一次start,否则会启动多个线程
 *************************************************************************************/
#ifndef __THDEO_THREAD_H__
#define __THDEO_THREAD_H__

#include <pthread.h>
#include "tdoeh_base.h"

namespace Tdoeh{

class CThread : public CBase{
    enum{
        STOPED = 0, //停止
        RUNING = 1, //正在运行
        STOPING = 2 //已经设置标识位
    };

    pthread_t m_id;
    volatile int m_iStat;

public:
    CThread();
    virtual ~CThread();
    
public:
    virtual int initialize();
    virtual void destory();
    virtual int work() = 0;

public:
    pthread_t id();
    int start();
    void stop();
    //int cancel();
    int stat();

private:
    static void *loop(void *pThis);
};

}

#endif
