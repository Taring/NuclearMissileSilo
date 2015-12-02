#include "mythread.h"
#include <cstdio>
#include <ctime>
#include <unistd.h>
mythread_mutex_t mutex;

void* work1(void *argv)
{
    //for (int i = 1; i <= 2; i++)
    //{
    int i = 1;
        mythread_mutex_lock(&mutex, 1);
        printf("1 get\n");
        printf("This is worker 1 in %d times\n", i);
        sleep(1);
        printf("1 release\n");
        mythread_mutex_unlock(&mutex);
    //}
    pthread_exit(0);
}

void* work2(void *argv)
{
    //for (int i = 1; i <= 2; i++)
    //{
    int i = 1;
        mythread_mutex_lock(&mutex, 2);
        printf("2 get\n");
        printf("This is worker 2 in %d\n", i);
        sleep(1);
        printf("2 release\n");
        mythread_mutex_unlock(&mutex);
    //}
    pthread_exit(0);
}

int main()
{
    mythread_mutex_init(&mutex, 0);
    pthread_t a,b;
    pthread_create(&a, 0, work1, 0);
    pthread_create(&b, 0, work2, 0);
    pthread_join(a,0);
    pthread_join(b,0);
    mythread_mutex_destroy(&mutex);
}
