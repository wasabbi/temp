#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>


const int nProcessNum = 2;
pthread_mutex_t mutex;

int race_data = 0;

void *A(void *ptr){
    cpu_set_t mask;
    cpu_set_t get;
    CPU_ZERO(&mask);
    CPU_ZERO(&get);
    
    CPU_SET(0, &mask);  //binding to CPU0
    if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
        fprintf(stderr,"A: set thread affinity failed\n");
    if (pthread_getaffinity_np(pthread_self(),sizeof(get),&get) < 0 )
        fprintf(stderr,"A: get thread affinity failed\n");
    
    int j;
    for (j = 0;j < nProcessNum;j++)
    {
        if (CPU_ISSET(j,&get))
            printf("A: thread %d is running in processor%d\n",pthread_self(),j);
    }
    sleep(2);
    
    for (j = 0;j < nProcessNum;j++)
    {
        if (CPU_ISSET(j,&get))
            printf("A: thread %d is running in processor%d\n",pthread_self(),j);
    }
    
    race_data = 100;
    
    for (j = 0;j < nProcessNum;j++)
    {
        if (CPU_ISSET(j,&get))
            printf("A: thread %d is running in processor%d\n",pthread_self(),j);
    }
    
    printf("%s: %d\n", (char*)ptr, race_data);
}

void *B(void *ptr){
    cpu_set_t mask;
    cpu_set_t get;
    CPU_ZERO(&mask);
    CPU_ZERO(&get);
    
    CPU_SET(1, &mask);  //binding to CPU1
    if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
        fprintf(stderr,"B: set thread affinity failed\n");
    if (pthread_getaffinity_np(pthread_self(),sizeof(get),&get) < 0 )
        fprintf(stderr,"B: get thread affinity failed\n");
    
    int j;
    for (j = 0;j < nProcessNum;j++)
    {
        if (CPU_ISSET(j,&get))
            printf("B: thread %d is running in processor%d\n",pthread_self(),j);
    }
    sleep(1);
    
    for (j = 0;j < nProcessNum;j++)
    {
        if (CPU_ISSET(j,&get))
            printf("B: thread %d is running in processor%d\n",pthread_self(),j);
    }
    race_data = 1;
    
    for (j = 0;j < nProcessNum;j++)
    {
        if (CPU_ISSET(j,&get))
            printf("B: thread %d is running in processor%d\n",pthread_self(),j);
    }
    
    printf("%s: %d\n", (char*)ptr, race_data);
}

int main(){
    char *msg1 = "threadA";
    char *msg2 = "threadB";
    
    pthread_t t1,t2;
    int ret_t1, ret_t2;
    ret_t1 = pthread_create(&t1,NULL,A,(void *)msg1);
    ret_t2 = pthread_create(&t2,NULL,B,(void *)msg2);
    
    if(ret_t1 == 0)
        printf("create threadA\n");
    if(ret_t2 == 0)
        printf("create threadB\n");
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("race_data: %d\n", race_data);
    
    return 0;
}





