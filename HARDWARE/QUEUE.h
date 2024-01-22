#ifndef _QUEUE_H
#define _QUEUE_H


#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdio.h>
#define ElemType char	// 队列节点类型 
#define MAXSIZE 256 		// 队列最大空间

typedef struct Queue
{
	ElemType *base; //指向队列空间的基址
	int front; 		//头指针
	int rear; 		//尾指针
}Queue;

int Length(Queue *Q);
void InitQueue(Queue *Q);
void EnQueue(Queue *Q, ElemType x);
void DeQueue(Queue *Q);
void ShowQueue(Queue *Q);
void GetHead(Queue *Q, ElemType *v);
void ClearQueue(Queue *Q);
void DestroyQueue(Queue *Q);

#endif
