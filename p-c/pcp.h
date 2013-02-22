//
//  pcp.h
//  p-c
//
//定义了循环队列以及相应的操作
//
//  Created by Charles Yin on 13-2-19.
//  Copyright (c) 2013年 Charles Yin. All rights reserved.
//

#ifndef p_c_pcp_h
#define p_c_pcp_h

#define QueueSize 10  //定义缓冲区大小为10
typedef struct
{
    int front; //头指针，队非空时指向队头元素
    int rear; //尾指针，队非空时指向队尾元素的下一位置
    int count; //计数器，记录队中元素总数
    char data[QueueSize];
}CirQueue;

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
        perror("Queue overflow"); //队满上溢
    Q->count ++; //队列元素个数加1
    Q->data[Q->rear]=x; //新元素插入队尾
    Q->rear=(Q->rear+1)%QueueSize; //循环意义下将尾指针加1
}

//出队
char deQueue(CirQueue *Q)
{
    char temp;
    if(queueEmpty(Q)) 
        perror("Queue underflow"); //队空下溢
    temp=Q->data[Q->front];
    Q->count--; //队列元素个数减1
    Q->front=(Q->front+1)%QueueSize; //循环意义下的头指针加1
    return temp;
}

#endif
