# Exercise Sheet 8

In this exercise sheet you will continue to use POSIX threads and experiment
with advanced techniques for mutual exclusion and synchronization.

# Task 1

Write a program that initializes an integer value `X` to 10,000,000 and
subsequently creates 1,000 threads. Each of these newly created threads should
execute a loop of 10,000 iterations. In each of these iterations, the value `X`
should be decreased by one. The main thread waits for all the other threads to
finish and then writes the final value of `X` to the console.

Then, implement two variants:

1. Protect access to `X` using a mutex. All threads should be able to advance in
   the loop concurrently.
2. Use C11 atomics to perform the decrement operation.

How does the program behavior differ between the two variants? Measure the
execution time of both using `/usr/bin/time -v` and report your findings. Make
sure to use an optimized build (`-O2` or `-O3`).

# Task 2

Implement the single producer, multiple consumer pattern as in last weeks'
[Exercise Sheet 7 Task 2](../exercise07/README.md), but increase the number
of consumers to 50 and the number of `1`s produced to 100,000.

Instead of using `pthread_mutex` to synchronize access to the queue, you will
now implement your own mutex using atomics. Your solution should at least
provide two functions: `my_mutex_lock` and `my_mutex_unlock`. You may come up
with additional functions, if needed.

Think about ways of representing the state of your mutex using an atomic
variable. Instead of manipulating that variable directly however, use the
`atomic_flag_test_and_set` and `atomic_flag_clear` functions. _Alternatively_,
if you prefer, you can also use `atomic_compare_exchange_weak`. Make sure you
fully understand the behavior of these functions and how they could be used to
implement your mutex before proceeding.

Measure the execution time with `/usr/bin/time -v`, compare it to the execution
time of last week's task (also using 50 consumers and 100,000 values!), report
your findings and interpret the results. Make sure to use an optimized build
(`-O2` or `-O3`).

Provide both variants (using your own mutex and `pthread_mutex_t`) in your
uploaded solution. An elegant way of doing this while minimizing code
duplication could be to use preprocessor macros.

**Hints**:

- Locking your mutex will likely require a loop of some sort.
- It may be helpful to `assert` your expectations about the state of the mutex
  within your lock/unlock functions.
- _Optional_: If you want to improve the performance of your mutex, take a look
  at the `sched_yield` function.

# Task 3

Continuing with the single producer, multiple consumer example, revert back to
using `pthread_mutex` and increase the number of consumers to 500 (keeping the
number of values at 100,000).

Now use a pthread _condition variable_ to signal the availability of new
elements to the consumers. What is the advantage of using a condition variable
in this case, compared to using plain mutexes?

Again take measurements using `/usr/bin/time -v` and interpret the results.
Make sure to use an optimized build (`-O2` or `-O3`).
