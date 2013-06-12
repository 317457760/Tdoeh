/*************************************************************************************
 * 文件名: tdoeh_thread.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月09日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_thread.h"

namespace Tdoeh{

CThread::CThread() : m_iStat(CThread::STOPED),m_id(-1)
{
}

CThread::~CThread()
{
}

pthread_t CThread::id()
{
    return m_id;
}

int CThread::stat()
{
    return m_iStat;
}

int CThread::initialize()
{
    return E_SUCCESS;
}

void CThread::destory()
{
}

int CThread::start()
{
    TDOEH_RETURN_IF_ERROR(initialize());
    int iReturn = pthread_create(&m_id, 0, (void *(*)(void *))&loop, (void *)this);
    if(iReturn != 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(iReturn);
    }

    return 0;
}

void CThread::stop()
{
    m_iStat =  STOPING;
}

/*
int CThread::cancel()
{
    int iReturn = pthread_cancel(m_id);
    if(iReturn != 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(iReturn);
    }

    return 0;
}
*/

void CThread::loop(CThread *pThis)
{
    pThis->m_iStat = RUNING;

    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while(pThis->m_iStat == RUNING){
        //pthread_testcancel();
        if(pThis->work() < 0){
            break;
        }
    }

    pThis->destory();
    pThis->m_iStat = STOPED;
}

}
