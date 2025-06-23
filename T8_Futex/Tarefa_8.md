# Tarefa 8: Mutexes
Aluno: João Victor Moura Lucas da Silva


Dada a implementação em linguagem C de um Mutex abaixo:
```c
atomic_bool trava = false;
void enter_region(void)
{
    bool v;
    do {
        v = false;
    } while (!atomic_compare_exchange_strong(&trava, &v, true));
}
void leave_region(void)
{
    atomic_store(&trava, false);
}
```
Responda:
1. Por que a implementação acima não é eficiente quando há contenção?
2. Como a implementação poderia ser minimamente modificada para usar a chamada
de sistema futex do Linux? Como essa versão seria melhor que a original?
3. Por que a versão anterior ainda não é ideal do ponto de vista de desempenho?
4. Implemente uma versão que resolve os problemas da questão anterior. Como ela
resolve os problemas?
Enviar relatório em PDF com no máximo 4 páginas ou 2000 palavras.

## Corpo da Resposta
1. A implementação acima não é eficiente quando há contenção porque ela utiliza um loop busy-waiting, onde a thread fica em um loop ativo tentando adquirir o mutex, consumindo CPU desnecessariamente enquanto espera. Isso pode levar a um alto consumo de recursos e degradação do desempenho do sistema.

2. 
```c
atomic_bool trava = false;
void enter_region(void)
{
    bool v = false;
    while (!atomic_compare_exchange_strong(&trava, &v, true))
    {
         futex((int *)&trava, FUTEX_WAIT, 1, NULL, NULL, 0);
    }
}
void leave_region(void)
{
    atomic_store(&trava, false);
    futex((int *)&trava, FUTEX_WAKE, 1, NULL, NULL, 0);
}
```
    Essa versão é melhor que a original porque utiliza a chamada de sistema `futex`, que bota a thread para dormir quando não consegue adquirir o mutex, liberando a CPU para outras tarefas.
    3. A versão anterior ainda não é ideal do ponto de vista de desempenho porque, mesmo utilizando `futex`, ela ainda pode levar a um alto número de chamadas de sistema se houver muita contenção.

4.
```c
uint64_t valor = 0;
_Atomic uint32_t trava = 0;
void enter_region(void)       
{
    uint32_t v = 0;
    if (atomic_compare_exchange_strong(&trava, &v, 1)) {
        return;
    }
    do {
        if (v == 2 || atomic_compare_exchange_strong(&trava,&v, 2)) {
            syscall(SYS_futex, &trava, FUTEX_WAIT, 2);
        }
        v=0;
    }while(!atomic_compare_exchange_strong(&trava, &v,2));
}
 
void leave_region(void)       
{
    uint32_t v = atomic_fetch_sub(&trava,1);
    if (v != 1) {
        atomic_store(&trava, 0);
        syscall(SYS_futex, &trava, FUTEX_WAKE, 1);
    }

}
```
    Esta versao resolve os problemas criando dois caminhos diferentes para a thread que está tentando adquirir o mutex. Se a thread conseguir adquirir o mutex, ela retorna imediatamente. Se não, ela verifica se o mutex está em um estado de espera (valor 2) e, se estiver, chama `futex` para esperar.