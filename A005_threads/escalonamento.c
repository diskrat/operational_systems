#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#define N 12

int main() {
    printf("Threads: %d\n", N);

    // Cria N processos consumidores de CPU
    for (int i = 0; i < N; i++) {
        if (fork() == 0) {
            while (1); // Consome CPU
        }
    }

    // Cria 1 processo bloqueado por entrada
    if (fork() == 0) {
        char buffer[100];
        printf("Processo bloqueado aguardando entrada...\n");
        scanf("%s", buffer);
        printf("Entrada recebida: %s\n", buffer);
        while (1);
    }
    while(1);
}