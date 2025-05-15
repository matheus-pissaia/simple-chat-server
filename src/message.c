#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "message.h"
#include "room.h"
#include "thread_pool.h"

// Cria uma fila de mensagens com capacidade definida
MessageQueue* message_queue_create(int capacity) {
    MessageQueue *queue = (MessageQueue*)malloc(sizeof(MessageQueue));
    
    queue->messages = (Message**)malloc(sizeof(Message*) * capacity);
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    
    return queue;
}

// Adiciona uma mensagem à fila (operação do produtor)
int message_queue_enqueue(MessageQueue *queue, Message *message) {
    pthread_mutex_lock(&queue->mutex);
    
    // Aguarda enquanto a fila estiver cheia
    while (queue->size == queue->capacity) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }
    
    // Incrementa rear e adiciona a mensagem
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->messages[queue->rear] = message;
    queue->size++;
    
    // Sinaliza que a fila não está mais vazia
    pthread_cond_signal(&queue->not_empty);
    
    pthread_mutex_unlock(&queue->mutex);
    return 0;
}

// Remove uma mensagem da fila (operação do consumidor)
Message* message_queue_dequeue(MessageQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    
    // Aguarda enquanto a fila estiver vazia
    while (queue->size == 0) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    
    // Remove a mensagem da frente da fila
    Message *message = queue->messages[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    // Sinaliza que a fila não está mais cheia
    pthread_cond_signal(&queue->not_full);
    
    pthread_mutex_unlock(&queue->mutex);
    return message;
}

// Libera recursos da fila
void message_queue_destroy(MessageQueue *queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
    free(queue->messages);
    free(queue);
}

// Cria uma nova mensagem
Message* create_message(int sender_id, int room_id, const char *content) {
    Message *message = (Message*)malloc(sizeof(Message));
    
    message->sender_id = sender_id;
    message->room_id = room_id;
    strncpy(message->content, content, MAX_MESSAGE_SIZE - 1);
    message->content[MAX_MESSAGE_SIZE - 1] = '\0';  // Garante terminação NULL
    message->timestamp = time(NULL);
    
    return message;
}

// Array de filas de mensagens para cada sala
static MessageQueue *room_message_queues[MAX_ROOMS];

// Inicializa as filas de mensagens para todas as salas
void init_message_queues() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        room_message_queues[i] = message_queue_create(100);  // 100 mensagens por sala
    }
}

// Função que envia uma mensagem para a fila da sala (produtor)
void broadcast_message(int room_id, Message *message) {
    if (room_id < 0 || room_id >= MAX_ROOMS) {
        printf("❌ ERRO: ID de sala inválido: %d\n", room_id);
        return;
    }
    
    // Adiciona mensagem à fila da sala
    message_queue_enqueue(room_message_queues[room_id], message);
    // Log removido para evitar duplicação de informação
}

// Função para enviar mensagem para todos os clientes da sala
void send_message_to_clients(Message *message) {
    Room *room = get_room(&message->room_id);
    if (!room) {
        printf("❌ ERRO: Sala #%d não encontrada\n", message->room_id);
        return;
    }
    
    pthread_mutex_lock(&room->mutex);
    
    // Obter a lista de clientes na sala
    LinkedList *clients = room->clients;
    Node *current = clients->head;
    int recipients = 0;
    
    // Enviar mensagem para cada cliente
    while (current != NULL) {
        Client *client = (Client*)current->data;
        
        // Pula o remetente para não enviar a mensagem de volta
        if (client->socket_id != message->sender_id) {
            // Simula o envio da mensagem (em uma implementação real, enviaríamos via socket)
            // send(client->socket_id, message->content, strlen(message->content), 0);
            recipients++;
        }
        
        current = current->next;
    }
    
    if (recipients > 0) {
        printf("📥 [SALA #%d] Mensagem de Cliente %d entregue a %d cliente(s)\n", 
               message->room_id, message->sender_id, recipients);
    } else {
        printf("   [SALA #%d] Nenhum outro cliente para receber a mensagem\n", message->room_id);
    }
    
    pthread_mutex_unlock(&room->mutex);
}

// Função executada pelo consumidor para processar mensagens da fila
void* message_consumer(void *arg) {
    int room_id = *((int*)arg);
    free(arg);
    
    printf("🔄 Iniciando consumidor de mensagens para Sala #%d\n", room_id);
    
    while (1) {
        // Recupera a próxima mensagem da fila
        Message *message = message_queue_dequeue(room_message_queues[room_id]);
        
        // Processa a mensagem (envia para todos os clientes na sala)
        send_message_to_clients(message);
        
        // Libera a mensagem após processá-la
        free(message);
    }
    
    return NULL;
}

// Inicia o thread consumidor para processar mensagens de uma sala
void start_message_consumer(int room_id) {
    int *room_id_ptr = (int*)malloc(sizeof(int));
    *room_id_ptr = room_id;
    
    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, message_consumer, room_id_ptr);
    pthread_detach(consumer_thread);
}