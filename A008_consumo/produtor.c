#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define TAMANHO 10
volatile int dados [TAMANHO];
volatile size_t inserir = 0;
volatile size_t remover = 0;

void *produtor (void *arg)
{
    int v;
    for (v= 1;;v++){
        while(((inserir + 1) % TAMANHO) == remover);
        printf("Produzindo %d\n",v);
        dados[inserir] = v;
        inserir = (inserir + 1) % TAMANHO;
        usleep(500000);
    }
    return NULL;
}

void *consumidor (void *arg)
{
    for(;;){
        while(inserir == remover);
        printf("%zu: Consumindo %d\n",(size_t) arg,dados[remover]);
        remover = (remover + 1) % TAMANHO;
    }
}

int main() {
    pthread_t produtor_thread, consumidor_thread;

    if (pthread_create(&produtor_thread, NULL, produtor, NULL) != 0) {
        perror("Failed to create produtor thread");
        return 1;
    }

    if (pthread_create(&consumidor_thread, NULL, consumidor, NULL) != 0) {
        perror("Failed to create consumidor thread");
        return 1;
    }

    pthread_join(produtor_thread, NULL);
    pthread_join(consumidor_thread, NULL);

    return 0;
}
