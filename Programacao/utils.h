// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#ifndef TP_UTILS_H
#define TP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "paragens.h"
#include "linhas.h"

int comparaNome(Paragem* paragens, int tamanho_array, char* nome);

void gerarCodigo(char* codigo);

int comparaCodigo(Paragem* paragens, int tamanho_array, char* codigo);

int comparaNomeLinhas(struct linha* listaLinhas, char* nome);

void cortaEspacos(char *nomeP);

void calculaPercurso(struct linha* listaLinhas);

void guardarInfo(Paragem* paragens, int tamanho_array, struct linha* listaLinhas, char* nomeFicheiro);

void reconstruirEstruturas(Paragem** paragens, int* tamanho_array, Linha** listaLinhas, char* nomeFicheiro);

#endif //TP_UTILS_H