#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

uint64_t valor = 0;
void *thread(void *arg)
{
    size_t i = 1000000;
    while (i--)
    {
        valor++;
    }
}

int main(void)
{
    pthread_t th, th2;
    int rc = pthread_create(&th, NULL, thread, NULL);
    int rc2 = pthread_create(&th2, NULL, thread, NULL);
    pthread_join(th, NULL);
    pthread_join(th2, NULL);
    printf("Final value: %lu\n", valor);
    return 0;
}