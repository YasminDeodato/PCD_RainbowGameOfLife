/*
* Programacao Concorrente e Distribuida
* Rainbow Game of Life com Openmp
* Helio Didzec Junior
* Yasmin Beatriz Deodato
*/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "rainbowGameOfLifeSerialAdaptado.h"

#define NMATRIZ 50
#define LARGURA 700.0
#define ALTURA 700.0

float** matriz;
int geracaoAtual = 0;

void setarCorDaCelula(int i, int j) {
    float cor = (matriz[i][j])/1.0;
    glColor3f(cor, cor, cor);
}

void desenha(){
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    int larguraCelula = LARGURA / NMATRIZ;
    int alturaCelula = ALTURA / NMATRIZ;

    for (int i = 0; i < NMATRIZ; i++) {
        for (int j = 0; j < NMATRIZ; j++) {
            if (matriz[i][j] > 0.0) {
                setarCorDaCelula(i, j);
                glBegin(GL_QUADS);
                glVertex2i(j * larguraCelula, (NMATRIZ - 1 - i) * alturaCelula);
                glVertex2i((j + 1) * larguraCelula, (NMATRIZ - 1 - i) * alturaCelula);
                glVertex2i((j + 1) * larguraCelula, (NMATRIZ - i) * alturaCelula);
                glVertex2i(j * larguraCelula, (NMATRIZ - i) * alturaCelula);
                glEnd();
            }
        }
    }

    glFlush();
}

void configuracoesIniciais() {
    matriz=(float**)malloc(sizeof(float*)*NMATRIZ);
    for (int i = 0; i < NMATRIZ; i++){
        matriz[i] = (float*)malloc(sizeof(float)*NMATRIZ);
        for (int j = 0; j < NMATRIZ; j++){ 
            matriz[i][j] = 0;
        }
    }    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LARGURA, 0, ALTURA);
    glMatrixMode(GL_MODELVIEW);
}

void atualizarEExibir() {
    geracaoAtual++;
    executaInteracao(geracaoAtual);
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(LARGURA,ALTURA);
    glutCreateWindow("Rainbow Game of Life");
    configuracoesIniciais();
    glutDisplayFunc(desenha);
    desenha();
    rainbowGameOfLife();
    glutIdleFunc(atualizarEExibir);
    glutMainLoop();
    return 0;
}