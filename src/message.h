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

// Cria uma nova mensagem
Message *create_message(int sender_id, int room_id, const char *content);

// Envia uma mensagem para uma sala (broadcast)
void broadcast_message(int room_id, Message *message);

// Envia mensagem para todos os clientes da sala
void send_message_to_clients(Message *message);

#endif
