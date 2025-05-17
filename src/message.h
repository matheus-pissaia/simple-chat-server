#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>
#include <pthread.h>
#include "client.h"

#define MAX_MESSAGE_SIZE 1024

typedef struct
{
    int sender_id;                  // ID do cliente que enviou a mensagem
    int room_id;                    // ID da sala onde a mensagem foi enviada
    char content[MAX_MESSAGE_SIZE]; // Conteúdo da mensagem
    time_t timestamp;               // Timestamp de quando a mensagem foi enviada
} Message;

// Fila de mensagens para implementação do padrão produtor/consumidor
typedef struct
{
    Message **messages;       // Array de ponteiros para mensagens
    int capacity;             // Capacidade máxima da fila
    int size;                 // Tamanho atual da fila
    int front;                // Índice do primeiro elemento
    int rear;                 // Índice do último elemento
    pthread_mutex_t mutex;    // Mutex para sincronização
    pthread_cond_t not_empty; // Condição para fila não vazia
    pthread_cond_t not_full;  // Condição para fila não cheia
} MessageQueue;

// Inicializa uma nova fila de mensagens
MessageQueue *message_queue_create(int capacity);

// Adiciona uma mensagem à fila (produtor)
int message_queue_enqueue(MessageQueue *queue, Message *message);

// Remove uma mensagem da fila (consumidor)
Message *message_queue_dequeue(MessageQueue *queue);

// Libera recursos da fila
void message_queue_destroy(MessageQueue *queue);

// Cria uma nova mensagem
Message *create_message(int sender_id, int room_id, const char *content);

// Envia uma mensagem para uma sala (broadcast)
void broadcast_message(int room_id, Message *message);

// Inicia o thread consumidor para processar mensagens
void start_message_consumer(int room_id);

// Inicializa as filas de mensagens para todas as salas
void init_message_queues();

#endif
