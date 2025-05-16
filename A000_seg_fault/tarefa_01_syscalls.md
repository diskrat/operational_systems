# Tarefa 1: Modo usuário e chamadas de sistema
Escreva um código mínimo em linguagem de montagem que contenha apenas uma instrução para atribuir um valor a um registrador qualquer da CPU. Compile-o com as flags apropriadas para evitar que o compilador adicione bibliotecas ou instruções extras. Ao executar o programa, você deverá observar uma falha de segmentação (segmentation fault).

1. Explique por que essa falha ocorre.
2. Corrija o código para que a falha não aconteça mais.
3. Execute o programa com o comando strace antes e depois da correção e observe a diferença na sequência de chamadas de sistema.
4. Explique por que foi necessário incluir uma chamada de sistema para encerrar corretamente o programa.
5. Discuta brevemente quais seriam as implicações se os programas de usuário pudessem ser executados livremente, sem recorrer ao sistema operacional para acessar recursos básicos como o encerramento

## Corpo da tarefa

Foi criado um arquivo `.s` e inserindo o seguinte:

```
.text
.globl _start
_start:
    mov $0x3c, %eax

```
O resultado esperado foi observado:

```
Segmentation fault (core dumped)
```

## 1. Motivo da falha

A falha ocorre por conta da execução eventualmente chegar a um endereço de memória invalido.

## 2. Correção

Para concertar este problema o programa é rescrito:
```asm
.text
.globl _start
_start:
    mov     $12,%rdx
    leaq    msg,%rsi
    movq $1, %rdi
    movl $0x01, %eax
    syscall

    xorl    %edi, %edi 
    mov     $0x3c, %eax
    syscall

.section .rodata
msg: .asciz "Hello world\n"

```

## 3. Execução das duas versoes usando `strace`

Ao usar `strace` no programa que causa `Segmentation fault` o seguinte é mostrado:
```
execve("./wrong_answer", ["./wrong_answer"], 0x7fff9eb2ad70 /* 35 vars */) = 0
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=0x3c} ---
+++ killed by SIGSEGV (core dumped) +++
Segmentation fault (core dumped)
```
Ao usar o mesmo no programa que foi implementado corretamente:
```
execve("./right_answer", ["./right_answer"], 0x7fff6cc02e80 /* 35 vars */) = 0
write(1, "Hello world\n", 12Hello world
)           = 12
exit(0)                                 = ?
+++ exited with 0 +++
```

## 4. Motivo para usar chamada de sistema

Usar uma chamada de sistema é necessária para encerrar corretamente, pois em modo usuário o processo não tem permissão para finalizar sua execução sem usar uma chamada de sistema.

## 5. Implicações da execução livre de programas de usuário
Se os 