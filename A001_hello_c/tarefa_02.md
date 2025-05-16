# Tarefa 2: Modo usuário e chamadas de sistema

Aluno: João Victor Moura Lucas da Silva

Utilizando a linguagem C, escreva um programa simples que utilize a função `write()` da biblioteca padrão para imprimir uma mensagem na tela. Compile e execute o programa normalmente e, em seguida, utilize o comando `strace` para observar as chamadas de sistema realizadas durante a execução. Identifique no `strace` a chamada que realiza a escrita no terminal e observe qual mecanismo é utilizado para a transição entre o modo usuário e o modo kernel.

A seguir, reescreva o mesmo programa utilizando linguagem de montagem, implementando a chamada de sistema correspondente de forma manual (por exemplo, usando a instrução `syscall`). Compile e execute o código e, novamente, utilize `strace` para observar a diferença no fluxo de execução.
1. Qual é o papel da instrução syscall no programa em Assembly?
2. O que muda na forma de execução entre o programa em C e o em Assembly?
3. Por que é necessário utilizar chamadas de sistema para realizar tarefas simples como imprimir na tela?
4. O que garante a transição segura entre os modos de execução? O que aconteceria se o modo usuário tivesse acesso direto aos dispositivos?
---

## Corpo da tarefa

### Programa em C

```c
#include <unistd.h>
int main() {
	write(1,"Hello World!\n",13);
	return 0;
}
```

### Programa em Assembly

```asm
.text
.globl _start
_start:
    mov     $13,%rdx
    leaq    msg,%rsi
    movq $1, %rdi
    movl $0x01, %eax
    syscall

    xorl    %edi, %edi 
    mov     $0x3c, %eax
    syscall

.section .rodata
msg: .asciz "Hello world!\n"

```

---

### Execução com `strace`

No programa em C:

```
write(1, "Hello World!\n", 13Hello World!
)          = 13
exit_group(0)                           = ?
+++ exited with 0 +++
```

No programa em Assembly:

```
write(1, "Hello world!\n", 13Hello world!
)          = 13
exit(0)                                 = ?
+++ exited with 0 +++
```

Após a compilação, o código é muito semelhante nas duas instâncias, a única diferença foi o boilerplate criado na versão em C, que foi omitido para facilitar a clareza.

---

## 1. Papel da instrução syscall em Assembly

A instrução `syscall` é responsável por solicitar ao kernel a execução de uma chamada de sistema. Ela realiza a transição do modo usuário para o modo kernel, permitindo que o programa acesse recursos protegidos do sistema operacional.

## 2. O que muda na forma de execução entre o programa em C e o em Assembly?

No programa em C, a chamada de sistema é feita implicitamente ao final do código.

## 3. Por que é necessário utilizar chamadas de sistema

Chamadas de sistema são necessárias porque operações que envolvem acesso a interfaces de hardware são protegidas pelo sistema operacional e apenas o kernel tem permissão para acessar diretamente esses recursos.

## 4. O que garante a transição segura entre os modos de execução? O que aconteceria se o modo usuário tivesse acesso direto aos dispositivos?

A transição segura é garantida pelas chamadas de sistema, que validam as operações dos programas em modo usuário antes de executar qualquer operação privilegiada. Se o modo usuário tivesse acesso direto aos dispositivos, haveria riscos de corrupção de dados, falhas de segurança e instabilidade do sistema, pois qualquer programa poderia interferir no funcionamento do hardware e de outros processos.