
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
 
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
    uint32_t v = atomic_fetch_sub(&trava,1);
    if (v != 1) {
        atomic_store(&trava, 0);
        syscall(SYS_futex, &trava, FUTEX_WAKE, 1);
    }

}

uint64_t something = 0;


void *thread_something(void *arg)
{
    for(int i = 0; i < 10000; i++){
        enter_region();
        something++;
        leave_region();
    }
    return NULL;
}


int main(void)
{
    pthread_t th, th2;
    pthread_create(&th, NULL,thread_something,0);
    pthread_create(&th2, NULL,thread_something,(void*)1);
    pthread_join(th, NULL);
    pthread_join(th2, NULL);
    printf("Final value of something: %lu\n", something);
    return 0;
}