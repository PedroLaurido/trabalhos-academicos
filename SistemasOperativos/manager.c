#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include "utils.h"

#define SERVER_FIFO_RECEBE "pipeManagerRecebe"
#define SERVER_FIFO_BROADCAST "pipeManagerBroadcast"

char utilizadoresOnline[MAX_UTILIZADORES][10];
pthread_mutex_t mutexUtilizadores = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexFeeds = PTHREAD_MUTEX_INITIALIZER;

int managerLigado = 1;
TDADOS feeds[MAX_UTILIZADORES];
int numFeeds = 0;
Topico topicos[MAX_TOPICOS];
int numTopicos = 0;

void adicionarFeed(int pid);
void enviarMensagemParaFeeds(const char *mensagem);
void *threadRecebe(void *arg);

int verificaUser(const char *username) {
    for (int i = 0; i < MAX_UTILIZADORES; i++) {
        if (strcmp(utilizadoresOnline[i], username) == 0) {
            return 1;
        }
    }
    return 0;
}

void adicionaUtilizador(const char *username) {
    for (int i = 0; i < MAX_UTILIZADORES; i++) {
        if (utilizadoresOnline[i][0] == '\0') {
            strncpy(utilizadoresOnline[i], username, sizeof(utilizadoresOnline[i]) - 1);
            break;
        }
    }
}

void removeUtilizador(const char *username) {
    pthread_mutex_lock(&mutexFeeds);
    pthread_mutex_lock(&mutexUtilizadores);

    int feedRemovido = 0;

    for (int i = 0; i < numFeeds; i++) {
        TDADOS *feed = &feeds[i];
        if (strcmp(utilizadoresOnline[i], username) == 0) {
            // Envia mensagem ao feed informando que foi removido
            int fd = open(feed->nameFifo, O_WRONLY);
            if (fd != -1) {
                write(fd, "Foi removido pelo manager.\n", 28);
                close(fd);
            } else {
                printf("Erro ao abrir FIFO do feed para remover '%s'.\n", username);
            }

            // Remove o feed da lista
            unlink(feed->nameFifo);
            for (int j = i; j < numFeeds - 1; j++) {
                feeds[j] = feeds[j + 1];
            }
            numFeeds--;

            // Remove o utilizador da lista de online
            for (int k = 0; k < MAX_UTILIZADORES; k++) {
                if (strcmp(utilizadoresOnline[k], username) == 0) {
                    utilizadoresOnline[k][0] = '\0';
                    break;
                }
            }

            printf("Utilizador '%s' foi removido.\n", username);
            feedRemovido = 1;

            char mensagem[BUFFER_SIZE];
            snprintf(mensagem, sizeof(mensagem), "O utilizador '%s' foi removido da plataforma.\n", username);

            for (int j = 0; j < numFeeds; j++) {
                TDADOS *outroFeed = &feeds[j];
                int fd_outro = open(outroFeed->nameFifo, O_WRONLY);
                if (fd_outro != -1) {
                    write(fd_outro, mensagem, strlen(mensagem) + 1);
                    close(fd_outro);
                } else {
                    printf("Erro ao abrir FIFO do feed %d para notificar.\n", outroFeed->pid);
                }
            }

            break;
        }
    }

    pthread_mutex_unlock(&mutexUtilizadores);
    pthread_mutex_unlock(&mutexFeeds);

    if (feedRemovido) {
        // Informa todos os outros feeds sobre a remoção
        char mensagem[BUFFER_SIZE];
        snprintf(mensagem, sizeof(mensagem), "O utilizador '%s' foi removido da plataforma.\n", username);
        enviarMensagemParaFeeds(mensagem);
    } else {
        printf("Nenhum feed associado ao utilizador '%s' foi encontrado.\n", username);
    }
}

