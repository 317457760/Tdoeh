/*************************************************************************************
 * 文件名: tdoeh_queue.h
 * 作  者: 思凡
 * 日  期: 2013年06月26日
 * 描  述: 单进程写入，单进程读取的无锁队列
 * 修  改: 
 * 注  意: 1.在使用时要先进行initialize,再执行其它操作
 *         2.要先清空队列再执行destory时,否则会造成内存泄露
 *         3.在执行destory后不能再操作除非重新执行initialize
 *         4.initialize和destory非线程安全
 *************************************************************************************/
#include "tdoeh_base.h"

namespace Tdoeh{

template <class T>
class CQueue : public CBase {
    enum{
        E_NOEMPTY = E_MAX_ERROR_NUMBER - 1,  //队列非空
    };

    struct SNode{
        T data;
        struct SNode *next;
    };

    struct SNode *m_pHead;
    struct SNode *m_pTail;
    volatile int m_iSize;

public:
    CQueue();
    ~CQueue();

public:
    int initialize();
    int destory();
    int pop(T &data);
    int push(const T &data);
    bool empty();
    int size();
};

template<typename T>
CQueue<T>::CQueue() : m_pHead(NULL), m_pTail(NULL), m_iSize(0)
{
}

template<typename T>
CQueue<T>::~CQueue()
{
}

template<typename T>
int CQueue<T>::initialize()
{
    if(m_pHead != NULL){
        return E_SUCCESS;
    }

    m_pHead = TDOEH_NEW SNode;
    m_pTail = TDOEH_NEW SNode;

    if(m_pHead == NULL || m_pTail == NULL){
        TDOEH_SET_ERROR_NUMBER_RETURN(E_NOMEMORY); 
    }

    m_pHead->next = m_pTail;
    m_pTail->next = NULL;

    return E_SUCCESS;
}

template<typename T>
int CQueue<T>::destory()
{
    if(!empty()){
        TDOEH_SET_ERROR_RETURN(E_NOEMPTY, "the queue is not empty, size:%d", m_iSize);
    }

    if(m_pHead != NULL){
        delete m_pHead;
        m_pHead = NULL;
    }
    if(m_pTail != NULL){
        delete m_pTail;
        m_pTail = NULL;
    }
    return E_SUCCESS;
}

template<typename T>
bool CQueue<T>::empty()
{
    return (m_iSize == 0);
}

template<typename T>
int CQueue<T>::size()
{
    return m_iSize;
}

template<typename T>
int CQueue<T>::push(const T &data)
{
    struct SNode *pTemp = TDOEH_NEW struct SNode;
    if(pTemp == NULL){
        TDOEH_SET_ERROR_NUMBER_RETURN(E_NOMEMORY); 
    }
#if 0
    struct SNode *pTempTail = NULL;
    do{
        m_pTail->data = data;
        m_pTail->next = pTemp;
        pTempTail = m_pTail;
    }while(!__sync_bool_compare_and_swap(&m_pTail, pTempTail, pTemp));
#endif

#if 1
    m_pTail->data = data;
    m_pTail->next = pTemp;

    m_pTail = pTemp;
#endif

    __sync_fetch_and_add(&m_iSize, 1);

    return E_SUCCESS;
}

template<typename T>
int CQueue<T>::pop(T &data)
{
    if(empty()){
        return 0;
    }

#if 0
    struct SNode *pTempHead = NULL;
    do{
        temp = m_pHead->next;
        pTempHead = m_pHead->next;
    }while(!__sync_bool_compare_and_swap(&(m_pHead->next), pTempHead, temp->next));
#endif

#if 1
    struct SNode *temp = m_pHead->next;
    m_pHead->next = temp->next;
#endif

    __sync_fetch_and_sub(&m_iSize, 1);

    data = temp->data;
    delete temp;
    return 1;
}

};
