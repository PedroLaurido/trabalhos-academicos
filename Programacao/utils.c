// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#include "utils.h"
#include "paragens.h"
#include "linhas.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparaNome(Paragem* paragens, int tamanho_array, char* nome){
    for(int i = 0; i < tamanho_array; i++){
        if(strcasecmp(paragens[i].nome, nome) == 0){
            return 1;
        }
    }
    return 0;
}

void gerarCodigo(char* codigo){
    codigo[0] = 'A' + rand() % 26;     // gera uma letra maiuscula aleatória; 26 -> numero de letras
    for(int i=1; i<4; i++)             // gera um numero aleatório; 10 -> numero de algarismos
        codigo[i] = '0' + rand() % 10;
    codigo[4] = '\0';                  // marca o fim da string
}

int comparaCodigo(Paragem* paragens, int tamanho_array, char* codigo){
    for(int i=0; i<tamanho_array; i++){
        if(strcmp(paragens[i].codigo, codigo) == 0){   // compara o codigo de cada paragem com o codigo gerado
            return 1;
        }
    }
    return 0;
}

int comparaNomeLinhas(struct linha* listaLinhas, char* nome) {
    struct linha* aux = listaLinhas;

    while(aux != NULL){
        if(strcasecmp(aux->nome, nome) == 0){
            return 1;
        }
        aux = aux->proximaLinha;
    }
    return 0;
}

void cortaEspacos(char *nomeP){
    int i, j;
    int n = strlen(nomeP);

    i = 0;
    while(isspace(nomeP[i])){                 // verifica se o caracter é um espaço em branco
        i++;                                       // Tira espaços à Esquerda
    }

    j = n-1;
    while(j >= i && isspace(nomeP[j])){
        j--;                                       // Tira espaços à Direita
    }

    // Mover a string para a esquerda para remover espaços em branco extras
    if(i>0){
        memmove(nomeP, nomeP+i, j-i+2);
    }

    nomeP[j-i+1] = '\0';                           // Marca o fim da string
}

void calculaPercurso(Linha* listaLinhas) {         // só mostra se tiverem na mesma linha as paragens
    char partida[50], chegada[50];
    printf("Digite a paragem de partida: ");
    scanf("%s", partida);
    printf("Digite a paragem de chegada: ");
    scanf("%s", chegada);

    Linha* auxPartida = listaLinhas;
    int i, j, k;

    while (auxPartida != NULL) {
        for (i = 0; i < auxPartida->numParagens; i++) {
            if (strcmp(auxPartida->primeiraParagem[i].nome, partida) == 0) {
                break;
            }
        }

        if (i < auxPartida->numParagens) {
            for (j = 0; j < auxPartida->numParagens; j++) {
                if (strcmp(auxPartida->primeiraParagem[j].nome, chegada) == 0) {   // Verifica se a paragem de chegada está na mesma linha da partida
                    printf("\nPercurso encontrado usando a linha %s:\n", auxPartida->nome);
                    if (i < j) {
                        for (k = i; k <= j; k++) {
                            printf("%s -> ", auxPartida->primeiraParagem[k].nome);
                        }
                    } else {
                        for (k = i; k >= j; k--) {
                            printf("%s -> ", auxPartida->primeiraParagem[k].nome);
                        }
                    }
                    printf("\n");
                    return;
                }
            }
        }
        auxPartida = auxPartida->proximaLinha;
    }
    printf("Nao e possivel encontrar um percurso entre as paragens de partida e chegada.\n");
}

void guardarInfo(Paragem* paragens, int tamanho_array, struct linha* listaLinhas, char* nomeFicheiro) {
    if(tamanho_array == 0){ //caso não haja paragens
        return;
    }
    FILE* arquivo = fopen(nomeFicheiro, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fwrite(&tamanho_array, sizeof(int), 1, arquivo);               // escreve o numero de paragens
    fwrite(paragens, sizeof(Paragem), tamanho_array, arquivo);     // escreve os elementos (paragens) do array das paragens

    if (listaLinhas != NULL) {                   // para o caso de não haver linhas registadas
        int num_linhas = 0;
        struct linha* aux = listaLinhas;         // ponteiro aux para percorrer a lista de linhas
        while (aux != NULL) {                    // enquanto não chegar ao último elemento
            num_linhas++;
            aux = aux->proximaLinha;
        }

        fwrite(&num_linhas, sizeof(int), 1, arquivo);   // escreve o numero de linhas

        aux = listaLinhas;
        while (aux != NULL) {      // volta a percorrer a lista
            fwrite(aux->nome, sizeof(char), sizeof(aux->nome), arquivo);
            fwrite(&aux->numParagens, sizeof(int), 1, arquivo);
            fwrite(aux->primeiraParagem, sizeof(Paragem), aux->numParagens, arquivo);  // escreve as paragens dessa linha
            aux = aux->proximaLinha;
        }
    }
    else  // caso não tenha linhas registadas
    {
        int num_linhas = 0;
        fwrite(&num_linhas, sizeof(int), 1, arquivo);
    }

    fclose(arquivo);
}

void reconstruirEstruturas(Paragem** paragens, int* tamanho_array, Linha** listaLinhas, char* nomeFicheiro) {
    FILE* arquivo = fopen(nomeFicheiro, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeFicheiro);
        return;
    }

    fread(tamanho_array, sizeof(int), 1, arquivo); // Ler o tamanho do array dinâmico

    *paragens = (Paragem*)malloc((*tamanho_array) * sizeof(Paragem)); // Alocar memória para o array dinâmico
    if (*paragens == NULL) {
        printf("Erro ao alocar memória para o array dinâmico.\n");
        fclose(arquivo);
        return;
    }

    fread(*paragens, sizeof(Paragem), *tamanho_array, arquivo); // Ler o array dinâmico

    int num_linhas;
    fread(&num_linhas, sizeof(int), 1, arquivo);                // Ler o número de linhas


    *listaLinhas = NULL;
    for (int i = 0; i < num_linhas; i++) {                           // Reconstruir a lista ligada de linhas
        Linha* linha = (Linha*)malloc(sizeof(Linha));
        if (linha == NULL) {
            printf("Erro ao alocar memória para a lista ligada de linhas.\n");
            fclose(arquivo);
            return;
        }


        fread(linha->nome, sizeof(char), 50, arquivo);    // Ler o nome da linha
        linha->nome[49] = '\0';                                         // colocar o caracter de terminação


        int num_paragens;        // Ler o número de paragens para esta linha
        fread(&num_paragens, sizeof(int), 1, arquivo);

        linha->primeiraParagem = (Paragem*)malloc(num_paragens * sizeof(Paragem)); // Alocar memória para as paragens da linha
        if (linha->primeiraParagem == NULL) {
            printf("Erro ao alocar memória para as paragens da linha.\n");
            fclose(arquivo);
            free(linha);
            return;
        }

        fread(linha->primeiraParagem, sizeof(Paragem), num_paragens, arquivo); // Ler as paragens da linha

        linha->numParagens = num_paragens;   // Inserir a linha na lista ligada
        linha->proximaLinha = *listaLinhas;
        *listaLinhas = linha;
    }

    fclose(arquivo);
}