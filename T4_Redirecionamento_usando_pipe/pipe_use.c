#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define BUFFER_SIZE 1000
#define READ_END 0
#define WRITE_END 1

int main()
{
    /* 3.7 Examples of IPC Systems */
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pipe(fd);
    pid_t pid;
    pid = fork();
    if (pid == 0) { /* child process */
        /* Close Read, write Message,close Write */
        
        close(fd[READ_END]);
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);
        execlp("ls", "ls", "-l", NULL);
    }
    else { /* parent process */
        /* Close Write, read Message,close Read */
        close(fd[WRITE_END]);
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        close(fd[READ_END]);
        printf("Read: %s", read_msg);
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("----------- Child Complete -----------\n");
    }
    return 0;
}