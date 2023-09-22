#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4
#define N 2048
#define GERACOES 2000

float **matriz;
float **copia;
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
    // printf("%f - %d, %d - Vivos %d\n", matriz[i][j], i, j, vivos);


    //qualquer celula morta com 3 (tres) vizinhos torna-se viva;
    if ((matriz[i][j] == 0.0) && (vivos == 3)) {
        return mediaCelula(matriz, i, j, n);
    //qualquer celula viva com 2 (dois) ou 3 (tres) vizinhos deve sobreviver;
    //seu valor permanece o mesmo ou reseta pra 1.0?
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
    int elementosPorThread = (N * N) / NTHREADS;
    int inicio = threadID * elementosPorThread;
    int fim = (threadID == NTHREADS - 1) ? (N * N) : (threadID + 1) * elementosPorThread;
    int vivosLocal = 0;

    for (int i = inicio; i < fim; i++) {
        int linha = i / N;
        int coluna = i % N;
        copia[linha][coluna] = verificarNovoEstadoCelula(matriz, N, linha, coluna);
                
        if (copia[linha][coluna] > 0.0) vivosLocal++; 
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

    matriz=(float**)malloc(sizeof(float*)*N);
    copia=(float**)malloc(sizeof(float*)*N);
    
    for (int i = 0; i < N; i++){
        matriz[i] = (float*)malloc(sizeof(float)*N);
        copia[i] = (float*)malloc(sizeof(float)*N);
    }

    zeraMatriz(matriz, N);
    glider(matriz, 1, 1);
    rPentomino(matriz, 10, 30);

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

        copiaMatrizes(copia, matriz, N);

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