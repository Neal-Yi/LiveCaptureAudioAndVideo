
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
struct QueueRecord
{
	int Capacity;
	int Size;
	int Rear;
	int Front;
	ElementType *Array;
};
int IsEmpty(Queue Q)
{
	return Q->Size == 0;	
}
int IsFull(Queue Q)
{
	return Q->Size == Q->Capacity;	
}
Queue CreateQueue(int MaxElements)
{
	Queue Q=(Queue)malloc(sizeof(struct QueueRecord));
	Q->Capacity=MaxElements;
	Q->Size=0;
	Q->Rear=0;
	Q->Front=1;
	Q->Array=(ElementType *)malloc(MaxElements*sizeof(ElementType));
	return Q;
}
void DisposeQueue(Queue Q)
{
	if(Q!=NULL)
	{
		free(Q->Array);
		free(Q);
	}
}
void MakeEmpty(Queue Q)
{
	Q->Size=0;
	Q->Front=1;
	Q->Rear=0;
}
void Enqueue(ElementType X,Queue Q)
{
	if(!IsFull(Q))
	{
		if(Q->Rear==Q->Capacity-1)Q->Rear=0;
		else Q->Rear++;
		Q->Array[Q->Rear]=X;
		Q->Size++;
	}else
	{
		printf("Full queue!!\n");
	}
}
ElementType Front(Queue Q)
{
	if(!IsEmpty(Q))
		return Q->Array[Q->Front];
	return 0;
}
void ModifyFront(Queue Q, ElementType X)
{
	Q->Array[Q->Front] = X;
}
void Dequeue(Queue Q)
{
	if(!IsEmpty(Q))
	{
		if (Q->Front==Q->Capacity-1)
		{
			Q->Front=0;
		}else Q->Front++;
		Q->Size--;
	}
}
ElementType FrontAndDequeue(Queue Q)
{
	if(!IsEmpty(Q))
	{
		ElementType X=Q->Array[Q->Front];
		if (Q->Front==Q->Capacity-1)
		{
			Q->Front=0;
		}else Q->Front++;
		return X;
	}
	return 0;
}
void PrintQueue(Queue Q,void *print(ElementType X))
{
	int i=Q->Front;
	if(!IsEmpty(Q))
	{	while(i!=Q->Rear)
		{
			print(Q->Array[i++]);
			if(i==Q->Capacity)
			{
				i=0;
			}
		}
		print(Q->Array[Q->Rear]);
		printf("\n");
	}else{
		printf("The Queue is empty!\n");
	}
}