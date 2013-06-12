/*************************************************************************************
 * 文件名: main.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月06日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

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
