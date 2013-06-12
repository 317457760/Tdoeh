/*************************************************************************************
 * 文件名: test_config.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月07日
 * 描  述: 测试CConfig类
 * 修  改: 
 *************************************************************************************/
#include "gtest/gtest.h"
#include "tdoeh_config.h"
using namespace Tdoeh;

TEST(ConfigTest, ValidData)
{
    CConfig temp;
    char content[] = " \
                    [global] \n\
                        logpath = .         #日志目录 \n\
                        netthread = 2       #网络线程数 \n\
                     \n\
                    [admin] \n\
                        endpoint = udp|127.0.0.1|8888   #管理端口 \n\
                     \n\
                    [server] \n\
                        name = test1                        #服务名 \n\
                        endpoint = tcp|127.0.0.1|8412|6000  #服务端口   协议|ip|端口|链接空闲保持时间(udp无效,为0时不主动断开) \n\
                        threads = 1                         #服务线程数 \n\
                     \n\
                    [server] \n\
                        name = test3 \n\
                        endpoint = udp|127.0.0.2|8412 \n\
                        threads = 2 \n\
                    \n\
                    [server] \n\
                        name = test3 \n\
                        endpoint = *|127.0.0.3|8413|0 \n\
                        threads = 3 ";

    ASSERT_EQ(0, temp.parse(content));
    ASSERT_EQ(3, temp.get("server").size());
    ASSERT_EQ(0, temp.get("no_exist").size());
    ASSERT_STREQ("2", temp.get("global")[0]["netthread"].c_str());
    ASSERT_STREQ(".", temp.get("global")[0]["logpath"].c_str());
    ASSERT_STREQ("udp|127.0.0.1|8888", temp.get("admin")[0]["endpoint"].c_str());
    ASSERT_STREQ("", temp.get("admin")[0]["no_exist"].c_str());
}

TEST(ConfigTest, InValidTag)
{
    CConfig temp;
    char content[] = "[main]\nkey=value\n[xx";
    temp.parse(content);
    ASSERT_EQ(CConfig::E_INVALID_TAG, temp.getLastErrorNumber());
    ASSERT_STREQ("invalid tag found, line 3", temp.getLastErrorMessage());
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
