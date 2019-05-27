#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>


const int nProcessNum = 2;
pthread_mutex_t mutex;

int race_data = 0;

int get_current_CPU(){
    int j;
    cpu_set_t get;
    CPU_ZERO(&get);
    
    if (pthread_getaffinity_np(pthread_self(),sizeof(get),&get) < 0 )
        fprintf(stderr,"get thread affinity failed\n");
    for (j = 0;j < nProcessNum;j++){
        if (CPU_ISSET(j,&get))
            return j;
    }
    return -1;
}


void *A(void *ptr){
    printf("A: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    sleep(2);
    
    printf("A: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    race_data = 100;
    
    printf("A: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    printf("%s: %d\n", (char*)ptr, race_data);
}

void *B(void *ptr){
    printf("B: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    sleep(1);
    
    printf("B: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    race_data = 1;

    printf("B: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    printf("%s: %d\n", (char*)ptr, race_data);
}

int main(){
    char *msg1 = "threadA";
    char *msg2 = "threadB";
    
    pthread_t t1,t2;
    int ret_t1, ret_t2;
    ret_t1 = pthread_create(&t1,NULL,A,(void *)msg1);
    printf("main: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    ret_t2 = pthread_create(&t2,NULL,B,(void *)msg2);
    printf("main: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());

    if(ret_t1 == 0)
        printf("create threadA\n");
    if(ret_t2 == 0)
        printf("create threadB\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("main: thread %x is running in processor%d\n",pthread_self(),get_current_CPU());
    
    return 0;
}





