// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#include "paragens.h"
#include "linhas.h"
#include "utils.h"

#include <stdio.h>

int main()
{
    Paragem* paragens = NULL;
    int tamanho_array = 0;
    int capacidade_array = 1;
    int opcao;
    paragens = malloc(capacidade_array * sizeof(Paragem));
    if (paragens == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 0;
    }

    struct linha* listaLinhas = NULL;
    reconstruirEstruturas(&paragens, &tamanho_array, &listaLinhas, "info.bin");

    do {
        printf("\n\n-------Paragens-------\n");
        printf("1 - Registar Paragem\n");
        printf("2 - Eliminar Paragem\n");
        printf("3 - Visualizar Paragens\n");
        printf("--------Linhas--------\n");
        printf("4 - Adicionar Linha\n");
        printf("5 - Atualizar Linha\n");
        printf("6 - Visualizar Linhas\n");
        printf("--------Outros--------\n");
        printf("7 - Calcular Percurso\n");
        printf("8 - Sair\n");
        printf("----------------------\n");
        printf("\nOpcao que pretender escolher: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                registaParagem(&paragens, &tamanho_array, &capacidade_array);
                break;
            case 2:
                eliminaParagem(&paragens, &tamanho_array);
                break;
            case 3:
                visualizaParagem(paragens, tamanho_array);
                break;
            case 4:
                adicionaLinha(&listaLinhas, paragens, tamanho_array);
                break;
            case 5:
                atualizaLinha(listaLinhas, paragens, tamanho_array);
                break;
            case 6:
                visualizaLinhas(listaLinhas, paragens);
                break;
            case 7:
                calculaPercurso(listaLinhas);
                break;
            case 8:
                printf("\nGuardando e saindo...");
                break;
        }
    } while (opcao != 8);

    guardarInfo(paragens, tamanho_array, listaLinhas, "info.bin");
    free(paragens);

    return 0;
}
