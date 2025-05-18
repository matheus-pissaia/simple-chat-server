#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "room.h"
#include "message.h"

#define MAX_CLIENTS 10
#define MAX_FAKE_MESSAGES 5 // Número máximo de mensagens falsas enviadas por um cliente

// Lista de mensagens de exemplo para simular conversa
const char *fake_messages[] = {
    "Olá! Como vocês estão?",
    "Alguém pode me ajudar com uma dúvida?",
    "Estou gostando muito deste chat!",
    "Qual o assunto principal desta sala?",
    "Tenho que ir embora, até mais!",
    "Alguém sabe como resolver este problema?",
    "Isso é apenas um teste do sistema.",
    "Estou tendo problemas de conexão...",
    "Quanta gente online hoje!",
    "Bom dia a todos!",
};

#define NUM_FAKE_MESSAGES (sizeof(fake_messages) / sizeof(fake_messages[0]))

int clients_count = 0;
Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Manipula a conexão de um cliente
void *handle_client(void *arg)
{
    Client *client = (Client *)arg;
    printf("Manipulando cliente %d\n", client->socket_id);

    // Aqui ficaria socket para servidor real

    return NULL;
}

Client *get_client(int *client_id)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i].socket_id == *client_id)
            return &clients[i];

    return NULL;
}

Client *add_client(int *client_id)
{
    pthread_mutex_lock(&clients_mutex);

    if (clients_count >= MAX_CLIENTS)
    {
        pthread_mutex_unlock(&clients_mutex);
        return NULL; // Não há espaço para mais clientes
    }

    Client *client = &clients[clients_count];
    client->socket_id = *client_id;
    client->current_room = -1; // Inicialmente, o cliente não está em nenhuma sala
    clients_count++;

    pthread_mutex_unlock(&clients_mutex);

    return client;
};

void remove_client(int *client_id)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < clients_count; i++)
    {
        if (clients[i].socket_id == *client_id)
        {
            if (clients[i].current_room >= 0)
            {
                int room_id = clients[i].current_room;
                leave_room(&room_id, &clients[i]);
            }

            // Remove o cliente movendo os elementos posteriores
            for (int j = i; j < clients_count - 1; j++)
            {
                clients[j] = clients[j + 1];
            }
            clients_count--;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

// Simula o envio de uma mensagem de um cliente
void send_client_message(Client *client, const char *content)
{
    if (client->current_room < 0)
    {
        printf("Cliente %d não está em nenhuma sala para enviar mensagem.\n", client->socket_id);
        return;
    }

    // Cria uma nova mensagem
    Message *message = create_message(client->socket_id, client->current_room, content);

    // Envia a mensagem para a fila (produtor)
    broadcast_message(client->current_room, message);

    printf("[SALA #%d] Cliente %d: \"%s\"\n",
           client->current_room, client->socket_id, content);
}

// Simula a chegada de um cliente e suas ações no servidor
void *create_fake_client()
{
    // Gera um ID aleatório para o cliente
    int client_id = rand() % 1000;

    // Adiciona o cliente ao servidor
    Client *client = add_client(&client_id);
    if (client == NULL)
    {
        printf("❌ ERRO: Não foi possível adicionar o cliente %d (servidor lotado).\n", client_id);
        return NULL;
    }

    printf("\n🔌 CONEXÃO: Cliente %d conectou-se ao servidor\n", client_id);

    // Escolhe uma sala aleatória
    int room_id = rand() % MAX_ROOMS;

    // Cliente tenta entrar na sala
    if (join_room(&room_id, client) == 0)
    {
        printf("ENTRADA: Cliente %d entrou na Sala #%d\n", client_id, room_id);
        client->current_room = room_id;

        // Simula o envio de várias mensagens com delays
        int num_messages = rand() % MAX_FAKE_MESSAGES + 1;
        printf("   Cliente %d enviará %d mensagem(s) na Sala #%d\n", client_id, num_messages, room_id);

        for (int i = 0; i < num_messages; i++)
        {
            // Escolhe uma mensagem aleatória
            const char *message_content = fake_messages[rand() % NUM_FAKE_MESSAGES];

            // Simula o delay na digitação
            sleep(rand() % 3 + 1);

            // Envia a mensagem
            send_client_message(client, message_content);
        }

        // Cliente permanece na sala por um tempo
        int stay_time = rand() % 3 + 1;
        sleep(stay_time);

        // Cliente deixa a sala
        leave_room(&room_id, client);
        printf("SAÍDA: Cliente %d saiu da Sala #%d após %d segundo(s)\n", client_id, room_id, stay_time);
        client->current_room = -1;
    }
    else
    {
        printf("❌ ERRO: Cliente %d não conseguiu entrar na Sala #%d (lotada)\n", client_id, room_id);
    }

    // Remove o cliente após um tempo de inatividade
    sleep(1);
    remove_client(&client_id);
    printf("DESCONEXÃO: Cliente %d desconectou do servidor\n", client_id);

    return NULL;
};
