// Nome: Pedro Tomás Laurido Pereira
// Nº: 2021130905

#include "linhas.h"
#include "paragens.h"
#include "utils.h"

void adicionaLinha(struct linha** listaLinhas, Paragem* paragens, int tamanho_array) {
    int opcao;
    printf("\n1 - Adicionar linha atraves de input\n");
    printf("2 - Adicionar linha atraves de ficheiro de texto\n");
    do {
        getchar();
        printf("\nOpcao que pretender escolher: ");
        scanf("%d", &opcao);
    } while (opcao != 1 && opcao != 2);

    if (opcao == 1){
        if(tamanho_array == 0){
            printf("Nao existem paragens registadas! Tem de registar para que possam ser atribuidas a linhas");
            return;
        }

        struct linha* novaLinha = malloc(sizeof(struct linha)); // para criar um novo nó
        if(novaLinha == NULL) {
            printf("Erro ao alocar memória para a nova linha.\n");
            return;
        }
        int numParagens;

        getchar();      // Limpa do buffer o "\n" do Enter do teclado
        printf("Insira o nome da linha: ");
        scanf("%49[^\n]", novaLinha->nome);
        if(comparaNomeLinhas(*listaLinhas, novaLinha->nome)) {
            printf("Ja existe uma linha com o nome '%s'!\n", novaLinha->nome);
            free(novaLinha);
            return;
        }

        printf("\nInsira o numero de paragens para esta linha: ");
        scanf("%d", &numParagens);
        getchar();

        if(numParagens <= 0) {
            printf("Numero invalido de paragens.\n");
            free(novaLinha);
            return;
        }

        novaLinha->primeiraParagem = malloc(numParagens * sizeof(Paragem));     // cria um array dinâmico para armazenar as paragens para cada nó (nova linha) adicionado
        if(novaLinha->primeiraParagem == NULL) {
            printf("Erro ao alocar memória para as paragens.\n");
            free(novaLinha);
            return;
        }

        novaLinha->numParagens = numParagens;  // Atribui o número de paragens à nova linha

        for(int i = 0; i < numParagens; i++) {
            int paragemRegistada = 0;
            do {
                printf("Insira o nome da paragem %d: ", i + 1);
                fgets(novaLinha->primeiraParagem[i].nome, sizeof(novaLinha->primeiraParagem[i].nome), stdin);
                novaLinha->primeiraParagem[i].nome[strcspn(novaLinha->primeiraParagem[i].nome, "\n")] = '\0';

                // Verifica se a paragem já foi registada
                int j;
                for(j = 0; j < tamanho_array; j++) {
                    if(strcasecmp(paragens[j].nome, novaLinha->primeiraParagem[i].nome) == 0){
                        paragemRegistada = 1;
                        strcpy(novaLinha->primeiraParagem[i].nome, paragens[j].nome);
                        paragens[j].quantLinhas++;
                        break;
                    }
                }

                if(paragemRegistada == 0) {
                    printf("A paragem '%s' nao esta registada na lista de paragens.\n", novaLinha->primeiraParagem[i].nome);
                }
            } while(paragemRegistada == 0);
        }

        novaLinha->proximaLinha = NULL;  // "aponta" o ponteiro proximaLinha da nova linha/nó para NULL pois é a última linha/nó da lista

        if(*listaLinhas == NULL){             // caso a lista esteja vazia este será o 1º nó da lista
            *listaLinhas = novaLinha;
        }
        else{                                  // caso não seja então vai percorrer a lista até ao fim
            struct linha* aux = *listaLinhas;
            while(aux->proximaLinha != NULL){
                aux = aux->proximaLinha;
            }

            aux->proximaLinha = novaLinha; // o ponteiro 'proximaLinha' do nó que anteriormente era o último fica a apontar para o novo nó (que passou a ser o último)
        }

        printf("Linha %s adicionada com sucesso.\n", novaLinha->nome);

    }
    else if (opcao == 2){
        getchar();
        char nomeFicheiro[100];
        printf("Insira o nome do ficheiro de texto: ");
        fgets(nomeFicheiro, sizeof(nomeFicheiro), stdin);
        nomeFicheiro[strcspn(nomeFicheiro, "\n")] = '\0';

        FILE* arquivo = fopen(nomeFicheiro, "r");
        if (arquivo == NULL) {
            printf("Nao foi possivel abrir o ficheiro.\n");
            return;
        }

        struct linha* novaLinha = malloc(sizeof(struct linha));
        if (novaLinha == NULL) {
            printf("Erro ao alocar memoria para a nova linha.\n");
            fclose(arquivo);
            return;
        }

        int numParagens;
        fscanf(arquivo, "%49[^\n]%*c", novaLinha->nome);  // Lê o nome da linha do arquivo

        if (comparaNomeLinhas(*listaLinhas, novaLinha->nome)) {
            printf("Ja existe uma linha com o nome '%s'!\n", novaLinha->nome);
            free(novaLinha);
            fclose(arquivo);
            return;
        }

        fscanf(arquivo, "%d%*c", &numParagens);  // Lê o número de paragens do arquivo

        if (numParagens <= 0) {
            printf("Numero invalido de paragens.\n");
            free(novaLinha);
            fclose(arquivo);
            return;
        }

        novaLinha->primeiraParagem = malloc(numParagens * sizeof(Paragem));
        if (novaLinha->primeiraParagem == NULL) {
            printf("Erro ao alocar memoria para as paragens.\n");
            free(novaLinha);
            fclose(arquivo);
            return;
        }

        novaLinha->numParagens = numParagens;

        for (int i = 0; i < numParagens; i++) {
            char nomeParagem[50];
            fscanf(arquivo, "%49[^\n]%*c", nomeParagem);  // Lê o nome da paragem do arquivo

            char* sinal = strtok(nomeParagem, "#"); // strtok divide nomeParagem antes e depois do sinal (#)
            if (sinal != NULL) {
                // Remove espaços em branco extras antes e depois do nome da paragem
                strcpy(nomeParagem, sinal);
                cortaEspacos(nomeParagem);

                int paragemRegistada = 0;
                for (int j = 0; j < tamanho_array; j++) {
                    if (strcasecmp(paragens[j].nome, nomeParagem) == 0) {
                        novaLinha->primeiraParagem[i] = paragens[j];
                        paragemRegistada = 1;
                        break;
                    }
                }
                if (!paragemRegistada) {
                    printf("Paragem '%s' nao encontrada no sistema.\n", nomeParagem);
                    free(novaLinha->primeiraParagem);
                    free(novaLinha);
                    fclose(arquivo);
                    return;
                }
            }
        }

        novaLinha->proximaLinha = *listaLinhas;
        *listaLinhas = novaLinha;

        printf("\nLinha adicionada com sucesso!\n");

        fclose(arquivo);
    }
}

