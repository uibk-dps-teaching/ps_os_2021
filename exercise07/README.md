# Exercise Sheet 7

In this exercise sheet you will work with POSIX threads and synchronize them
using mutexes.

# Task 1

Begin by skimming through `man pthreads`. Take note of additional compiler
flags that may be required.

Write a program that stores a single global variable `my_global` of type
`int` and initialize it to `1`.

The program begins by printing the value of `my_global` and then creates a
child process. The child increments the value of `my_global` by `1` and
exits. The parent waits for the child to exit and prints the value again.

Next, the program (now referred to as the _main thread_) spawns a single
POSIX thread. The thread does the same thing as the child process, i.e.,
increments the global variable and immediately exits. The main thread waits
for the thread to finish, and prints the value one more time.

What do you notice? Explain the behavior.

# Task 2

In this task you will revisit the producer-consumer pattern, this time with
multiple consumers implemented through POSIX threads.

Your program should spawn 5 threads, the _consumers_, which all try to read
elements from a queue storing integers. For the queue, you can use the basic
implementation that is provided in [myqueue.h](myqueue.h).

- When a consumer thread successfully reads an element, it adds it to its
  local `sum`. When the element is `0`, it prints out the sum, returns it
  to the main thread and exits.
- The main thread acts as the _producer_. After spawning all 5 consumers, it
  feeds 10.000 entries of value `1` into the queue, followed by 5 entries of
  value `0`.
- The main thread then waits until all consumers have finished and computes
  the final sum from all the partial results, prints it to the console and
  exits.

To avoid race conditions, synchronize access to the queue by using the
`pthread_mutex` facilities. Carefully consider all locations where you might
need synchronization.

**Important**: The consumer threads should be able to begin processing
elements from the queue as soon as the producer has added them, NOT only once
all 10.000 elements have been added.

Example output:

```
> ./task2
Consumer 2 sum: 2165
Consumer 1 sum: 1501
Consumer 4 sum: 2320
Consumer 0 sum: 2219
Consumer 3 sum: 1795
Final sum: 10000
```

Instead of a mutex, could a semaphore be used in this situation?

# Task 3

In the provided file [philosophers.c](philosophers.c) you can find an
implementation of the classic
[Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).
However, the implementation is flawed: In some situations multiple philosophers
will wait forever for a chopstick being released, constituting a
**deadlock**.

- Explain the program.
- How can a deadlock occur? Does it always happen?
- Change the program to prevent any potential deadlocks from occurring.
  Submit your solution.
  Explain how your changes prevent deadlocks.
