#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include "threadpool.h"



//#include <stdlib.h>
//#include <unistd.h>
//#include <signal.h>
//#include <fstream>

using namespace std;

ThreadPool::ThreadPool(int noOfThrds=1)
{
	thrdCount = noOfThrds;
	pThrds = new pthread_t[thrdCount];
	for(int i=0;i<thrdCount;i++)
	{
		pthread_create(&pThrds[i], NULL, thread_starter, this);
		pthread_detach(pThrds[i]);
	}
}

int ThreadPool::executeJob(object o)
{
	q.enqueue(o);
	pthread_cond_signal(&cv);
	return 0;
}

ThreadPool::~ThreadPool() 
{
	for(int i=0;i<thrdCount;i++)
	{
		object o;
		o.x=-1;
		q.enqueue(o);
		pthread_cond_signal(&cv);
	}

	for(int i=0;i<thrdCount;i++)
	{
		pthread_join(pThrds[i], NULL); //TO DO: Read the status
	}

	delete[] pThrds;

	printf("ThreadPool: All threads terminated and memory freed\n");
}

void * ThreadPool::thread_starter(void *v)
{
	ThreadPool *tp = (ThreadPool *)v;
	return tp->thread();
}

void * ThreadPool::thread(void)
{
	pthread_t myid= pthread_self();

	while(1)
	{
		printf("Thrd %d waiting\n", (int)myid);
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&cv, &lock);
		pthread_mutex_unlock(&lock);
		object o;
		if(!q.dequeue(o) )
			printf("Thrd %d : woken up : job: %d\n", (int)myid, o.x);
		else
			printf("Thrd %d : woken up : no job \n", (int)myid);

		if(o.x == -1)
			break;


		if(NULL != o.runme)
		{
			o.runme(o.sock);
		}

	}
	//pthread_mutex_unlock(&lock);
	printf("Thread %d exiting\n", (int)myid);
	return NULL;
}

#if 0
#include <unistd.h>
void just_sleep(sockt4 &sock)
{
    printf("just sleep called\n");
    sleep(4);
    printf("just sleep done\n");
}


int main()
{
	ThreadPool tp(5);
	sleep(1);
	object o;

	for(int i=0;i<5;i++)
	{
	o.x = 11;
	o.runme = just_sleep;
	tp.executeJob(o);
	printf("executeJob done\n");
	}

	sleep(20);

}
#endif
