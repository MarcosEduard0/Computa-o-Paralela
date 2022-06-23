#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAMBUFFER 10 //Tamanho do buffer principal
#define C 2          //numero de threads leitoras
#define P 3          //numero de threads escritoras
#define NTHREADS 5

#define RED "\x1b[31m"
#define YEL "\x1b[33m"
#define GRN "\x1b[32m"
#define CYN "\x1b[36m"
#define RST "\x1b[0m"

// Variaveis globais
int mainBuffer[TAMBUFFER];
sem_t mutexCons, mutexProd, bufferCheio, bufferVazio; //semaforos para sincronizar a ordem de execucao das threads

void insere(int id)
{
    static int in = 0;

    sem_wait(&bufferVazio);
    sem_wait(&mutexProd);

    mainBuffer[in] = id;
    in = (in + 1) % TAMBUFFER;

    if (!in)
    {
        printf(RED "P[%d] buffer cheio, enviando sinal para o consumidor.\n" RST, id);
        sem_post(&bufferCheio);
    }
    sem_post(&mutexProd);
}

void *produtor(void *arg)
{

    int id = *(int *)arg;
    while (1)
    {
        printf(GRN "P[%d] vai inserir\n" RST, id);
        insere(id);
        printf(GRN "P[%d] terminou de inserir\n" RST, id);
        sleep(1);
    }

    pthread_exit(NULL);
}

int retira()
{
    sem_wait(&bufferCheio);
    for (int i = 0; i < TAMBUFFER; i++)
        sem_post(&bufferVazio);

    return 1;
}

void *consumidor(void *arg)
{
    int id = *(int *)arg;

    while (1)
    {
        printf(YEL "C[%d] quer retirar\n" RST, id);
        retira();
        printf(YEL "C[%d] terminou de retirar\n" RST, id);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t *threads;
    int *id, i;
    //inicia os semaforos
    sem_init(&bufferCheio, 0, 0);
    sem_init(&bufferVazio, 0, TAMBUFFER);
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);

    //Alocano memoria
    if ((threads = malloc(sizeof(pthread_t) * NTHREADS)) == NULL)
    {
        printf("ERRO-- malloc\n");
        return 1;
    }
    if ((id = malloc(sizeof(int) * NTHREADS)) == NULL)
    {
        printf("ERRO-- malloc\n");
        return 1;
    }

    //cria as threads Consumidoras
    for (i = 0; i < C; i++)
    {
        id[i] = i + 1;
        if (pthread_create(&threads[i], NULL, consumidor, (void *)&id[i]))
            exit(-1);
    }

    //cria as threads Produtoras
    for (i = 0; i < P; i++)
    {
        id[i + P] = i + 1;
        if (pthread_create(&threads[i + P], NULL, produtor, (void *)&id[i + P]))
            exit(-1);
    }

    // Espera todas as threads completarem
    for (i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    free(threads);
    free(id);
    pthread_exit(NULL);
}