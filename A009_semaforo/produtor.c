#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>

#define TAMANHO 10
struct esperando{
    pthread_mutex_t m;
    struct esperando *prox;
};

struct semaforo {
    pthread_mutex_t trava;
    size_t valor;
    struct esperando *cabeca;
    struct esperando *cauda;
};
void sem_inicializar(struct semaforo *s)
{
    pthread_mutex_init(&s->trava,NULL);
    s->valor = 1;
    s->cabeca = NULL;
    s->cauda = NULL;
};

void sem_incrementar(struct semaforo *s)
{
    struct esperando *esp;
    pthread_mutex_lock(&s->trava);
    esp = s->cabeca;
    if(esp != NULL){
        s->cabeca = esp->prox;
        if(s->cabeca == NULL){
            s->cauda = NULL;
        }
    }
    s->valor++;
    pthread_mutex_unlock(&s->trava);
    if(esp != NULL){
        pthread_mutex_unlock(&esp->m);
    }
};
void sem_decrementar(struct semaforo *s){
    struct esperando esp;
    for(;;){
        pthread_mutex_lock( &s->trava);
        if(s->valor > 0){
            s->valor--;
            pthread_mutex_unlock(&s->trava);
            return;
        }
        pthread_mutex_init(&esp.m,NULL);
        pthread_mutex_lock(&esp.m);
        esp.prox = NULL;
        if(s->cauda){
            s->cauda->prox = &esp;
        }else{
            s->cabeca = &esp;
        }
        s->cauda = &esp;
        pthread_mutex_unlock(&s->trava);
        pthread_mutex_lock(&esp.m);
    }
};


volatile int dados [TAMANHO];
volatile size_t inserir = 0;
volatile size_t remover = 0;
// implementar semaforo

                   
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

void *produtor (void *arg)
{
    int v;
    for (v= 1;;v++){
        enter_region();
        printf("Produzindo %d\n",v);
        dados[inserir] = v;
        inserir = (inserir + 1) % TAMANHO;
        leave_region();
        // sem_incrementar(&sem_fila);
        usleep(1);
    }
    return NULL;
}

void *consumidor (void *arg)
{
    for(;;){
        enter_region();
        while(inserir == remover){leave_region();enter_region();};
        printf("%zu: Consumindo %d\n",(size_t) arg,dados[remover]);
        remover = (remover + 1) % TAMANHO;
        leave_region();
    }
}

int main() {
    pthread_t produtor_thread, consumidor_thread,consumidor_2_thread;

    if (pthread_create(&produtor_thread, NULL, produtor, NULL) != 0) {
        perror("Failed to create produtor thread");
        return 1;
    }

    if (pthread_create(&consumidor_thread, NULL, consumidor, NULL) != 0) {
        perror("Failed to create consumidor thread");
        return 1;
    }
    if (pthread_create(&consumidor_2_thread, NULL, consumidor, NULL) != 0) {
        perror("Failed to create consumidor thread");
        return 1;
    }

    pthread_join(produtor_thread, NULL);
    pthread_join(consumidor_thread, NULL);
    pthread_join(consumidor_2_thread, NULL);

    return 0;
}
