#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "room.h"
#include "client.h"
#include "thread_pool.h"
#include "message.h"

#define NUM_CLIENTS 5           // Número de clientes falsos a serem criados (reduzido para clareza)
#define CLIENT_CREATION_DELAY 2 // Delay em segundos entre a criação de clientes

int main()
{
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Limpa a tela para melhor visualização (apenas para sistemas Unix)
    system("clear");

    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    CHAT SERVER SIMULATOR                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");

    // Inicializa o pool de threads
    thread_pool_init();

    // Inicializa as salas
    init_rooms();

    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     SIMULAÇÃO DE CLIENTES                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("Iniciando simulação com %d clientes (delay de %d segundo(s) entre conexões)\n\n",
           NUM_CLIENTS, CLIENT_CREATION_DELAY);

    // Cria threads para simular clientes com um intervalo para melhor visualização
    for (int i = 0; i < NUM_CLIENTS; i++)
    {
        thread_pool_add_task((void *)create_fake_client, NULL);

        // Pausa entre a criação de clientes para melhor visualização
        if (i < NUM_CLIENTS - 1)
            sleep(CLIENT_CREATION_DELAY);
    }

    // Aguarda a conclusão de todas as tarefas antes de encerrar a simulação
    thread_pool_wait();

    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    SIMULAÇÃO CONCLUÍDA                       ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("Todas as atividades dos clientes foram finalizadas.\n");

    // Neste ponto, em uma implementação real, esperaríamos por novos clientes,
    // mas para esta simulação, vamos encerrar.
    return 0;
}
