/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno.: Marcos Eduardo */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

// Variaveis globais
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, x_cond3;

//Declaração das funções
void *Frase1(void *);
void *Frase2(void *);
void *Frase3(void *);
void *Frase4(void *);

// Funcao principal
int main(int argc, char *argv[])
{
  pthread_t threads[NTHREADS];

  // Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init(&x_cond, NULL);
  pthread_cond_init(&x_cond3, NULL);

  // Cria as threads
  pthread_create(&threads[0], NULL, Frase1, NULL);
  pthread_create(&threads[1], NULL, Frase2, NULL);
  pthread_create(&threads[2], NULL, Frase3, NULL);
  pthread_create(&threads[3], NULL, Frase4, NULL);

  // Espera todas as threads terminarem
  for (int i = 0; i < NTHREADS; i++)
    pthread_join(threads[i], NULL);

  // Desaloca variaveis e termina
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  pthread_exit(NULL);
  return 0;
}

void *Frase1(void *arg)
{

  pthread_mutex_lock(&x_mutex);
  while (x < 1)
    pthread_cond_wait(&x_cond, &x_mutex);

  printf("Fique a vontade.\n");
  x++;
  pthread_cond_signal(&x_cond3);
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

void *Frase2(void *arg)
{
  pthread_mutex_lock(&x_mutex);
  printf("Seja bem-vindo!\n");
  x++;
  pthread_cond_broadcast(&x_cond);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

void *Frase3(void *arg)
{
  pthread_mutex_lock(&x_mutex);
  while (x < 3)
    pthread_cond_wait(&x_cond3, &x_mutex);

  printf("Volte sempre!.\n");
  pthread_exit(NULL);
}

void *Frase4(void *arg)
{

  pthread_mutex_lock(&x_mutex);
  while (x < 1)
    pthread_cond_wait(&x_cond, &x_mutex);

  printf("Sente-se por favor.\n");
  x++;
  pthread_cond_signal(&x_cond3);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}