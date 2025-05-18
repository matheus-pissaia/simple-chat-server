#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "message.h"
#include "room.h"
#include "thread_pool.h"

// Cria uma nova mensagem
Message *create_message(int sender_id, int room_id, const char *content)
{
    Message *message = (Message *)malloc(sizeof(Message));

    message->sender_id = sender_id;
    message->room_id = room_id;
    strncpy(message->content, content, MAX_MESSAGE_SIZE - 1);
    message->content[MAX_MESSAGE_SIZE - 1] = '\0'; // Garante terminação NULL
    message->timestamp = time(NULL);

    return message;
}

// Função que envia uma mensagem para a fila da sala (produtor)
void broadcast_message(int room_id, Message *message)
{
    if (room_id < 0 || room_id >= MAX_ROOMS)
    {
        printf("❌ ERRO: ID de sala inválido: %d\n", room_id);
        return;
    }

    // Envia a mensagem para os clientes
    send_message_to_clients(message);
}

// Função para enviar mensagem para todos os clientes da sala
void send_message_to_clients(Message *message)
{
    Room *room = get_room(&message->room_id);
    if (!room)
    {
        printf("❌ ERRO: Sala #%d não encontrada\n", message->room_id);
        return;
    }

    pthread_mutex_lock(&room->mutex);

    // Obter a lista de clientes na sala
    LinkedList *clients = room->clients;
    Node *current = clients->head;
    int recipients = 0;

    // Enviar mensagem para cada cliente
    while (current != NULL)
    {
        Client *client = (Client *)current->data;

        // Pula o remetente para não enviar a mensagem de volta
        if (client->socket_id != message->sender_id)
        {
            // Simula o envio da mensagem (em uma implementação real, enviaríamos via socket)
            // send(client->socket_id, message->content, strlen(message->content), 0);
            recipients++;
        }

        current = current->next;
    }

    if (recipients > 0)
    {
        printf("📥 [SALA #%d] Mensagem de Cliente %d entregue a %d cliente(s)\n",
               message->room_id, message->sender_id, recipients);
    }
    else
    {
        printf("   [SALA #%d] Nenhum outro cliente para receber a mensagem\n", message->room_id);
    }

    pthread_mutex_unlock(&room->mutex);
}
