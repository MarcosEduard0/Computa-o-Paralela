/* Disciplina: Computacao Concorrente 2021.1*/
/* Aluno: Marcos Eduardo */
/* Módulo 1 - Laboratório: 2 */
/* Data: 21/07/2021 */
/* Codigo: "Atividade 5" usando threads em C para calcular potência de cada elemento de um vetor */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

float *mat;
float *vet;
float *out;

//Funcao principal
int main(int argc, char *argv[])
{
    int dim, i, j;

    //leitura e avalicao de paramentros
    if (argc < 2)
    {
        printf("Digite : %s <dimensao da matriz>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);

    //alocacao de memoria
    mat = (float *)malloc(sizeof(float) * dim * dim);
    vet = (float *)malloc(sizeof(float) * dim);
    out = (float *)malloc(sizeof(float) * dim);
    if (mat == NULL || vet == NULL || out == NULL)
    {
        printf("ERRO-- malloc\n");
        return 2;
    }

    //inicializacao das estrutura de dados
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
            mat[i * dim + j] = 1; //igual a mat[i][j]
        vet[i] = 1;
        out[i] = 0;
    }

    //multiplicação
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
            out[i] += mat[i * dim + j] * vet[j];
    }

    //exibicao
    // printf("matriz de entrada:\n");
    // for (int i = 0; i < dim; i++)
    // {
    //     for (int j = 0; j < dim; j++)
    //         printf("%.1f ", mat[i * dim + j]);
    //     puts("");
    // }
    // printf("vetor de entrada:\n");
    // for (int j = 0; j < dim; j++)
    //     printf("%.1f ", vet[j]);
    // puts("");
    printf("vetor de saida:\n");
    for (int j = 0; j < dim; j++)
        printf("%.1f ", out[j]);
    puts("");

    //liberacao da memoria
    free(mat);
    free(vet);
    free(out);

    return 0;
}

int main(int argc, char *argv[])
{
    int linha;
    int coluna;
    int i;
    int somaprod;
    int mat1[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int mat2[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    int mat3[3][3];
    int M1L = 3, M1C = 3, M2L = 3, M2C = 3;
    for (linha = 0; linha < M1L; linha++)
        for (coluna = 0; coluna < M2C; coluna++)
        {
            somaprod = 0;
            for (i = 0; i < M1L; i++)
                somaprod += mat1[linha][i] * mat2[i][coluna];
            mat3[linha][coluna] = somaprod;
        }
    //
    //imprime mat3
    //
    for (linha = 0; linha < M1L; linha++)
    {
        for (coluna = 0; coluna < M2C; coluna++)
            printf("%d ", mat3[linha][coluna]);
        printf("\n");
    }
    system("PAUSE");
    return 0;
}