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

    m_iNetThread = CCommon::strto<int>(confPart[0]["netthreads"]);
    if(m_iNetThread <= 0){
        m_iNetThread = 1;
    }

    m_iWorkThread = CCommon::strto<int>(confPart[0]["workthreads"]);
    if(m_iWorkThread <= 0){
        //TODO:根据cpu个数设置线程数,线程数为cpu数目-1
        m_iWorkThread = 4;
    }


    m_pWorkThread = new CWorkThread[m_iWorkThread];
    m_pMessageQueue = new CMessageQueue[m_iNetThread];
    m_pNetThread = new CNetThread[m_iNetThread];

    for(int i = 0; i < m_iNetThread; i++){
        m_pNetThread[i]->setApplication(this);
        m_pNetThread[i]->setMasterMessageQueue(m_pMessageQueue[i]); 

        for(int j = 0; j < m_iWorkThread; j++){
            if(i == 0){
                m_pworkThread[j]->setApplication(this);
            }

            //TODO:这是一个双向消息队列
            CMessageQueue *pMessageQueue = new CMessageQueue();
            
            m_pNetThread[i]->setWorkMessageQueue(j , pMessageQueue);
            m_pworkThread[j]->setMessageQueue(i , pMessageQueue);
        }
    }

    return E_SUCCESS;
}

int CApplication::run()
{
    ADD_IO(/*监听端口*/);
    ADD_IO(/*与net thread消息队列*/);
    //添加时间时间定时打印服务状态
    ADD_TIME_EVENT(/* 到期时间 */, /*事件句柄*/ );

    while(!m_bTerminal){
        //处理时间事件 
        //
        
        //处理io事件
        epoll_wait();

        switch(ioType){
            case LISTEN_SOCKET: //有新连接
                accept();
                //把新socket通知给net thread
                //m_pMessageQueue.insert(/*消息类型*/, /*消息内容*/);
                m_pMessageQueue.insert(NEW_CONNCET, socketid);
                break;

            case MANAGER_SOCKET: //管理命令请示
                accept();
                //把新socket通知给net thread
                //m_pMessageQueue.insert(/*消息类型*/, /*消息内容*/);
                m_pMessageQueue.insert(NEW_CONNCET, socketid);
                break;

            case MESSAGE:   //有来自net thread的消息
                dealMessage(message);
        }
    }
}

int CApplication::dealMessage(message)
{
    switch(message.iMessageType){
        case STAT: //统计信息
            break;
            /*
        case ADD_IO:    //io请求,可以设置只监听一次,收到一次消息后就删除
            break;
        case DEL_IO:    //删除监听io
            break;
            */
    }
}


}
