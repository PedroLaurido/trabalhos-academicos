// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#include "paragens.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void registaParagem(Paragem** paragens, int* tamanho_array, int* capacidade_array){

    char nome[50];
    char codigo[5];

    if(*tamanho_array >= *capacidade_array){
        (*capacidade_array) = (*tamanho_array)+1;
        *paragens = realloc(*paragens,(*capacidade_array) * sizeof(Paragem));
        if(*paragens == NULL){
            printf("Erro ao alocar memoria\n");
            return;
        }
    }

    do{
        printf("\nInsira o nome da paragem: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0';
        if(comparaNome(*paragens, *tamanho_array, nome)){
            printf("\nO nome ja foi registado!\n");
        }
    }while(strlen(nome) == 0 || strlen(nome) >= 50 || comparaNome(*paragens, *tamanho_array, nome));

    srand(time(NULL));
    do{
        gerarCodigo(codigo);
    }while(comparaCodigo(*paragens, *tamanho_array, codigo));

    strcpy((*paragens)[*tamanho_array].nome, nome);
    strcpy((*paragens)[*tamanho_array].codigo, codigo);
    (*paragens)[*tamanho_array].quantLinhas = 0;
    (*tamanho_array)++;
    printf("\nParagem adicionada...");
}

void eliminaParagem(Paragem** paragens, int* tamanho_array){

    int indice = -100;
    char codigo[5];

    do{
        if(*tamanho_array == 0){
            printf("\nNao existem paragens para ser eliminadas!");
            return;
        }
        else{
            printf("Insira o codigo da paragem a eliminar: ");
            scanf("%s", codigo);

            for(int i = 0; i < *tamanho_array; i++){
                if(strcmp((*paragens)[i].codigo, codigo) == 0){
                    indice = i;
                    break;
                }
            }

            if(indice == -100){
                printf("Paragem com o codigo %s nao encontrada.\n", codigo);
            }
            else if((*paragens)[indice].quantLinhas > 0){
                printf("A paragem '%s' com o codigo '%s' nao pode ser eliminada pois esta atribuida a pelo menos uma linha.\n", (*paragens)[indice].nome, codigo);
                indice = -100; // Reinicia o índice para continuar o loop
            }
        }
    } while(indice == -100);

    for(int i = indice; i <(*tamanho_array)-1; i++){
       (*paragens)[i] =(*paragens)[i + 1];   // desloca "as paragens" -1 indice para "eliminar" a paragem de indice i
    }

   (*tamanho_array)--;

    printf("Paragem com o codigo '%s' eliminada com sucesso.\n", codigo);
}

void visualizaParagem(Paragem* paragens, int tamanho_array){
    if(tamanho_array == 0){
        printf("\nNao existem paragens registadas...");
        return;
    }

    printf("\n");
    printf("\nLista de Paragens:\n");
    for(int i = 0; i < tamanho_array; i++){
        printf("\nParagem %d\n", i + 1);
        printf("Nome: %s\n", paragens[i].nome);
        printf("Codigo: %s\n", paragens[i].codigo);
        printf("Numero de linhas a que pertence: %d\n", paragens[i].quantLinhas);
    }
}