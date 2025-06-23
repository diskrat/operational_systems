# Tarefa 4: Redirecionamento usando pipe
Aluno: João Victor Moura Lucas da Silva


Escreva um programa em linguagem C que executa um processo filho (por exemplo, ```ls -l```) e redireciona a sua saída padrão para um pipe cujo outro lado é lido pelo pai até ser fechado.

Responda:
1. Como funciona o mecanismo de redirecionamento?
2. Por que o processo pai precisa fechar o lado da escrita da pipe?
3. O que mudaria se o processo pai quisesse redirecionar a saída de erro padrão?
4. E se quisesse redirecionar a entrada padrão?

## Corpo da Resposta
```c
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
```
## Respostas
1. O mecanismo de redirecionamento `dup2()` permite que a saída padrão de um processo seja enviada para um pipe.
2. O processo pai precisa fechar o lado da escrita da pipe para indicar que não haverá mais dados a serem escritos, e detectar o fim da leitura.
3. Se o processo pai quisesse redirecionar a saída de erro padrão, poderia usar `dup2(fd[WRITE_END], STDERR_FILENO)` para redirecionar a saída de erro para o pipe.
4. Para redirecionar a entrada padrão, o pai poderia usar `dup2(fd[READ_END], STDIN_FILENO)` para ler dados do pipe como entrada padrão, porém os papeis seriam invertidos, e o pai escreveria no pipe e o filho leria dele. O exemplo também deveria ser outro que aceite uma entrada padrão como `less`.