// Função para registrar o feed e criar o FIFO exclusivo
void adicionarFeed(int pid) { // regista o feed e cria o fifo
    pthread_mutex_lock(&mutexFeeds);

    if (numFeeds < MAX_UTILIZADORES) {
        TDADOS *novoFeed = &feeds[numFeeds++];
        snprintf(novoFeed->nameFifo, sizeof(novoFeed->nameFifo), "FIFOCLIENTE%d", pid); // FIFO exclusivo para o feed
        novoFeed->pid = pid;

        if (mkfifo(novoFeed->nameFifo, 0666) == -1) {
            perror("Erro ao criar FIFO para o feed");
        } else {
            printf("FIFO criado para PID %d: %s\n", pid, novoFeed->nameFifo);
        }
    } else {
        printf("Número máximo de feeds alcançado.\n");
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void enviarMensagemParaFeeds(const char *mensagem) {
    pthread_mutex_lock(&mutexFeeds);

    for (int i = 0; i < numFeeds; i++) {
        TDADOS *feed = &feeds[i];
        int fd = open(feed->nameFifo, O_WRONLY);
        if (fd != -1) {
            write(fd, mensagem, strlen(mensagem) + 1);
            close(fd);
        } else {
            printf("Erro ao abrir FIFO para o feed %d\n", feed->pid);
        }
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void listarTopicos(int fd_feed) {
    char resposta[BUFFER_SIZE] = "Tópicos:\n";

    for (int i = 0; i < numTopicos; i++) {
        char linha[BUFFER_SIZE];
        snprintf(linha, sizeof(linha), "- %s: %d mensagens persistentes, %s\n",
                 topicos[i].nome,
                 topicos[i].numMensagensPersistentes,
                 topicos[i].bloqueado ? "bloqueado": "desbloqueado");
        strncat(resposta, linha, sizeof(resposta) - strlen(resposta) - 1);
    }

    write(fd_feed, resposta, strlen(resposta) + 1);
}

void subscreverTopico(char *nomeTopico, int pid) {
    int topicoEncontrado = -1;

    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, nomeTopico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        if (numTopicos < MAX_TOPICOS) {
            topicoEncontrado = numTopicos++;
            strcpy(topicos[topicoEncontrado].nome, nomeTopico);
            topicos[topicoEncontrado].numMensagensPersistentes = 0;
            topicos[topicoEncontrado].bloqueado = 0;
            topicos[topicoEncontrado].numSubscritores = 0;
        } else {
            printf("Número máximo de tópicos alcançado.\n");
            return;
        }
    }

    topicos[topicoEncontrado].numSubscritores++;  // Adiciona o feed como subscritor do tópico
    printf("Feed %d subscreveu o tópico %s\n", pid, nomeTopico);
}

void processarMensagem(char *comando, int pid) {
    char topico[BUFFER_SIZE], mensagem[MAX_TAM_MSG];
    int duracao;

    if (sscanf(comando, "msg %s %d %[^\n]", topico, &duracao, mensagem) != 3) {
        printf("Comando 'msg' inválido. Formato esperado: msg <topico> <duração> <mensagem>\n");
        return;
    }

    pthread_mutex_lock(&mutexFeeds);

    int topicoEncontrado = -1;
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, topico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    // Se o tópico não existir, cria um novo
    if (topicoEncontrado == -1) {
        if (numTopicos < MAX_TOPICOS) {
            topicoEncontrado = numTopicos++;
            strncpy(topicos[topicoEncontrado].nome, topico, sizeof(topicos[topicoEncontrado].nome) - 1);
            topicos[topicoEncontrado].numMensagensPersistentes = 0;
            topicos[topicoEncontrado].bloqueado = 0;
            topicos[topicoEncontrado].numSubscritores = 0;
            printf("Tópico '%s' criado.\n", topico);
        } else {
            printf("Número máximo de tópicos atingido. Mensagem não será enviada.\n");
            pthread_mutex_unlock(&mutexFeeds);
            return;
        }
    }

    Topico *topicoAtual = &topicos[topicoEncontrado];

    if (topicoAtual->bloqueado) {
        printf("Tópico '%s' está bloqueado. Mensagem não será enviada.\n", topico);
        pthread_mutex_unlock(&mutexFeeds);
        
        return;
    }

    for (int i = 0; i < topicoAtual->numSubscritores; i++) {
        int subscritoPID = topicoAtual->subscritores[i];

        for (int j = 0; j < numFeeds; j++) {
            if (feeds[j].pid == subscritoPID) {
                int fd = open(feeds[j].nameFifo, O_WRONLY);
                if (fd != -1) {
                    char mensagemFinal[BUFFER_SIZE];
                    snprintf(mensagemFinal, sizeof(mensagemFinal), "[%s]: %s", topico, mensagem);
                    write(fd, mensagemFinal, strlen(mensagemFinal) + 1);
                    close(fd);
                } else {
                    printf("Erro ao abrir FIFO para o feed %d.\n", feeds[j].pid);
                }
                break;
            }
        }
    }

    if (duracao > 0) { // mensagem persistente
        if (topicoAtual->numMensagensPersistentes < MAX_MENSAGENS) {
            MensagemPersistente *novaMensagem = &topicoAtual->mensagens[topicoAtual->numMensagensPersistentes++];
            strncpy(novaMensagem->corpo, mensagem, sizeof(novaMensagem->corpo) - 1);
            novaMensagem->tempo_vida = duracao;
            novaMensagem->tempoRestante = duracao;
            printf("Mensagem persistente adicionada ao tópico '%s'.\n", topico);
        } else {
            printf("Tópico '%s' atingiu o limite de mensagens persistentes.\n", topico);
        }
    }

    pthread_mutex_unlock(&mutexFeeds);
}



void enviarMensagemTopico(char *nomeTopico, int duracao, char *mensagem) {
    pthread_mutex_lock(&mutexFeeds);

    int topicoEncontrado = -1;
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, nomeTopico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        printf("Tópico '%s' não encontrado. Mensagem não enviada.\n", nomeTopico);
        pthread_mutex_unlock(&mutexFeeds);
        return;
    }

    Topico *topico = &topicos[topicoEncontrado];

    // Enviar mensagem para os feeds subscritos
    for (int i = 0; i < numFeeds; i++) {
        TDADOS *feed = &feeds[i];
        int fd = open(feed->nameFifo, O_WRONLY);
        if (fd != -1) {
            write(fd, mensagem, strlen(mensagem) + 1);
            close(fd);
            printf("Mensagem enviada ao feed %d para o tópico '%s'.\n", feed->pid, nomeTopico);
        } else {
            printf("Erro ao enviar mensagem para o feed %d.\n", feed->pid);
        }
    }

    if (duracao > 0) {
        if (topico->numMensagensPersistentes < MAX_MENSAGENS) {
            MensagemPersistente *novaMensagem = &topico->mensagens[topico->numMensagensPersistentes++];
            strncpy(novaMensagem->corpo, mensagem, MAX_TAM_MSG);
            novaMensagem->tempo_vida = duracao;
            novaMensagem->tempoRestante = duracao;
            printf("Mensagem persistente adicionada ao tópico '%s'.\n", nomeTopico);
        } else {
            printf("Tópico '%s' atingiu o limite de mensagens persistentes.\n", nomeTopico);
        }
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void processarSubscribe(char *comando, int pid) {
    char topico[BUFFER_SIZE];

    if (sscanf(comando, "subscribe %s", topico) != 1) {
        printf("Comando inválido para subscribe.\n");
        return;
    }

    pthread_mutex_lock(&mutexFeeds);

    int topicoEncontrado = -1;

    // Verifica se o tópico já existe
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, topico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        if (numTopicos < MAX_TOPICOS) {
            topicoEncontrado = numTopicos++;
            strncpy(topicos[topicoEncontrado].nome, topico, sizeof(topicos[topicoEncontrado].nome) - 1);
            topicos[topicoEncontrado].numMensagensPersistentes = 0;
            topicos[topicoEncontrado].bloqueado = 0;
            topicos[topicoEncontrado].numSubscritores = 0;
            printf("Tópico '%s' criado e subscrito\n", topico);
        } else {
            printf("Número máximo de tópicos atingido. Não foi possível subscrever.\n");
            pthread_mutex_unlock(&mutexFeeds);
            return;
        }
    }

    // Adiciona o feed como subscritor
    Topico *topicoAtual = &topicos[topicoEncontrado];
    //topicoAtual->numSubscritores++;
    topicoAtual->subscritores[topicoAtual->numSubscritores++] = pid;
    printf("Feed %d subscreveu o tópico '%s'.\n", pid, topico);

    // Envia mensagens persistentes existentes para o feed
    char nameFifo[BUFFER_SIZE];
    snprintf(nameFifo, sizeof(nameFifo), "FIFOCLIENTE%d", pid);
    int fd = open(nameFifo, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao abrir FIFO do feed para enviar mensagens persistentes");
    } else {
        for (int i = 0; i < topicoAtual->numMensagensPersistentes; i++) {
            MensagemPersistente *mensagem = &topicoAtual->mensagens[i];

            char mensagemFormatada[BUFFER_SIZE];
            snprintf(mensagemFormatada, sizeof(mensagemFormatada), "[%s] %s", topicoAtual->nome, mensagem->corpo);
            write(fd, mensagemFormatada, strlen(mensagemFormatada) + 1);
        }
        close(fd);
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void cancelarSubscricaoNoManager(const char *nomeTopico, int pid) {
    pthread_mutex_lock(&mutexFeeds);

    // Procura o tópico
    int topicoEncontrado = -1;
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, nomeTopico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        printf("Tópico '%s' não encontrado para cancelar subscrição.\n", nomeTopico);
        pthread_mutex_unlock(&mutexFeeds);
        return;
    }

    Topico *topico = &topicos[topicoEncontrado];

    // Procura o PID na lista de subscritores
    int pidEncontrado = -1;
    for (int i = 0; i < topico->numSubscritores; i++) {
        if (topico->subscritores[i] == pid) {
            pidEncontrado = i;
            break;
        }
    }

    if (pidEncontrado != -1) {
        for (int i = pidEncontrado; i < topico->numSubscritores - 1; i++) {
            topico->subscritores[i] = topico->subscritores[i + 1];
        }
        topico->numSubscritores--;
        printf("Feed %d cancelou a subscrição do tópico '%s'.\n", pid, nomeTopico);
    } else {
        printf("Feed %d não estava subscrito no tópico '%s'.\n", pid, nomeTopico);
    }

    // Remove o tópico se não houver subscritores e mensagens persistentes
    if (topico->numSubscritores == 0 && topico->numMensagensPersistentes == 0) {
        printf("Tópico '%s' não possui mais subscritores ou mensagens. Removendo...\n", nomeTopico);
        for (int j = topicoEncontrado; j < numTopicos - 1; j++) { // Remove o tópico da lista de tópicos
            topicos[j] = topicos[j + 1];
        }
        numTopicos--;
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void bloquearTopico(const char *nomeTopico) {
    pthread_mutex_lock(&mutexFeeds);

    int topicoEncontrado = -1;
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, nomeTopico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        printf("Tópico '%s' não encontrado.\n", nomeTopico);
    } else {
        topicos[topicoEncontrado].bloqueado = 1;
        printf("Tópico '%s' foi bloqueado.\n", nomeTopico);
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void desbloquearTopico(const char *nomeTopico) {
    pthread_mutex_lock(&mutexFeeds);

    int topicoEncontrado = -1;
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, nomeTopico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        printf("Tópico '%s' não encontrado.\n", nomeTopico);
    } else {
        topicos[topicoEncontrado].bloqueado = 0;
        printf("Tópico '%s' foi desbloqueado.\n", nomeTopico);
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void listarMensagensTopico(const char *nomeTopico) {
    pthread_mutex_lock(&mutexFeeds);

    // Procura o tópico
    int topicoEncontrado = -1;
    for (int i = 0; i < numTopicos; i++) {
        if (strcmp(topicos[i].nome, nomeTopico) == 0) {
            topicoEncontrado = i;
            break;
        }
    }

    if (topicoEncontrado == -1) {
        printf("Tópico '%s' não encontrado.\n", nomeTopico);
        pthread_mutex_unlock(&mutexFeeds);
        return;
    }

    Topico *topico = &topicos[topicoEncontrado];

    // Lista as mensagens persistentes
    printf("Mensagens persistentes no tópico '%s':\n", nomeTopico);
    if (topico->numMensagensPersistentes == 0) {
        printf("Nenhuma mensagem persistente no tópico '%s'.\n", nomeTopico);
    } else {
        for (int i = 0; i < topico->numMensagensPersistentes; i++) {
            printf("- [%d segundos restantes] %s\n",
                   topico->mensagens[i].tempoRestante,
                   topico->mensagens[i].corpo);
        }
    }

    pthread_mutex_unlock(&mutexFeeds);
}

void guardarMensagensPersistentes() {
    // Obtém o nome do arquivo a partir da variável de ambiente
    const char *filename = getenv("MSG_FICH");
    if (filename == NULL) {
        printf("Erro: Variável de ambiente MSG_FICH não definida.\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para salvaguarda");
        return;
    }

    pthread_mutex_lock(&mutexFeeds);

    // Itera sobre os tópicos e salva as mensagens persistentes
    for (int i = 0; i < numTopicos; i++) {
        Topico *topico = &topicos[i];
        for (int j = 0; j < topico->numMensagensPersistentes; j++) {
            MensagemPersistente *mensagem = &topico->mensagens[j];
            fprintf(file, "%s %d %s\n",
                    topico->nome,
                    mensagem->tempoRestante,
                    mensagem->corpo);
        }
    }

    pthread_mutex_unlock(&mutexFeeds);
    fclose(file);

    printf("Mensagens persistentes salvaguardadas em %s.\n", filename);
}

void carregarMensagensPersistentes() {
    // Obtém o nome do arquivo a partir da variável de ambiente
    const char *filename = getenv("MSG_FICH");
    if (filename == NULL) {
        printf("Erro: Variável de ambiente MSG_FICH não definida.\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Aviso: Nenhum arquivo de mensagens persistentes encontrado");
        return;
    }

    pthread_mutex_lock(&mutexFeeds);

    char linha[BUFFER_SIZE];
    while (fgets(linha, sizeof(linha), file)) {
        char topico[BUFFER_SIZE], corpo[MAX_TAM_MSG];
        int tempoRestante;

        // Parse da linha, ignorando o autor
        if (sscanf(linha, "%s %d %[^\n]", topico, &tempoRestante, corpo) != 3) {
            printf("Linha inválida no arquivo: %s", linha);
            continue;
        }

        // Procura o tópico ou cria um novo
        int topicoEncontrado = -1;
        for (int i = 0; i < numTopicos; i++) {
            if (strcmp(topicos[i].nome, topico) == 0) {
                topicoEncontrado = i;
                break;
            }
        }

        if (topicoEncontrado == -1) {
            if (numTopicos < MAX_TOPICOS) {
                topicoEncontrado = numTopicos++;
                strncpy(topicos[topicoEncontrado].nome, topico, sizeof(topicos[topicoEncontrado].nome) - 1);
                topicos[topicoEncontrado].numMensagensPersistentes = 0;
                topicos[topicoEncontrado].bloqueado = 0;
                topicos[topicoEncontrado].numSubscritores = 0;
            } else {
                printf("Número máximo de tópicos atingido. Ignorando mensagem.\n");
                continue;
            }
        }

        // Adiciona a mensagem ao tópico
        Topico *topicoAtual = &topicos[topicoEncontrado];
        if (topicoAtual->numMensagensPersistentes < MAX_MENSAGENS) {
            MensagemPersistente *novaMensagem = &topicoAtual->mensagens[topicoAtual->numMensagensPersistentes++];
            strncpy(novaMensagem->corpo, corpo, sizeof(novaMensagem->corpo) - 1);
            novaMensagem->tempoRestante = tempoRestante;
            novaMensagem->tempo_vida = tempoRestante; // assumir que é igual só para facilitar
        }else {
            printf("Número máximo de mensagens persistentes atingido para o tópico '%s'. Mensagem ignorada.\n", topico);
        }
    }

    pthread_mutex_unlock(&mutexFeeds);
    fclose(file);

    printf("Mensagens persistentes carregadas de %s.\n", filename);
}



// Função de thread para ler dados do FIFO de entrada (do feed)
void *threadRecebe(void *arg) {
    int fd_recebe = open(SERVER_FIFO_RECEBE, O_RDONLY | O_NONBLOCK);
    MSG mensagem;

    if (fd_recebe == -1) {
        perror("Erro ao abrir FIFO principal");
        pthread_exit(NULL);
    }

    while (managerLigado) {
        if (read(fd_recebe, &mensagem, sizeof(MSG)) > 0) {
            if (mensagem.val == 0) {  // Novo feed se conectando
                pthread_mutex_lock(&mutexUtilizadores);
                if (verificaUser(mensagem.nome)) {
                    printf("\nUtilizador %s já conectado.\n", mensagem.nome);
                } else {
                    adicionaUtilizador(mensagem.nome);
                    adicionarFeed(mensagem.pid);  // Adiciona o feed e cria o FIFO
                    printf("\nUtilizador %s conectado (PID: %d).\n", mensagem.nome, mensagem.pid);
                }
                pthread_mutex_unlock(&mutexUtilizadores);
            } else if (mensagem.val == -1) {  // Feed se desconectando
                pthread_mutex_lock(&mutexUtilizadores);
                removeUtilizador(mensagem.nome);
                printf("\nUtilizador %s desconectado.\n", mensagem.nome);
                pthread_mutex_unlock(&mutexUtilizadores);
            } else if (mensagem.val == 2) {  // Comando 'topics'
                // Nome do FIFO exclusivo para o feed que enviou o comando
                char nameFifo[BUFFER_SIZE];
                snprintf(nameFifo, sizeof(nameFifo), "FIFOCLIENTE%d", mensagem.pid);

                int fd_envia = open(nameFifo, O_WRONLY);
                if (fd_envia == -1) {
                    perror("Erro ao abrir FIFO exclusivo para enviar tópicos");
                    continue;
                }

                // Chama a função listarTopicos passando o descritor do FIFO
                listarTopicos(fd_envia);
                close(fd_envia);
            }
            else if(mensagem.val == 3){
                processarMensagem(mensagem.comando, mensagem.pid);
            }
            else if (mensagem.val == 4) {  // Comando subscribe
                processarSubscribe(mensagem.comando, mensagem.pid);
            }
            else if (mensagem.val == 5){
                char topico[BUFFER_SIZE];
                if (sscanf(mensagem.comando, "unsubscribe %s", topico) == 1) {
                    cancelarSubscricaoNoManager(topico, mensagem.pid);
                } else {
                    printf("Topico inválido %d.\n", mensagem.pid);
                }
            }
        }
    }

    close(fd_recebe);
    pthread_exit(NULL);
}


void *threadGerenciaMensagens(void *arg) {
    while (managerLigado) {
        pthread_mutex_lock(&mutexFeeds);

        for (int i = 0; i < numTopicos; i++) {
            Topico *topico = &topicos[i];

            for (int j = 0; j < topico->numMensagensPersistentes; j++) {
                MensagemPersistente *mensagem = &topico->mensagens[j];
                mensagem->tempoRestante--;

                if (mensagem->tempoRestante <= 0) {
                    printf("Mensagem expirada no tópico '%s': %s\n", topico->nome, mensagem->corpo);

                    // Remove a mensagem persistente
                    for (int k = j; k < topico->numMensagensPersistentes - 1; k++) {
                        topico->mensagens[k] = topico->mensagens[k + 1];
                    }
                    topico->numMensagensPersistentes--;
                    j--; // Ajusta o índice para a próxima iteração
                }
            }
        }

        pthread_mutex_unlock(&mutexFeeds);
        sleep(1); // Checa as mensagens a cada segundo
    }
    pthread_exit(NULL);
}

void tratarCtrlC(int signum) {
    printf("\nA encerrar o manager...\n");

    guardarMensagensPersistentes(); 
    unlink(SERVER_FIFO_RECEBE);     
    enviarMensagemParaFeeds("Encerrando a plataforma. Todos os feeds devem terminar.");
    exit(0); 
}

int main() {
    signal(SIGINT, tratarCtrlC);

    pthread_t tRecebe, tGerenciaMensagens;
    char comando[BUFFER_SIZE];

    memset(utilizadoresOnline, 0, sizeof(utilizadoresOnline));

    mkfifo(SERVER_FIFO_RECEBE, 0666);

    printf("Manager iniciado. À espera de conexões...\n");

    pthread_create(&tRecebe, NULL, threadRecebe, NULL);
    pthread_create(&tGerenciaMensagens, NULL, threadGerenciaMensagens, NULL);

    carregarMensagensPersistentes();

    while (managerLigado) {
        printf("> ");
        if (fgets(comando, BUFFER_SIZE, stdin)) {
            comando[strcspn(comando, "\n")] = '\0';

            if (strcmp(comando, "users") == 0) {
                pthread_mutex_lock(&mutexUtilizadores);
                printf("Utilizadores online:\n");
                for (int i = 0; i < MAX_UTILIZADORES; i++) {
                    if (utilizadoresOnline[i][0] != '\0') {
                        printf("- %s\n", utilizadoresOnline[i]);
                    }
                }
                pthread_mutex_unlock(&mutexUtilizadores);
                //listarUtilizadoresOnline();
            } else if (strcmp(comando, "close") == 0) {
                printf("Encerrando o manager...\n");
                guardarMensagensPersistentes();
                enviarMensagemParaFeeds("Encerrando a plataforma. Todos os feeds devem terminar.");
                managerLigado = 0; // Encerra o manager
                break;
            } else if(strncmp(comando, "remove", 6) == 0){
                char username[10];
                if (sscanf(comando + 7, "%s", username) == 1) {
                    removeUtilizador(username);
                }else{
                    printf("Comando inválido. Use: remove <username>\n");
                }
            } else if (strncmp(comando, "lock", 4) == 0) {
                char topico[BUFFER_SIZE];
                if (sscanf(comando + 5, "%s", topico) == 1) {
                    bloquearTopico(topico);
                } else {
                    printf("Comando inválido. Use: lock <topico>\n");
                }
            } else if (strncmp(comando, "unlock", 6) == 0) {
                char topico[BUFFER_SIZE];
                if (sscanf(comando + 7, "%s", topico) == 1) {
                    desbloquearTopico(topico);
                } else {
                    printf("Comando inválido. Use: unlock <topico>\n");
                }
            } else if (strncmp(comando, "show", 4) == 0) {
                char topico[BUFFER_SIZE];
                if (sscanf(comando + 5, "%s", topico) == 1) {
                    listarMensagensTopico(topico);
                } else {
                    printf("Comando inválido. Use: show <topico>\n");
                }
            } else {
                printf("Comando inválido\n");
            }
        }
    }

    pthread_join(tRecebe, NULL);
    pthread_join(tGerenciaMensagens, NULL);

    unlink(SERVER_FIFO_RECEBE);

    return 0;
}
