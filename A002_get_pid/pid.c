#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("PID: %d, PPID: %d\n", getpid(), getppid());
    return 0;
}