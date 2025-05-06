#include <pthread.h>
#include <stdlib.h>
#include "client.h"

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

    Client new_client;

    new_client.socket_id = *client_id;
    clients[clients_count] = new_client;
    clients_count++;

    pthread_mutex_unlock(&clients_mutex);

    return &new_client;
};

// TODO implement
void remove_client(int *client_id) {};

// Should be executed in a separate thread to simulate different clients
// connecting to the server.
void *create_fake_client()
{
    int client_id = rand();

    add_client(&client_id);
};