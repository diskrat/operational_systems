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
    s->valor = 1; // Initialize to 1 for mutual exclusion, adjust as needed for counting semaphore
    s->cabeca = NULL;
    s->cauda = NULL;
};

void sem_incrementar(struct semaforo *s)
{
    struct esperando *esp;
    pthread_mutex_lock(&s->trava);
    s->valor++; // Increment the semaphore value
    esp = s->cabeca;
    if(esp != NULL){
        s->cabeca = esp->prox;
        if(s->cabeca == NULL){
            s->cauda = NULL;
        }
    }
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

// Semaphores for buffer management
struct semaforo sem_vazio; // Counts empty slots
struct semaforo sem_cheio; // Counts filled slots

void *produtor (void *arg)
{
    int v;
    for (v= 1;;v++){
        sem_decrementar(&sem_vazio); // Wait for an empty slot
        //enter_region(); // Remove this, as semaphores handle synchronization
        printf("Produzindo %d\n",v);
        dados[inserir] = v;
        inserir = (inserir + 1) % TAMANHO;
        //leave_region(); // Remove this
        sem_incrementar(&sem_cheio); // Signal that a slot is filled
        usleep(1);
    }
    return NULL;
}

void *consumidor (void *arg)
{
    for(;;){
        sem_decrementar(&sem_cheio); // Wait for a filled slot
        //enter_region(); // Remove this
        printf("%zu: Consumindo %d\n",(size_t) arg,dados[remover]);
        remover = (remover + 1) % TAMANHO;
        //leave_region(); // Remove this
        sem_incrementar(&sem_vazio); // Signal that a slot is empty
    }
}

int main() {
    pthread_t produtor_thread, consumidor_thread,consumidor_2_thread;

    // Initialize semaphores
    sem_inicializar(&sem_vazio);
    sem_inicializar(&sem_cheio);
    sem_vazio.valor = TAMANHO; // Initially, all slots are empty
    sem_cheio.valor = 0;       // Initially, no slots are filled

    if (pthread_create(&produtor_thread, NULL, produtor, NULL) != 0) {
        perror("Failed to create produtor thread");
        return 1;
    }

    if (pthread_create(&consumidor_thread, NULL, consumidor, (void*)1) != 0) { // Pass an identifier to the consumer
        perror("Failed to create consumidor thread");
        return 1;
    }
    if (pthread_create(&consumidor_2_thread, NULL, consumidor, (void*)2) != 0) { // Pass an identifier to the consumer
        perror("Failed to create consumidor thread");
        return 1;
    }

    pthread_join(produtor_thread, NULL);
    pthread_join(consumidor_thread, NULL);
    pthread_join(consumidor_2_thread, NULL);

    return 0;
}
