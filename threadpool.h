#ifndef _threadpool___
#define _threadpool___

#include "sockt4.h"

typedef void (*RunMe)(sockt4 &sock);

class object
{
	public:
		int x;
		sockt4 sock;
		RunMe runme;
};


class ThreadPool
{
	public:
		ThreadPool(int noOfThrds);
		int executeJob(object o);

		~ThreadPool();
	private:

		class queue
		{
			typedef struct ll
			{
				object obj;
				struct ll *next;
			}ll;

			pthread_mutex_t m;

			ll *start;

			inline int enq(object o)
			{
				ll *tmp = start;
				if(!tmp)
				{
					tmp = new ll();
					tmp->next = NULL;
					tmp->obj = o;
					start = tmp;
				}
				else
				{
					while(tmp->next)
						tmp = tmp->next;

					tmp->next = new ll();
					tmp = tmp->next;
					tmp->obj = o;
					tmp->next = NULL;
				}
				return 0;
			}

			inline int dq(object &o)
			{
				ll *tmp = start;
				if(!tmp)
					return -1;

				start = start->next;
				o = tmp->obj;
				delete(tmp);
				return 0;
			}

			public:
			queue()
			{
				start = NULL;
				pthread_mutex_init(&m, NULL);
			}

			int enqueue(object o)
			{
				pthread_mutex_lock(&m);
				int ret = enq(o);
				pthread_mutex_unlock(&m);
				return ret;
			}

			int dequeue(object &o)
			{
				pthread_mutex_lock(&m);
				int ret = dq(o);
				pthread_mutex_unlock(&m);
				return ret;
			}

			~queue()
			{
				pthread_mutex_destroy(&m);
				ll *tmp = start;
				while(start)
				{
					tmp = start;
					start = start->next;
					delete(tmp);
				}
			}
		};
		queue q;
		int thrdCount;
		pthread_t *pThrds;
		pthread_cond_t cv =  PTHREAD_COND_INITIALIZER;
		pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
		static void * thread_starter(void *v);
		void * thread(void);


};

#endif
