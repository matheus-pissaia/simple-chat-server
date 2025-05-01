#include "client.c"

#define MAX_ROOMS 10
#define MAX_CLIENTS_PER_ROOM 10

typedef struct
{
    int id;
    Client *clients[MAX_CLIENTS_PER_ROOM];
    int client_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Room;

Room rooms[MAX_ROOMS];

Room *get_room(const int *room_id)
{
    for (int i = 0; i < MAX_ROOMS; i++)
        if (rooms[i].id == room_id)
            return &rooms[i];
}

int join_room(const int *room_id, Client *client)
{
    Room *room = get_room(room_id);

    if (room->client_count < MAX_CLIENTS_PER_ROOM)
    {
        pthread_mutex_lock(&room->mutex);

        room->clients[room->client_count] = client;
        room->client_count++;

        pthread_mutex_unlock(&room->mutex);

        return 0;
    }

    return -1;
}

int leave_room(const int *room_id, Client *client)
{
    Room *room = get_room(room_id);

    pthread_mutex_lock(&room->mutex);

    for (int i = 0; i < room->client_count; i++)
        if (room->clients[i] == client)
        // TODO We should use a linked list, since this duplicates items.
        {
            room->clients[i] = room->clients[room->client_count - 1];
            room->client_count--;
            break;
        }

    pthread_mutex_unlock(&room->mutex);

    return 0;
}