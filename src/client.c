#include <pthread.h>
#include <stdlib.h>
#include "client.h"
#include "room.h"



#define MAX_CLIENTS 10

int clients_count = 0;
Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// TODO implement
void *handle_client(void *arg) {};

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

// TODO implement
void remove_client(int *client_id) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < clients_count; i++) {
        if (clients[i].socket_id == *client_id) {
            // Remove o cliente movendo os elementos posteriores
            for (int j = i; j < clients_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            clients_count--;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

// Should be executed in a separate thread to simulate different clients
// connecting to the server.
void *create_fake_client(void *arg)
{
    int client_id = rand() % 1000;

    Client *client = add_client(&client_id);
    if (client == NULL)
    {
        printf("Erro ao adicionar cliente.\n");
        return NULL;
    }

    int room_id = rand() % MAX_ROOMS;

    if (join_room(&room_id, client) == 0) {
        printf("Cliente %d entrou na sala %d.\n", client_id, room_id);
        client->current_room = room_id;
    } else {
        printf("Cliente %d não conseguiu entrar na sala %d (cheia ou inexistente).\n", client_id, room_id);
    }

    // Simula uma ação (ex: enviar mensagem)
    sleep(1);  // Espera 1 segundo
    printf("Cliente %d na sala %d: 'Olá, mundo!'\n", client_id, room_id);

    // Sai da sala após um tempo
    leave_room(&room_id, client);
    printf("Cliente %d saiu da sala %d.\n", client_id, room_id);

    // Remove o cliente
    remove_client(&client_id);
    return NULL;

    
};