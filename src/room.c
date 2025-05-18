#include <pthread.h>
#include <stdio.h>
#include "room.h"
#include "client.h"
#include "llist.h"
#include "message.h"

Room rooms[MAX_ROOMS];

void init_rooms()
{
    printf("\n===== INICIALIZAÇÃO DO SERVIDOR =====\n");
    printf("Criando %d salas de chat...\n", MAX_ROOMS);

    for (int i = 0; i < MAX_ROOMS; i++)
    {
        rooms[i].id = i;
        rooms[i].clients = llist_create();
        rooms[i].client_count = 0;
        pthread_mutex_init(&rooms[i].mutex, NULL);
        pthread_cond_init(&rooms[i].cond, NULL);

        printf("✓ Sala #%d criada e inicializada\n", i);
    }

    printf("Todas as salas estão prontas para receber clientes\n");
    printf("=======================================\n\n");
}

Room *get_room(const int *room_id)
{
    for (int i = 0; i < MAX_ROOMS; i++)
        if (rooms[i].id == *room_id)
            return &rooms[i];

    return NULL;
}

int join_room(const int *room_id, Client *client)
{
    Room *room = get_room(room_id);

    if (room != NULL && room->client_count < MAX_CLIENTS_PER_ROOM)
    {
        pthread_mutex_lock(&room->mutex);

        llist_push(room->clients, client);
        room->client_count++;

        pthread_mutex_unlock(&room->mutex);

        return 0;
    }

    return -1;
}

void leave_room(const int *room_id, Client *client)
{
    Room *room = get_room(room_id);

    if (room == NULL)
        return;

    pthread_mutex_lock(&room->mutex);

    int status = llist_remove(room->clients, client);

    if (status == 0)
        room->client_count--;

    pthread_mutex_unlock(&room->mutex);
}
