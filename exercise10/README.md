# Exercise Sheet 10

In this exercise sheet you will implement _custom allocators_ and compare their
performance to the system allocator.

# Task 1

In some cases the system allocator (`malloc()` / `free()`) may not be suitable
(for example in embedded programming) or may not perform well for atypical
allocation patterns (for example in video games and other highly optimized
programs). In these situations, it can be beneficial to implement a _custom
allocator_.

One of the simplest allocators is the so-called _free-list allocator_, which you
will implement in this task. Free-list allocators come in different forms, but
the most basic variant allows to allocate _blocks_ of a fixed size from a given
area of memory, called the _memory pool_.

As the name implies, a free-list allocator consists of a linked list of free
blocks which are available to be allocated. Importantly, this list is embedded
within the memory pool itself, i.e., lives inside the very memory region that is
being managed for the user (this is called an _intrusive data structure_). More
specifically, each block should be preceded by a _header_ that contains a
pointer to the next free block in the list. This diagram illustrates how your
free-list may look like initially:

```
           │------------------ Memory pool ------------------
           ┌───┬────────┬───┬────────┬───┬────────┬───┬──────
           │   │        │   │        │   │        │   │
Next Free  │   │  Free  │   │  Free  │   │  Free  │   │  ...
    │      │   │        │   │        │   │        │   │
    │      └▲─┬┴────────┴▲─┬┴────────┴▲─┬┴────────┴▲─┬┴──────
    └───────┘ └──────────┘ └──────────┘ └──────────┘ └───────
```

The user can then allocate one block at a time. After the first two blocks have
been allocated, the list may look like this:

```
           │------------------ Memory pool ------------------
           ┌───┬────────┬───┬────────┬───┬────────┬───┬──────
           │   │        │   │        │   │        │   │
Next Free  │   │ Allocd │   │ Allocd │   │  Free  │   │  ...
    │      │   │        │   │        │   │        │   │
    │      └───┴────────┴───┴────────┴▲─┬┴────────┴▲─┬┴──────
    └─────────────────────────────────┘ └──────────┘ └───────
```

If the first block is then free'd again, it's header will point to the third
block, which is the next free block:

```
           │------------------ Memory pool ------------------
           ┌───┬────────┬───┬────────┬───┬────────┬───┬──────
           │   │        │   │        │   │        │   │
Next Free  │   │  Free  │   │ Allocd │   │  Free  │   │  ...
    │      │   │        │   │        │   │        │   │
    │      └▲─┬┴────────┴───┴────────┴▲─┬┴────────┴▲─┬┴──────
    └───────┘ └───────────────────────┘ └──────────┘ └───────
```

While in real-world applications, custom allocators may obtain the memory pool
they manage through various means, for simplicity we will resort to using the
system allocator: Your allocator first uses the system allocator to obtain a
large pool of memory, which it then manages. Importantly, this is the only time
your custom allocator should interact with the system allocator (except when
freeing the pool at the very end).

Your allocator should provide these four functions:

- `void* my_malloc(size_t size)` allocates a single block from the list,
  returning its address. If no free block is available, it returns `NULL`. While
  the signature of this function mimics `malloc()`, the `size` parameter does not
  influence the size of your blocks (they all have the same fixed block size).
  Return `NULL` if `size` exceeds the size of a single block.
- `void my_free(void* ptr)` frees a block, returning it to the list.
- `void my_allocator_init(size_t size)` initializes the memory pool for the
  allocator to a total size of `size`. Your allocator can at most allocate
  however many blocks (including list headers) fit into `size`.
- `void my_allocator_destroy()` frees the memory pool for the allocator.

Use the `run_membench_global()` function provided in the [membench](membench.h)
utility that comes with this exercise sheet to evaluate the performance of your
allocator against the system allocator. Briefly interpret the results.

**Additional notes:**

- Your allocator is not required to support allocating more than one block at a
  time.
- Assume a fixed block size of `BLOCK_SIZE = 1024` bytes.
- Your allocator must be thread-safe. Make sure to properly synchronize critical
  sections.
- Make sure to use Valgrind to find potential errors in your implementation.
- There should be no need to modify any of the provided files.
- Use an optimized build for benchmarking.

# Task 2

One of the shortcomings of the simple free-list allocator from Task 1 is that it
can only allocate blocks of a fixed size. To alleviate this problem, turn your
allocator into a _best fit_ allocator that supports blocks of varying size.

The basic working principle remains the same, however your list initially starts
out with only one huge block that spans the entire memory pool. Once an
allocation of a given `size` is made, the block is split into two: One part of
size `size` is returned to the user, while the remainder is reinserted into the
free list.

Given that the free-list now consists of blocks of varying size, your allocator
should not simply allocate the first block from the free-list. Instead, it
should try and find the block in the list whose size is closest to the requested
`size` (hence the name "best fit" allocator).

Again benchmark the performance of your allocator using `run_membench_global()`.

**Additional notes:**

- Your list headers will no longer be spaced evenly throughout the memory pool.
  Instead, you have to create them dynamically based on the number of blocks
  (and their sizes) currently in the list.
- Additionally, headers will now need to store more information than simply a
  pointer to the next block.
- Keep the number of entries in the free-list as small as possible. Once a block
  is free'd, check whether it can be merged with any of its neighboring blocks.

# Task 3

For this task, instead of a single global allocator that requires
synchronization to be thread-safe, create a separate _thread local_ allocator
for each thread that uses it. To do so, you can leverage C11's thread local
storage class specifier (the `_Thread_local` keyword).

Convert the allocator(s) you implemented in Task 1 and/or Task 2, and again
measure the performance, now using the `run_membench_thread_local()` function.
