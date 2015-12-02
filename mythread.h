#ifndef __mythread_h__
#define __mythread_h__

#include <pthread.h>
#include <stdlib.h>
#include <cstdio>
#include <map>

std::map<int, int> G;
void link(int x,int y)
{
    printf("G[%d] will change to %d\n", x, y);
    G[x] = y;
    printf("G[%d] change to %d have done\n", x, y);
}
int check_circle(int x)
{
    for (int t = G[x]; t; t=G[t])
      if (t == x) return 1;
    return 0;
}
void del(int x)
{
    printf("G[%d] changed to 0\n", x);
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
    printf("Show the mutex %d build\n", mutex->id);
    return pthread_mutex_init(&(mutex->my_mutex), attr);
}

int mythread_mutex_destroy(mythread_mutex_t *mutex)
{
    return pthread_mutex_destroy(&(mutex->my_mutex));
}

int mythread_mutex_lock(mythread_mutex_t *mutex, int dx)
{
    int x = __mythread_check_thread__(pthread_self());
    printf("%d-%d wants %d but now is %d\n", dx, x, mutex->id, G[mutex->id]);
    if (G[mutex->id] == 0)
    {
        int tmp = pthread_mutex_lock(&(mutex->my_mutex));
        link(mutex->id, x);
        return tmp;
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
            printf("Warning : Error %d on id %d\n", ret, mutex->id);
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
