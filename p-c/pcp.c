//
//  pcp.c
//  p-c
//
//  Created by Charles Yin on 13-2-22.
//  Copyright (c) 2013年 Charles Yin. All rights reserved.
//

#include <stdio.h>
#include "pcp.h"

//初始化队列
void initQueue(CirQueue *Q)
{
    Q->front=Q->rear=0;
    Q->count=0; //计数器置0
}

//判断队列是否为空
int queueEmpty(CirQueue *Q)
{
    return Q->count==0; //队列无元素为空
}

//判队满
int queueFull(CirQueue *Q)
{
    return Q->count==QueueSize; //队中元素个数等于QueueSize时队满
}

//入队
void enQueue(CirQueue *Q,char x)
{
    if(queueFull(Q))
    {
        perror("Queue overflow"); //队满上溢
        return;
    }
    Q->count ++; //队列元素个数加1
    Q->data[Q->rear]=x; //新元素插入队尾
    Q->rear=(Q->rear+1)%QueueSize; //循环意义下将尾指针加1
}

//出队
char deQueue(CirQueue *Q)
{
    char temp;
    if(queueEmpty(Q))
    {
        perror("Queue underflow"); //队空下溢
        return '*';
    }
    temp=Q->data[Q->front];
    Q->count--; //队列元素个数减1
    Q->front=(Q->front+1)%QueueSize; //循环意义下的头指针加1
    return temp;
}

void prQueue(CirQueue *Q)
{
    if(queueEmpty(Q))
    {
        printf("[Queue Empty]\n");
    }else{
        for(int i = 0; i < Q->count; i++)
        {
            printf("%c ",Q->data[(Q->front+i)%QueueSize]);
        }
        printf("\n");
    }
}

///////////////

int P(int semid, int semnum)
{
    struct sembuf sops={semnum,-1, 0};
    return (semop(semid,&sops,1));
}

int V(int semid, int semnum)
{
    struct sembuf sops={semnum,+1, 0};
    return (semop(semid,&sops,1));
}

