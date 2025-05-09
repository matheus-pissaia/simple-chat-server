# Simple Chat Server

TODO add description

## How to run

1. Compile the project running the `make` command in the root folder:

```bash
make
```

2. Execute the server from generated binaries:

```bash
./bin/a.out
```

# TODOs

@dev

We need to create a dedicated thread to handle each client "on connect" - For now, we will focus on implementing core functionalities, so the client connection and all actions should be simulated. After implementing all core features, we can create a real server with sockets.

- [ ] Generate rooms.
- [ ] Generate fake clients.
- [ ] Handle client join room.
- [ ] Handle client leave room.
- [ ] Handle client send message inside room.
- [ ] Handle client receive message inside room.

## Design Patterns

Some ideas of design patterns that we could implement for this project:

### Thread Pool

Instead of creating a thread for each client, we keep a fixed pool of threads ready to process connections or message queues.

### Producer / Consumer

In each room, producer threads queue messages in a buffer, and consumer threads remove them to send (broadcast). In this way we separate who generates the message (client) from who sends it, decoupling socket reading and broadcast writing.

### Controlled Suspension

Suspend consumer thread when the message queue is empty to avoid CPU spinlock, using something like:

```C
pthread_cond_wait(&room.cond, &room.mutex)
```

And wake up when there's a new message:

```C
pthread_cond_signal(&room.cond)
```
