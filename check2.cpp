#include "mythread.h"
#include <cstdio>
#include <ctime>
#include <unistd.h>
mythread_mutex_t mutex1, mutex2;


void* work1(void* argv)
{
    mythread_mutex_lock(&mutex1, 1);
    sleep(1);
    mythread_mutex_lock(&mutex2, 1);
    pthread_exit(0);
}

void* work2(void* argv)
{
    mythread_mutex_lock(&mutex2, 2);
    sleep(1);
    mythread_mutex_lock(&mutex1, 2);
    pthread_exit(0);
}

int main()
{
    mythread_init();
    mythread_mutex_init(&mutex1, 0);
    mythread_mutex_init(&mutex2, 0);
    pthread_t a,b;
    pthread_create(&a, 0, work1, 0);
    pthread_create(&b, 0, work2, 0);
    pthread_join(a,0);
    pthread_join(b,0);
    mythread_mutex_destroy(&mutex1);
    mythread_mutex_destroy(&mutex2);
    mythread_destroy();
    exit(0);
}
