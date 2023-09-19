#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void printMatriz(float **matriz, int n) {
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%.1f ", matriz[i][j]);
        }
        printf("\n");
    }
}

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

void glider(float **matriz, int x, int y) { // x == 0 no glider e y == 0 no glider
    /*// matriz[x][y] = 0.0;
    matriz[x+1][y] = 1.0;
    // matriz[x+2][y] = 0.0;
    // matriz[x][y+1] = 0.0;
    // matriz[x+1][y+1] = 0.0;
    matriz[x+2][y+1] = 1.0;
    matriz[x][y+2] = 1.0;
    matriz[x+1][y+2] = 1.0;
    matriz[x+2][y+2] = 1.0;*/

    //Glinger girado
    // matriz[x  ][y  ] = 0.0;
    matriz[x  ][y+1] = 1.0;
    // matriz[x  ][y+2] = 0.0;
    // matriz[x+1][y] = 0.0;
    // matriz[x+1][y+1] = 0.0;
    matriz[x+1][y+2] = 1.0;
    matriz[x+2][y] = 1.0;
    matriz[x+2][y+1] = 1.0;
    matriz[x+2][y+2] = 1.0;
}

void rPentomino(float **matriz, int x, int y) { // x == 0 no rPentomino e y == 0 no rPentomino
    // matriz[x  ][y  ] = 0.0;
    matriz[x+1][y  ] = 1.0;
    // matriz[x+2][y  ] = 0.0;
    matriz[x  ][y+1] = 1.0;
    matriz[x+1][y+1] = 1.0;
    matriz[x+2][y+1] = 1.0;
    matriz[x  ][y+2] = 1.0;
    // matriz[x+1][y+2] = 0.0;
    // matriz[x+2][y+2] = 0.0;
}

void geracao(float **matriz, float **copia, int n) {
    for(int i = 0 ; i <= n-1; i++){   
        for(int j = 0; j <= n-1; j++){
            int vivos = contaCelula(matriz, i, j, n);
            // printf("%f - %d, %d - Vivos %d\n", matriz[i][j], i, j, vivos);


            //qualquer celula morta com 3 (tres) vizinhos torna-se viva;
            if ((matriz[i][j] == 0.0) && (vivos == 3)) {
                copia[i][j] = mediaCelula(matriz, i, j, n);
            
            //qualquer celula viva com 2 (dois) ou 3 (tres) vizinhos deve sobreviver;
            //seu valor permanece o mesmo ou reseta pra 1.0?
            } else if ((matriz[i][j] > 0.0) && ((vivos == 2) || (vivos == 3))) {
                copia[i][j] = 1.0;
            } 

            //qualquer outro caso, celulas vivas devem morrer e celulas ja mortas devem continuar mortas.
            else {
                copia[i][j] = 0.0;
            }
        }
    }
}

void copiaMatrizes(float **matriz, float ** copia, int n) {
    for(int i = 0 ; i <= n-1; i++){   
        for(int j = 0; j <= n-1; j++){
            copia[i][j] = matriz[i][j];
        }
    }
}

int contaCelulasVivas(float **matriz, int n) {
    int contador = 0;
    for(int i = 0 ; i <= n-1; i++){   
        for(int j = 0; j <= n-1; j++){
            if(matriz[i][j] > 0.0) contador++;
        }
    }
    return contador;
}

int interacoes(float **matriz, float nGeracoes, int n) {
    float** copia = (float**)malloc(sizeof(float*)*n);
    for (int i = 0; i < n; i++){
        copia[i] = (float*)malloc(sizeof(float)*n);
    }

    for(int i = 0 ; i < nGeracoes; i++){   
        copiaMatrizes(matriz, copia, n);
        
        geracao(matriz, copia, n);

        copiaMatrizes(copia, matriz, n);

        // printMatriz(matriz, n);
        // printf("____________________\n");
        printf("Geracao n %d - Celulas vivas: %d\n", i+1, contaCelulasVivas(matriz, n));
    }

    int contador = contaCelulasVivas(matriz, n);
    return contador;
}


void main() {
    clock_t tempoInicial;
    double tempoTotalSegundos;
    int n = 2048;

    float** matriz=(float**)malloc(sizeof(float*)*n);
    for (int i = 0; i < n; i++){
        matriz[i] = (float*)malloc(sizeof(float)*n);
    }

    zeraMatriz(matriz, n);

    glider(matriz, 1, 1);
    rPentomino(matriz, 10, 30);

    tempoInicial = clock();
    int resultado = interacoes(matriz, 100, n);
    tempoTotalSegundos = ((double)clock() - tempoInicial)/ CLOCKS_PER_SEC;

    //printMatriz(matriz, n);

    printf("____________________\n");
    printf("resultado: %d\n", resultado);
    printf("tempo de execucao: %f segundos\n", tempoTotalSegundos);
}