//
//  main.c
//  p-c
//
//  生产者和消费者问题大作业
//
//  Created by Charles Yin on 13-2-19.
//  Copyright (c) 2013年 Charles Yin. All rights reserved.
//

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include <err.h>
#include "pcp.h"

int main(int argc, const char * argv[])
{
    
    //申请一块内存，并获取共享内存块的表识id
    int buf_id = shmget(IPC_PRIVATE, sizeof(CirQueue), IPC_CREAT|IPC_EXCL|0600);
    if ( buf_id < 0 )
    {
        perror("get shm  ipc_id error") ;
        return -1 ;
    }
    
    int i; //循环变量
    pid_t status;  存储进程的pid，以区分是否为
    
    //使用循环创建4个子进程（生产者和消费者）
    for(i = 0; i < 4; i++){
        status = fork();
        //当发现处于子进程中，不继续fork，直接执行自己的代码
        if(status == 0 || status == -1) break;
        
    }
    if (status == -1)   //若进程创建失败
    {
        return -1;
    }
    else if (status == 0) //若创建成功，以下为每个子进程都会执行的代码
    {
        //令生产者为当i＝0和2时创建的子进程，消费者为当i＝1和3时创建的子进程
        if(i%2 == 0)
        {
            //生产者的代码
            printf("producer %d,\n",i);
            CirQueue* buf = (CirQueue*)shmat(buf_id, NULL, 0);
            if ( (int)buf == -1 )
            {
                perror("shmat addr error");
                return -1 ;
            }
            enQueue(buf, 'A');
            shmdt(buf);
            return 0;
        }
        else
        {
            //消费者的代码
            printf("consumer %d,\n",i);
            CirQueue* buf = (CirQueue*)shmat(buf_id, NULL, 0);
            if ( (int)buf == -1 )
            {
                perror("shmat addr error");
                return -1 ;
            }
            printf("%c\n",deQueue(buf));
            shmdt(buf);
            return 0;
        }
    }
    else
    {
        sleep(3) ;
        int flag = shmctl( buf_id, IPC_RMID, NULL) ;
        if ( flag == -1 )
        {
            perror("shmctl shm error") ;
            return -1 ;
        }
        printf("sdfsdf\n");
    }
    //printf("%d",buf_array);
    return 0;
}