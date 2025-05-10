#ifndef ROOM_H
#define ROOM_H

#include <pthread.h>
#include "client.h"
#include "llist.h"

#define MAX_ROOMS 10
#define MAX_CLIENTS_PER_ROOM 10

typedef struct
{
    int id;
    LinkedList *clients;
    int client_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Room;

void init_rooms();
Room *get_room(const int *room_id);
int join_room(const int *room_id, Client *client);
void leave_room(const int *room_id, Client *client);

#endif
