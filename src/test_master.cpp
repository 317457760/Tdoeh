/*************************************************************************************
 * 文件名: test_master.cpp
 * 作  者: 思凡
 * 日  期: 2013年07月23日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_master.h"
using namespace Tdoeh;

int main(int argc, char **argv)
{
#if 0
    CMaster test;
    test.initialize();
    test.setManagerServer("127.0.0.1", 8412);
    test.setServer("127.0.0.1", 8413);
    test.setNetThreadCount(1);
    test.setWorkThreadCount(2);
    test.start();
#endif

#if 1
    CMaster::Instance().initialize();
    CMaster::Instance().setManagerServer("127.0.0.1", 8412);
    CMaster::Instance().setServer("127.0.0.1", 8413);
    CMaster::Instance().setNetThreadCount(1);
    CMaster::Instance().setWorkThreadCount(2);
    CMaster::Instance().start();
#endif
    return 0;
}
