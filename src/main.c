#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.c"
#include "message.c"

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // server_socket = socket(...);
    // bind(server_socket, ...);
    // listen(server_socket, ...);

    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

        add_client(&client_socket);
    }
}
