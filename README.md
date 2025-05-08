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
