# Exercise Sheet 12

The following is a (slightly modified) _real_ exam from a previous year. It
should give you a general idea of what to expect from this year's exam.

This exercise sheet is optional, and provides up to 3 **bonus points**: These
points are not included in the total number of points reachable this semester
(33), but will count towards your personal final score (theoretically, it is
thus possible to achieve 36/33 points).

While the exam itself is not split into explicit tasks, we define the following
tasks for receiving the bonus points:

- **Task 1:** Implement the basic functionality without load balancing (i.e.,
  reassigning workers).
- **Task 2:** Implement load balancing based on door throughput.
- **Task 3:** Provide a fully functional solution that compiles without
  warnings, executes correctly producing only the expected output and
  finishes cleanly without leaking any resources.

**As usual, please mark your completed tasks in OLAT**.

**NOTE**: We recommend that you attempt to solve this exercise sheet by
simulating the real exam scenario: Prepare any materials you may need, such as
your solutions to previous exercises, for quick access (the real exam will be open
book!). Take an hour and 45 minutes and only then continue reading the rest of this
sheet. Try to solve it in time.

---

## General Information

**Please read this description carefully.**

Talking, chatting, texting, or any other form of a live human to human
communication is not allowed and may lead to a negative grade. However, you are
allowed to use all other offline and online resources such as previous homework
solutions, manpages, StackOverflow, etc.

If you directly copy code from one of these sources, please add a
**citation**
as a comment before the code fragment. i.e.

```C
// The following code is from my solution to exercise 5.2
```

or

```C
// Source: https://stackoverflow.com/questions/14888027/mutex-lock-threads
```

If we find the same code in multiple solutions without citation, we have to
assume cheating, and you will receive a failing grade. Note that it does not
matter whether you are the person copying from, or providing your solution to
another -- both will receive a failing grade.

Your program **must compile** on ZID-GPL (or ZID-Desk) and include a Makefile.
If something does not work as expected, provide comments in the source code
explaining what you think the problem is. It might improve your grade.

**You have 1 hour and 45 minutes to complete this exercise.**

**Please make sure to upload your solution to OLAT in time.**

### General Hints

- Don't forget you can load a newer version of GCC on ZID-GPL using e.g.
  `module load gcc/9.2.0`.
- Remember that you can use `valgrind` to check your program for memory leaks.
- Try to develop your program incrementally, and focus on the core
  functionality first.

## Task: Door Factory

A factory producing doors wants to dynamically reassign workers to optimize the
throughput of doors.

To make a door, a **door knob** is required, which is also produced at the
factory. The factory employs **20 workers**, which can either produce knobs or
doors.

Some workers are more lazy than others, taking longer to complete their work.
The following table lists the time required to produce a knob or door:

| Item | Time to produce         | Required materials |
| ---- | ----------------------- | ------------------ |
| Knob | `10 + laziness * 30` ms | -                  |
| Door | `10 + laziness * 90` ms | 1x Knob            |

The factory starts out with zero knobs and doors produced. Initially, 50% of
workers are assigned to making knobs, while the other 50% makes doors.
**Every second**, the factory manager evaluates the knob and door throughput,
and decides whether to reassign a worker. If the computed knob rate exceeds the
door rate by more than 10%, a knob-worker is reassigned to making doors, and
vice-versa.

---

Model each worker as a separate thread. Randomly determine the `laziness` value
within the range `[0, 1]` for each worker once at the beginning. The main thread
should act as the factory manager, assigning the type of work to be performed by
each thread.

The main thread should read the number of doors to produce from the command line
and continuously reassign workers (if required) until a sufficient number of
doors has been produced, at which point the main thread should clean up and
exit. In addition to evaluating throughputs once per second, the main thread
should print the computed values and any worker reassignments to the console.
Once all work is complete, the main thread should also print the total number of
knobs and doors produced, as well as the overall time required.

Your solution should produce a single executable called `factory`, which can be
called like so: `./factory <enable load balancing> <number of doors to produce>`.
The first parameter can either be `0` or `1` and indicates whether the main
thread should attempt to balance the workload by reassigning workers or not. If
load balancing is disabled, it should still compute and print the throughputs.

Example output:

```
> ./factory 1 2000
Producing 473.00 knobs/s, 308.00 doors/s
        Workers reassigned: 9 making knobs, 11 making doors
Producing 463.50 knobs/s, 319.50 doors/s
        Workers reassigned: 8 making knobs, 12 making doors
Producing 440.67 knobs/s, 334.00 doors/s
        Workers reassigned: 7 making knobs, 13 making doors
Producing 420.25 knobs/s, 345.25 doors/s
        Workers reassigned: 6 making knobs, 14 making doors
Producing 388.80 knobs/s, 369.00 doors/s
Producing 367.83 knobs/s, 366.17 doors/s
2197 doors produced in 6.00s (366.17 doors/s)
2207 knobs produced
```

Notes:

- As usual, make sure to properly synchronize access to shared data structures
  in a way that still allows multiple threads to work simultaneously.
- To get a different random value for your `laziness` level in each thread,
  you may use `rand_r` with a seed initialized like so:
  `unsigned seed = (unsigned)pthread_self()`.
- Notice that the given times to produce knobs and doors are in milliseconds.
  You can use e.g. `usleep(ms * 1000)` to simulate the working time.
- We recommend that you compute the knob and door throughputs based on the
  total number of produced items, instead of just those produced within the
  last second.
