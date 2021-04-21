# Exercise 6

This week you will be investigating various _scheduling algorithms_ by
implementing them in C. To this end, we provide a _scheduler simulation
framework_, which will be explained in more detail in the next section.

**IMPORTANT**: Read the entire exercise carefully before proceeding. Part of
the goal is to learn programming within the confines of a given framework,
while adhering to a specified behavior.

## Scheduler Simulation Framework

The scheduler simulation framework is provided in the files alongside this
documentation. Run `make && ./scheduling_sim input.csv` to try it out.

The basic operation of the simulation framework is as follows:

- First, a list of processes (including all their attributes, such as arrival
  time, priority, and so on) is read from a given CSV file.
- The framework begins its simulation at time step 0, and runs until all
  processes have been serviced.
- For each time step, the framework calls a
  **user provided scheduling function** that decides which process should run
  next.
  - Only processes that can already be executed and have not finished
    are provided to the scheduling function, i.e.
    `time step >= process.arrival_time` and `process.remaining_time > 0`.
- The process that was scheduled has its remaining time decremented by 1.
- The simulation stops once all processes have no more time remaining.

The first file you should inspect is [main.c](main.c). From here, the
simulation framework is called using the different scheduling algorithms.
**This is also the place where you should implement your own schedulers**.
It is **NOT ALLOWED** to modify any other files.

We've provided reference implementations of two scheduling algorithms for
you: **first come first serve** scheduling, and **round robin** (with quantum
= 1) scheduling. Make sure you fully understand these two implementations
before you proceed to implementing your own.

A scheduling function has the following signature:

```c
void (*scheduling_function)(scheduler_context_t* ctx)
```

For each time step, the scheduling function receives a variable of type
`scheduler_context_t*`. This type can be used with the following functions:

- `size_t get_num_processes(const scheduler_context_t* ctx)` returns the number of
  processes currently eligible for scheduling.
- `process_t* get_process(const scheduler_context_t* ctx, size_t idx)`
  returns the process at the given index.
- `int get_timestep(const scheduler_context_t* ctx)` returns the current
  time step.
- `void schedule_process(scheduler_context_t* ctx, process_t* process)` marks
  the given process for scheduling in the current time step.
- `void move_process_to_end(scheduler_context_t* ctx, process_t* process)`
  moves the given process to the end of the list of eligible processes.

Processes are described by the following struct, defined in
[scheduling_sim.h](scheduling_sim.h):

```c
typedef struct {
  const char name;
  const int arrival_time;
  const int service_time;
  const int priority;
  const int remaining_time;

  int user1;
  int user2;
  int user3;
} process_t;
```

Most of its fields should be self-explanatory, however there are a few that
warrant further explanation. First, note that each process has a single
character `name`, which is used during the output and which might also be
useful for debugging.

Most of the member variables of `process_t` are marked as `const`, indicating
that they should not be modified by your scheduler. However, there are three
non-const `int` variables `user1, user2, user3` that can be used to store
**anything you want**. This is often times useful, when you need to retain
state between calls to your scheduler function. You can assume these
variables to be initialized to zero.

## Example Output

In the file [example_output.log](example_output.log) you can find the output
produced by our own reference implementations for each algorithm. Feel free to
use this as a guide for what output your system might produce.

Here you can see the example output produced for our FCFS implementation:

```
===================================        fcfs        ===================================

    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
A:  x   x   x   x   x   x   x
B:              *               x   x   x
C:                  *                       x   x
D:                          *                       x   x   x   x   x
E:                                  *                                   x   x
```

For each time step, the letter indicates the process name (`process_t.name`)
that was returned by the scheduling function. An `x` means that a process has
been scheduled at this time step. A `*` indicates that a process has arrived
at this time step (if it is scheduled immediately, an `x` will be printed
instead).

**DISCLAIMER**: We do not want to guarantee the correctness of our own
implementations. There might very well be cases where your implementation
produces different results (for example, due to some ambiguity in the
algorithm's specification). Please take note of any such cases and try to
justify why your output is correct.

## Additional Notes

- If two processes are equally eligible for scheduling (e.g., have the same
  priority), fall back to first-come-first-served.
- All state (if any) should be stored using the user data fields, i.e., your
  schedulers must NOT use any global variables (or `static` local variables).

---

## Task 1

- Implement **shortest remaining time first** scheduling.
- Implement **round robin** scheduling with a quantum of 4.

## Task 2

- Implement **priority** scheduling **with preemption**.
- Implement **priority** scheduling **without preemption**.

Processes with higher `priority` values should be scheduled _before_
processes with `lower` priority values.

## Task 3

Consider the downsides of priority scheduling, in particular with preemption.
Come up with and implement a variant of priority scheduling that maintains
its basic principles, but is a bit more fair.
