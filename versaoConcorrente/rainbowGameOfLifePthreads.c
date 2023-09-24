/*
* Programacao Concorrente e Distribuida
* Rainbow Game of Life com Pthreads
* Helio Didzec Junior
* Yasmin Beatriz Deodato
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define NTHREADS 8
#define N 2048
#define GERACOES 500

float **matrizP;
float **copiaP;
int resultado[NTHREADS] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void zeraMatriz(float **matriz, int n) {
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matriz[i][j] = 0.0;
        }
    }
}

int contaCelula(float **matriz, int x, int y, int n){
    int contador = 0;

    if (((x > 0) && (x < n-1)) && ((y > 0) && (y < n-1))) {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                if (matriz[i][j] > 0.0) contador += 1;
            }
        }
    } else {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                if ((i < 0) && (j < 0)) {
                    if (matriz[n-1][n-1] > 0.0) contador += 1;
                } else if((i < 0) && (j > n-1)){
                    if (matriz[n-1][0] > 0.0) contador += 1;
                } else if ((i > n-1) && (j > n-1)) {
                    if (matriz[0][0] > 0.0) contador += 1;
                } else if((i > n-1) && (j < 0)){
                    if (matriz[0][n-1] > 0.0) contador += 1;
                } else if (i < 0){
                    if (matriz[n-1][j] > 0.0) contador += 1;
                } else if(j < 0){
                    if (matriz[i][n-1] > 0.0) contador += 1;
                } else if(i > n-1){
                    if (matriz[0][j] > 0.0) contador += 1;
                } else if(j > n-1){
                    if (matriz[i][0] > 0.0) contador += 1;
                } else {
                    if (matriz[i][j] > 0.0) contador += 1;
                }
            }
        }
    }
    if (matriz[x][y] > 0.0) contador -= 1;


    return contador;
}

float mediaCelula(float **matriz, int x, int y, int n){
    float contador = 0.0;

    if (((x > 0) && (x < n-1)) && ((y > 0) && (y < n-1))) {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                contador += matriz[i][j];
            }
        }
    } else {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                if ((i < 0) && (j < 0)) {
                    contador += matriz[n-1][n-1];
                } else if((i < 0) && (j > n-1)){
                    contador += matriz[n-1][0];
                } else if ((i > n-1) && (j > n-1)) {
                    contador += matriz[0][0];
                } else if((i > n-1) && (j < 0)){
                    contador += matriz[0][n-1];
                } else if (i < 0){
                    contador += matriz[n-1][j];
                } else if(j < 0){
                    contador += matriz[i][n-1];
                } else if(i > n-1){
                    contador += matriz[0][j];
                } else if(j > n-1){
                    contador += matriz[i][0];
                } else {
                    contador += matriz[i][j];
                }
            }
        }
    }
    contador -= matriz[x][y];


    return (contador/8.0);
}

void glider(float **matriz, int x, int y) {
    matriz[x  ][y+1] = 1.0;
    matriz[x+1][y+2] = 1.0;
    matriz[x+2][y] = 1.0;
    matriz[x+2][y+1] = 1.0;
    matriz[x+2][y+2] = 1.0;
}

void rPentomino(float **matriz, int x, int y) { 
    matriz[x+1][y  ] = 1.0;
    matriz[x  ][y+1] = 1.0;
    matriz[x+1][y+1] = 1.0;
    matriz[x+2][y+1] = 1.0;
    matriz[x  ][y+2] = 1.0;
}

float verificarNovoEstadoCelula(float **matriz, int n, int i, int j) {
    int vivos = contaCelula(matriz, i, j, n);

    //qualquer celula morta com 3 (tres) vizinhos torna-se viva;
    if ((matriz[i][j] == 0.0) && (vivos == 3)) {
        return mediaCelula(matriz, i, j, n);
    //qualquer celula viva com 2 (dois) ou 3 (tres) vizinhos deve sobreviver;
    } else if ((matriz[i][j] > 0.0) && ((vivos == 2) || (vivos == 3))) {
        return 1.0;
    } 
    //qualquer outro caso, celulas vivas devem morrer e celulas ja mortas devem continuar mortas.
    else {
       return 0.0;
    }
}

void copiaMatrizes(float **matriz, float ** copia, int n) {
    for(int i = 0 ; i <= n-1; i++){   
        for(int j = 0; j <= n-1; j++){
            copia[i][j] = matriz[i][j];
        }
    }
}

void *percorrerMatriz(void *arg) {
    long threadID = (long)arg;
    int passo = floor(N/NTHREADS);
    int inicio = passo * threadID;
    int fim = passo * (threadID + 1);
    int vivosLocal = 0;

    if (fim > N){
        fim = N;
    }

    for (int i = 0; i < N; i++) {
        for (int j = inicio; j < fim; j++){
            copiaP[i][j] = verificarNovoEstadoCelula(matrizP, N, i, j);
            if (copiaP[i][j] > 0.0) vivosLocal++; 
        }
    }

    pthread_mutex_lock(&mutex);
    resultado[threadID] = vivosLocal;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];
    clock_t tempoInicial;
    double tempoTotalSegundos;
    int vivosPorGeracao = 0;

    matrizP=(float**)malloc(sizeof(float*)*N);
    copiaP=(float**)malloc(sizeof(float*)*N);
    
    for (int i = 0; i < N; i++){
        matrizP[i] = (float*)malloc(sizeof(float)*N);
        copiaP[i] = (float*)malloc(sizeof(float)*N);
    }

    zeraMatriz(matrizP, N);
    glider(matrizP, 1, 1);
    rPentomino(matrizP, 10, 30);

    tempoInicial = clock();
    for (int g=0; g < GERACOES; g++) {

        // criar e executa threads
        for (long t = 0; t < NTHREADS; t++) {
            resultado[t] = 0;

            if (pthread_create(&threads[t], NULL, percorrerMatriz, (void *)t) != 0) {
                fprintf(stderr, "Erro ao criar a thread %ld\n", t);
                return 1;
            }
        }

        // aguarda ate que as threads terminem
        for (long t = 0; t < NTHREADS; t++) {
            if (pthread_join(threads[t], NULL) != 0) {
                fprintf(stderr, "Erro ao esperar pela thread %ld\n", t);
                return 1;
            }
        }

        copiaMatrizes(copiaP, matrizP, N);

        // soma as celulas vivas contadas por cada thread
        vivosPorGeracao = 0;
        for (int i = 0; i < NTHREADS; i++) {
            vivosPorGeracao += resultado[i];
        }

        printf("Geracao n %d - Celulas vivas: %d\n", g+1, vivosPorGeracao);
    }
    

    tempoTotalSegundos = ((double)clock() - tempoInicial)/ CLOCKS_PER_SEC;
    printf("____________________\n");
    printf("resultado: %d\n", vivosPorGeracao);
    printf("tempo de execucao: %f segundos\n", tempoTotalSegundos);


    pthread_exit(NULL);
}