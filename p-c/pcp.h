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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include<unistd.h>
#include<sys/types.h>

#define QueueSize 10  //定义缓冲区大小为10
typedef struct
{
    int front; //头指针，队非空时指向队头元素
    int rear; //尾指针，队非空时指向队尾元素的下一位置
    int count; //计数器，记录队中元素总数
    char data[QueueSize];
}CirQueue;

//初始化队列
void initQueue(CirQueue *Q);

//判断队列是否为空
int queueEmpty(CirQueue *Q);

//判队满
int queueFull(CirQueue *Q);

//入队
void enQueue(CirQueue *Q,char x);

//出队
char deQueue(CirQueue *Q);

//打印队列
void prQueue(CirQueue *Q);


///////////////

int P(int semid, int semnum);
int V(int semid, int semnum);

#endif
