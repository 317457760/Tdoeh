/*************************************************************************************
 * 文件名: tdoeh_application.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月08日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_application.h"
#include "tdoeh_config.h"
#include "tdoeh_file.h"

namespace Tdoeh{

int CApplication::main(int argc, char **argv)
{
    if(argc != 2){
        TDOEH_SET_ERROR_RETURN(-1, "usage:%s server.conf", argv[0]); 
    }

    char acConfig[CConfig::MAX_CONFIG_SIZE]; 
    ssize_t nLength = CFile::read(argv[1], acConfig, sizeof(acConfig));
    if(nLength >= sizeof(acConfig)){
        CLog::printf(CLog::RED, "config file %s have a big size\n", argv[1])
        return -1;
    }

    if(m_conf.parse(acConfig) != CBase::E_SUCCESS){
        CLog::printf(CLog::RED, "parse config error %s\n", m_conf.getLastErrorMessage())
        return -1;
    }

    vector<map<string, string> > confPart = m_conf.get("global");   
    if(confPart.size() != 1){
        CLog::printf(CLog::RED, "no global conf\n")
        return -1;
    }

    //TODO:初始化log
    //CLog::init(confPart[0]["logpath"], confPart[0]["loglevel"]);

    int iNetThread = CCommon::strto<int>(confPart[0]["netthreads"]);
    if(iNetThread <= 0){
        iNetThread = 1;
    }

    int iWorkThread = CCommon::strto<int>(confPart[0]["workthreads"]);
    if(iWorkThread <= 0){
        //TODO:根据cpu个数设置线程数,线程数为cpu数目-1
    }
    //TODO:启动工作线程


    //TODO:启动网络线程
    for(int i = 0; i < iNetThread; i++){
        CNetThread *pThread = new CNetThread(CMessageQueue);
        pThread->start();
    }

    //TODO:启动服务端口监听

    return E_SUCCESS;
}


}
