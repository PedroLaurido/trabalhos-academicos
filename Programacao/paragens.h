// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#ifndef TP_PARAGENS_H
#define TP_PARAGENS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


typedef struct paragem Paragem;       // struct paragem <==> Paragem

struct paragem{
    char nome[50];
    char codigo[5];
    int quantLinhas;
};

void registaParagem(Paragem** paragens, int* tamanho_array, int* capacidade_array);

void eliminaParagem(Paragem** paragens, int* tamanho_array);

void visualizaParagem(Paragem* paragens, int tamanho_array);

#endif //TP_PARAGENS_H
