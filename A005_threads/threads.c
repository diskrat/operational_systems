#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

uint64_t something = 0;

void *thread(void *arg)
{
    for(int i = 0; i < 100000000; i++){
        something++;
    }
    return NULL;
}


int main(void)
{
    pthread_t th, th2;
    int rc = pthread_create(&th, NULL,thread,NULL);
    int rc2 = pthread_create(&th2, NULL,thread,NULL);
    if(rc !=0 || rc2 !=0) {
        fprintf(stderr," Erro ao criar thread:  %d\n",rc);
        exit(1);
    }
    pthread_join(th, NULL);
    pthread_join(th2, NULL);
    printf("Final value of something: %lu\n", something);
    return 0;
}