void atualizaLinha(struct linha* listaLinhas, Paragem* paragens, int tamanho_array) {
    if(listaLinhas == NULL) {
        printf("\nNao existem linhas registadas...");
        return;
    }

    char nomeLinha[50];
    printf("Insira o nome da linha que deseja atualizar: ");
    getchar();
    fgets(nomeLinha, sizeof(nomeLinha), stdin);
    nomeLinha[strcspn(nomeLinha, "\n")] = '\0';

    struct linha* aux = listaLinhas;

    while(aux != NULL) {
        if(strcasecmp(aux->nome, nomeLinha) == 0) {
            int opcao;
            printf("\n1 - Adicionar paragem\n");
            printf("2 - Remover paragem\n");
            printf("\nOpcao que pretende escolher: ");
            scanf("%d", &opcao);
            getchar();

            if(opcao == 1) {
                char nomeParagem[50];
                printf("Insira o nome da paragem a adicionar: ");
                fgets(nomeParagem, sizeof(nomeParagem), stdin);
                nomeParagem[strcspn(nomeParagem, "\n")] = '\0';

                // Verifica se a paragem já foi registada
                int i;
                int paragemRegistada = 0;
                for(i = 0; i < tamanho_array; i++) {
                    if(strcasecmp(paragens[i].nome, nomeParagem) == 0) {
                        paragemRegistada = 1;
                        break;
                    }
                }

                if(paragemRegistada == 0) {
                    printf("A paragem '%s' nao esta registada na lista de paragens.\n", nomeParagem);
                    return;
                }

                // Verifica se a paragem já está na linha
                for(int j = 0; j < aux->numParagens; j++) {
                    if(strcasecmp(aux->primeiraParagem[j].nome, nomeParagem) == 0) {
                        printf("A paragem '%s' ja esta presente nesta linha.\n", nomeParagem);
                        return;
                    }
                }

                // Adiciona a nova paragem à linha
                strcpy(aux->primeiraParagem[aux->numParagens].nome, paragens[i].nome);
                paragens[i].quantLinhas++;
                aux->numParagens++;

                printf("Paragem '%s' adicionada com sucesso a linha '%s'.\n", paragens[i].nome, aux->nome);
            } else if(opcao == 2) {
                if(aux->numParagens <= 0) {
                    printf("Nao existem paragens registadas nesta linha.\n");
                    return;
                }

                char nomeParagem[50];
                printf("Insira o nome da paragem a remover: ");
                fgets(nomeParagem, sizeof(nomeParagem), stdin);
                nomeParagem[strcspn(nomeParagem, "\n")] = '\0';

                int paragemEncontrada = 0;

                // Procura a paragem na linha
                for (int j = 0; j < aux->numParagens; j++) {
                    if (strcasecmp(aux->primeiraParagem[j].nome, nomeParagem) == 0) {
                        // Atualiza quantLinhas na estrutura paragem
                        for (int k = 0; k < tamanho_array; k++) {
                            if (strcasecmp(paragens[k].nome, nomeParagem) == 0) {
                                paragens[k].quantLinhas--;
                                break;
                            }
                        }

                        // Remove a paragem da linha
                        for (int k = j; k < aux->numParagens - 1; k++) {
                            strcpy(aux->primeiraParagem[k].nome, aux->primeiraParagem[k + 1].nome);
                        }
                        aux->numParagens--;
                        paragemEncontrada = 1;
                        break;
                    }
                }

                if(paragemEncontrada == 0) {
                    printf("A paragem '%s' nao esta presente nesta linha.\n", nomeParagem);
                    return;
                }

                printf("Paragem '%s' removida com sucesso da linha '%s'.\n", nomeParagem, aux->nome);
            } else {
                printf("Opcao invalida.\n");
                return;
            }

            return;
        }

        aux = aux->proximaLinha;
    }

    printf("A linha '%s' nao foi encontrada.\n", nomeLinha);
}

