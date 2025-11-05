#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "utils.h"

#define SERVER_FIFO_RECEBE "pipeManagerRecebe"

char username[10];
int feedAtivo = 1;

void enviarMensagem(char *comando);
void subscreverTopico(char *comando);
void cancelarSubscricao(char *comando);
void listarTopicos();

// Função da thread para receber mensagens (broadcast)
void *threadRecebeMensagens(void *arg) {
    // Cria o nome do FIFO exclusivo para este feed
    char nameFifo[BUFFER_SIZE];
    snprintf(nameFifo, sizeof(nameFifo), "FIFOCLIENTE%d", getpid());

    FILE *fifo;
    while ((fifo = fopen(nameFifo, "r")) == NULL) { // para verificar se o fifo foi criado
        usleep(100000); // Aguarda 100ms entre cada "tentativa"
    }
    fclose(fifo);


    // Abre o FIFO exclusivo para leitura em modo não bloqueante
    int fd_broadcast = open(nameFifo, O_RDONLY | O_NONBLOCK);
    char mensagem[BUFFER_SIZE];

    if (fd_broadcast == -1) {
        perror("Erro ao abrir FIFO exclusivo");
        pthread_exit(NULL);
    }

    // Loop para leitura das mensagens
    while (feedAtivo) {
        int bytesLidos = read(fd_broadcast, mensagem, BUFFER_SIZE);
        //ssize_t bytesLidos = read(fd_broadcast, mensagem, BUFFER_SIZE);
        if (bytesLidos > 0) {
            printf("\n%s\n", mensagem);

            // Verifica se a mensagem é de encerramento
            if (strcmp(mensagem, "Encerrando a plataforma. Todos os feeds devem terminar.") == 0) {
                feedAtivo = 0;  // Encerra o feed imediatamente
            }
            // Verifica se a mensagem é de remoção específica do feed
            else if (strcmp(mensagem, "Foi removido pelo manager.\n") == 0) {
                feedAtivo = 0;  // Sinaliza para encerrar o feed
            }
        } else if (bytesLidos == -1 && errno != EAGAIN) {
            perror("Erro ao ler FIFO");
            break;
        }

        usleep(100000);
    }

    close(fd_broadcast);
    unlink(nameFifo);
    pthread_exit(NULL);
}

// Função da thread para processar comandos do usuário
void *threadComandos(void *arg) {
    char comando[BUFFER_SIZE];

    while (feedAtivo) {
        printf("> ");
        if (fgets(comando, BUFFER_SIZE, stdin)) {
            comando[strcspn(comando, "\n")] = '\0'; // Remove o '\n' da string

            if (strcmp(comando, "exit") == 0) {
                // Envia logout ao manager
                int fd_envia = open(SERVER_FIFO_RECEBE, O_WRONLY);
                if (fd_envia == -1) {
                    perror("Erro ao abrir FIFO principal para logout");
                } else {
                    MSG mensagem;
                    mensagem.pid = getpid();
                    mensagem.val = -1; // Indica logout
                    strncpy(mensagem.nome, username, sizeof(mensagem.nome) - 1);
                    write(fd_envia, &mensagem, sizeof(MSG));
                    close(fd_envia);
                }

                feedAtivo = 0; // Sinaliza para encerrar o feed
                break;
            } else if (strncmp(comando, "msg", 3) == 0) {
                enviarMensagem(comando);
            } else if (strncmp(comando, "subscribe", 9) == 0) {
                subscreverTopico(comando);
            } else if (strncmp(comando, "unsubscribe", 11) == 0) {
                cancelarSubscricao(comando);
            } else if (strncmp(comando, "topics", 6) == 0) {
                listarTopicos();
            } else {
                printf("Comando desconhecido: %s\n", comando);
            }
        }
    }

    pthread_exit(NULL);
}

// Função para enviar uma mensagem
void enviarMensagem(char *comando) {
    char topico[BUFFER_SIZE], mensagem[MAX_TAM_MSG];
    int duracao;

    // Extrai os argumentos do comando 'msg'
    if (sscanf(comando, "msg %s %d %[^\n]", topico, &duracao, mensagem) != 3) {
        printf("Comando inválido. O formato correto é: msg <topico> <duracao> <mensagem>\n");
        return;
    }

    int fd_envia = open(SERVER_FIFO_RECEBE, O_WRONLY);
    if (fd_envia == -1) {
        perror("Erro ao abrir FIFO principal");
        return;
    }

    MSG mensagemStruct;
    mensagemStruct.pid = getpid();
    mensagemStruct.val = 3;
    snprintf(mensagemStruct.comando, sizeof(mensagemStruct.comando), "msg %s %d %s", topico, duracao, mensagem);

    write(fd_envia, &mensagemStruct, sizeof(MSG));

    close(fd_envia);
}

