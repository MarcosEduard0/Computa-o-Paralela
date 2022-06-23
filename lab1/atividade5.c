/* Disciplina: Computacao Concorrente 2021.1*/
/* Aluno: Marcos Eduardo */
/* Módulo 1 - Laboratório: 1 */
/* Data: 13/07/2021 */
/* Codigo: "Atividade 5" usando threads em C para calcular potência de cada elemento de um vetor */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NTHREADS  2 //Total de threads a serem criadas
#define TAM 10000 //Tamanho do vetor

double out[TAM]; //Vetor de inteiros para realização da potencia

//Funcao que as threads irão executar
void *Potencia (void* arg) {
    int idThread = *(int*) arg, i;

    for(i=idThread; i<TAM; i+=2){ out[i]*=out[i];}
    printf("|---> A thread %d terminou <---|\n", idThread);
    pthread_exit(NULL);
}

//Funcao principal
int main() {
    pthread_t tid_sistema[NTHREADS]; //Identificadores das threads no sistema
    int thread, i, tid_local[NTHREADS];
    double in[TAM]; 
    srand (time(NULL));

    //Gerando valores aleatórios entre -2500 e 2500, para termos numeros reais, e amazenando no vetor auxiliar e original global
    for (i=0; i<TAM; i++){ in[i] = out[i] = ((rand() % 10000)/2.0) + (-2500.0);}

    for(thread=0; thread<NTHREADS; thread++) {
        printf("|--> Criando a thread %d <--|\n", thread);
        tid_local[thread] = thread;
        if (pthread_create(&tid_sistema[thread], NULL, Potencia, (void*) &tid_local[thread])) {
        printf("\x1b[31m|-->ERRO: pthread_create() <--|\n"); exit(-1);
        }
    }

    //Espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("\x1b[31m|-->ERRO: pthread_join()<--| \n"); exit(-1); 
        } 
    }
    // out[rand() % TAM]= -50000; //verificação de erro
    for (i = 0; i<TAM; i++){
        if(fabs(in[i]) != sqrt(out[i])){
            printf("\x1b[31m|--> ERRO: Elemento [%d], (%.2lf)² ≠ %.2lf <---|\n",i, in[i], out[i]);exit(-1);
        }
    }

    printf("\x1b[32m|--> Sucesso, thread principal finalizada <--|\n");
    pthread_exit(NULL);
}
