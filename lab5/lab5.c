/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementação e uso de sincronização por barreira */

/***** Condicao logica da aplicacao: a cada iteracao, as threads fazem uma parte do processamento e só podem continuar depois que todas as threads completaram essa iteração. ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define RED "\x1b[31m"
#define YEL "\x1b[33m"
#define WHT "\x1b[37m"
#define GRN "\x1b[32m"

#define NTHREADS 4

// Variaveis globais
int bloqueadas = 0;
int esperando = 0;
int vet[NTHREADS];
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

//funcao barreira
void barreira(int nthreads)
{
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads - 1))
    {
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&x_cond);
        bloqueadas = 0;
    }
    else
    {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa(void *arg)
{
    int id = *(int *)arg, i, j;
    long somaLocal = 0;

    for (i = 0; i < NTHREADS; i++)
    {
        //Somatorio da i interacao
        for (j = 0; j < NTHREADS; j++)
            somaLocal += vet[j];

        //sincronizacao condicional
        printf("Thread %d terminou a %d interacao de somatorio com valor: %ld\n", id, i + 1, somaLocal);
        pthread_mutex_lock(&x_mutex); //inicio secao critica
        esperando++;
        if (esperando == NTHREADS)
        {
            printf(GRN "Terminamos a %d interacao.\n\n" WHT, i + 1);
            esperando = 0;
        }
        pthread_mutex_unlock(&x_mutex); //fim secao critica
        barreira(NTHREADS);

        vet[id] = rand() % 10;
        barreira(NTHREADS);
    }
    pthread_exit((void *)somaLocal);
}

// Funcao principal
int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    int id[NTHREADS], somaThread[NTHREADS], i = 0;
    long retorno;
    srand(time(NULL));

    //  Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    //Inicializando o vetor com valores sortidos de 0 a 9
    for (i = 0; i < NTHREADS; i++)
        vet[i] = rand() % 10;

    // Cria as threads
    for (i = 0; i < NTHREADS; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, tarefa, (void *)&id[i]);
    }

    // Espera todas as threads completarem
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], (void **)&retorno);
        somaThread[i] = retorno;
        printf("Valor recebido da thread" YEL " %d" WHT ": %d\n", i, somaThread[i]);

        //Verificacao do retorno incoerente
        if (somaThread[0] != retorno)
        {
            printf(RED "O valor recebido da thread %d é incoerente.\n", i);
            exit(-1);
        }
    }

    printf("FIM.\n");

    // Desaloca variaveis e termina
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}