void visualizaLinhas(struct linha* listaLinhas, Paragem* listaParagens) {
    if (listaLinhas == NULL) {
        printf("\nNao existem linhas registadas...");
        return;
    }

    int opcao;
    printf("\n1 - Ver lista completa de linhas\n");
    printf("2 - Ver linhas que passam numa determinada paragem\n");
    do {
        printf("\nOpcao que pretender escolher: ");
        scanf("%d", &opcao);
        getchar();  // Limpa o buffer de entrada
    } while (opcao != 1 && opcao != 2);

    if (opcao == 1) {
        printf("\nLista de linhas:\n");
        printf("-----------------------\n");

        struct linha* aux = listaLinhas;

        while (aux != NULL) {
            printf("Nome: %s\n\n", aux->nome);
            for (int i = 0; i < aux->numParagens; i++) {
                printf("Paragem %d: %s\n", i + 1, aux->primeiraParagem[i].nome);
            }
            printf("-----------------------");
            printf("\n");
            aux = aux->proximaLinha;
        }
    } else if (opcao == 2) {
        char nomeParagem[50];
        int paragemEncontrada = 0;

        do {
            printf("Insira o nome da paragem: ");
            scanf("%49[^\n]", nomeParagem);
            getchar();  // Limpa o buffer de entrada

            printf("\n");

            paragemEncontrada = 0; // Reinicializa
            struct linha* aux = listaLinhas; // Reinicializa

            while (aux != NULL) {
                for (int i = 0; i < aux->numParagens; i++) {
                    if (strcmp(aux->primeiraParagem[i].nome, nomeParagem) == 0) {
                        if (paragemEncontrada == 0) {
                            printf("Linhas que passam pela paragem '%s':\n", nomeParagem);
                            printf("-----------------------\n");
                            paragemEncontrada = 1;
                        }
                        printf("Nome: %s\n\n", aux->nome);
                        for (int j = 0; j < aux->numParagens; j++) {
                            printf("Paragem %d: %s\n", j + 1, aux->primeiraParagem[j].nome);
                        }
                        printf("-----------------------");
                        printf("\n");
                    }
                }
                aux = aux->proximaLinha;
            }

            if (paragemEncontrada == 0) {
                printf("A paragem '%s' nao tem nenhuma linha atribuida ou nao esta registada. Por favor, tente novamente.\n", nomeParagem);
            }
        } while (paragemEncontrada == 0);
    }
}