#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
                   
uint64_t something = 0;
uint64_t valor = 0;
_Atomic uint32_t trava = 0;
void enter_region(void)       
{
    uint32_t v = 0;
    if (atomic_compare_exchange_strong_explicit(&trava, &v, 1,memory_order_acquire,memory_order_relaxed)) {
        return;
    }
    do {
        if (v == 2 || atomic_compare_exchange_weak_explicit(&trava,&v, 2,memory_order_relaxed,memory_order_relaxed)) {
            syscall(SYS_futex, &trava, FUTEX_WAIT, 2);
        }
        v=0;
    }while(!atomic_compare_exchange_weak_explicit(&trava, &v,2,memory_order_acquire,memory_order_relaxed));
}
 
void leave_region(void)       
{
    uint32_t v = atomic_fetch_sub_explicit(&trava,1,memory_order_release);
    if (v != 1) {
        atomic_store_explicit(&trava, 0,memory_order_relaxed);
        syscall(SYS_futex, &trava, FUTEX_WAKE, 1);
    }

}



void *thread_something(void *arg)
{
    for(int i = 0; i < 1000000000; i++){
        enter_region();
        something++;
        leave_region();
    }
    return NULL;
}


int main(void)
{   
    int num_threads = 2;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, thread_something, NULL) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final value of something: %lu\n", something);
    return 0;
}