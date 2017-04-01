#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 

typedef struct ListNode
{
	int data;
	struct ListNode* _next;
}Node;

Node* creatNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if(newNode == NULL)
	{
		perror("malloc error\n");
		return NULL;
	}
	newNode->data = data;
	newNode->_next = NULL;
	return newNode;
}

void InitList(Node** head)
{
	*head = creatNode(0);
}

void Push_head(Node* head,int data)
{
	assert(head);
	Node* newNode = creatNode(data);
	newNode->_next = head->_next;
	head->_next = newNode;
	return;
}

int IsEmpty(Node* head)
{
	assert(head);
	if(head->_next == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void DelNode(Node* del)
{
	assert(del);
	free(del);
	del = NULL;
	return;
}

void Pop_head(Node* head,int *data)
{
	assert(head);
	if(IsEmpty(head) != 0)
	{
		Node* del = head->_next;
		head->_next = del->_next;
		*data = del->data;
		DelNode(del);
		return;
	}
	printf("List if Empty!\n");
	return;
}

void DestroyList(Node* head)
{
	assert(head);
	Node* cur = head->_next;
	while(cur)
	{
		Pop_head(head,NULL);
		cur = cur->_next;
	}
	return;
}

void ShowList(Node* head)
{
	assert(head);
	Node* cur = head->_next;
	while(cur)
	{
		printf("%d ",cur->data);
		cur = cur->_next;
	}
	printf("\n");
	return;
}


int test()
{
	Node* head;
	InitList(&head);
	int i = 0;
	while(i < 10)
	{
		Push_head(head,i);
		i++;
		ShowList(head);
	}
	for(i = 0; i<5;i++)
	{
		Pop_head(head,NULL);
		ShowList(head);
	}
	return 0;
}

void* thread_producer(void* arg)
{
	Node* head = (Node*)arg;
	while(1)
	{
		int data = rand()%100000;
		pthread_mutex_lock(&mutex);
        usleep(10000);
		//sleep(1);
	    Push_head(head,data);
		pthread_mutex_unlock(&mutex);
		printf("Producer sell : %d\n",data);
		pthread_cond_signal(&cond);
	}
	return NULL;
}
void* thread_consumer(void *arg)
{
	Node* head = (Node*)arg;
	while(1)
	{
		int data = 0;
		pthread_mutex_lock(&mutex);
		//sleep(1);
		while(IsEmpty(head) == 0)
		{
			pthread_cond_wait(&cond,&mutex);
		}
	    Pop_head(head,&data);
		pthread_mutex_unlock(&mutex);
		printf("Consumer get : %d\n",data);
	}
	return NULL;
}


int main()
{

	Node* head;
	InitList(&head);
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,thread_producer,(void*)head);
	pthread_create(&tid2,NULL,thread_consumer,(void*)head);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;

}
