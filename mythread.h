#ifndef __mythread_h__
#define __mythread_h__

#include <pthread.h>
#include <stdlib.h>
#include <cstdio>
#include <map>

pthread_mutex_t mutex_G, mutex_map;

void mythread_init() {
  pthread_mutex_init(&mutex_G, 0);
  pthread_mutex_init(&mutex_map, 0);
}

void mythread_destroy() {
  pthread_mutex_destroy(&mutex_G);
  pthread_mutex_destroy(&mutex_map);
}

std::map<int, int> G;
void link(int x,int y)
{
  pthread_mutex_lock(&mutex_G);
  printf("WILL: G[%d] = %d\n", x, y);
  G[x] = y;
  printf("DONE: G[%d] = %d\n", x, y);
  pthread_mutex_unlock(&mutex_G);
}
int check_circle(int x)
{
  pthread_mutex_lock(&mutex_G);
  for (int t = G[x]; t; t=G[t])
    if (t == x) {
      pthread_mutex_unlock(&mutex_G);
      return 1;
    }
  pthread_mutex_unlock(&mutex_G);
  return 0;
}
void del(int x) {
  pthread_mutex_lock(&mutex_G);
  printf("WILL: G[%d] = 0\n", x);
  G[x] = 0;
  printf("DONE: G[%d] = 0\n", x);
  pthread_mutex_unlock(&mutex_G);
}
int ask(int x) {
  pthread_mutex_lock(&mutex_G);
  int tmp = G[x];
  pthread_mutex_unlock(&mutex_G);
  return tmp;
}

typedef struct{pthread_mutex_t my_mutex;int id;} mythread_mutex_t;
std::map<unsigned long, int> __mythread_map_of_threads;
int __mythread_num_of_nodes__ = 0;

inline void Travel() {
  printf("-----begin Travel-----\n");
  for(std::map<unsigned long, int>::iterator itr = __mythread_map_of_threads.begin(); itr != __mythread_map_of_threads.end(); itr++) {
    printf("(%u,%d) ", itr->first, itr->second);
  }
  printf("\n-----end Travel-----\n");
}

int __mythread_check_thread__(pthread_t x)
{
  pthread_mutex_lock(&mutex_map);
  int* c = &__mythread_map_of_threads[(unsigned long) x];
  printf("---- %u %d----\n", x, c);
  if (*c == 0)
    *c = ++__mythread_num_of_nodes__;
  printf("SHOW: Thread %u = %d\n", x, __mythread_num_of_nodes__);
  //Travel();
  pthread_mutex_unlock(&mutex_map);
  return *c;
}

int mythread_mutex_init(mythread_mutex_t * mutex,const pthread_mutexattr_t * attr)
{
    mutex->id = ++__mythread_num_of_nodes__;
    printf("SHOW: Mutex ? = %d\n", mutex->id);
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
    if (ask(mutex->id) == 0)
    {
        int tmp = pthread_mutex_lock(&(mutex->my_mutex));
        link(mutex->id, x);
        return tmp;
    }
    else
    {
        printf("WARNING is coming!\n");
        link(x, mutex->id);
        if (check_circle(x))
        {
            printf("WARNING: Nuclear Launch Detected _(:」∠)_ \n");
            exit(-1);
        }
        else
        {
            int ret = pthread_mutex_lock(&(mutex->my_mutex));
            printf("WARNING: Error %d on id %d\n", ret, mutex->id);
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
