/*************************************************************************************
 * 文件名: main.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月27日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "tdoeh_queue.h"
using namespace Tdoeh;

struct SData{
    int index;
};

CQueue<int> testQueue;

void *funPop(void *p)
{
    int i = 0;
    while(1){
        int data;
        while(testQueue.pop(data) == 1){
            fprintf(stderr, "pop %d\n", data);
            i++;
        }
        fprintf(stderr, "pop %d %d\n", i, testQueue.size());
    }
    return 0;
}

int main(int argc, char **argv)
{
    testQueue.initialize();

    pthread_t pid;
    pthread_create(&pid, 0, funPop, NULL);

    int size = atoi(argv[1]);
    for(int i = 0; i < size; i++){
        while(testQueue.push(i) != 0){
        }

        //if(i % 1000 == 0){
        //    fprintf(stderr, "push %d size %d pop %d\n", i , testQueue.size(), i-testQueue.size());
        //}
    }

    while(testQueue.destory() != 0){
        usleep(1);
    }
    return 0;
}
