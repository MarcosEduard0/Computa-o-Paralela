/* Disciplina: Computacao Concorrente */
/* Aluno: Marcos Eduardo */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

// Variaveis globais
sem_t condt2, condt3; //semaforos para sincronizar a ordem de execucao das threads

//funcao executada pela thread 1
void *t1(void *threadid)
{
    int *tid = (int *)threadid;
    sem_wait(&condt2); //espera T2 executar
    printf("Thread %d: Fique a vontade.\n", *tid);
    sem_post(&condt3); //permite que T3 execute

    pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2(void *threadid)
{
    int *tid = (int *)threadid;

    printf("Thread %d: Seja bem-vindo!\n", *tid);
    sem_post(&condt2); //permite que T1 ou T4 execute
    sem_post(&condt2); //permite que T4 ou T1 execute

    pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3(void *threadid)
{

    int *tid = (int *)threadid;

    sem_wait(&condt3); //espera T1 ou T4 executar
    sem_wait(&condt3); //espera T1 ou T4 executar
    printf("Thread %d: Volte sempre!\n", *tid);

    pthread_exit(NULL);
}

//funcao executada pela thread 4
void *t4(void *threadid)
{
    int *tid = (int *)threadid;
    sem_wait(&condt2); //espera T2 executar
    printf("Thread %d: Sente-se por favor.\n", *tid);
    sem_post(&condt3); //permite que T3 execute

    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[])
{
    pthread_t tid[NTHREADS];
    int *id[4], t;

    for (t = 0; t < NTHREADS; t++)
    {
        if ((id[t] = malloc(sizeof(int))) == NULL)
        {
            pthread_exit(NULL);
            return 1;
        }
        *id[t] = t + 1;
    }

    //inicia os semaforos
    sem_init(&condt2, 0, 0);
    sem_init(&condt3, 0, 0);

    //cria as tres threads
    if (pthread_create(&tid[1], NULL, t2, (void *)id[1]))
    {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }
    if (pthread_create(&tid[2], NULL, t3, (void *)id[2]))
    {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }
    if (pthread_create(&tid[0], NULL, t1, (void *)id[0]))
    {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }
    if (pthread_create(&tid[3], NULL, t4, (void *)id[3]))
    {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    //espera todas as threads terminarem
    for (t = 0; t < NTHREADS; t++)
    {
        if (pthread_join(tid[t], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
        free(id[t]);
    }
    pthread_exit(NULL);
}
