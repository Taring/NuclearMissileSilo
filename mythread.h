#ifndef __mythread_h__
#define __mythread_h__

#include <pthread.h>
#include <stdlib.h>
#include <cstdio>
#include <map>

std::map<int, int> G;
void link(int x,int y)
{
    printf("%d changed to %d\n", x, y);
    G[x] = y;
    printf("Done?\n");
}
int check_circle(int x)
{
    for (int t = G[x]; t; t=G[t])
      if (t == x) return 1;
    return 0;
}
void del(int x)
{
    printf("%d changed to 0\n", x);
    G[x] = 0;
}

typedef struct{pthread_mutex_t my_mutex;int id;} mythread_mutex_t;
std::map<unsigned long, int> __mythread_map_of_threads;
int __mythread_num_of_nodes__ = 0;

int __mythread_check_thread__(pthread_t x)
{
    int c = __mythread_map_of_threads[(unsigned long) x];
    if (c != 0) return c;
    else return __mythread_map_of_threads[(unsigned long) x] = ++__mythread_num_of_nodes__;
}

int mythread_mutex_init(mythread_mutex_t * mutex,const pthread_mutexattr_t * attr)
{
    mutex->id = ++__mythread_num_of_nodes__;
    return pthread_mutex_init(&(mutex->my_mutex), attr);
}

int mythread_mutex_destroy(mythread_mutex_t *mutex)
{
    return pthread_mutex_destroy(&(mutex->my_mutex));
}

int mythread_mutex_lock(mythread_mutex_t *mutex)
{
    int x = (int)__mythread_check_thread__(pthread_self());
    printf("%d wants %d but %d\n", x, mutex->id, G[mutex->id]);
    if (G[mutex->id] == 0)
    {
        link(mutex->id, x);
        return pthread_mutex_lock(&(mutex->my_mutex));
    }
    else
    {
        link(x, mutex->id);
        if (check_circle(x))
        {
            printf("Warning. Nuclear Launch Detected _(:」∠)_ \n");
            exit(-1);
        }
        else
        {
            int ret = pthread_mutex_lock(&(mutex->my_mutex));
            printf("Error %d\n", ret);
            del(x);
            if (ret == 0)
            {
                link(mutex->id, x);
            }
            return ret;
        }
    }
}

int mythread_mutex_unlock(mythread_mutex_t *mutex)
{
    del(mutex->id);
    return pthread_mutex_unlock(&(mutex->my_mutex));
}
#endif
