#include "QUEUE.H"


//队列初始化
void InitQueue(Queue *Q)
{
	Q->base = (ElemType *)malloc(sizeof(ElemType) * MAXSIZE);
	Q->front = Q->rear = 0;
}

//入队操作
void EnQueue(Queue *Q, ElemType x)
{
	if(((Q->rear+1)%MAXSIZE) == Q->front) return;
	Q->base[Q->rear] = x;
	Q->rear = (Q->rear+1)%MAXSIZE;
}

//出队操作
void DeQueue(Queue *Q)
{
	if(Q->front == Q->rear) return;
	Q->front = (Q->front+1)%MAXSIZE;
}


//打印循环队列中的数据
void ShowQueue(Queue *Q)
{
	int i;
	for(i=Q->front; i!=Q->rear;)
	{
		printf("%c",Q->base[i]);
		//此操作是为了实现循环遍历
		i = (i+1)%MAXSIZE;
	}
}

//获取队头元素
void GetHead(Queue *Q, ElemType *v)
{
	if(Q->front == Q->rear) return;
	//如果队列不为空，获取队头元素
	*v = Q->base[Q->front];
}

//获取队列长度（元素个数）
int Length(Queue *Q)
{
	int len= Q->rear - Q->front;
	len = (len>=0) ? len : MAXSIZE+len;
	return len;
}

//清空队列
void ClearQueue(Queue *Q)
{
	Q->front = Q->rear = 0;
}

//销毁队列
void DestroyQueue(Queue *Q)
{
	free(Q->base);
	Q->base = NULL;
}
