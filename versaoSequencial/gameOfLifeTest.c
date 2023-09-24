/*
* Programacao Concorrente e Distribuida
* Game of Life Serial
* Helio Didzec Junior
* Yasmin Beatriz Deodato
*/
#include <stdlib.h>
#include <stdio.h>

void printMatriz(int **matriz, int n) {
    for( int i = 0; i < n; i++){
        for(int j =0; j < n; j++){
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void zeraMatriz(int **matriz, int n) {
    for( int i = 0; i < n; i++){
        for(int j =0; j < n; j++){
            matriz[i][j] = 0;
        }
    }
}

int contaCelula(int **matriz, int x, int y, int n){
    int contador = 0;

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

    return contador;
}

void glider(int **matriz, int x, int y) { // x == 0 no glider e y == 0 no glider
    // matriz[x][y] = 0;
    matriz[x+1][y] = 1;
    // matriz[x+2][y] = 0;
    // matriz[x][y+1] = 0;
    // matriz[x+1][y+1] = 0;
    matriz[x+2][y+1] = 1;
    matriz[x][y+2] = 1;
    matriz[x+1][y+2] = 1;
    matriz[x+2][y+2] = 1;

    /* Glinger girado
    // matriz[x  ][y  ] = 0;
    matriz[x  ][y+1] = 1;
    // matriz[x  ][y+2] = 0;
    // matriz[x+1][y] = 0;
    // matriz[x+1][y+1] = 0;
    matriz[x+1][y+2] = 1;
    matriz[x+2][y] = 1;
    matriz[x+2][y+1] = 1;
    matriz[x+2][y+2] = 1;
     */
}

void rPentomino(int **matriz, int x, int y) { // x == 0 no rPentomino e y == 0 no rPentomino
    // matriz[x  ][y  ] = 0;
    matriz[x+1][y  ] = 1;
    // matriz[x+2][y  ] = 0;
    matriz[x  ][y+1] = 1;
    matriz[x+1][y+1] = 1;
    matriz[x+2][y+1] = 1;
    matriz[x  ][y+2] = 1;
    // matriz[x+1][y+2] = 0;
    // matriz[x+2][y+2] = 0;
}

void copiaMatrizes(int **matriz, int ** copia, int n) {
    for(int i = 0 ; i <= n-1; i++){   
        for(int j = 0; j <= n-1; j++){
            copia[i][j] = matriz[i][j];
        }
    }
}


void geracao(int **matriz, int **copia, int n) {
    for(int i = 0 ; i <= n-1; i++){   
        for(int j = 0; j <= n-1; j++){
            int vivos = contaCelula(matriz, i, j, n);

            if ((matriz[i][j] == 0) && (vivos == 3)) {
                copia[i][j] = 1;
            } else if ((matriz[i][j] == 1) && ((vivos < 2) || (vivos > 3))) {
                copia[i][j] = 0;
            }
        }
    }

}

void interacoes(int **matriz, int nGeracoes, int n) {
    int** copia = (int**)malloc(sizeof(int*)*n);
    for (int i = 0; i < n; i++){
        copia[i] = (int*)malloc(sizeof(int)*n);
    }

    for(int i = 0 ; i < nGeracoes; i++){   
        copiaMatrizes(matriz, copia, n);
        
        geracao(matriz, copia, n);

        copiaMatrizes(copia, matriz, n);

        printMatriz(matriz, n);
        printf("____________________\n");
    }
}

void main() {
    int n = 10;
    int** matriz = (int**)malloc(sizeof(int*)*n);
    for (int i = 0; i < n; i++){
        matriz[i] = (int*)malloc(sizeof(int)*n);
    }

    zeraMatriz(matriz, n);
    printMatriz(matriz, n);
    printf("____________________\n");
    // glider(matriz, 1, 1);
    // printMatriz(matriz, n);
    // printf("____________________\n");
    matriz[5][5] = 1;
    matriz[5][6] = 1;
    matriz[5][7] = 1;

    printMatriz(matriz, n); 
    printf("____________________\n");
    interacoes(matriz, 20, n);
    printMatriz(matriz, n);
    printf("____________________\n");
}