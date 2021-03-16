# Exercise Sheet 3

In this exercise sheet you will work with **processes** and **signals**.

Remember to provide a `Makefile`, and to adhere to our [general guidelines
for programming exercises](../README.md).

## Task 1

Write a program which creates `N` child processes, each computing an estimate
of the number PI using a Monte Carlo simulation with `S` random samples. Both
`N` and `S` can be specified on the command line.

The parent first **creates all children**, then **waits** until they have
exited, and finally prints `"Done."`.

Every child

- Computes an estimate of PI using `S` random samples.
- Prints a message of the form `"Child <i> PID = <PID>. mc_pi(<S>) = <mc_pi(S)>."`
  where PID refers to the _process id_.

You may use the following implementation for the Monte Carlo simulation:

```c
double mc_pi(int64_t S) {
    int64_t in_count = 0;
    for(int64_t i = 0; i < S; ++i) {
        const double x = rand() / (double)RAND_MAX;
        const double y = rand() / (double)RAND_MAX;
        if(x*x + y*y <= 1.f) {
            in_count++;
        }
    }
    return 4 * in_count / (double)S;
}
```

**Hint**: Remember that a child process can be created with `fork()`.
A parent process can wait for its children using `wait()` or `waitpid()`.

Example output:

```
./task1 3 1000
Child 0 PID = 27877. mc_pi(1000) = 3.192000.
Child 1 PID = 27878. mc_pi(1000) = 3.136000.
Child 2 PID = 27879. mc_pi(1000) = 3.180000.
Done.
```

Experiment with different values for `N` and `S`. Analyze the obtained
output. Is the order of the messages consistent? Can the order of these
messages be predicted? What does it depend on?

Notice that the function `mc_pi()` uses `rand()` to repeatedly generate
pseudo-random numbers. By default this function returns the same sequence of
numbers every time. To get different estimates from each process, _seed_ the
random number generator using `srand(getpid())`. Does it matter whether you
do this before or after the call to `fork()`? Explain your answer.

## Task 2

Write a program which receives a single parameter `T` from the command line.
The program then creates a single child process. The child prints the message
`"Child <PID> sleeping for <T> seconds..."` and then sleeps for `T` seconds,
using the `sleep()` function. Afterwards it prints `"Child done."` and exits.
The parent process waits until it is sure that the child has terminated,
prints `"Parent done."` and exits as well. **There is a catch, however**: In
this task, you **must not** use `wait()` or `waitpid()`. The parent must also
not use `sleep()`.

Example output:

```
./task2 3
Child 20196 sleeping for 3 seconds...
Child done.
Parent done.
```

**Hint**: Check the man page of `fork()` to see whether the parent is somehow
notified of child termination. Also see `man sigaction` to determine how to
respond to an incoming signal\*. For a general overview on signals see `man 7 signal`.
Please also read `man signal-safety` regarding some pitfalls to be aware of.

\* There also exists an older API called `signal()`, which you **must not**
use. Use `sigaction()` instead.

## Task 3

Sometimes we don't want to receive signals right away, for example during a
critical computation that should not be interrupted.

Write a program that continuously creates child processes and only exits
upon receiving `SIGINT` (which can be sent by pressing CTRL-C in most
terminals).

Parent behavior:

- In an endless loop, the parent first creates a single child process and
  then waits for it to exit.
- **After** the child has finished, the parent checks whether `SIGINT` has
  been received in the meantime.
- If `SIGINT` _has not_ been received, it prints `"Did not receive SIGINT. Continuing."`.
- If `SIGINT` _has_ been received, it prints `"Received SIGINT. Exiting."`
  and exits.

**Hint**: Use `sigprocmask` to block signals. You may also want to use
`sigtimedwait` to check for signals that have been received.

Child behavior:

- The child first prints `"Child <PID> started."`.
- It then sleeps for 5 seconds.
- Finally, it prints `"Child <PID> done."` and exits.

**Note**: Make sure that the child process is not being interrupted by `SIGINT` either.

Example output:

```
./task3
Child 22019 started.
Child 22019 done.
Did not receive SIGINT. Continuing.
Child 22030 started.
Child 22030 done.
Did not receive SIGINT. Continuing.
Child 22041 started.
^C
Child 22041 done.
Received SIGINT. Exiting.
```
