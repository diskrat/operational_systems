# Tarefa 3: Criação e identificação de processos

Aluno: João Victor Moura Lucas da Silva

Escreva um programa em linguagem C que imprima na tela o identificador do processo atual (PID) e o identificador do processo pai (PPID), utilizando as funções `getpid()` e `getppid()`. Compile e execute o programa, e observe os valores exibidos. Em seguida, execute o programa múltiplas vezes e registre se os valores de PID e PPID permanecem os mesmos ou variam entre execuções. O que isso nos diz sobre a natureza dos processos?

A seguir, modifique o programa para que ele crie um novo processo utilizando a função `fork()`. O processo pai e o processo filho devem imprimir suas respectivas identificações, informando claramente quem é quem. Compile e execute o programa várias vezes, e analise os resultados.

Agora, utilize os comandos `ps`, `top` ou `htop` durante a execução do programa para observar os processos ativos. Tente identificar o processo pai e o filho em tempo real.
1. Qual é a diferença entre getpid() e getppid()? O que eles representam?
2. O que acontece com o PID do processo filho após o fork()?
3. Como o sistema operacional identifica e organiza os processos em execução?
4. É possível prever quantas vezes o programa imprimirá mensagens após o fork()? Justifique.
5. Por que o mesmo programa pode ter múltiplos processos com identidades distintas?

---

## Corpo da tarefa

### Programa em C - PID e PPID

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("PID: %d, PPID: %d\n", getpid(), getppid());
    return 0;
}
```

Ao executar o programa várias vezes, observa-se que o PID muda a cada execução, enquanto o PPID se mantém o mesmo, resultando em:

```
PID: 35980, PPID: 14039
PID: 35981, PPID: 14039
PID: 35982, PPID: 14039
PID: 35983, PPID: 14039
PID: 35984, PPID: 14039
```

### Programa em C - Com fork()

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("My PID is: %d\n", pid);
    printf("My parent's PID is: %d\n\n", ppid);

    pid_t child_pid = fork();
    if (child_pid == 0) {
        printf("I am the child process PID: %d\n", getpid());
        printf("My parent's PID is: %d\n\n", getppid());
    } else if (child_pid > 0) {
        printf("I am the parent process PID: %d\n", getpid());
        printf("My child's PID is: %d\n\n", child_pid);
        wait(NULL);
		printf("----------------------------\n");
    } else {
        printf("Fork failed\n");
        return 1;
    }

    
    return 0;
}
```
Comportamento observado: 
```
My PID is: 39553
My parent's PID is: 14039

I am the parent process PID: 39553
My child's PID is: 39554

I am the child process PID: 39554
My parent's PID is: 39553

----------------------------
My PID is: 39555
My parent's PID is: 14039

I am the parent process PID: 39555
My child's PID is: 39556

I am the child process PID: 39556
My parent's PID is: 39555

----------------------------
My PID is: 39557
My parent's PID is: 14039

I am the parent process PID: 39557
My child's PID is: 39558

I am the child process PID: 39558
My parent's PID is: 39557

----------------------------
```

## 1. Qual é a diferença entre getpid() e getppid()? O que eles representam?

- `getpid()` retorna o identificador do processo atual (PID).
- `getppid()` retorna o identificador do processo pai (PPID). 

Esses valores permitem que cada processo saiba seu próprio identificador e o identificador do processo que o criou.

## 2. O que acontece com o PID do processo filho após o fork()?

Após o `fork()`, o processo filho recebe um novo PID único, diferente do PID do processo pai. O PPID do filho será igual ao PID do pai.

## 3. Como o sistema operacional identifica e organiza os processos em execução?

O sistema operacional utiliza o PID para identificar cada processo de forma única. A relação entre processos é mantida por meio do PPID, formando uma árvore de processos.

## 4. É possível prever quantas vezes o programa imprimirá mensagens após o fork()? Justifique.

Sim. Após o `fork()`, tanto o processo pai quanto o filho executam o código seguinte. Como no exemplo é checado se estamos no processo pai ou no processo filho, cada parte do código será executado em seu devido processo.

## 5. Por que o mesmo programa pode ter múltiplos processos com identidades distintas?

Cada vez que o programa é executado, o sistema operacional cria um novo processo com um PID único. Quando o programa chama `fork()`, ele gera um novo processo filho, que também recebe um PID único. Assim, mesmo executando o mesmo código, cada instância possui sua própria identidade no sistema.