// Função para subscrever a um tópico
void subscreverTopico(char *comando) {
    char topico[BUFFER_SIZE];

    // Extrai o tópico do comando 'subscribe'
    if (sscanf(comando, "subscribe %s", topico) != 1) {
        printf("Comando inválido. O formato correto é: subscribe <topico>\n");
        return;
    }

    int fd_envia = open(SERVER_FIFO_RECEBE, O_WRONLY);
    if (fd_envia == -1) {
        perror("Erro ao abrir FIFO principal");
        return;
    }

    MSG mensagem;
    mensagem.pid = getpid();
    mensagem.val = 4; // Indica comando subscribe
    strncpy(mensagem.nome, username, sizeof(mensagem.nome) - 1);
    snprintf(mensagem.comando, sizeof(mensagem.comando), "subscribe %s", topico);

    write(fd_envia, &mensagem, sizeof(MSG)); // Envia o comando ao manager
    close(fd_envia);
}

// Função para cancelar a subscrição de um tópico
void cancelarSubscricao(char *comando) {
    char topico[BUFFER_SIZE];

    // Extrai o tópico do comando 'unsubscribe'
    if (sscanf(comando, "unsubscribe %s", topico) != 1) {
        printf("Comando inválido. O formato correto é: unsubscribe <topico>\n");
        return;
    }

    int fd_envia = open(SERVER_FIFO_RECEBE, O_WRONLY);
    if (fd_envia == -1) {
        perror("Erro ao abrir FIFO principal");
        return;
    }

    MSG mensagem;
    mensagem.pid = getpid();
    mensagem.val = 5; // Indica comando unsubscribe
    strncpy(mensagem.nome, username, sizeof(mensagem.nome) - 1);
    snprintf(mensagem.comando, sizeof(mensagem.comando), "unsubscribe %s", topico);

    write(fd_envia, &mensagem, sizeof(MSG)); // Envia o comando ao manager
    close(fd_envia);
}

// Função para listar tópicos
void listarTopicos() {
    int fd_envia = open(SERVER_FIFO_RECEBE, O_WRONLY);
    if (fd_envia == -1) {
        perror("Erro ao abrir FIFO principal");
        return;
    }

    MSG mensagem;
    mensagem.pid = getpid();
    mensagem.val = 2; // Indica comando topics
    strncpy(mensagem.nome, username, sizeof(mensagem.nome) - 1);
    snprintf(mensagem.comando, sizeof(mensagem.comando), "topics");

    write(fd_envia, &mensagem, sizeof(MSG));
    close(fd_envia);

    // Agora, espera pela resposta do manager
    char nameFifo[BUFFER_SIZE];
    snprintf(nameFifo, sizeof(nameFifo), "FIFOCLIENTE%d", getpid());
    int fd_recebe = open(nameFifo, O_RDONLY);
    if (fd_recebe == -1) {
        perror("Erro ao abrir FIFO exclusivo para tópicos");
        return;
    }

    char resposta[BUFFER_SIZE];
    while (read(fd_recebe, resposta, sizeof(resposta)) > 0) {
        printf("%s\n", resposta);
    }

    close(fd_recebe);
}

void tratarCtrlCFeed(int signum) {
    printf("\nA encerrar o feed...\n");
    char nameFifo[BUFFER_SIZE];
    snprintf(nameFifo, sizeof(nameFifo), "FIFOCLIENTE%d", getpid());
    unlink(nameFifo);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, tratarCtrlCFeed);
    pthread_t tRecebeMensagens, tComandos;
    MSG mensagem;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(username, argv[1], sizeof(username) - 1);

    // Envia login ao manager
    int fd_envia = open(SERVER_FIFO_RECEBE, O_WRONLY);
    if (fd_envia == -1) {
        perror("Erro ao abrir FIFO principal");
        exit(EXIT_FAILURE);
    }

    mensagem.pid = getpid();
    mensagem.val = 0; // Indica login
    strncpy(mensagem.nome, username, sizeof(mensagem.nome) - 1);
    write(fd_envia, &mensagem, sizeof(MSG));
    close(fd_envia);

    // Cria as threads
    pthread_create(&tRecebeMensagens, NULL, threadRecebeMensagens, NULL);
    pthread_create(&tComandos, NULL, threadComandos, NULL);

    // Aguarda as threads terminarem
    pthread_join(tRecebeMensagens, NULL);
    pthread_join(tComandos, NULL);

    return 0;
}
