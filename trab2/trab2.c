#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "timer.h"

#define TAMBUFFER 10 //Tamanho do buffer principal

// Variaveis globais
int NTHREADS, N_blocos;
int *mainBuffer;                                        //Buffer principal
FILE *arqRead, *arqWrite;                               //Arquivo de leitura e de escrita
sem_t mutexCons, bufferCheio, bufferVazio, finalizacao; //semaforos para sincronizar a ordem de execucao das threads
int fim = 0, out = 0;
int totalEscr = 0, totalLeit = 0; //contador leitura e escrita
int escr = 0;                     //contador de threads escrevendo
pthread_mutex_t mutex;            //sincronização para leitor/escritor
pthread_cond_t cond_escr;         //sincronização para leitor/escritor

//funcao das threads
void insere()
{
    int in = 0;

    while (1)
    {
        sem_wait(&bufferVazio);
        // escaneia os numeros da linha e coloca no buffer principal
        for (int j = 0; j < N_blocos; j++)
            if (fscanf(arqRead, "%d", &mainBuffer[in * N_blocos + j]) == EOF)
                return;
        in = (in + 1) % TAMBUFFER;
        totalLeit++;
        sem_post(&bufferCheio);
    }
}
//retira um bloco do buff princial para o buff local da thread
int retira(int v[], int id)
{

    printf("R[%d] quer retirar.\n", id);

    sem_wait(&bufferCheio);
    if (fim)
    {
        sem_post(&bufferCheio);
        return 0;
    }
    sem_wait(&mutexCons); // Perimite apenas um consumidor por vez

    printf("R[%d] esta retirando\n", id);

    for (int i = 0; i < N_blocos; i++)
        v[i] = mainBuffer[out * N_blocos + i];
    out = (out + 1) % TAMBUFFER;
    sem_post(&bufferVazio);
    sem_post(&mutexCons);

    return 1;
}
//Ordenacao do vetor
void insertionSort(int v[], int n)
{
    for (int j = 1; j < n; ++j)
    {
        int x = v[j];
        int i;
        for (i = j - 1; i >= 0 && v[i] > x; --i)
            v[i + 1] = v[i];
        v[i + 1] = x;
    }
}
//entrada de escrita
void InicEscr(int id)
{
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    while (escr > 0)
    {
        printf("E[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}
//saida de escrita
void FimEscr(int id)
{
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);
    escr--;
    totalEscr++;
    if (totalLeit == totalEscr && feof(arqRead))
        sem_post(&finalizacao);

    pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}
// "produz" os blocos
void *produtor(void *arg)
{
    insere();

    sem_wait(&finalizacao);

    fim = 1;
    sem_post(&bufferCheio); //sinaliza uma thread consumidora para sair da espera de novos blocos

    pthread_exit(NULL);
}
//Consome blocos do buff principal
void *consumidor(void *arg)
{
    int id = *(int *)arg;
    int bufferLocal[N_blocos];
    while (retira(bufferLocal, id))
    {
        //ordenando o bloco
        insertionSort(bufferLocal, N_blocos);

        InicEscr(id);
        printf("Escritora %d esta escrevendo\n", id);
        for (int i = 0; i < N_blocos; i++)
        {
            fprintf(arqWrite, "%d", bufferLocal[i]);
            if (i < (N_blocos - 1))
                fprintf(arqWrite, " ");
            else
                fprintf(arqWrite, "\n");
        }
        FimEscr(id);
    }
    pthread_exit(NULL);
}

void solucao_sequencial(FILE *seqRead, FILE *seqWrite)
{
    int buf[N_blocos]; // inicia buffer local
    while (1)
    {
        for (int j = 0; j < N_blocos; j++)             // lê coluna até o N
            if (fscanf(seqRead, "%d", &buf[j]) == EOF) // checa se arquivo está no fim
                return;                                // retorna se for o caso
        for (int j = 1; j < N_blocos; ++j)
        { // inicia o sort
            int x = buf[j];
            int i;
            for (i = j - 1; i >= 0 && buf[i] > x; --i)
                buf[i + 1] = buf[i];
            buf[i + 1] = x; // termina o sort
        }
        for (int i = 0; i < N_blocos - 1; i++) // printa a matriz no arquivo
            fprintf(seqWrite, "%d ", buf[i]);
        fprintf(seqWrite, "%d\n", buf[N_blocos - 1]); // pula a linha
    }
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t *threads;
    int *id;
    //inicia os semaforos
    sem_init(&bufferCheio, 0, 0);
    sem_init(&bufferVazio, 0, TAMBUFFER);
    sem_init(&mutexCons, 0, 1);
    sem_init(&finalizacao, 0, 0);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_escr, NULL);

    /* Medicao de tempo */
    double t1, t2;

    if (argc < 5)
    {
        fprintf(stderr, "Digite : %s <nº de thread> <Tam. Blocos> <nome arq. entrada>.txt <nome arq. saida>.txt\n", argv[0]);
        return 1;
    }
    NTHREADS = atoi(argv[1]); // NTHREADS = C + P
    N_blocos = atoi(argv[2]); // N_blocos = N
    arqRead = fopen(argv[3], "r");
    arqWrite = fopen(argv[4], "w");

    FILE *seqArqRead = fopen(argv[3], "r");
    FILE *seqArqWrite = fopen("solucao_sequencial.txt", "w");

    //consome o primeiro número
    int consume;
    fscanf(seqArqRead, "%d", &consume);
    fscanf(arqRead, "%d", &consume);

    //Testando a abertura do arquivo
    if (arqRead == NULL)
    {
        printf("Erro ao tentar abrir o arquivo de leitura %s\n", argv[3]);
        return 1;
    }
    if (arqWrite == NULL)
    {
        printf("Erro ao tentar abrir o arquivo de escrita %s\n", argv[4]);
        return 1;
    }
    //alocacao de memoria
    if ((mainBuffer = (int *)malloc(sizeof(int) * N_blocos * TAMBUFFER)) == NULL)
    {
        printf("ERRO-- malloc\n");
        return 1;
    }

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

    GET_TIME(t1);
    solucao_sequencial(seqArqRead, seqArqWrite);
    GET_TIME(t2);
    fclose(seqArqRead);
    fclose(seqArqWrite);

    // comente essa linha para obter o resultado da função sequencial
    // if(remove("solucao_sequencial.txt")!=0) printf("Arquivo temporario não deletado!\n");
    printf("Tempo sequencial =%f\n", t2 - t1);

    /* Cria as threads */
    GET_TIME(t1);
    for (i = 0; i < NTHREADS; i++)
    {
        id[i] = i;
        if (i == 0) //cria um produtor
            pthread_create(&threads[i], NULL, produtor, (void *)&id[i]);
        else //cria (nthreads-1) consumidores
            pthread_create(&threads[i], NULL, consumidor, (void *)&id[i]);
    }

    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    GET_TIME(t2);
    printf("Tempo concorrente =%f\n", t2 - t1);

    fclose(arqRead);
    fclose(arqWrite);
    fclose(seqArqRead);
    free(mainBuffer);
    free(threads);
    free(id);
    pthread_exit(NULL);
}
