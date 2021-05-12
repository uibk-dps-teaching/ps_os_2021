# Exercise Sheet 9

In this exercise sheet you will again work with threads. In particular, you will
explore how threads can be used for handling concurrent, blocking communication
between multiple processes.

To this end, you will implement a multi-threaded chat application consisting of a
single server and multiple clients that communicate over sockets.
In its basic form, the application is similar to [task 3 of exercise sheet
4](../exercise04/README.md#Task-3).
However this time you will be using TCP/IP sockets instead of UNIX sockets --
fortunately they work almost exactly the same way. You may want to begin by
skimming through `man 7 ip`. Here is a quick recap:

1. To create a TCP/IP socket, use the `socket()` function with `domain = AF_INET`
   and `type = SOCK_STREAM`.
2. Next, fill out the `sockaddr_in` struct to inform the socket where to bind to
   (on the server) / connect to (on the client), like so:

   ```c
   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   addr.sin_addr.s_addr = htonl(INADDR_ANY); // on server
   addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // on client
   ```

   Notice that you need to specify different IP addresses for the server and
   client.

3. On the server `bind()` the socket to the address, and `listen()` for incoming
   connections. On the client, simply `connect()` to the server.
4. Server only: Use `accept()` to accept incoming connections, and to receive a
   unique communication socket for each client.

## Task 1

Begin by implementing a basic client/server application, where multiple clients
can dynamically connect and disconnect and send messages to the server. The
sever then prints the received messages to the console, similar to the "one-way
shoutbox" of [task 1 of exercise sheet 4](../exercise04/README.md#Task-1).
Your solution should consist of two executables, one for the server, and
another for the client.

The server:

- Receives as a single command line argument the `port` it should listen on.
- After creating and binding the socket as described above, the server should
  spawn a _listener thread_ for accepting incoming connections.
  - The listener thread should terminate once the `/shutdown` command has been
    received (see below). A good way of implementing this is to use the
    `pthread_cancel()` function.
- For each client that connects, a new _client thread_ is spawned:
  - The client thread continuously waits for incoming messages, and upon
    receiving them, prints them to the console, in the form `"<username>: <message>"`.
  - Additionally, a client thread should be able to detect when a client
    disconnects, and subsequently terminate.
- The server waits until the listener thread and all client threads have
  terminated, cleans up and exits.

The client:

- Receives two command line arguments: The `port` to connect to, as well as a
  username it will be identified by.
- Upon connecting, the client first sends its username to the server.
- It then continuously prompts messages from the user, and sends them to the
  server.

Your chat application should support two special commands:

- `/quit` causes a client to exit, and the server to print the message
  `"<username> disconnected."`.
- `/shutdown` informs the server to shut down, no longer accepting new
  connections. Upon receiving the command, the server prints
  `"Server is shutting down. Waiting for <N> clients to disconnect.`" where `N`
  is the number of currently connected clients. It then waits for all clients to
  have disconnected, cleans up and exits.

**Example output:**

```
TERMINAL 1                      TERMINAL 2                      TERMINAL 3

> ./server 24831
Listening on port 24831.
                                > ./client 24831 shelly
shelly connected.
                                                                > ./client 24831 bob
bob connected.
                                > hi all
shelly: hi all
                                                                > whats up?
bob: whats up?
                                > /shutdown
shelly disconnected.
Server is shutting down. Waiting for 1 clients to disconnected.
                                                                > /quit
bob disconnected.
```

**Additional notes & hints:**

- You can assume that every message sent over a socket is exactly
  `MSG_SIZE = 128` bytes long.
- As always, ensure proper synchronization for all shared data structures.
- Make sure to properly `close()` all sockets before exiting.
- `bind()` returning `EADDRINUSE` ("Address already in use"), may indicate that
  you did not properly close a socket. You can use a different port or wait a
  few seconds to circumvent this. Ultimately you should however try to find the
  root cause of the problem.
- You may assume that at most 32 clients will connect to a server.

## Task 2

Now turn you application from the "one-way shoutbox" into a real chat: Instead
of printing messages on the server, each message should be forwarded to all
other clients.

Notably, your client now needs to be able to both read user input, as well as
receive incoming messages forwarded by the server. Use threads to implement this
functionality.

Again make sure to properly synchronize access to any shared data. Don't
overthink your solution, and make use of the fact that a single socket can be
simultaneously used for reading and writing from within different threads.

**Example output (server omitted):**

```
TERMINAL 1                      TERMINAL 2                      TERMINAL 3

> ./client 24831 loui           > ./client 24831 shelly         > ./client 24831 bob
                                > hi all
shelly: hi all                                                  shelly: hi all
                                                                > whats up?
bob: whats up?                  bob: whats up?
```

## Task 3

Extend your chat application to support _whispering_ to other people. By typing
`/w <username> <message>`, the message `<message>` should only be visible to
`<username>`.

This task should not require any modifications to your client.

**Example output (server omitted):**

```
TERMINAL 1                      TERMINAL 2                      TERMINAL 3

> ./client 24831 loui           > ./client 24831 shelly         > ./client 24831 bob
                                > hi all
shelly: hi all                                                  shelly: hi all
                                > /w loui hey dude
shelly (whispers): hey dude
```
