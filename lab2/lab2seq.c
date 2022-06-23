/* Disciplina: Computacao Concorrente 2021.1*/
/* Aluno: Marcos Eduardo */
/* Módulo 1 - Laboratório: 2 */
/* Data: 21/07/2021 */
/* Codigo: "Atividade 5" usando threads em C para calcular potência de cada elemento de um mat2or */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"

//Funcao principal
int main(int argc, char *argv[])
{
    int linha, coluna, somaprod, dim;
    float *mat1, *mat2, *mat3;
    double inicio, fim, delta, total;
    srand(time(NULL));

    GET_TIME(inicio);
    //leitura e avalicao de paramentros
    if (argc < 2)
    {
        printf("Digite : %s <dimensao da matriz>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);

    //alocacao de memoria
    mat1 = (float *)malloc(sizeof(float) * dim * dim);
    mat2 = (float *)malloc(sizeof(float) * dim * dim);
    mat3 = (float *)malloc(sizeof(float) * dim * dim);

    if (mat1 == NULL || mat2 == NULL || mat3 == NULL)
    {
        printf("ERRO-- malloc\n");
        return 2;
    }

    //inicializacao das estruturas de dados
    for (linha = 0; linha < dim; linha++)
    {
        for (coluna = 0; coluna < dim; coluna++)
        {
            mat1[linha * dim + coluna] = ((rand() % 1000) / 2.0) + (-250.0);
            mat2[linha * dim + coluna] = ((rand() % 1000) / 2.0) + (-250.0);
        }
    }

    GET_TIME(fim);
    delta = fim - inicio;
    total = delta;
    printf("Tempo de inicialização: %lf\n", delta);

    GET_TIME(inicio);
    //multiplicação sequencial para verificação de resultado
    for (linha = 0; linha < dim; linha++)
        for (coluna = 0; coluna < dim; coluna++)
        {
            somaprod = 0;
            for (int k = 0; k < dim; k++)
                somaprod += mat1[linha * dim + k] * mat2[k * dim + coluna];
            mat3[linha * dim + coluna] = somaprod;
        }
    GET_TIME(fim);
    delta = fim - inicio;
    total += delta;
    printf("Tempo de multiplicacao: %lf\n", delta);

    GET_TIME(inicio);
    //liberacao da memoria
    free(mat1);
    free(mat2);
    free(mat3);
    GET_TIME(fim);
    delta = fim - inicio;
    total += delta;
    printf("Tempo de finalização: %lf\n", delta);
    printf("Tempo total de execução: %lf\n", total);
    return 0;
}
