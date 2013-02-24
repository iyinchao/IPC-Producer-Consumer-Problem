//
//  main.c
//  p-c
//
//  生产者和消费者问题大作业
//
//  Created by Charles Yin on 13-2-19.
//  Copyright (c) 2013年 Charles Yin. All rights reserved.
//

#include "pcp.h"


int main(int argc, const char * argv[])
{
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    //申请一块内存，并获取共享内存块的表识id
    int buf_id = shmget(IPC_PRIVATE, sizeof(CirQueue), IPC_CREAT|IPC_EXCL|0600);
    if ( buf_id < 0 )
    {
        perror("get shm  ipc_id error");
        return -1 ;
    }
    
    int i; //循环变量
    pid_t status;  //存储进程的pid，以区分是否为子进程
    union semun arg; //用于操作信号量的辅助联合体
    
    //获得IPC Key，以便信号量操作使用
    int key = ftok("/tmp", 0x66 );
    if ( key < 0 )
    {
        perror("ftok key error") ;
        return -1 ;
    }
    //创建一个信号量集（2个信号量）,令0号为empty，1号为full，2号为mutex
    int sem_id = semget(key,3,IPC_CREAT|0600);
    //为信号量设置初始值
    arg.val = QueueSize;  //empty=N
    int ret = semctl(sem_id,0,SETVAL,arg);
    if (ret < 0 )
    {
        perror("ctl sem \"empty\" error");
        semctl(sem_id,0,IPC_RMID,arg);
        return -1 ;
    }
    arg.val = 0;  //full=0
    ret = semctl(sem_id,1,SETVAL,arg);
    if (ret < 0 )
    {
        perror("ctl sem \"full\" error");
        semctl(sem_id,1,IPC_RMID,arg);
        return -1 ;
    }
    arg.val = 1; //mutex=1
    ret = semctl(sem_id,2,SETVAL,arg);
    if (ret < 0 )
    {
        perror("ctl sem \"mutex\" error");
        semctl(sem_id,2,IPC_RMID,arg);
        return -1 ;
    }
    
    //使用循环创建4个子进程（生产者和消费者）
    for(i = 0; i < 4; i++){
        if(i%2==0)sleep(1); //每隔一秒创建一个新的进程，使得随机种子中的time函数发挥作用
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
            srand((int)time(NULL));
            //if(i == 0)sleep(3);
            //if(i == 2)sleep(1);
            //将共享的内存区域映射到自己的地址空间
            CirQueue* buf = (CirQueue*)shmat(buf_id, NULL, 0);
            if ( (int)buf == -1 )
            {
                perror("shmat addr error");
                return -1 ;
            }
            struct timeval now_time; //记录当前操作发生的时间
            long sec; //秒数
            int msec; //毫秒数
            char temp;
            for(int i = 0; i < 10; i++)
            {
                //usleep(rand()%900000);
                sleep(rand()%4);
                temp = (char)(65+rand()%26);
                //
                P(sem_id, 0); //down sem:empty
                P(sem_id, 2); //down sem:mutex
                
                enQueue(buf, temp);
                
                //处理运行时间记录
                gettimeofday(&now_time, NULL);
                sec = now_time.tv_sec-start_time.tv_sec;
                msec = (now_time.tv_usec-start_time.tv_usec)/1000;
                if(msec < 0)
                {
                    sec -= 1;
                    msec += 1000;
                }
                //
                printf("(%2ld.%03d)produce %c: ", sec, msec, temp);
                prQueue(buf);
                fflush(stdout); //刷新stdout的缓冲区，及时输出结果
                V(sem_id,2); //up sem:mutex
                V(sem_id,1); //up sem:full
                
            }
            shmdt(buf);
            return 0;
        }
        else
        {
            srand((int)time(NULL));
            //if(i == 1)sleep(2);
            //消费者的代码
            //将共享的内存区域映射到自己的地址空间
            CirQueue* buf = (CirQueue*)shmat(buf_id, NULL, 0);
            if ( (int)buf == -1 )
            {
                perror("shmat addr error");
                return -1 ;
            }
            struct timeval now_time; //记录当前操作发生的时间
            long sec; //秒数
            int msec; //毫秒数
            char temp;
            for(int i = 0; i < 10; i++)
            {
                //usleep(rand()%900000);
                sleep(rand()%4);
                P(sem_id, 1); //down sem:full
                P(sem_id, 2); //down sem:mutex
                
                temp = deQueue(buf);
                
                //处理运行时间记录
                gettimeofday(&now_time, NULL);
                sec = now_time.tv_sec-start_time.tv_sec;
                msec = (now_time.tv_usec-start_time.tv_usec)/1000;
                if(msec < 0)
                {
                    sec -= 1;
                    msec += 1000;
                }
                //
                printf("(%2ld.%03d)consume %c: ", sec, msec, temp);
                prQueue(buf);
                fflush(stdout);
                V(sem_id,2); //up sem:mutex
                V(sem_id,0); //up sem:empty
            }
            shmdt(buf);
            return 0;
        }
    }
    else
    {
        sleep(30);
        int flag = shmctl( buf_id, IPC_RMID, NULL);  //移除内存共享区域
        if ( flag == -1 )
        {
            perror("shmctl shm error");
            return -1 ;
        }
        //移除3个信号量
        semctl(sem_id, 0, IPC_RMID,arg);
        semctl(sem_id, 1, IPC_RMID,arg);
        semctl(sem_id, 2, IPC_RMID,arg);
        printf("\nOperation complete...\n");
    }
    return 0;
}