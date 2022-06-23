
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

#define RED "\x1b[31m"
#define WHT "\e[0;37m"
#define YEL "\x1b[33m"

double LetraA(double);
double LetraB(double);
double LetraC(double);
double LetraD(double);
void Menu();
void *somaRiemann();
double somaRiemannSequencial();

int NTHREADS;                     //Declaracao do nº de threads
double a, b;                      //Variaveis do intervalo definido pelo user
int nRetangulos;                  //Variavel para armazenmento da quant de divisoes a serem realizada
double delta, resultIntegral = 0; //Delta X dos retangulos e o rultado final.
pthread_mutex_t lock;             //Declaração do mutex
double (*function)(double);       //Declação ponteiro para função

int main(int argc, char *argv[])
{

    double inicio, fim; //Variaveis para marcação do tempo e ponteiro para armazenamento de adress de function
    double resultSeq = 0;
    long int thread;
    pthread_t *tid;

    //leitura e avalicao de paramentros
    if (argc < 4)
    {
        fprintf(stderr, "Digite : %s <Intervalo a> <Intervalo b> <nº de retangulos> <nº de thread>\n", argv[0]);
        return 1;
    }
    a = atof(argv[1]);
    b = atof(argv[2]);
    nRetangulos = atoi(argv[3]);
    NTHREADS = atoi(argv[4]);
    delta = (b - a) / nRetangulos;

    //Alocamento dos identificadores das threads no sistema
    tid = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);
    if (tid == NULL)
    {
        printf(RED "|-->ERRO: malloc\n");
        return 2;
    }

    Menu(); //acessando o menu para o usuario e recebendo o adress da função escolhida

    //Inicialização do mutex e criação das thread e execução da soma
    pthread_mutex_init(&lock, NULL);

    printf(YEL "\nCalculo sequencial:\n" WHT);
    GET_TIME(inicio);
    resultSeq = somaRiemannSequencial();
    GET_TIME(fim)
    printf("\nResultado: %lf\n", resultSeq);
    printf("Tempo de calculo: %lf\n\n", fim - inicio);

    //Criando as threads e delegando as tarefas
    GET_TIME(inicio);
    for (thread = 0; thread < NTHREADS; thread++)
    {

        if (pthread_create(&tid[thread], NULL, somaRiemann, (void *)thread))
        {
            fprintf(stderr, RED "|-->ERRO: pthread_create() <--|\n");
            return 3;
        }
    }

    //Aguardabdo as threads terminarem
    for (int i = 0; i < NTHREADS; i++)
        if (pthread_join(tid[i], NULL))
        {
            printf(RED "|-->ERRO: pthread_join()<--| \n");
            exit(-1);
        }
    GET_TIME(fim);
    printf(YEL "\nCalculo concorrente:\n" WHT);
    printf("\nResultado: %lf\n", resultIntegral);
    printf("Tempo de calculo: %lf\n\n", fim - inicio);

    pthread_mutex_destroy(&lock);

    free(tid);
    pthread_exit(NULL);
    return 0;
}

void *somaRiemann(void *arg)
{
    long int idThread = (long int)arg;
    double somaLocal = 0;

    for (int i = (idThread + 1); i <= nRetangulos; i += NTHREADS)
    {
        somaLocal += (function(i * delta) * delta);
    }

    pthread_mutex_lock(&lock);
    resultIntegral += somaLocal;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

double somaRiemannSequencial()
{
    double soma = 0;

    for (int i = 0; i <= nRetangulos; i++)
    {
        soma += (function(i * delta) * delta);
    }

    return soma;
}

void Menu()
{

    char letra;
    printf("Escolha a função:\n\na- x*2\nb- x^2\nc- (x^3)-6*x\nd- seno(x^2)\n\n");

    printf("Digite a letra da função que deseja executar: \n");
    scanf("%c", &letra);

    switch (letra)
    {
    case 'a':
    case 'A':
        function = LetraA;
        break;
    case 'b':
    case 'B':
        function = LetraB;
        break;
    case 'c':
    case 'C':
        function = LetraC;
        break;
    case 'd':
    case 'D':
        function = LetraD;
        break;
    default:
        printf(RED "Opção Invalida.\n" WHT);
        exit(-1);
        break;
    }
}

double LetraA(double x)
{
    return x * 2;
}
double LetraB(double x)
{
    return pow(x, 2);
}
double LetraC(double x)
{
    return pow(x, 3) - (6 * x);
}

double LetraD(double x)
{
    return sin(pow(x, 2));
}
