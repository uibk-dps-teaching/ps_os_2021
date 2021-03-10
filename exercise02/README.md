# Exercise 2 - Binary Search Tree

In this exercise sheet you will implement a _binary search tree storing
numeric values_ and follow some core principles of modular programming. The
purpose of the exercise is to check (for yourself) whether you are
sufficiently fluent in using the C programming language. Most exercises in
this course will build directly on top of what you learned in the previous
semester.

If you are struggling with this exercise sheet, please consider doing this
course at a later point in your studies. Otherwise you need to be prepared
for the fact that subsequent exercises will probably require greater effort
from you than originally planned by us.

### General remarks about this exercise:

- Read this task from top to bottom before starting your implementation.
- Ensure you fully understand the concept of a sorted binary tree as well as
  the provided header file.
- If you find an error, note it down and make _reasonable_ assumptions to
  continue.
- Keep your code clean and simple. It should be as readable as possible.
- Think about how you want to implement your solution before you begin.
  Hint: Many operations on a tree can be implemented more elegantly using
  recursive functions.
- This exercise provides several existing files. You should not be required
  to make changes to any of them, except for `bstree.c`.
- You are free to include additional headers within `bstree.c`.
- Implement `bstree` and the required functions in `bstree.c`.
  - Your implementation may (and should) utilize additional functions not
    stated in the header file, but make sure they are not exported (by
    marking them as `static`).
  - Only the `bstree_print` function produces output during normal execution.
    There should not be any other debug messages / logs of any kind.
  - Try to reuse already implemented functions (e.g. `bstree_remove` could make
    use of `bstree_maximum`).
  - Everything (variable names, comments, etc) must be written in English.
- Ensure proper error handling.
  - Use `assert` to ensure that `NULL` is not passed to functions which can not
    handle this case.
  - Check return values of functions that may fail, like `malloc`.
- The two programming tasks (1 & 2) come with _tests_ that you can use to
  check the correctness of your solution.
- For this task a `Makefile` is provided. Aside from the standard targets
  (`all`, `clean`), the following targets are provided (and can be built/run
  using `make <target>`):
  - `task1_tests`: Compile tests for task 1.
  - `check_task1`: Compare output produced by printing the tree within
    the tests for task 1 to the expected output in `bstree_task1_output.txt`.
    Additionally run `Valgrind` to ensure no memory leaks or other
    memory-related bugs are present.
  - `task2_tests`: Compile tests for task 2.
  - `check_task2`: Compare output of task 2, check for memory bugs.

## Task 1

Implement the first part of a module (`bstree.h`, `bstree.c`) providing a
sorted binary tree for numeric values of type `value_t`. With your
implementation it should be possible to:

1. Create and destroy a tree: `bstree_create()` and `bstree_destroy()`.
2. Insert values in the tree: `bstree_insert()`.
3. Check whether the tree contains a given value: `bstree_contains()`.
4. Print the entire tree to an output stream: `bstree_print()`.

The header file is already provided and holds the signatures of the
functions alongside short descriptions. Your implementation should fulfill
these descriptions as closely as possible.

The `bstree` type is used as a _handle_ to work with the whole tree, and can
also be used as storage for information concerning the entire tree. How you
implement the tree structure itself is up to you. You might for example have
a `bstree_node`, which holds on to a `value_t` and references to the left and
right child nodes.

Use `make check_task1` to check whether the produced output matches the
expected output, and whether your implementation has any memory-related bugs.

## Task 2

This task builds on top of the previous one and extends the capabilities of
your tree. You are asked to implement the following functionalities:

1. Get the minimum / maximum value stored within the tree: `bstree_minimum()`
   and `bstree_maximum()`.
2. Get the depth of a value (the root node has depth 0): `bstree_depth()`.
3. Get the size of (the total number of values stored within) the tree:
   `bstree_size()`.
4. Remove values from the tree: `bstree_remove()`.

   **NOTE**: When removing nodes from a binary search tree, you sometimes have
   the choice of replacing the node by either the _maximum_ of the _left
   subtree_ or the _minimum_ of the _right subtree_. The provided tests and
   example outputs assume that you pick the former strategy (maximum of left
   subtree).

Use `make check_task2` to check whether the produced output matches the
expected output, and whether your implementation has any memory-related bugs.

## Task 3

In this task no implementation is required. Its purpose is to ensure you
understand the background behind the details of the previous task. You should
be able to answer the following questions without the use of any additional
notes. Presenting images and examples is fine, though not required.

- When would you use a sorted binary tree over a linked list?
- What would you change if the tree had to support `int` values instead of
  `float`s? Can you use this approach to support any value type, or are there
  limitations?
- Why do we use multiple translation units instead of putting everything a
  single file?
- Is the development cycle (i.e. changing a source file, recompiling and
  running tests) faster when using multiple translation units? Explain your answer.
- What is a _header guard_ and why is it needed?
- Why is `struct bstree` not _defined_ in the header file? What are the
  implications?
- Explain the `const` in the parameter list of `bstree_print`, is it
  required?
- Explain memory leaks. Why are memory leaks bad?
- What is the reason behind writing _everything_ in English?
- Why should you use `static` for _non-exported_ functions?
- Why should we comment our source code? Is it always needed? What should the
  comment state? What is _self-documenting code_?
- Why should the module not output debug messages?
- Why and when should you use `assert`?
- What are the benefits of using a `Makefile` over calling the compiler by hand?
- Imagine it was your job to design the interface for the bstree module
  (`bstree.h`). What, if anything, would you have done differently, and why?
