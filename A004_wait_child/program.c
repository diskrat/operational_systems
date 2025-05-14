#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    pid_t completed, pid = fork();
    int v;
    if(pid < 0) {
        return -1;
    }
    if(pid ==0) {
        printf("Novo processo: %d\n", getpid());
        exit(10);
    }else {
        printf("Processo original, filho=%d\n",pid);
        completed = wait(NULL);
        printf("fillho acabou,pid= %d\n",completed);
    }
    return 0;
}