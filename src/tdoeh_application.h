/*************************************************************************************
 * 文件名: tdoeh_application.h
 * 作  者: 思凡
 * 日  期: 2013年06月08日
 * 描  述: 服务实例
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_APPLICATION_H__
#define __TDOEH_APPLICATION_H__

#include "tdoeh_singleton.h"

namespace Tdoeh{

class CApplication : public CSingleton<CApplication>{
    CConfig m_conf;

    int m_iNetThread;
    CNetThread *m_pNetThread;

    int m_iWorkThread;
    CWorkThread *m_pWorkThread;

    CMessageQueue *m_pMessageQueue;

    bool m_bTerminal;

public:
    int main(int argc, char **argv);

    template <typename T>
    int setServer(string strServerName);

    //int setPriority(string strServerFunction, int iLevel);

    int run();
};

}

#endif
