/* Disciplina: Computacao Concorrente 2021.1*/
/* Aluno: Marcos Eduardo */
/* Módulo 1 - Laboratório: 2 */
/* Data: 22/07/2021 */
/* Codigo: "Atividade 1" usando threads em C para calcular a multiplicacao de matrizes quadraticas*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

float *mat1, *mat2, *mat3;
int dim, NTHREADS;

void verificacao(float *mat);
void *Multiplicacao(void *arg); //Funcao que as threads irão executar

int main(int argc, char *argv[])
{
    int linha, coluna, thread;
    double inicio, fim, delta, total = 0;
    srand(time(NULL));

    GET_TIME(inicio);
    //leitura e avalicao de paramentros
    if (argc < 3)
    {
        printf("Digite : %s <dimensao da matriz> <nº de thread>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);
    NTHREADS = atoi(argv[2]);

    pthread_t tid_sistema[NTHREADS]; //Identificadores das threads no sistema
    int tid_local[NTHREADS];

    //alocacao de memoria
    mat1 = (float *)malloc(sizeof(float) * dim * dim);
    mat2 = (float *)malloc(sizeof(float) * dim * dim);
    mat3 = (float *)malloc(sizeof(float) * dim * dim);

    if (mat1 == NULL || mat2 == NULL || mat3 == NULL)
    {
        printf("ERRO-- malloc\n");
        return 2;
    }

    //inicializacao com valores sortidos entre -250.0 e 250.0, para termos numeros reais
    for (linha = 0; linha < dim; linha++)
        for (coluna = 0; coluna < dim; coluna++)
        {
            mat1[linha * dim + coluna] = ((rand() % 1000) / 2.0) + (-250.0);
            mat2[linha * dim + coluna] = ((rand() % 1000) / 2.0) + (-250.0);
        }

    GET_TIME(fim);
    delta = fim - inicio;
    total = delta;
    printf("Tempo de inicialização: %lf\n", delta);

    GET_TIME(inicio);
    //criação das thread e execução da multiplicação em paralelo
    for (thread = 0; thread < NTHREADS; thread++)
    {
        tid_local[thread] = thread;
        if (pthread_create(&tid_sistema[thread], NULL, Multiplicacao, (void *)&tid_local[thread]))
        {
            printf("\x1b[31m|-->ERRO: pthread_create() <--|\n");
            exit(-1);
        }
    }

    //Espera todas as threads terminarem
    for (thread = 0; thread < NTHREADS; thread++)
        if (pthread_join(tid_sistema[thread], NULL))
        {
            printf("\x1b[31m|-->ERRO: pthread_join()<--| \n");
            exit(-1);
        }

    GET_TIME(fim);
    delta = fim - inicio;
    total += delta;
    printf("Tempo da multiplicação com %d threads: %lf\n", NTHREADS, delta);

    GET_TIME(inicio);

    //verificacao(mat3); //multiplicação sequencial para verificação de resultado

    //liberacao da memoria
    free(mat1);
    free(mat2);
    free(mat3);
    GET_TIME(fim);
    delta = fim - inicio;
    total += delta;
    printf("Tempo de finalização: %lf\n", delta);
    printf("Tempo total de execução: %lf\n", total);
    pthread_exit(NULL);

    return 0;
}

void *Multiplicacao(void *arg)
{
    int idThread = *(int *)arg, linha, coluna;
    float somaprod;

    for (linha = idThread; linha < dim; linha += NTHREADS)
        for (coluna = 0; coluna < dim; coluna++)
        {
            somaprod = 0;
            for (int k = 0; k < dim; k++)
                somaprod += mat1[linha * dim + k] * mat2[k * dim + coluna];
            mat3[linha * dim + coluna] = somaprod;
        }
    pthread_exit(NULL);
}

void verificacao(float *mat)
{
    float somaprod;
    for (int linha = 0; linha < dim; linha++)
        for (int coluna = 0; coluna < dim; coluna++)
        {
            somaprod = 0;
            for (int k = 0; k < dim; k++)
                somaprod += mat1[linha * dim + k] * mat2[k * dim + coluna];
            if (mat3[linha * dim + coluna] != somaprod)
            {
                printf("\x1b[31m|--->Erro de multiplicação.<---|\nLinha: %d, Coluna: %d\nValor esperado: %.1f, Valor encontrado %.1f\n\x1b[0m", linha, coluna, somaprod, mat3[linha * dim + coluna]);
                exit(-1);
            }
        }
}