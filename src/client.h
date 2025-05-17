#ifndef CLIENT_H
#define CLIENT_H

typedef struct
{
    int socket_id;
    int current_room;
} Client;

void *handle_client(void *arg);
Client *get_client(int *client_id);
Client *add_client(int *client_id);
void remove_client(int *client_id);
void *create_fake_client();

#endif
