#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#define FALSE 0
#define TRUE  1
#define N     8 
                   
uint64_t valor = 0;
_Atomic uint32_t trava = 0;
void enter_region(void)       
{
    uint32_t v = 0;
    if (atomic_compare_exchange_strong(&trava, &v, 1)) {
        return;
    }
    do {
        if (v == 2 || atomic_compare_exchange_strong(&trava,&v, 2)) {
            syscall(SYS_futex, &trava, FUTEX_WAIT, 2);
        }
    }while(!atomic_compare_exchange_strong(&trava, &v,2));
}
 
void leave_region(void)       
{
    
}

uint64_t something = 0;


void *thread(void *arg)
{
    int proc = (size_t)arg;
    for(int i = 0; i < 100000000; i++){
        enter_region();
        something++;
        leave_region();
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