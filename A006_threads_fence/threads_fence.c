
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>

#define FALSE 0
#define TRUE  1
#define N     2                      /* number of processes */


int turn;                            /* whose turn is it? */
int interested[N];                   /* all values initially 0 (FALSE) */

void enter_region(int process)       /* process is 0 or 1 */
{
    int other;                      /* number of the other process */
    other = 1 - process;            /* the opposite of process */
    interested[process] = TRUE;     /* show that you are interested */
    turn = process;                 /* set flag */
    atomic_thread_fence(memory_order_seq_cst);
    while (turn == process && interested[other] == TRUE) /* null statement */ ;
}
 
void leave_region(int process)       /* process: who is leaving */
{
    interested[process] = FALSE;    /* indicate departure from critical region */
}

uint64_t something = 0;

void *thread(void *arg)
{
    int proc = (size_t)arg;
    
    
    for(int i = 0; i < 100000; i++){
        enter_region(proc);
        something++;
        leave_region(proc);
    }
    return NULL;
}

int main(void)
{
    pthread_t th, th2;
    pthread_create(&th, NULL,thread,0);
    pthread_create(&th2, NULL,thread,(void*)1);
    pthread_join(th, NULL);
    pthread_join(th2, NULL);
    printf("Final value of something: %lu\n", something);
    return 0;
}