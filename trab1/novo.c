#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

#define RED "\x1b[31m"
#define WHT "\e[0;37m"

void *somaRiemannLetraA();
void *somaRiemannLetraB();
void *somaRiemannLetraC();
void *somaRiemannLetraD();

int NTHREADS;					  //Declaracao do nº de threads
double a, b;					  //Variaveis do intervalo definido pelo user
int nRetangulos;				  //Variavel para armazenmento da quant de divisoes a serem realizada
double delta, resultIntegral = 0; //Delta X dos retangulos e o resultado final.
pthread_mutex_t lock;			  //Declaração do mutex

void *somaRiemannLetraA(void *arg)
{
	long int id = (long int)arg;
	double somaLocal = 0;
	for (long int i = id + 1; i <= nRetangulos; i += NTHREADS)
	{
		somaLocal += 2 * i * delta;
	}
	pthread_mutex_lock(&lock);
	resultIntegral += somaLocal;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

void *somaRiemannLetraB(void *arg)
{
	long int id = (long int)arg;
	double somaLocal = 0;
	for (long int i = id + 1; i <= nRetangulos; i += NTHREADS)
	{
		somaLocal += pow(i * delta, 2);
	}
	pthread_mutex_lock(&lock);
	resultIntegral += somaLocal;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}
void *somaRiemannLetraC(void *arg)
{
	long int id = (long int)arg;
	double somaLocal = 0;
	for (long int i = id + 1; i <= nRetangulos; i += NTHREADS)
	{
		somaLocal += pow(i * delta, 3) - 6 * i * delta;
	}
	pthread_mutex_lock(&lock);
	resultIntegral += somaLocal;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}
void *somaRiemannLetraD(void *arg)
{
	long int id = (long int)arg;
	double somaLocal = 0;
	for (long int i = id + 1; i <= nRetangulos; i += NTHREADS)
	{
		somaLocal += sin(pow(i * delta, 2));
	}
	pthread_mutex_lock(&lock);
	resultIntegral += somaLocal;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

static inline void preThread()
{
	pthread_mutex_init(&lock, NULL);
	double inicio, fim;
	pthread_t *tid;
	tid = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);

	char letra;
	printf("Escolha a função:\n\na- x*2\nb- x^2\nc- (x^3)-6*x\nd- seno(x^2)\n\n");

	printf("Digite a letra da função que deseja executar: \n");
	scanf("%c", &letra);

	if (tid == NULL)
	{
		printf(RED "|-->ERRO: malloc\n");
		exit(2);
	}
	GET_TIME(inicio);
	switch (letra)
	{
	case 'a':
	case 'A':
		for (long int i = 0; i < NTHREADS; i++)
		{
			if (pthread_create(tid + i, NULL, somaRiemannLetraA, (void *)i))
			{
				fprintf(stderr, RED "|-->ERRO: pthread_create() <--|\n");
				exit(3);
			}
		}
		break;
	case 'b':
	case 'B':
		for (long int i = 0; i < NTHREADS; i++)
		{
			if (pthread_create(tid + i, NULL, somaRiemannLetraB, (void *)i))
			{
				fprintf(stderr, RED "|-->ERRO: pthread_create() <--|\n");
				exit(3);
			}
		}
		break;
	case 'c':
	case 'C':
		for (long int i = 0; i < NTHREADS; i++)
		{
			if (pthread_create(tid + i, NULL, somaRiemannLetraC, (void *)i))
			{
				fprintf(stderr, RED "|-->ERRO: pthread_create() <--|\n");
				exit(3);
			}
		}
		break;
	case 'd':
	case 'D':
		for (long int i = 0; i < NTHREADS; i++)
		{
			if (pthread_create(tid + i, NULL, somaRiemannLetraD, (void *)i))
			{
				fprintf(stderr, RED "|-->ERRO: pthread_create() <--|\n");
				exit(3);
			}
		}
		break;
	default:
		printf(RED "Opção Invalida.\n" WHT);
		exit(-1);
		break;
	}

	for (int i = 0; i < NTHREADS; i++)
		if (pthread_join(*(tid + i), NULL))
		{
			printf(RED "|-->ERRO: pthread_join()<--| \n");
			exit(-1);
		}
	GET_TIME(fim);
	printf("\nResultado: %lf\n", resultIntegral * delta);
	printf("Tempo de calculo: %lf\n\n", fim - inicio);

	pthread_mutex_destroy(&lock);
	free(tid);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	//leitura e avalicao de paramentros
	if (argc < 4)
	{
		fprintf(stderr, "Digite : %s <Intervalo a> <Intervalo b> <nº de retangulos> <nº de thread>\n", argv[0]);
		exit(1);
	}
	a = atof(argv[1]);
	b = atof(argv[2]);
	nRetangulos = atoi(argv[3]);
	NTHREADS = atoi(argv[4]);
	delta = (b - a) / nRetangulos;
	preThread();
	//Inicialização do mutex e criação das thread e execução da soma
	return 0;
}
