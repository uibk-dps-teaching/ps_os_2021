# Exercise Sheet 4

In this exercise sheet you will practice inter-process communication with
FIFOs, pipes and UNIX sockets.

Begin by reading `man 7 pipe`.

# Task 1

In this task you will work with FIFOs, also called **named** pipes.

The goal is to write a simple "one-way chat" or "shoutbox" service, where
multiple interactive _clients_ can send messages to a _server_, which will
then print said messages to the console. Your solution should produce two
separate executables, `task1_client` and `task1_server`. There is no need to
use `fork()` in this task.

A single **server** can be started with a list of clients that may connect to it,
like so: `./task1_server shelly bob`. In this case, the two clients `shelly`
and `bob` can connect, however it should be possible to specify an arbitrary
number of clients.

The server then creates a FIFO for each client. Think about a way of
associating each FIFO with a specific client. **NOTE**: If you have problems
creating FIFOs in a certain location, make sure that your filesystem actually
supports them (this can be a problem e.g. in WSL, when working on an NTFS
drive). A good location to store FIFOs is somewhere inside `/tmp`.

The server then waits for clients to connect by opening each FIFO for
reading. You can assume that clients connect in the same order they are
specified on the command line. Once a client connects, the server prints the
message `"<client name> connected"`.

A **client** is started by also providing it with a name, for example
`./task1_client shelly`. The client will then attempt to connect to the
server by opening one of the FIFOs for writing. Once connected, the client
continuously prompts for a message to send. Each message is then written to
the FIFO. You can assume a message to be at most `PIPE_BUF` long (why is this
important?). If the message is empty, the client disconnects by closing the
FIFO and exiting.

Meanwhile, the server continuously monitors all FIFOs for incoming messages
using `poll()` (see `man poll`). Once a message is received, it is printed to
the console like so `"<client name>: <message>"`. If a client disconnects,
the server prints the message `"<client name> disconnected"`. Once all
clients are disconnected, the server cleans up the FIFOs and exits.

Here's what an example session could look like (each line represents a
different point in time, `>` indicates user input).

```
TERMINAL 1                      TERMINAL 2                  TERMINAL 3

> ./task1_server shelly bob
                                > ./task1_client shelly
shelly connected                Message:
                                                            > ./task1_client bob
bob connected                                               Message:
                                > hi all
shelly: hi all                  Message:
                                                            > whats up?
bob: whats up?                                              Message:
                                > (empty)
shelly disconnected
```

Additional notes and hints:

- You can assume that all clients are connected before handling their messages.
- Your server doesn't need to support clients re-connecting, i.e., they can
  (and must) connect exactly once.
- Your solution **must** use `poll()`, NOT `select()`.
- Make sure to use appropriate file access modes (which we discussed in
  connection to `chmod`) to create and open your FIFOs. Justify your choice.
- Your FIFOs should be **blocking**, i.e., you must NOT use `O_NONBLOCK` in
  your calls to `open()`.
- As always, make sure to properly release all allocated resources (e.g.
  FIFOs) upon exiting.
- If you want to create multiple processes side by side for testing, there
  are several different options available to you:
  1. Open multiple terminal windows / tabs
  2. Use shell jobs to switch between different processes (`&`, `fg`, `bg`,
     `jobs` and so on)
  3. Use a terminal multiplexer such as `tmux` or `screen`

# Task 2

Create a program that implements your shell's piping functionality (`|`)
between two arbitrary commands. For example, a call to `./task2 "ls -la" "grep foo"`
should list all files in the current working directory that contain the word `foo`.

To implement this behavior, your program is required to create a process that
executes the first command, and another process that executes the second
command. Communicate between both processes by using an **unnamed** pipe.

You can assume that each program and its (optional) arguments are passed
using quotes on the command line, which means that in the example above, the
full string `"ls -la"` would be available within `argv[1]`. You can use the
following function to split this string into an array of tokens (`["ls", "-la"]`):

```c
#define MAX_ARGS 8
void parse_args(char* str, char* (*result)[MAX_ARGS]) {
	memset(*result, 0, sizeof(char*) * MAX_ARGS);
	for(int i = 0; i < MAX_ARGS - 1; ++i, str = NULL) {
		(*result)[i] = strtok(str, " ");
	}
}
// Usage example:
char* args[MAX_ARGS];
parse_args(argv[1], &args);
```

This assignment **requires** that you explicitly use `fork()`, `pipe()`, and
`dup()/dup2()`. You can use the `exec*` family of functions to execute the
two commands (see `man exec`). Try to find an appropriate variant of `exec*`
for use with the results of `parse_args()`. Also make sure you understand
what a call to the `exec*` functions actually _does_ to the calling process.

# Task 3

In this task you will implement a "remote command execution service" by using
**UNIX sockets**. Begin by reading `man 7 unix`.

Your solution should again consist of two separate applications,
`task3_client` and `task3_server`.

The **server** opens a UNIX socket and listens for incoming connections from
the client. Once a connection has been established, the server waits for
_commands_ sent by the client. Each command that is received is executed by
the server, similarly to the commands in Task 2. To parse the full command
line (for example `ls -la`), you can again use the `parse_args()` function
provided above. The output of the command is then returned to the client
through the socket. Think about ways of how you can redirect the command's
output somewhere that lets you then write it to the socket. Note that you
only need to return the command's `stdout` output, and can ignore `stderr`
(feel free to implement it if you want). You can also define a maximum output
length, after which you can discard the rest.

The **client** connects to the server's UNIX socket and then continuously
waits for input on the command line. After reading a command, it sends it to
the server and then waits for the response, printing it to the console upon
receiving it. When the command is empty, the client somehow notifies the
server that it can shut down and subsequently exits itself.

Before shutting down, the server ensures that the UNIX socket is deleted
properly.

Example output (since `task3_server` should not produce any output, it is not
shown here):

```
./task3_client
Command:
> ls -a
.
..
Makefile
task1_client.c
task1_server.c
task2.c
task3_client
task3_client.c
task3_server
task3_server.c

Command:
> date
Wed Apr 7 10:30:00 CEST 2021
```

Why is it advantageous to use sockets instead of named pipes here?

Consider the implications of executing the server as a different user, such
as `root`, while running the client as yourself. If you have root-access to
your system, try this only if you are confident that you have understood the
potential risks.

Additional notes and hints:

- Your server only needs to support a single client connection.
- Linux/POSIX supports many different socket types. Make sure you are using
  UNIX sockets (`AF_UNIX`).
- Note that the server maintains two separate sockets: One for listening for
  incoming connections (using `listen()`), and another for communicating with
  the client (returned by `accept()`).
