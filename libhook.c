#define _GNU_SOURCE


#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>

typedef int (*orig_pthread_create_ftype)(pthread_t *, const pthread_attr_t *, void* (void *), void *);

int exec_count = 0;

void* thread1 = 0x4009fb;
void* thread2 = 0x400ab4;

int pthread_create(pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg){

    
    if(__start_routine == thread1){
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(0, &mask);  //binding to CPU0
        if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
            fprintf(stderr,"A: set thread affinity failed\n");
        printf("%x binds to CPU1\n", pthread_self());
    }else if(__start_routine == thread2){
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(1, &mask);  //binding to CPU1
        if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
            fprintf(stderr,"B: set thread affinity failed\n");
        printf("%x binds to CPU1\n", pthread_self());
    }
    
    orig_pthread_create_ftype orig_pthread_create;
    orig_pthread_create = (orig_pthread_create_ftype)dlsym(RTLD_NEXT, "pthread_create");
    
    return (*orig_pthread_create)(__newthread, __attr, __start_routine, __arg);
}

