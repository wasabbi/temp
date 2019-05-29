#define _GNU_SOURCE


#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>

typedef int (*orig_pthread_create_ftype)(pthread_t *, const pthread_attr_t *, void* (void *), void *);

void* thread1 = 0x4009fb;
void* thread2 = 0x400ab4;

int pthread_create(pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg){

    int ret;

    orig_pthread_create_ftype orig_pthread_create;
    orig_pthread_create = (orig_pthread_create_ftype)dlsym(RTLD_NEXT, "pthread_create");

    if(__start_routine == thread1){
        int j;
        cpu_set_t orig_cpu_set;
        CPU_ZERO(&orig_cpu_set);
        if (pthread_getaffinity_np(pthread_self(),sizeof(orig_cpu_set),&orig_cpu_set) < 0 )   //backup orig_cpu_set
            fprintf(stderr,"get thread affinity failed\n");
            
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(0, &mask);
        if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)                  //bind the thread to CPU0
            fprintf(stderr,"set thread affinity failed\n");
        printf("Thread1 binds to CPU1\n");
        ret = (*orig_pthread_create)(__newthread, __attr, __start_routine, __arg);
        
        if (pthread_setaffinity_np(pthread_self(),sizeof(orig_cpu_set),&orig_cpu_set) < 0)  //recover orig_cpu_set
            fprintf(stderr,"set thread affinity failed\n");
    }else if(__start_routine == thread2){
        int j;
        cpu_set_t orig_cpu_set;
        CPU_ZERO(&orig_cpu_set);
        if (pthread_getaffinity_np(pthread_self(),sizeof(orig_cpu_set),&orig_cpu_set) < 0 )   //backup orig_cpu_set
            fprintf(stderr,"get thread affinity failed\n");
            
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(1, &mask);
        if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)                  //bind the thread to CPU1
            fprintf(stderr,"set thread affinity failed\n");
        ret = (*orig_pthread_create)(__newthread, __attr, __start_routine, __arg);
        printf("Thread2 binds to CPU1\n");
        
        if (pthread_setaffinity_np(pthread_self(),sizeof(orig_cpu_set),&orig_cpu_set) < 0)  //recover orig_cpu_set
            fprintf(stderr,"set thread affinity failed\n");
    }else{
        ret = (*orig_pthread_create)(__newthread, __attr, __start_routine, __arg);
    }
    
    return ret;
}

