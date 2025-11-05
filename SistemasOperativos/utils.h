#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE 256
#define MAX_MENSAGENS 5
#define MAX_TAM_MSG 300
#define MAX_TOPICOS 20
#define MAX_UTILIZADORES 10


// Mensagens entre feed e manager (e não persistentes)
typedef struct {
    char nome[10];
    int pid;
    int val;
    char comando[BUFFER_SIZE];
} MSG;

// Estrutura para gerir FIFOs e threads
typedef struct {
    char nameFifo[BUFFER_SIZE];
    pid_t pid;
    int trinco;
    int fd;
} TDADOS;

typedef struct {
    char autor[10];              
    char corpo[MAX_TAM_MSG];   
    int tempo_vida;              
    int tempoRestante;         
} MensagemPersistente;

typedef struct {
    char nome[20];               
    MensagemPersistente mensagens[MAX_MENSAGENS]; // Lista de mensagens persistentes
    int numMensagensPersistentes;
    int bloqueado;               // 0 = desbloqueado, 1 = bloqueado
    int numSubscritores; 
    int subscritores[MAX_UTILIZADORES]; // PIDs     
} Topico;

#endif // UTILS_H
