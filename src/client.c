#include <pthread.h>

#define MAX_CLIENTS 10

typedef struct
{
    int socket;
    pthread_t thread;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// TODO implement
void *handle_client(void *arg);

Client *get_client(int *client_socket)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].socket == *client_socket)
        {
            return &clients[i];
        }
    }
}

Client *add_client(int *client_socket)
{
    // TODO lock and unlock mutex here?
    pthread_mutex_lock(&clients_mutex);

    Client *new_client;

    new_client->socket = *client_socket;
    // TODO thread id should be initialized as current clients length
    // new_client->thread = 0;

    pthread_create(new_client->thread, NULL, handle_client, &new_client);

    pthread_mutex_unlock(&clients_mutex);
};

void remove_client(int *client_socket)
{
    // TODO lock and unlock mutex here?
    pthread_mutex_lock(&clients_mutex);

    Client *client = get_client(client_socket);
    pthread_cancel(client->thread);
    close(client->socket);

    pthread_mutex_unlock(&clients_mutex);
};