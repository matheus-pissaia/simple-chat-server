# Simple Chat Server

Um servidor de chat simples implementado em C que utiliza thread pool para lidar com múltiplos clientes simultâneos e tarefas para gerenciamento de mensagens. Este projeto demonstra conceitos importantes de programação concorrente e design patterns.

## Como Executar

Execute o comando abaixo no seu shell para compilar e rodar o servidor:

```bash
bash run.sh
```

## Arquitetura do Sistema

O sistema é composto pelos seguintes componentes principais:

### Salas (Rooms)

-   Cada sala pode conter múltiplos clientes
-   As salas possuem uma fila de mensagens
-   Implementação em: `room.c` e `room.h`

### Clientes (Clients)

-   Simulação de clientes que entram/saem de salas e enviam mensagens
-   Implementação em: `client.c` e `client.h`

### Mensagens (Messages)

-   Sistema de mensagens com timestamp e informações do remetente
-   Implementação de broadcast de mensagem para todos os clientes da sala
-   Implementação em: `message.c` e `message.h`

### Pool de Threads

-   Gerenciamento eficiente de threads para processamento de tarefas
-   Implementação em: `thread_pool.c` e `thread_pool.h`

### Lista Encadeada (Linked List)

-   Estrutura de dados para armazenar clientes nas salas
-   Implementação em: `llist.c` e `llist.h`

## Design Patterns Implementados

### Thread Pool

Em vez de criar uma thread para cada cliente, mantemos um conjunto fixo de threads pronto para processar conexões ou filas de mensagens. Isto melhora o desempenho e gerencia os recursos do sistema de forma mais eficiente.

### Suspensão Controlada

Suspendemos a thread consumidora quando a fila de tarefas está vazia para evitar CPU spinlock.

## Melhorias a serem feitas

-   Implementação de sockets para clientes reais
-   Interface de usuário (CLI ou GUI)
-   Persistência de mensagens
