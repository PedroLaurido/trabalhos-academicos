// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#ifndef TP_LINHAS_H
#define TP_LINHAS_H

#include <ctype.h>

#include "paragens.h"

typedef struct linha Linha;       // struct linha <==> Linha

struct linha {
    char nome[50];
    int numParagens;
    Paragem* primeiraParagem;         // ponteiro para a primeira paragem da linha
    struct linha* proximaLinha;       // ponteiro para a próxima linha/nó
};

void adicionaLinha(struct linha** listaLinhas, Paragem* paragens, int tamanho_array);

void atualizaLinha(struct linha* listaLinhas, Paragem* paragens, int tamanho_array);

void visualizaLinhas(struct linha* listaLinhas, Paragem* listaParagens);

#endif //TP_LINHAS_H