# 🌈 Rainbow Game Of Life

Repositório para códigos da atividade 1 da disciplina de Programação Concorrente e Distribuída, ministrada no 2sem/2023, no ICT-Unifesp.

### Alunos
Hélio Didzec Júnior \
Yasmin Beatriz Deodato

### Informações

#### 0️⃣ Compilar
```bash
# versao sequencial
gcc -o rainbow rainbowGameOfLife.c
# versao concorrente - pthreads
gcc -o rainbow rainbowGameOfLife.c -lpthread
# versao concorrente - openmp
gcc -o rainbow rainbowGameOfLife.c -fopenmp
```
#### 1️⃣ Executar
```bash
./rainbow
time ./rainbow
```
