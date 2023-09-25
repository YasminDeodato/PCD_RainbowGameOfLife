/*
* Programacao Concorrente e Distribuida
* Rainbow Game of Life com Openmp
* Helio Didzec Junior
* Yasmin Beatriz Deodato
*/
#include <math.h>
#include <time.h>

#define N 2048
#define GERACOES 2000

float **matriz;
float **copia;

void printMatriz(float **matriz) {
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.1f ", matriz[i][j]);
        }
        printf("\n");
    }
}

void zeraMatriz(float **matriz) {
    for( int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            matriz[i][j] = 0.0;
        }
    }
}

int contaCelula(float **matriz, int x, int y){
    int contador = 0;

    if (((x > 0) && (x < N-1)) && ((y > 0) && (y < N-1))) {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                if (matriz[i][j] > 0.0) contador += 1;
            }
        }
    } else {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                if ((i < 0) && (j < 0)) {
                    if (matriz[N-1][N-1] > 0.0) contador += 1;
                } else if((i < 0) && (j > N-1)){
                    if (matriz[N-1][0] > 0.0) contador += 1;
                } else if ((i > N-1) && (j > N-1)) {
                    if (matriz[0][0] > 0.0) contador += 1;
                } else if((i > N-1) && (j < 0)){
                    if (matriz[0][N-1] > 0.0) contador += 1;
                } else if (i < 0){
                    if (matriz[N-1][j] > 0.0) contador += 1;
                } else if(j < 0){
                    if (matriz[i][N-1] > 0.0) contador += 1;
                } else if(i > N-1){
                    if (matriz[0][j] > 0.0) contador += 1;
                } else if(j > N-1){
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

float mediaCelula(float **matriz, int x, int y){
    float contador = 0.0;

    if (((x > 0) && (x < N-1)) && ((y > 0) && (y < N-1))) {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                contador += matriz[i][j];
            }
        }
    } else {
        for(int i = x-1 ; i <= x+1; i++){   
            for(int j = y-1; j <= y+1; j++){
                if ((i < 0) && (j < 0)) {
                    contador += matriz[N-1][N-1];
                } else if((i < 0) && (j > N-1)){
                    contador += matriz[N-1][0];
                } else if ((i > N-1) && (j > N-1)) {
                    contador += matriz[0][0];
                } else if((i > N-1) && (j < 0)){
                    contador += matriz[0][N-1];
                } else if (i < 0){
                    contador += matriz[N-1][j];
                } else if(j < 0){
                    contador += matriz[i][N-1];
                } else if(i > N-1){
                    contador += matriz[0][j];
                } else if(j > N-1){
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

int geracao(float **matriz, float **copia) {
    int vivosGeracao = 0;
    for(int i = 0 ; i <= N-1; i++){   
        for(int j = 0; j <= N-1; j++){
            int vivos = contaCelula(matriz, i, j);

            //qualquer celula morta com 3 (tres) vizinhos torna-se viva;
            if ((matriz[i][j] == 0.0) && (vivos == 3)) {
                copia[i][j] = mediaCelula(matriz, i, j);
            
            //qualquer celula viva com 2 (dois) ou 3 (tres) vizinhos deve sobreviver;
            } else if ((matriz[i][j] > 0.0) && ((vivos == 2) || (vivos == 3))) {
                copia[i][j] = 1.0;
            } 

            //qualquer outro caso, celulas vivas devem morrer e celulas ja mortas devem continuar mortas.
            else {
                copia[i][j] = 0.0;
            }

            if (copia[i][j] > 0.0) {
                vivosGeracao++;
            }
        }
    }
    return vivosGeracao;
}

void copiaMatrizes(float **matriz, float ** copia) {
    for(int i = 0 ; i <= N-1; i++){   
        for(int j = 0; j <= N-1; j++){
            copia[i][j] = matriz[i][j];
        }
    }
}


int interacoes(float **matriz, int nGeracao) {   
    int vivosGeracao = 0;
    if (nGeracao <= GERACOES) {
        vivosGeracao = geracao(matriz, copia);

        copiaMatrizes(copia, matriz);

        printf("Geracao n %d - Celulas vivas: %d\n", nGeracao, vivosGeracao);
    }

    return vivosGeracao;
}

void executaInteracao(int geracao) {
    interacoes(matriz, geracao);
}

// void main(int argc, char *argv[]) {
void rainbowGameOfLife() {
    matriz=(float**)malloc(sizeof(float*)*N);
    copia = (float**)malloc(sizeof(float*)*N);
    for (int i = 0; i < N; i++){
        matriz[i] = (float*)malloc(sizeof(float)*N);
        copia[i] = (float*)malloc(sizeof(float)*N);
    }

    // condicoes iniciais do tabuleiro
    zeraMatriz(matriz);
    glider(matriz, 1, 1);
    rPentomino(matriz, 10, 30);
}