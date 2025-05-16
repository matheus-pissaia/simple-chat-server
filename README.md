# Simple Chat Server

Um servidor de chat simples implementado em C que utiliza threads para lidar com múltiplos clientes simultâneos e implementa o padrão produtor/consumidor para gerenciamento de mensagens. Este projeto demonstra conceitos importantes de programação concorrente e design patterns.


## Como Executar

1. Compile o projeto executando o comando `make` na pasta raiz:

```bash
make
```

2. Execute o servidor :

```bash
./bin/a.out
```

## Arquitetura do Sistema

O sistema é composto pelos seguintes componentes principais:

### Salas (Rooms)

- Cada sala pode conter múltiplos clientes
- As salas possuem uma fila de mensagens
- Implementação em: `room.c` e `room.h`

### Clientes (Clients)

- Simulação de clientes que entram/saem de salas e enviam mensagens
- Implementação em: `client.c` e `client.h`

### Mensagens (Messages)

- Sistema de mensagens com timestamp e informações do remetente
- Implementação do padrão produtor/consumidor para processamento assíncrono
- Implementação em: `message.c` e `message.h`

### Pool de Threads

- Gerenciamento eficiente de threads para processamento de tarefas
- Implementação em: `thread_pool.c` e `thread_pool.h`

### Lista Encadeada (Linked List)

- Estrutura de dados para armazenar clientes nas salas
- Implementação em: `llist.c` e `llist.h`

## Design Patterns Implementados

### Thread Pool

Em vez de criar uma thread para cada cliente, mantemos um conjunto fixo de threads pronto para processar conexões ou filas de mensagens. Isto melhora o desempenho e gerencia os recursos do sistema de forma mais eficiente.

### Produtor / Consumidor

Em cada sala, threads produtoras enfileiram mensagens em um buffer, e threads consumidoras as removem para enviar (broadcast). Desta forma, separamos quem gera a mensagem (cliente) de quem a envia, desacoplando a leitura do socket e a escrita do broadcast.

Implementação:
- Os clientes atuam como produtores ao enviar mensagens
- Cada sala possui uma thread consumidora que processa mensagens da fila
- A fila de mensagens é sincronizada usando mutex e variáveis de condição

### Suspensão Controlada

Suspendemos a thread consumidora quando a fila de mensagens está vazia para evitar CPU spinlock.

## Melhorias

- Implementação de sockets para clientes reais
- Interface de usuário (CLI ou GUI)
- Persistência de mensagens

