/*************************************************************************************
 * 文件名: main.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月06日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*
int main(int argc, char **argv)
{
    Application::Instance()->main(argc, argv);
    Application::Instance()->setServer<处理类>("servername");
    //指定优先级
    //Application::setPriority("servername::function", HIGH);
    //Application::setPriority("servername::function", LOW);
    //Application::setPriority("servername::function", NORMAL);
    Application::run();
    return 0;
}
*/

int main(int argc, char **argv)
{
    if(argc != 2){
        CLog::printf(CLog::RED, "usage:%s server.conf\n", argv[0]);
        return 0;
    }

    CMaster::getInstance()->setManagerServer("127.0.0.1", 8412);
    CMaster::getInstance()->setServer("127.0.0.1", 8413, CMaster::TCP);
    CMaster::getInstance()->setServer("127.0.0.1", 8413, CMaster::UDP);
    CMaster::getInstance()->setNetThreadCount(2);
    CMaster::getInstance()->setWorkThreadCount(10);
    CMaster::getInstance()->start();
    return 0;
}
