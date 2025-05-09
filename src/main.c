#include <stdlib.h>
#include "room.h"

#define NUM_CLIENTS 5  // Número de clientes falsos a serem criados


int main()
{
    init_rooms();

    pthread_t threads[NUM_CLIENTS];
    
    // Cria threads para simular clientes
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_create(&threads[i], NULL, create_fake_client, NULL);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Simulação concluída.\n");



    return 0;
}
