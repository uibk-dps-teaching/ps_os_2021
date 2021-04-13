# Exercise Sheet 5

In this exercise sheet you will practice shared memory communication and
synchronization with semaphores. As with previous exercises, **make sure to
use the newer POSIX APIs** instead of the System V APIs.

# Task 1

In this task, you will implement the classic producer-consumer pattern using
multiple processes operating on shared memory.

Begin by reading `man 7 shm_overview`. Take note of additional linker flags
that may be required.

Create a program that receives two integer parameters `N` and `B` through the
command line. The program acts as the parent of two child processes, the
_producer_ and the _consumer_. The parent begins by attaching a shared memory
segment that has space for a buffer array and the result of the computation
performed by the consumer (explained below). It then creates the two child
processes and waits for their termination. Finally, it prints the result of
the computation, releases the shared memory segment and exits.

The producer iteratively writes all numbers in `[1, N]` into an array of size
`B` that is stored in shared memory. If `N` > `B`, the producer continues
writing by wrapping around, overwriting the values previously stored within
the array (this is called a _ring buffer_). You can implement this
functionality using the modulo operator: `buffer[i % B]`.

The consumer iteratively sums up `N` subsequent numbers from the ring buffer,
thereby effectively computing the sum of all numbers in `[1, N]`. It then
writes the resulting sum into the shared memory segment and exits.

Example output:

```
> ./task1 10 10
Result: 55
```

Analyze the obtained output. Is the computed result the expected value? Does
the computed value change across different runs? What happens when you try
different and larger values for `N` and `B`, for example 10,000 or 100,000?
Try to explain the behavior.

Notes:

- There is no need to attach multiple shared memory segments. Think about a
  good way of storing the entire context (the ring buffer and the
  result) inside a single shared memory segment.
- Make sure to use numeric data types that can store sufficiently large
  values, for example `uint64_t`, which is defined in `<stdint.h`>.
- This exercise MUST be implemented using the POSIX shared memory mechanisms.

# Task 2

In this task, you will use _semaphores_ to synchronize access to shared data
structures. Begin by reading `man 7 sem_overview`. Take note of any
additional compiler flags that may be required. This exercise MUST be
implemented using the POSIX semaphore facilities.

## Task 2a

Implement the same program as before. However this time, ensure that the
consumer only reads each value in `[1, N]` exactly once from the ring buffer.
Likewise, ensure that the producer only overwrites values in the ring buffer
that have already been read by the consumer. Use two semaphores to implement
this behavior. Your program should now always compute the correct result.

**Important**: Make sure that the producer and consumer can perform work on
the ring buffer _concurrently_, i.e., the semaphores should be used for
_counting_.

Measure the execution time of your solutions for Task 1 and Task 2a using
`/usr/bin/time`. What do you notice?

## Task 2b

Now we want to examine the effect on our timings when the workload for the
producer and consumer varies over time. Extend the synchronized variant from
Task 2a: Before writing a number to the ring buffer, the producer passes it to
the function `expensive_calculation()`. Likewise, before adding a number to the
sum, the consumer calls that same function. Use the following definition for
the function:

```c
#define MAX_SLEEP_MS 10
uint64_t expensive_calculation(uint64_t value) {
	usleep((rand() / (float)RAND_MAX) * MAX_SLEEP_MS);
	return value;
}
```

**Important**: Make sure that the producer and consumer use different _random
seeds_ (using `srand`).

Measure the execution time of your solution for Task 2b for `N = 100000` and
`B = 1`, `B = 10` and `B = 100`. What do you notice? Try to explain the
behavior.

# Task 3

Answer the following questions. Provide your answers in a `*.md` or `*.txt`
file.

- What is the difference between shared memory and e.g. a global variable
  after calling `fork()`?
- What is a race condition?
- Why do race conditions occur?
- Is the output of a program with a race condition always incorrect?
- If you run your program 100 times and always get the correct result, does
  that mean that there is no race condition?
- What is synchronization?
- Why are semaphores well suited for synchronizing producer-consumer patterns?
- There are two ways of creating a POSIX semaphore. Which would you use when?
- Could Task 2 also be implemented using a single semaphore?
