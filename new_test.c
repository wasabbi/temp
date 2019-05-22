#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int race_data = 0;

void *A(void *ptr){
    sleep(1);
    printf("A is sleeping");
    sleep(1);
    race_data = 100;
    printf("%s: %d\n", (char*)ptr, race_data);
}

void *B(void *ptr){
    sleep(1);
    race_data = 1;
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





