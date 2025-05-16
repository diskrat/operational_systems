# Tarefa 1: Modo usuário e chamadas de sistema

Aluno: João Victor Moura Lucas da Silva

Escreva um código mínimo em linguagem de montagem que contenha apenas uma instrução para atribuir um valor a um registrador qualquer da CPU. Compile-o com as flags apropriadas para evitar que o compilador adicione bibliotecas ou instruções extras. Ao executar o programa, você deverá observar uma falha de segmentação (segmentation fault).

1. Explique por que essa falha ocorre.
2. Corrija o código para que a falha não aconteça mais.
3. Execute o programa com o comando strace antes e depois da correção e observe a diferença na sequência de chamadas de sistema.
4. Explique por que foi necessário incluir uma chamada de sistema para encerrar corretamente o programa.
5. Discuta brevemente quais seriam as implicações se os programas de usuário pudessem ser executados livremente, sem recorrer ao sistema operacional para acessar recursos básicos como o encerramento
---
## Corpo da tarefa

Foi criado um arquivo `.s` e inserindo o seguinte:

```asm
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

A falha ocorre porque a execução eventualmente chega a um endereço de memória inválido.

## 2. Correção

Para consertar este problema o programa é reescrito:
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

## 3. Execução das duas versões usando `strace`

Ao usar `strace` no programa que causa `Segmentation fault`, o seguinte é mostrado:

Ao usar o mesmo no programa que foi implementado corretamente:

## 4. Motivo para usar chamada de sistema

Usar uma chamada de sistema é necessário para encerrar corretamente, pois em modo usuário o processo não tem permissão para finalizar sua execução sem usar uma chamada de sistema.

## 5. Implicações da execução livre de programas de usuário

Se os usuários pudessem acessar livremente os recursos do sistema, poderia haver instabilidade no sistema e os programas poderiam interferir na execução de outros programas, alterando espaço de memória que já estava sendo usado por outro indiscriminadamente. Isso poderia resultar em problemas de segurança da informação e não haveria garantia de que as alterações feitas não iriam causar a corrupção do sistema como um todo, fazendo este se tornar inutilizável.