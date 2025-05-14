# Tarefa 1: Modo usuário e chamadas de sistema
Escreva um código mínimo em linguagem de montagem que contenha apenas uma instrução para atribuir um valor a um registrador qualquer da CPU. Compile-o com as flags apropriadas para evitar que o compilador adicione bibliotecas ou instruções extras. Ao executar o programa, você deverá observar uma falha de segmentação (segmentation fault).

1. Explique por que essa falha ocorre.
2. Corrija o código para que a falha não aconteça mais.
3. Execute o programa com o comando strace antes e depois da correção e observe a diferença na sequência de chamadas de sistema.
4. Explique por que foi necessário incluir uma chamada de sistema para encerrar corretamente o programa.
5. Discuta brevemente quais seriam as implicações se os programas de usuário pudessem ser executados livremente, sem recorrer ao sistema operacional para acessar recursos básicos como o encerramento

```
.text
.globl _start
_start:
    mov $0x3c, %eax
```

Em seguida executo no terminal:

```bash
gcc hello.s -nostdlib -static
```


