/*************************************************************************************
 * 文件名: tdoeh_eventfd.h
 * 作  者: 思凡
 * 日  期: 2013年06月09日
 * 描  述: 对于linux eventfd的封装,kernel 2.6.26后使用
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_base.h"

namespace Tdoeh{

class CEventFd : public CBase{
    int m_hFd;

public:
    CEventFd();
    ~CEventFd();

public:
    int open(int iFlags);
    int fd();
    int read(uint64_t *events);
    int write(uint64_t events);
    int notify();
    void close();
};

}
