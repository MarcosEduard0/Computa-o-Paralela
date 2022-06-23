/* Disciplina: Computacao Concorrente 2021.1*/
/* Aluno: Marcos Eduardo */
/* Módulo 1 - Laboratório: 3 */
/* Data: 26/07/2021 */
/* Codigo: "Atividade 1" usando threads em C para calcular a multiplicacao de matrizes quadraticas*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

#define RED "\x1b[31m"
#define YEL "\x1b[33m"
#define GRN "\x1b[32m"
#define CYN "\x1b[36m"

long long int dim;
int NTHREADS;
float *vet;

void *Somatorio(void *arg)
{
    long int idThread = (long int)arg;
    long int tamBloco = dim / NTHREADS;
    long int init = idThread * tamBloco;
    long int end = init + tamBloco;
    float *retorno;

    retorno = (float *)malloc(sizeof(float) * 2);

    if (retorno == NULL)
    {
        fprintf(stderr, "ERRO-- malloc\n");
        exit(1);
    }
    retorno[0] = 99999; //posição que ira representar o menor
    retorno[1] = 0;     //posição que ira representar o maior

    if (idThread == NTHREADS - 1)
        end = dim;

    for (long int i = init; i < end; i++)
    {
        if (vet[i] > retorno[1]) //verificar se o numero atual é maior que o anterior
        {
            retorno[1] = vet[i];
        }
        else if (vet[i] < retorno[0]) //verificar se o numero atual é menor que o anterior
        {
            retorno[0] = vet[i];
        }
    }

    // printf("|---> A thread %ld terminou, com valor: %ld <---|\n", idThread, somaLocal);
    pthread_exit((void *)retorno);
}

int main(int argc, char *argv[])
{
    long int thread;
    float maiorSeq = 0;       //variavel para guardar o maior valor do vertor do metodo sequencial
    float menorSeq = 10000;   //variavel para guardar o menor valor do vertor do metodo sequencial
    float maiorConc = 0;      //variavel para soma total do metodo com concorrentes
    float menorConc = 100000; //variavel para soma total do metodo com concorrentes
    float *retorno;           //variavel para o retorno das threads
    double inicio, fim;       //variaveis para controle de tempo
    srand(time(NULL));

    //leitura e avalicao de paramentros
    if (argc < 3)
    {
        fprintf(stderr, "Digite : %s <dimensao do vetor> <nº de thread>\n", argv[0]);
        return 1;
    }
    dim = atoll(argv[1]);
    NTHREADS = atoi(argv[2]);

    pthread_t tid_sistema[NTHREADS]; //Identificadores das threads no sistema

    //alocacao de memoria do vetor
    vet = (float *)malloc(sizeof(float) * dim);

    if (vet == NULL)
    {
        fprintf(stderr, RED "ERRO-- malloc\n");
        return 2;
    }

    //inicializacao com valores reais.
    for (long int i = 0; i < dim; i++)
        vet[i] = ((rand() % 10000) / (i + 1.0)) - (rand() % 5000); //gerando numero sortidos negativos e positivos entre [10000, -5000]

    GET_TIME(inicio);
    //funcao sequencual
    for (long int i = 0; i < dim; i++)
    {
        if (vet[i] > maiorSeq)
            maiorSeq = vet[i];
        else if (vet[i] < menorSeq)
        {
            menorSeq = vet[i];
        }
    }
    GET_TIME(fim);
    printf(YEL "Tempo de busca seq: %lf\n", fim - inicio);

    GET_TIME(inicio);
    //criação das thread e execução da funcao paralela
    for (thread = 0; thread < NTHREADS; thread++)
    {
        if (pthread_create(&tid_sistema[thread], NULL, Somatorio, (void *)thread))
        {
            fprintf(stderr, RED "|-->ERRO: pthread_create() <--|\n");
            return 3;
        }
    }

    //Espera todas as threads terminarem
    for (thread = 0; thread < NTHREADS; thread++)
    {
        if (pthread_join(tid_sistema[thread], (void **)&retorno))
        {
            printf(RED "|-->ERRO: pthread_join()<--| \n");
            exit(-1);
        }
        if (retorno[1] > maiorConc) //verificar se o numero atual é maior que o anterior
        {
            maiorConc = retorno[1];
        }
        else if (retorno[0] < menorConc) //verificar se o numero atual é menor que o anterior
        {
            menorConc = retorno[0];
        }

        free(retorno);
    }
    GET_TIME(fim);

    printf("Tempo de busca conc: %lf\n", fim - inicio);
    printf(GRN "Maior nº conc: %f\n", maiorConc);
    printf("Maior nº seq: %f\n", maiorSeq);
    printf(CYN "Menor nº conc: %f\n", menorConc);
    printf("Menor nº seq: %f\n", menorSeq);

    GET_TIME(inicio);

    //liberacao da memoria
    free(vet);
    pthread_exit(NULL);
    return 0;
}
